#include "nc_session.h"
#include "nc_server.h"
#include "nc_client.h"
#include "nguard.h"
#include "nlog_mt.h"

namespace nexus {

	const int MSG_HEAD	= sizeof(DWORD);		// ��Ϣͷ�Ĵ�С

	nc_session::nc_session() : m_session_id(0), m_server_ptr(NULL), m_client_ptr(NULL), m_socket(0), m_status(TSS_NONE), m_msg_head(0),
		m_recv_msg_ptr(NULL), m_read_rd(0), m_read_wr(0), m_write_index(0), m_wsa_send(0), m_wsa_recv(0), m_max_buffer_size(0)
	{
		m_write_buffer_ptr[0]	= NULL;
		m_write_buffer_ptr[1]	= NULL;
		m_write_rd[0]			= 0;
		m_write_rd[1]			= 0;
		m_write_wr[0]			= 0;
		m_write_wr[1]			= 0;
	}

	nc_session::~nc_session()
	{
		_free_memory();
		m_session_id			= 0;
		m_server_ptr			= NULL;
		m_socket				= 0;
		m_status				= TSS_NONE;
		m_msg_head				= 0;
		m_recv_msg_ptr			= NULL;
		m_read_rd				= 0;
		m_read_wr				= 0;
		m_write_buffer_ptr[0]	= NULL;
		m_write_buffer_ptr[1]	= NULL;
		m_write_rd[0]			= 0;
		m_write_rd[1]			= 0;
		m_write_wr[0]			= 0;
		m_write_wr[1]			= 0;
		m_write_index			= 0;
		m_wsa_send				= 0;
		m_wsa_recv				= 0;
		m_max_buffer_size		= 0;
	}

	uint32 nc_session::connect_to(const std::string& ip_addresss, uint16 port)
	{
		if (TSS_ACCEPTEX != m_status)
		{
			return 0XFFFFFFFF;
		}

		// Disable blocking send/recv calls
		u_long arg = 1;
		::ioctlsocket(m_socket, FIONBIO, &arg);

		if (!m_nagle)
		{
			// Disables the NAGLE algorithm for send coalescing
			BOOL nodelay = TRUE;
			::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));
		}
		// ���ó�ʱʱ��
		uint32 time_out = 10;
		::setsockopt(m_socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&time_out, sizeof(time_out));

		// Assigns the socket to his completion port
		::CreateIoCompletionPort((HANDLE)m_socket, m_client_ptr->m_completion_port, (ULONG_PTR)this, 0);

		LPFN_CONNECTEX fn_connectex = NULL;

		GUID	guid = WSAID_CONNECTEX;
		DWORD	bytes = 0;

		::WSAIoctl(m_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid,
			sizeof(GUID), &fn_connectex, sizeof(fn_connectex), &bytes, 0, 0);

		sockaddr_in address;

		ZeroMemory(&address, sizeof(address));
		address.sin_family      = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port        = htons(0);

		int ret = ::bind(m_socket, (sockaddr*)&address, sizeof(address));

		if (0 != ret)
		{
			_handle_connect(1);
			return 0XFFFFFFFF;
		}

		ZeroMemory(&address, sizeof(address));
		address.sin_addr.s_addr	= inet_addr(ip_addresss.c_str());
		address.sin_family		= AF_INET;
		address.sin_port		= htons(port);

		// Ͷ��connectex����
		m_operator.status		= IOCS_ACCEPT;
		m_operator.ov.hEvent	= NULL;

		DWORD last_error = ::WSAGetLastError();
		BOOL result = fn_connectex(m_socket, (const sockaddr*)&address, sizeof(address), NULL, 0, &bytes, (OVERLAPPED*)&m_operator);
		last_error = ::WSAGetLastError();

		if (TRUE != result && WSA_IO_PENDING != last_error)
		{
			_handle_connect(last_error);
			return 0XFFFFFFFF;
		}

		return m_session_id;
	}

	bool nc_session::send(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// ������
		if (!msg_ptr)
		{
			return false;
		}

		bool ret = send_msg(session_id, &(msg_ptr->session_id), size);
		msg_ptr->dec_reference();
		return ret;
	}

	bool nc_session::send_msg(uint32 session_id, const void* msg_ptr, uint32 size)
	{
		// �����������ӵ���Ч��,m_session_id��m_status���̷߳���
		if (session_id != m_session_id || 0 == size || !msg_ptr || TSS_OPEN != m_status)
		{
			return false;
		}

		{// ��������m_mutex�����ķ�Χ������ݹ�
			// �����
			nguard<nfast_mutex> scoped_lock(m_mutex);

			if (session_id != m_session_id || TSS_OPEN != m_status)
			{
				return false;
			}

			// �жϻ������Ƿ����
			uint32 index = (m_write_index + 1) % 2;

			if (m_write_wr[index] + size >= m_max_buffer_size)
			{
				return false;
			}

			// �������ݣ�1. ��Ϣ����
			*reinterpret_cast<WORD*>(m_write_buffer_ptr[index] + MSG_HEAD + m_write_wr[index]) = WORD(size);
			m_write_wr[index] += sizeof(WORD);
			// �������ݣ�2. ��Ϣ����
			memcpy(m_write_buffer_ptr[index] + MSG_HEAD + m_write_wr[index], msg_ptr, size);
			m_write_wr[index] += size;

			// �ж��Ƿ�Ͷ��send����, ע�ⲻҪ����m_mutex, m_wsa_send
			if (::InterlockedCompareExchange((LPLONG)&m_wsa_send, 1, 0) != 0)
			{
				return true;
			}

			m_write_index = index;
			*reinterpret_cast<DWORD*>(m_write_buffer_ptr[m_write_index]) = m_write_wr[m_write_index];
			m_write_rd[m_write_index] = 0;
			m_write_wr[m_write_index] += MSG_HEAD;
		}

		WSABUF wbuf;
		DWORD bytes = 0;

		wbuf.buf = m_write_buffer_ptr[m_write_index];
		wbuf.len = m_write_wr[m_write_index];

		m_write.status		= IOCS_WRITE;
		m_write.ov.hEvent	= NULL;

		int result = ::WSASend(m_socket, &wbuf, 1, &bytes, 0, &m_write.ov, NULL);
		DWORD last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_write(last_error, bytes);
			return false;
		}

		return true;
	}

	bool nc_session::_init(uint16 index, nc_server* server_ptr, nc_client* client_ptr, uint32 buffer_size, bool nagle)
	{
		if (0XFFFF == index || !(NULL != server_ptr || NULL != client_ptr) || 0 == buffer_size)
		{
			NLOG_NERROR(_T("ntcp_session init error!, index = %u, buffer_size = %u"), index, buffer_size);
			return false;
		}

		*reinterpret_cast<uint16*>(&m_session_id) = index;
		m_server_ptr			= server_ptr;
		m_client_ptr			= client_ptr;
		m_max_buffer_size		= buffer_size;
		m_nagle					= nagle;

		// alloc memory
		m_recv_msg_ptr = new char[m_max_buffer_size];

		if (!m_recv_msg_ptr)
		{
			NLOG_NERROR(_T("alloc recv_msg_ptr memory error!"));
			return false;
		}

		m_write_buffer_ptr[0] = new char[m_max_buffer_size];

		if (!m_write_buffer_ptr[0])
		{
			NLOG_NERROR(_T("alloc m_write_buffer_ptr memory error!"));
			_free_memory();
			return false;
		}

		m_write_buffer_ptr[1] = new char[m_max_buffer_size];

		if (!m_write_buffer_ptr[1])
		{
			NLOG_NERROR(_T("alloc m_write_buffer_ptr memory error!"));
			_free_memory();
			return false;
		}

		m_status = TSS_INIT;

		return true;
	}

	void nc_session::_free_memory()
	{
		m_status = TSS_NONE;

		// �ͷ�session new���ڴ�����
		if (m_recv_msg_ptr)
		{
			delete []m_recv_msg_ptr;
			m_recv_msg_ptr = NULL;
		}

		if (m_write_buffer_ptr[0])
		{
			delete []m_write_buffer_ptr[0];
			m_write_buffer_ptr[0] = NULL;
		}

		if (m_write_buffer_ptr[1])
		{
			delete []m_write_buffer_ptr[1];
			m_write_buffer_ptr[1] = NULL;
		}
	}

	void nc_session::_pre_accept_or_connect()
	{
		m_socket				= ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		m_operator.status		= IOCS_ACCEPT;
		m_operator.ov.hEvent	= NULL;
		m_operator.session_ptr	= this;

		// pool������closeʱͳһ�ͷ�
		m_msg_head				= 0;
		m_read_rd				= 0;
		m_read_wr				= 0;

		m_write_rd[0]			= 0;
		m_write_rd[1]			= 0;
		m_write_wr[0]			= 0;
		m_write_wr[1]			= 0;
		m_write_index			= 0;

		m_wsa_send				= 0;
		m_wsa_recv				= 0;

		m_status				= TSS_ACCEPTEX;
	}

	void nc_session::_open(HANDLE completion_port, DWORD address, DWORD port)
	{
		if (!m_server_ptr)
		{
			return;
		}

		// Disable blocking send/recv calls
		u_long arg = 1;
		::ioctlsocket(m_socket, FIONBIO, &arg);
		// Disables the NAGLE algorithm for send coalescing
		if (!m_nagle)
		{
			BOOL nodelay = TRUE;
			::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));
		}
		// Assigns the socket to his completion port
		::CreateIoCompletionPort((HANDLE)m_socket, completion_port, (ULONG_PTR)this, 0);
		// ����״̬
		m_status = TSS_OPEN;
		// �ڻص�����֮�������ñ�־λ
		::InterlockedIncrement((LPLONG)&m_wsa_recv);
		// �ص�����
		m_server_ptr->on_connect(m_session_id, (uint32)address, (uint32)port);

		// Ͷ�ݶ�����
		m_read_rd		= 0;
		m_read_wr		= sizeof(DWORD);

		// Ͷ��recv length����
		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		wbuf.buf = m_recv_msg_ptr;
		wbuf.len = m_read_wr;

		m_read.status		= IOCS_READ;
		m_read.ov.hEvent	= NULL;

		// ���ܻص�����������close�����������pool��Դ�����ͳһ��close_helper�ͷ�
		if (TSS_OPEN != m_status)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			return;
		}

		int result = ::WSARecv(m_socket, &wbuf, 1, &bytes, &flags, &m_read.ov, NULL);
		DWORD last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_read(last_error, bytes, NULL);
			return;
		}
	}

	void nc_session::_release()
	{
		// �����ж�
		if (TSS_SHUT != m_status || 0 != m_wsa_recv && 0 != m_wsa_send)
		{
			return;
		}

		// _release��helper��������������ã�����ݹ�
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSS_SHUT != m_status || 0 != m_wsa_recv && 0 != m_wsa_send)
			{
				return;
			}

			// ����session״̬
			m_status = TSS_CLOSE;
			uint32 session_id = m_session_id;
			// �ı�m_client_id
			++(*(reinterpret_cast<uint16*>(&m_session_id) + 1));

			// Ͷ�ݻص�close call back��������
			if (m_server_ptr)
			{
				m_server_ptr->_post_close_call_back(session_id);
			}
			else if (m_client_ptr)
			{
				m_client_ptr->_post_close_call_back(session_id);
			}
		}
	}

	void nc_session::_close()
	{
		{// ���ű���m_mutex���ʱ���ݹ飬helper��������ܷ���m_mutex
			nguard<nfast_mutex> guard(m_mutex);

			if (TSS_OPEN != m_status)
			{
				return;
			}

			m_status = TSS_SHUT;
		}

		::shutdown(m_socket, SD_BOTH);
		::closesocket(m_socket);
		m_socket = 0;

		// �����ͷ���Դ
		_release();
	}

	bool nc_session::_parse_msg(char* buffer_ptr, uint32 size, nsafe_mem_pool* pool_ptr)
	{
		// ����Ϊ��С��Ϣ��λ���ص�����
		DWORD temp_size = 0;
		uint32 ptr = 0;
		nmessage* nmsg_ptr = NULL;

		while (ptr < size)
		{
			temp_size = DWORD(*(WORD*)(buffer_ptr + ptr));
			ptr += sizeof(WORD);

			if (ptr + temp_size <= size)
			{
				// ����nmessage
				nmsg_ptr = reinterpret_cast<nmessage*>(pool_ptr->alloc(sizeof(nmessage) + temp_size));

				if (NULL == nmsg_ptr)
				{
					return false;
				}

				nmsg_ptr->pool_ptr		= pool_ptr;
				nmsg_ptr->next_ptr		= NULL;
				nmsg_ptr->reference		= 1;
				nmsg_ptr->size			= temp_size;
				nmsg_ptr->session_id	= 0;
				nmsg_ptr->server_id		= 0;

				// ��������
				memcpy(&(nmsg_ptr->session_id), buffer_ptr + ptr, temp_size);

				if (m_server_ptr)
				{
					m_server_ptr->on_datarecv(m_session_id, nmsg_ptr, temp_size);
				}
				else if (m_client_ptr)
				{
					m_client_ptr->on_datarecv(m_session_id, nmsg_ptr, temp_size);
				}
			}
			else
			{
				break;
			}

			ptr += temp_size;
		}

		return true;
	}

	void nc_session::_handle_connect(DWORD last_error)
	{
		if (!m_client_ptr)
		{
			return;
		}

		uint32 error_code = ECC_Connected;

		if (0 != last_error)
		{
			switch (last_error)
			{
			case WSAETIMEDOUT:
			case ERROR_SEM_TIMEOUT:
				{

					error_code = ECC_ConnectTimeout;
				}
				break;
			default:
				{
					error_code = ECC_Error;
				}
				break;
			}

			m_client_ptr->on_connect(m_session_id, error_code, 0);
			m_status = TSS_CLOSE;
			m_client_ptr->_return_session(this);
			return;
		}

		// ����״̬
		m_status = TSS_OPEN;
		// �ڻص�����֮�������ñ�־λ
		::InterlockedIncrement((LPLONG)&m_wsa_recv);
		// �ص�����
		m_client_ptr->on_connect(m_session_id, error_code, 0);

		// Ͷ�ݶ�����
		m_read_rd		= 0;
		m_read_wr		= sizeof(DWORD);

		// Ͷ��recv length����
		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		wbuf.buf = m_recv_msg_ptr;
		wbuf.len = m_read_wr;

		m_read.status		= IOCS_READ;
		m_read.ov.hEvent	= NULL;

		// ���ܻص�����������close�����������pool��Դ�����ͳһ��close_helper�ͷ�
		if (TSS_OPEN != m_status)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			return;
		}

		int result = ::WSARecv(m_socket, &wbuf, 1, &bytes, &flags, &m_read.ov, NULL);
		last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_read(last_error, bytes, NULL);
			return;
		}
	}

	void nc_session::_handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr)
	{
		nclose_helper helper(this);

		if (0 != last_error || 0 == bytes_transferred)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			_close();
			return;
		}

		m_read_rd += bytes_transferred;

		if (m_read_rd == m_read_wr)
		{
			if (0 == m_msg_head)
			{
				m_msg_head = *reinterpret_cast<DWORD*>(m_recv_msg_ptr);
				m_read_rd = 0;
				m_read_wr = m_msg_head;

				// ���������Ϣͷ
				if (0 == m_msg_head || m_msg_head > m_max_buffer_size)
				{
					::InterlockedDecrement((LPLONG)&m_wsa_recv);
					_close();
					return;
				}
			}
			else
			{
				// $$$ �ص���Ϣ����, ע�����ʱ����֤��Ϣ��ô����ԣ���wsarecv֮�����
				if (!_parse_msg(m_recv_msg_ptr, m_msg_head, pool_ptr))
				{
					::InterlockedDecrement((LPLONG)&m_wsa_recv);
					_close();
					NLOG_NERROR(_T("msg prase error!"));
					return;
				}

				// ����Ͷ�ݶ�����
				m_msg_head	= 0;
				m_read_rd	= 0;
				m_read_wr	= sizeof(DWORD);
			}
		}

		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		// Ͷ��recv����
		wbuf.buf = m_recv_msg_ptr + m_read_rd;
		wbuf.len = m_read_wr - m_read_rd;

		m_read.status		= IOCS_READ;
		m_read.ov.hEvent	= NULL;

		// �����ⲿ������close��pool��Դ�����ͳһ��close_helper�ͷ�
		if (TSS_OPEN != m_status)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			return;
		}

		int result = ::WSARecv(m_socket, &wbuf, 1, &bytes, &flags, &m_read.ov, NULL);
		last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_read(last_error, bytes, NULL);
			return;
		}
	}

	void nc_session::_handle_write(DWORD last_error, DWORD bytes_transferred)
	{
		nclose_helper helper(this);

		if (0 != last_error || 0 == bytes_transferred)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_send);
			_close();
			return;
		}

		m_write_rd[m_write_index] += bytes_transferred;

		if (m_write_rd[m_write_index] == m_write_wr[m_write_index])
		{
			m_write_rd[m_write_index] = 0;
			m_write_wr[m_write_index] = 0;

			nguard<nfast_mutex> scoped_lock(m_mutex);
			m_write_index = (m_write_index + 1) % 2;

			// �����TSS_OPEN != m_status����close����
			if (0 == m_write_wr[m_write_index] || TSS_OPEN != m_status)
			{
				::InterlockedDecrement((LPLONG)&m_wsa_send);
				return;
			}

			*reinterpret_cast<DWORD*>(m_write_buffer_ptr[m_write_index]) = m_write_wr[m_write_index];
			m_write_rd[m_write_index] = 0;
			m_write_wr[m_write_index] += MSG_HEAD;
		}

		WSABUF wbuf;
		DWORD bytes = 0;

		wbuf.buf = m_write_buffer_ptr[m_write_index] + m_write_rd[m_write_index];
		wbuf.len = m_write_wr[m_write_index] - m_write_rd[m_write_index];

		m_write.status		= IOCS_WRITE;
		m_write.ov.hEvent	= NULL;

		int result = ::WSASend(m_socket, &wbuf, 1, &bytes, 0, &m_write.ov, NULL);
		last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_write(last_error, bytes);
			return;
		}
	}

	nc_session::nclose_helper::~nclose_helper()
	{
		m_session_ptr->_release();
	}

} // namespace nexus

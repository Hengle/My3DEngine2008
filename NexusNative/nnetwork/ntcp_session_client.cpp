#include "ntcp_session_client.h"
#include "ntcp_client_iocp.h"
#include "nguard.h"
#include "nlzo_wrap.h"
#include "nlog_mt.h"

namespace nexus {

	ntcp_session_client::ntcp_session_client() : m_session_id(0), m_client_ptr(NULL), m_socket(0), m_status(TSS_NONE), 
		m_msg_head(0), m_lzo(false), m_recv_buffer_ptr(NULL), m_read_rd(0), m_read_wr(0), m_write_rd(0), m_write_wr(0), m_wsa_send(0), m_wsa_recv(0),
		m_send_msg_ptr(NULL), m_send_head_ptr(NULL), m_send_tail_ptr(NULL), m_cur_send_msg(0), m_max_recv_size(0), m_max_send_msg_size(0), 
		m_max_send_msg(0), m_nagle(false), m_ip_addresss(""), m_port(0), m_time_out_sec(0)
	{
	}

	ntcp_session_client::~ntcp_session_client()
	{
		_free_memory();
		m_session_id		= 0;
		m_client_ptr		= NULL;
		m_socket			= 0;
		m_status			= TSS_NONE;
		m_msg_head			= 0;
		m_lzo				= false;
		m_recv_buffer_ptr	= NULL;
		m_read_rd			= 0;
		m_read_wr			= 0;
		m_write_rd			= 0;
		m_write_wr			= 0;
		m_wsa_send			= 0;
		m_wsa_recv			= 0;
		m_send_msg_ptr		= NULL;
		m_send_head_ptr		= NULL;
		m_send_tail_ptr		= NULL;
		m_cur_send_msg		= 0;
		m_max_recv_size		= 0;
		m_max_send_msg_size	= 0;
		m_max_send_msg		= 0;
		m_nagle				= false;
		m_ip_addresss		= "";
		m_port				= 0;
		m_time_out_sec		= 0;
	}

	uint32 ntcp_session_client::connect_to(const std::string& ip_addresss, uint16 port)
	{
		if (TSS_ACCEPTEX != m_status)
		{
			return 0XFFFFFFFF;
		}

		m_ip_addresss	= ip_addresss;
		m_port			= port;

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
		uint32 time_out = 30;
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
		address.sin_addr.s_addr	= inet_addr(m_ip_addresss.c_str());
		address.sin_family		= AF_INET;
		address.sin_port		= htons(m_port);

		// Ͷ��connectex����
		m_read.status		= IOCS_ACCEPT;
		m_read.ov.hEvent	= NULL;

		DWORD last_error = ::WSAGetLastError();
		BOOL result = fn_connectex(m_socket, (const sockaddr*)&address, sizeof(address), NULL, 0, &bytes, (OVERLAPPED*)&m_read);
		last_error = ::WSAGetLastError();

		if (TRUE != result && WSA_IO_PENDING != last_error)
		{
			_handle_connect(last_error);
			return 0XFFFFFFFF;
		}

		return m_session_id;
	}

	// ������������send����ǰ���ú����е����ü�����send����ֻ�������ü����ļ���
	bool ntcp_session_client::send(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// ������
		if (!msg_ptr)
		{
			return false;
		}

		// �����������ӵ���Ч��,m_session_id��m_status���̷߳���
		if (session_id != m_session_id || 0 == size || m_cur_send_msg > m_max_send_msg || TSS_OPEN != m_status)
		{
			msg_ptr->dec_reference();
			return false;
		}

		{// ��������m_mutex�����ķ�Χ������ݹ�
			// �����
			nguard<nfast_mutex> scoped_lock(m_mutex);

			if (session_id != m_session_id || TSS_OPEN != m_status || m_cur_send_msg > m_max_send_msg)
			{
				msg_ptr->dec_reference();
				return false;
			}

			// ����Ϣ����ǰ�����Ϣ��������
			*reinterpret_cast<WORD*>(msg_ptr->buffer - sizeof(WORD)) = static_cast<WORD>(size);
			msg_ptr->size = size + sizeof(WORD);

			msg_ptr->next_ptr = NULL;
			// ���뵽���Ͷ���
			if (!m_send_tail_ptr)
			{
				m_send_head_ptr = m_send_tail_ptr = msg_ptr;
			}
			else
			{
				m_send_tail_ptr->next_ptr = msg_ptr;
				m_send_tail_ptr = msg_ptr;
			}

			++m_cur_send_msg;
			// �ж��Ƿ�Ͷ��send����, ע�ⲻҪ����m_mutex, m_wsa_send
			if (::InterlockedCompareExchange((LPLONG)&m_wsa_send, 1, 0) != 0)
			{
				return true;
			}

			m_send_msg_ptr = m_send_head_ptr;
			m_send_head_ptr = m_send_head_ptr->next_ptr;

			if (!m_send_head_ptr)
			{
				m_send_tail_ptr= NULL;
			}

			m_write_rd = 0;
			m_write_wr = m_send_msg_ptr->size;
		}

		WSABUF wbuf;
		DWORD bytes = 0;

		wbuf.buf = m_send_msg_ptr->buffer - sizeof(WORD);
		wbuf.len = m_write_wr;

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

	bool ntcp_session_client::_init(uint16 index, ntcp_client_iocp* client_ptr, uint32 max_recv_size, uint32 max_send_msg, uint32 max_send_msg_size, uint32 time_out_sec, bool nagle)
	{
		if (0XFFFF == index || !client_ptr || 0 == max_recv_size || 0 == max_send_msg || 0 == max_send_msg_size)
		{
			NLOG_NERROR(_T("ntcp_session_client init error!, index = %u, max_recv_size = %u, max_send_msg = %u, max_send_msg_size = %u, time_out_sec = %u, nagle = %u"), 
				index, max_recv_size, max_send_msg, max_send_msg_size, time_out_sec, nagle);
			return false;
		}

		*reinterpret_cast<uint16*>(&m_session_id) = index;
		m_client_ptr		= client_ptr;
		m_max_recv_size		= max_recv_size;
		m_max_send_msg		= max_send_msg;
		m_max_send_msg_size	= max_send_msg_size;
		m_time_out_sec		= time_out_sec;
		m_nagle				= nagle;

		// alloc memory
		m_recv_buffer_ptr = new char[m_max_recv_size];

		if (!m_recv_buffer_ptr)
		{
			NLOG_NERROR(_T("alloc recv_buffer memory error!"));
			return false;
		}

		m_status = TSS_INIT;

		return true;
	}

	void ntcp_session_client::_free_memory()
	{
		m_status = TSS_NONE;

		// �ͷ�session new���ڴ����ݣ����ڴ�������������closeʱ���Ѿ��ͷŸ��ڴ��
		if (m_recv_buffer_ptr)
		{
			delete []m_recv_buffer_ptr;
			m_recv_buffer_ptr = NULL;
		}
	}

	void ntcp_session_client::_pre_connect()
	{
		m_socket			= ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		m_read.status		= IOCS_ACCEPT;
		m_read.ov.hEvent	= NULL;

		// pool������closeʱͳһ�ͷ�
		m_msg_head			= 0;
		m_lzo				= false;
		m_read_rd			= 0;
		m_read_wr			= 0;
		m_write_rd			= 0;
		m_write_wr			= 0;

		m_wsa_send			= 0;
		m_wsa_recv			= 0;

		m_send_msg_ptr		= NULL;
		m_send_head_ptr		= NULL;
		m_send_tail_ptr		= NULL;

		m_status = TSS_ACCEPTEX;
	}

	void ntcp_session_client::_release()
	{
		// �����ж�
		if (TSS_SHUT != m_status || 0 != m_wsa_recv && 0 != m_wsa_send)
		{
			return;
		}
		// �ͷŸ�session���ڴ������
		nmessage* msg_ptr = NULL;

		// ��֤pool�ڴ���Ϣ�ͷ�һ��
		{// _release��helper��������������ã�����ݹ�
			nguard<nfast_mutex> guard(m_mutex);

			if (TSS_SHUT != m_status || 0 != m_wsa_recv && 0 != m_wsa_send)
			{
				return;
			}

			while (m_send_head_ptr)
			{
				msg_ptr = m_send_head_ptr;
				m_send_head_ptr = m_send_head_ptr->next_ptr;
				msg_ptr->dec_reference();
			}

			// ����session״̬
			m_status = TSS_CLOSE;
			uint32 session_id = m_session_id;
			// �ı�m_client_id
			++(*(reinterpret_cast<uint16*>(&m_session_id) + 1));

			// Ͷ�ݻص�close call back��������
			m_client_ptr->_post_close_call_back(session_id);
		}
	}

	void ntcp_session_client::_close()
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

	bool ntcp_session_client::parse_msg(char* buffer_ptr, uint32 size, nsafe_mem_pool* pool_ptr)
	{
		uint32 msg_ptr	= 0;
		uint32 msg_size = 0;
		nmessage* nmsg_ptr = NULL;

		while (msg_ptr < size)
		{
			msg_size = static_cast<DWORD>(*reinterpret_cast<WORD*>(buffer_ptr + msg_ptr));

			// ���������Ϣͷ
			if (0 == msg_size || msg_size > m_max_recv_size)
			{
				NLOG_NERROR(_T("parse msg head error, msg_size = %u"), msg_size);
				return false;
			}

			msg_ptr += sizeof(WORD);

			if (msg_ptr + msg_size <= size)
			{
				// ����nmessage
				nmsg_ptr = reinterpret_cast<nmessage*>(pool_ptr->alloc(sizeof(nmessage) + msg_size));

				if (NULL == nmsg_ptr)
				{
					NLOG_NERROR(_T("pool alloc error!"));
					return false;
				}

				nmsg_ptr->pool_ptr		= pool_ptr;
				nmsg_ptr->next_ptr		= NULL;
				nmsg_ptr->reference		= 1;
				nmsg_ptr->size			= msg_size;
				nmsg_ptr->session_id	= m_session_id;
				nmsg_ptr->server_id		= 0;

				// ��������
				memcpy(nmsg_ptr->buffer, buffer_ptr + msg_ptr, msg_size);
				m_client_ptr->on_datarecv(m_session_id, nmsg_ptr, msg_size);
			}
			else
			{
				break;
			}

			msg_ptr += msg_size;
		}

		return true;
	}

	void ntcp_session_client::_handle_connect(DWORD last_error)
	{
		uint32 error_code = ECC_Connected;

		if (0 != last_error)
		{
			switch (last_error)
			{
			case WSAETIMEDOUT:
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
		m_read_rd			= 0;
		m_read_wr			= sizeof(DWORD);

		// Ͷ��recv length����
		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		wbuf.buf = m_recv_buffer_ptr;
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
			_handle_read(last_error, bytes, NULL, NULL, NULL);
			return;
		}
	}

	void ntcp_session_client::_handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr, char* temp_ptr, nlzo_wrap* minilzo_ptr)
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
				m_msg_head = *reinterpret_cast<DWORD*>(m_recv_buffer_ptr);

				m_lzo = ((m_msg_head & 0x00000001) == 1);
				m_msg_head >>= 1;

				m_read_rd = 0;
				m_read_wr = m_msg_head;

				// ���������Ϣͷ
				if (0 == m_msg_head || m_msg_head > m_max_recv_size)
				{
					::InterlockedDecrement((LPLONG)&m_wsa_recv);
					_close();
					return;
				}
			}
			else
			{
				char* recv_buffer_ptr = NULL;

				if (!m_lzo)
				{
					recv_buffer_ptr = m_recv_buffer_ptr;
				}
				else
				{
					uint32 new_size = 0;

					if (!minilzo_ptr)
					{
						::InterlockedDecrement((LPLONG)&m_wsa_recv);
						_close();
						return;
					}

					// ��ѹ
					if (!minilzo_ptr->decompress(m_recv_buffer_ptr, m_msg_head, temp_ptr, new_size))
					{
						::InterlockedDecrement((LPLONG)&m_wsa_recv);
						_close();
						return;
					}

					recv_buffer_ptr = temp_ptr;
					m_msg_head = new_size;
				}
				// ������Ϣ
				if (false == parse_msg(recv_buffer_ptr, m_msg_head, pool_ptr))
				{
					::InterlockedDecrement((LPLONG)&m_wsa_recv);
					_close();
					return;
				}

				m_msg_head = 0;
				m_lzo = false;
				m_read_rd = 0;
				m_read_wr = sizeof(DWORD);
			}
		}

		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		// Ͷ��recv����
		wbuf.buf = m_recv_buffer_ptr + m_read_rd;
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
			_handle_read(last_error, bytes, NULL, NULL, NULL);
			return;
		}
	}

	void ntcp_session_client::_handle_write(DWORD last_error, DWORD bytes_transferred)
	{
		nclose_helper helper(this);

		if (0 != last_error || 0 == bytes_transferred)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_send);
			_close();
			return;
		}

		m_write_rd += bytes_transferred;

		if (m_write_rd == m_write_wr)
		{
			nmessage* msg_ptr = m_send_msg_ptr;

			{
				nguard<nfast_mutex> scoped_lock(m_mutex);
				--m_cur_send_msg;

				if (m_send_head_ptr)
				{
					m_send_msg_ptr = m_send_head_ptr;
					m_send_head_ptr = m_send_head_ptr->next_ptr;

					if (!m_send_head_ptr)
					{
						m_send_tail_ptr = NULL;
					}

					m_write_rd = 0;
					m_write_wr = m_send_msg_ptr->size;
				}
			}

			msg_ptr->dec_reference();
		}

		// �����TSS_OPEN != m_status����close����pool��Դ�����ͳһ��close_helper�ͷ�
		if (m_write_rd == m_write_wr || TSS_OPEN != m_status)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_send);
			return;
		}

		WSABUF wbuf;
		DWORD bytes = 0;

		wbuf.buf = m_send_msg_ptr->buffer - sizeof(WORD) + m_write_rd;
		wbuf.len = m_write_wr - m_write_rd;

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

	ntcp_session_client::nclose_helper::~nclose_helper()
	{
		m_session_ptr->_release();
	}

} // namespace nexus

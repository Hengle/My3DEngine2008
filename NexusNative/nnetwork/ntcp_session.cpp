#include "ntcp_session.h"
#include "ntcp_server.h"
#include "nguard.h"
#include "nlzo_wrap.h"
#include "nlog_mt.h"

namespace nexus {

	ntcp_session::ntcp_session() : m_session_id(0), m_server_ptr(NULL), m_socket(0), m_status(TSS_NONE), m_recv_msg_ptr(NULL), m_read_rd(0), m_read_wr(0), 
		m_write_buffer_ptr(NULL), m_write_rd(0), m_write_wr(0), m_wsa_send(0), m_wsa_recv(0),
		m_send_buffers_ptr(NULL), m_send_head_ptr(NULL), m_send_tail_ptr(NULL), m_empty_head_ptr(NULL), 
		m_max_recv_size(0), m_max_write_size(0), m_send_buffers(0), m_minilzo(false)
	{
	}

	ntcp_session::~ntcp_session()
	{
		_free_memory();
		m_session_id		= 0;
		m_server_ptr		= NULL;
		m_socket			= 0;
		m_status			= TSS_NONE;
		m_recv_msg_ptr		= NULL;
		m_read_rd			= 0;
		m_read_wr			= 0;
		m_write_buffer_ptr	= NULL;
		m_write_rd			= 0;
		m_write_wr			= 0;
		m_wsa_send			= 0;
		m_wsa_recv			= 0;
		m_send_buffers_ptr	= NULL;
		m_send_head_ptr		= NULL;
		m_send_tail_ptr		= NULL;
		m_empty_head_ptr	= NULL;
		m_max_recv_size		= 0;
		m_max_write_size	= 0;
		m_send_buffers		= 0;
		m_minilzo			= false;
	}

	// ������������send����ǰ���ú����е����ü�����send����ֻ�������ü����ļ���
	bool ntcp_session::send(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// ������
		if (!msg_ptr)
		{
			return false;
		}

		// �����������ӵ���Ч��,m_session_id��m_status���̷߳���
		if (session_id != m_session_id || 0 == size || !m_empty_head_ptr || TSS_OPEN != m_status)
		{
			msg_ptr->dec_reference();
			return false;
		}

		{// ��������m_mutex�����ķ�Χ������ݹ�
			// �����
			nguard<nfast_mutex> scoped_lock(m_mutex);

			if (session_id != m_session_id || TSS_OPEN != m_status || !m_empty_head_ptr)
			{
				msg_ptr->dec_reference();
				return false;
			}

			m_empty_head_ptr->buffer_ptr	= msg_ptr->buffer;
			m_empty_head_ptr->size			= size;

			// ���뵽���Ͷ���
			if (!m_send_tail_ptr)
			{
				m_send_head_ptr = m_send_tail_ptr = m_empty_head_ptr;
			}
			else
			{
				m_send_tail_ptr->next_ptr = m_empty_head_ptr;
				m_send_tail_ptr = m_empty_head_ptr;
			}

			m_empty_head_ptr = m_empty_head_ptr->next_ptr;
			// ����������Ϳ���������֮���ǶϿ��ģ������ͨ�õ�
			m_send_tail_ptr->next_ptr = NULL;	

			// �ж��Ƿ�Ͷ��send����, ע�ⲻҪ����m_mutex, m_wsa_send
			if (::InterlockedCompareExchange((LPLONG)&m_wsa_send, 1, 0) != 0)
			{
				return true;
			}
			// ƴ��
			m_write_rd = 0;
			m_write_wr += sizeof(DWORD) + _packeted_buffer(m_write_buffer_ptr, sizeof(DWORD));
			// ��һλ��־�Ƿ�ѹ��, 0��ʾ��ʹ��ѹ����send �����ﲻѹ��, ͳ��ѹ����
			*reinterpret_cast<DWORD*>(m_write_buffer_ptr) = ((m_write_wr - sizeof(DWORD)) << 1);
		}

		WSABUF wbuf;
		DWORD bytes = 0;

		wbuf.buf = m_write_buffer_ptr;
		wbuf.len = m_write_wr;

		m_write.status		= IOCS_WRITE;
		m_write.ov.hEvent	= NULL;

		int result = ::WSASend(m_socket, &wbuf, 1, &bytes, 0, &m_write.ov, NULL);
		DWORD last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_write(last_error, bytes, NULL, NULL);
			return false;
		}

		return true;
	}

	bool ntcp_session::_init(uint16 index, ntcp_server* server_ptr, uint32 max_recv_size, uint32 max_write_size, uint32 send_buffers, bool minilzo)
	{
		if (0XFFFF == index || !server_ptr || 0 == max_recv_size || 0 == max_write_size || 0 == send_buffers)
		{
			NLOG_NERROR(_T("ntcp_session init error!, index = %u, server_ptr = %u, max_recv_size = %u, max_write_size = %u, send_buffers = %u"), 
				index, server_ptr, max_recv_size, max_write_size, send_buffers);
			return false;
		}

		*reinterpret_cast<uint16*>(&m_session_id) = index;
		m_server_ptr			= server_ptr;
		m_max_recv_size			= max_recv_size;
		m_max_write_size		= max_write_size;
		m_send_buffers			= send_buffers;
		m_minilzo				= minilzo;

		// alloc memory
		m_write_buffer_ptr = new char[m_max_write_size];

		if (!m_write_buffer_ptr)
		{
			NLOG_NERROR(_T("alloc write_buffer memory error!"));
			return false;
		}

		m_send_buffers_ptr = new nsend_buffer[m_send_buffers];

		if (!m_send_buffers_ptr)
		{
			NLOG_NERROR(_T("alloc nsend_buffer memory error!"));
			_free_memory();
			return false;
		}

		for (DWORD i = 0; i < m_send_buffers - 1; ++i)
		{
			m_send_buffers_ptr[i].next_ptr = &(m_send_buffers_ptr[i + 1]);
		}

		m_send_buffers_ptr[m_send_buffers - 1].next_ptr = NULL;
		m_empty_head_ptr = &(m_send_buffers_ptr[0]);

		m_status = TSS_INIT;

		return true;
	}

	void ntcp_session::_free_memory()
	{
		m_status = TSS_NONE;

		// �ͷ�session new���ڴ����ݣ����ڴ�������������closeʱ���Ѿ��ͷŸ��ڴ��
		if (m_send_buffers_ptr)
		{
			delete []m_send_buffers_ptr;
			m_send_buffers_ptr = NULL;
		}

		if (m_write_buffer_ptr)
		{
			delete []m_write_buffer_ptr;
			m_write_buffer_ptr = NULL;
		}
	}

	void ntcp_session::_pre_accept()
	{
		m_socket			= ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		m_accept.status		= IOCS_ACCEPT;
		m_accept.ov.hEvent	= NULL;
		m_accept.session_ptr= this;

		// pool������closeʱͳһ�ͷ�
		m_recv_msg_ptr		= NULL;
		m_read_rd			= 0;
		m_read_wr			= 0;
		m_write_rd			= 0;
		m_write_wr			= 0;

		m_wsa_send			= 0;
		m_wsa_recv			= 0;

		m_send_head_ptr		= NULL;
		m_send_tail_ptr		= NULL;

		for (DWORD i = 0; i < m_send_buffers - 1; ++i)
		{
			m_send_buffers_ptr[i].buffer_ptr = NULL;
			m_send_buffers_ptr[i].size = 0;
			m_send_buffers_ptr[i].next_ptr = &(m_send_buffers_ptr[i + 1]);
		}

		m_send_buffers_ptr[m_send_buffers - 1].buffer_ptr = NULL;
		m_send_buffers_ptr[m_send_buffers - 1].size = 0;
		m_send_buffers_ptr[m_send_buffers - 1].next_ptr = NULL;

		m_empty_head_ptr = &(m_send_buffers_ptr[0]);

		m_status = TSS_ACCEPTEX;
	}

	void ntcp_session::_open(HANDLE completion_port, nsafe_mem_pool* pool_ptr, DWORD address, DWORD port)
	{
		// Disable blocking send/recv calls
		u_long arg = 1;
		::ioctlsocket(m_socket, FIONBIO, &arg);
		// Disables the NAGLE algorithm for send coalescing
		BOOL nodelay = TRUE;
		::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));
		// Assigns the socket to his completion port
		::CreateIoCompletionPort((HANDLE)m_socket, completion_port, (ULONG_PTR)this, 0);
		// ����״̬
		m_status = TSS_OPEN;
		// �ڻص�����֮�������ñ�־λ
		::InterlockedIncrement((LPLONG)&m_wsa_recv);
		// �ص�����
		m_server_ptr->on_connect(m_session_id, (uint32)address, (uint32)port);

		// Ͷ�ݶ�����
		m_recv_msg_ptr = reinterpret_cast<nmessage*>(pool_ptr->alloc(RECV_SIZE(m_max_recv_size)));

		if (!m_recv_msg_ptr)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_recv);
			_close();
			NLOG_NERROR(_T("pool alloc error!"));
			return;
		}

		m_recv_msg_ptr->pool_ptr	= pool_ptr;
		m_recv_msg_ptr->next_ptr	= NULL;
		m_recv_msg_ptr->reference	= 1;
		m_recv_msg_ptr->size		= 0;
		m_recv_msg_ptr->session_id	= m_session_id;
		m_recv_msg_ptr->server_id	= 0;
		m_read_rd					= 0;
		m_read_wr					= sizeof(WORD);

		// Ͷ��recv length����
		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		wbuf.buf = m_recv_msg_ptr->buffer;
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

	// ���ݴ��
	DWORD ntcp_session::_packeted_buffer(char* temp_ptr, DWORD off_size)
	{
		DWORD _off_size = off_size;
		WORD head_size = 0;

		nsend_buffer* empty_buffer_ptr = NULL;
		nmessage* temp_msg_ptr = NULL;

		while (m_send_head_ptr)
		{
			head_size = WORD(m_send_head_ptr->size);

			if (_off_size + head_size + sizeof(WORD) > m_max_write_size)
			{
				break;
			}

			memcpy(temp_ptr + _off_size, &head_size, sizeof(WORD));
			_off_size += sizeof(WORD);
			memcpy(temp_ptr + _off_size, m_send_head_ptr->buffer_ptr, m_send_head_ptr->size);
			_off_size += m_send_head_ptr->size;

			// �޸�buffer�����ü������ͷ���Դ
			temp_msg_ptr = NC2MSG(m_send_head_ptr->buffer_ptr);
			temp_msg_ptr->dec_reference();

			empty_buffer_ptr = m_send_head_ptr;
			m_send_head_ptr = m_send_head_ptr->next_ptr;

			// �黹��buffer
			empty_buffer_ptr->buffer_ptr = NULL;
			empty_buffer_ptr->size = 0;
			empty_buffer_ptr->next_ptr = m_empty_head_ptr;
			m_empty_head_ptr = empty_buffer_ptr;
		}

		// ���������ݿ���û����ȫ������buffer��
		if (!m_send_head_ptr)
		{
			m_send_tail_ptr = m_send_head_ptr;
		}

		return _off_size - off_size;
	}

	void ntcp_session::_release()
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
				// �޸�buffer�����ü��㣬�ͷ���Դ, û�����������ݶ��к�û�����ݶ���_pre_accept��ͳһ����
				msg_ptr = NC2MSG(m_send_head_ptr->buffer_ptr);
				msg_ptr->dec_reference();
				m_send_head_ptr = m_send_head_ptr->next_ptr;
			}

			if (NULL != m_recv_msg_ptr)
			{
				m_recv_msg_ptr->pool_ptr->free(m_recv_msg_ptr);
			}

			// ����session״̬
			m_status = TSS_CLOSE;
			uint32 session_id = m_session_id;
			// �ı�m_client_id
			++(*(reinterpret_cast<uint16*>(&m_session_id) + 1));

			// Ͷ�ݻص�close call back��������
			m_server_ptr->_post_close_call_back(session_id);
		}
	}

	void ntcp_session::_close()
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

	void ntcp_session::_handle_read(DWORD last_error, DWORD bytes_transferred, nsafe_mem_pool* pool_ptr)
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
			if (0 == m_recv_msg_ptr->size)
			{
				m_recv_msg_ptr->size = static_cast<DWORD>(*reinterpret_cast<WORD*>(m_recv_msg_ptr->buffer));
				m_read_rd = 0;
				m_read_wr = m_recv_msg_ptr->size;

				// ���������Ϣͷ
				if (0 == m_recv_msg_ptr->size || m_recv_msg_ptr->size > m_max_recv_size)
				{
					::InterlockedDecrement((LPLONG)&m_wsa_recv);
					_close();
					return;
				}
			}
			else
			{
				// $$$ �ص���Ϣ����, ע�����ʱ����֤��Ϣ��ô����ԣ���wsarecv֮�����
				m_server_ptr->on_datarecv(m_session_id, m_recv_msg_ptr, m_recv_msg_ptr->size);

				// ���»����Ϣ
				m_recv_msg_ptr = reinterpret_cast<nmessage*>(pool_ptr->alloc(RECV_SIZE(m_max_recv_size)));

				if (!m_recv_msg_ptr)
				{
					::InterlockedDecrement((LPLONG)&m_wsa_recv);
					_close();
					NLOG_NERROR(_T("pool alloc error!"));
					return;
				}

				m_recv_msg_ptr->pool_ptr	= pool_ptr;
				m_recv_msg_ptr->next_ptr	= NULL;
				m_recv_msg_ptr->reference	= 1;
				m_recv_msg_ptr->size		= 0;
				m_recv_msg_ptr->session_id	= m_session_id;
				m_recv_msg_ptr->server_id	= 0;
				m_read_rd					= 0;
				m_read_wr					= sizeof(WORD);
			}
		}

		WSABUF wbuf;
		DWORD flags = 0;
		DWORD bytes = 0;

		// Ͷ��recv length����
		wbuf.buf = m_recv_msg_ptr->buffer + m_read_rd;
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

	void ntcp_session::_handle_write(DWORD last_error, DWORD bytes_transferred, char* temp_ptr, nlzo_wrap* minilzo_ptr)
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
			m_write_rd = 0;
			m_write_wr = 0;
		}
		else
		{
			// ��������ᷢ��(������), ���ϸô�����Ӱ�ȫ
			uint32 const len = m_write_wr - m_write_rd;
			::memmove(m_write_buffer_ptr, m_write_buffer_ptr + m_write_rd, len);
			m_write_rd = 0;
			m_write_wr = len;
		}

		DWORD temp_size = 0;

		{// ���ű���m_mutex���ʱ���ݹ飬helper��������ܷ���m_mutex
			nguard<nfast_mutex> scoped_lock(m_mutex);
			temp_size = _packeted_buffer(temp_ptr, 0);					// ƴ��
		}

		// �����TSS_OPEN != m_status����close����pool��Դ�����ͳһ��close_helper�ͷ�
		if (0 == temp_size || TSS_OPEN != m_status)
		{
			::InterlockedDecrement((LPLONG)&m_wsa_send);
			return;
		}

		// �ж��Ƿ����ѹ��
		DWORD head = 0;

		if (!m_minilzo || temp_size <= 100 || !minilzo_ptr)
		{
		// ���һλ��־�Ƿ�ѹ��, 1��ʾʹ��ѹ��, 0��ʾ��ʹ��ѹ��
			head = (temp_size << 1);

		memcpy(m_write_buffer_ptr + m_write_wr, &head, sizeof(DWORD));
		m_write_wr += sizeof(DWORD);
			memcpy(m_write_buffer_ptr + m_write_wr, temp_ptr, temp_size);
			m_write_wr += temp_size;
		}
		else
		{
			// ���ѹ������buff�Ĵ�С, !!!���������Ʒ��͸��ͻ��˵�ԭʼ���ݲ�Ҫ�����ͻ��˵Ļ�����MAX_RECV_SIZE
			if (!nlzo_wrap::check_out_buff(temp_size, m_max_write_size))
			{
				::InterlockedDecrement((LPLONG)&m_wsa_send);
				_close();
				return;
			}

			uint32 new_size = 0;
			// ѹ��
			if (!minilzo_ptr->compress(temp_ptr, temp_size, m_write_buffer_ptr + m_write_wr + sizeof(DWORD), new_size))
			{
				::InterlockedDecrement((LPLONG)&m_wsa_send);
				_close();
				return;
			}
			// ���һλ��־�Ƿ�ѹ��, 1��ʾʹ��ѹ��, 0��ʾ��ʹ��ѹ��
			head = (new_size << 1) + 1;

			memcpy(m_write_buffer_ptr + m_write_wr, &head, sizeof(DWORD));
			m_write_wr += sizeof(DWORD) + new_size;
		}

		WSABUF wbuf;
		DWORD bytes = 0;

		wbuf.buf = m_write_buffer_ptr;
		wbuf.len = m_write_wr;

		m_write.status		= IOCS_WRITE;
		m_write.ov.hEvent	= NULL;

		int result = ::WSASend(m_socket, &wbuf, 1, &bytes, 0, &m_write.ov, NULL);
		last_error = ::WSAGetLastError();

		if (0 != result && WSA_IO_PENDING != last_error)
		{
			_handle_write(last_error, bytes, NULL, NULL);
			return;
		}
	}

	ntcp_session::nclose_helper::~nclose_helper()
	{
		m_session_ptr->_release();
	}

} // namespace nexus

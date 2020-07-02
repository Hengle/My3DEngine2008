#include "nc_server.h"
#include "nc_session.h"
#include "nwinsock_init.h"
#include "nlog_mt.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace nexus {

	nc_server::nc_server() : m_sock_listen(0), m_completion_port(0), m_status(TSVRS_NONE), m_fn_acceptex(NULL), m_fn_get_acceptex_sock_addr(NULL), 
		m_acceptexs(0), m_sessions_ptr(NULL), m_max_acceptex(0), m_port(0), m_max_session(0), m_reuse_port(false), m_nagle(false), m_pools_ptr(NULL)
	{
	}

	nc_server::~nc_server()
	{
		_free_memory();
	}

	bool nc_server::startup(uint16 port, uint32 max_session, uint32 thread_num, uint32 max_buffer_size, bool reuse_port, bool nagle)
	{
		if (0 == max_session || 0 == thread_num || 0 == max_buffer_size)
		{
			NLOG_NERROR(_T("startup param error!, max_session = %u, thread_num = %u, max_buffer_size = %u"), max_session, thread_num, max_buffer_size);
			return false;
		}

		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_NONE != m_status)
			{
				return false;
			}

			m_status = TSVRS_INIT;
		}

		m_port			= port;						// �˿�, �����е�ַ
		m_max_session	= max_session;				// �������������������
		m_max_acceptex	= thread_num;				// �����߳���Ŀ
		m_reuse_port	= reuse_port;				// �Ƿ����ö˿�
		m_nagle			= nagle;					// �Ƿ�ʹ��Nagle�㷨

		m_sessions_ptr = new nc_session[m_max_session];

		if (!m_sessions_ptr)
		{
			NLOG_NERROR(_T("alloc ntcp_session memory error!"));
			return false;
		}

		for (uint32 i = 0; i < m_max_session; ++i)
		{
			if (m_sessions_ptr[i]._init(uint16(i), this, NULL, max_buffer_size, m_nagle))
			{
				_return_session(&(m_sessions_ptr[i]));
			}
		}

		m_pools_ptr = new nsafe_mem_pool[m_max_acceptex];

		if (!m_pools_ptr)
		{
			NLOG_NERROR(_T("alloc nsafe_mem_pool memory error!"));
			_free_memory();
			return false;
		}

		// ����socket
		m_sock_listen = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

		// reuse��ɶ�ΰ󶨳ɹ��������Ҳ�����ȷ���õĶ˿�
		// �����̨����������ͬһ̨������ʹ��ͬһ���˿ڣ���ɻ���
		if (m_reuse_port)
		{
			uint32 option = 1;
			::setsockopt(m_sock_listen, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));
		}

		// disables the NAGLE algorithm for send coalescing
		if (!m_nagle)
		{
			uint32 arg = 1;
			::setsockopt(m_sock_listen, IPPROTO_TCP, TCP_NODELAY, (char*)&arg, sizeof(arg));
		}

		sockaddr_in address;
		ZeroMemory(&address, sizeof(address));
		address.sin_family      = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port        = htons(m_port);

		int result = ::bind(m_sock_listen, (sockaddr*)&address, sizeof(address));

		if (0 != result)
		{
			NLOG_NERROR(_T("bind error, error_code = %u"), ::WSAGetLastError());
			_free_memory();
			return false;
		}

		m_completion_port = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)0, 0);

		result = ::listen(m_sock_listen, SOMAXCONN);

		if (0 != result)
		{
			NLOG_NERROR(_T("listen error, error_code = %u, port = %u"), ::WSAGetLastError(), m_port);
			_free_memory();
			return false;
		}

		::CreateIoCompletionPort((HANDLE)m_sock_listen, m_completion_port, (ULONG_PTR)0, 0);

		// ���AcceptEx\GetAcceptExSockAddrs�ĺ���ָ�룬ֱ�ӵ��ø��ӿ��
		GUID	guid = WSAID_ACCEPTEX;
		GUID	guid2 = WSAID_GETACCEPTEXSOCKADDRS;
		DWORD	bytes = 0;

		::WSAIoctl(m_sock_listen, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid,
			sizeof(GUID), &m_fn_acceptex, sizeof(m_fn_acceptex), &bytes, 0, 0);
		::WSAIoctl(m_sock_listen, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid2,
			sizeof(GUID), &m_fn_get_acceptex_sock_addr, sizeof(m_fn_get_acceptex_sock_addr), &bytes, 0, 0);

		// ������ɰ������߳�
		for (int32 i = 0; i < m_max_acceptex; ++i)
		{
			m_threads.create_thread(boost::bind(&nc_server::_worker_thread, this, i));
		}

		// open״̬
		m_status = TSVRS_OPEN;

		// Ͷ���첽AcceptEx����
		for (int32 i = 0; i < m_max_acceptex; ++i)
		{
			_async_accept();
		}

		return true;
	}

	void nc_server::shutdown()
	{
		{
			nguard<nfast_mutex> guard(m_mutex);
	
			if (TSVRS_OPEN != m_status)
			{
				return;
			}

			// �ر�����
			m_status = TSVRS_SHUT;
		}

		for (uint32 i = 0; i < m_max_session; ++i)
		{
			m_sessions_ptr[i]._close();
		}

		::shutdown(m_sock_listen, SD_BOTH);
		::closesocket(m_sock_listen);
		m_sock_listen = 0;

		bool exit = true;
		TCPSessionStatus status = TSS_NONE;

		while (true)
		{
			exit = true;

			for (uint32 i = 0; i < m_max_session; ++i)
			{
				status = m_sessions_ptr[i]._get_status();

				// TSS_ACCEPTEX�ȴ���ΪTSS_CLOSE��TSS_OPEN��TSS_OPEN����Ͷ��close����TSS_SHUT�ȴ���ΪTSS_CLOSE
				if (TSS_ACCEPTEX == status || TSS_OPEN == status || TSS_SHUT == status)
				{
					exit = false;

					if (TSS_OPEN == status)
					{
						m_sessions_ptr[i]._close();			// ��һ��Ͷ�ݵ�close����ʧ�ܣ�����Ͷ��
					}
				}
			}

			::Sleep(100);

			if (exit)
			{
				break;
			}
		}
		// �رչ����߳�
		::PostQueuedCompletionStatus(m_completion_port, 0, (ULONG_PTR)0XFFFFFFFF, 0);

		// �ȴ������̵߳��˳�
		m_threads.join_all();

		if (m_completion_port)
		{ 
			::CloseHandle(m_completion_port);
			m_completion_port = 0;
		}

		m_status = TSVRS_CLOSE;
		_free_memory();
	}

	void nc_server::_free_memory()
	{
		// ע���ڴ���ͷ�˳��, session���õ����ڴ�������ڹ黹��ʱ�ͷ�
		if (m_sessions_ptr)
		{
			delete []m_sessions_ptr;
			m_sessions_ptr = NULL;
		}

		if (m_pools_ptr)
		{
			delete []m_pools_ptr;
			m_pools_ptr = NULL;
		}

		if (m_sock_listen)
		{
			::closesocket(m_sock_listen);
			m_sock_listen = 0;
		}
	}

	void nc_server::_worker_thread(int32 thread_index)
	{
		BOOL			ok = 0;
		DWORD			bytes_transferred = 0;
		DWORD			last_error = 0;
		LPOVERLAPPED	overlapped_ptr = NULL;
		noverlappedex*	ov_ptr = NULL;
		noverlappedex2*	ov2_ptr = NULL;
		nsafe_mem_pool*	pool_ptr = &(m_pools_ptr[thread_index]);

#if defined(WINVER) && (WINVER < 0x0500)
		DWORD			completion_key = 0;
#else
		DWORD_PTR		completion_key = 0;
#endif

		for (;;)
		{
			::WSASetLastError(0);
			ok = ::GetQueuedCompletionStatus(m_completion_port, &bytes_transferred, &completion_key, &overlapped_ptr, 500);
			last_error = ::WSAGetLastError();

			if (!ok && NULL == overlapped_ptr)
			{
				if (WAIT_TIMEOUT == last_error)
				{
					continue;
				}
				else
				{
					break;
				}
			}
			else if (NULL != overlapped_ptr)
			{
				ov_ptr = CONTAINING_RECORD(overlapped_ptr, noverlappedex, ov);

				switch (ov_ptr->status)
				{
				case IOCS_ACCEPT:
					{
						ov2_ptr = CONTAINING_RECORD(overlapped_ptr, noverlappedex2, ov);
						_handle_accept((nc_session*)(ov2_ptr->session_ptr), last_error);
					}
					break;
				case IOCS_READ:
					{
						reinterpret_cast<nc_session*>(completion_key)->_handle_read(last_error, bytes_transferred, pool_ptr);
					}
					break;
				case IOCS_WRITE:
					{
						reinterpret_cast<nc_session*>(completion_key)->_handle_write(last_error, bytes_transferred);
					}
					break;
				default:
					{
					}
					break;
				}
			}
			else if (0XFFFFFFFF == completion_key)
			{
				// Wake up next thread that is blocked on GetQueuedCompletionStatus.
				::PostQueuedCompletionStatus(m_completion_port, 0, (ULONG_PTR)0XFFFFFFFF, 0);
				break;
			}
			else
			{
				// close�Ļص�����,��֤���еĻص������������̶߳�Ϊiocp�Ĺ����̣߳������ص�����û�еݹ����
				_handle_close(uint32(completion_key));
			}
		}
	}

	void nc_server::_async_accept()
	{
		if (TSVRS_OPEN != m_status || m_acceptexs >= m_max_acceptex)
		{
			return;
		}

		nc_session* session_ptr = _get_available_session();

		if (!session_ptr)
		{
			return;
		}

		BOOL result = FALSE;
		DWORD bytes = 0;
		DWORD last_error = 0;

		// �޸�acceptex����
		::InterlockedIncrement((LPLONG)&m_acceptexs);

		result = m_fn_acceptex(m_sock_listen, session_ptr->m_socket, session_ptr->m_recv_msg_ptr, 0, 
			sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &bytes, (OVERLAPPED*)&(session_ptr->m_operator));
		last_error = ::WSAGetLastError();

		// Check if the operation completed immediately.
		if (!result && ERROR_IO_PENDING != last_error)
		{
			_handle_accept(session_ptr, last_error);
			return;
		}
	}

	void nc_server::_handle_accept(nc_session* session_ptr, DWORD last_error)
	{
		// �޸�acceptex����
		::InterlockedDecrement((LPLONG)&m_acceptexs);

		if (0 != last_error || TSVRS_OPEN != m_status)
		{
			session_ptr->_set_status(TSS_CLOSE);
			_return_session(session_ptr);
			_async_accept();
			return;
		}

		// Ͷ���µ�accpetex����
		_async_accept();

		BOOL dont_linger = TRUE;
		sockaddr_in* remote_addr_ptr = NULL;
		sockaddr_in* local_addr_ptr = NULL;
		DWORD remote_addr_size = 0;
		DWORD local_addr_size = 0;

		::setsockopt(session_ptr->m_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_sock_listen, sizeof(m_sock_listen));
		::setsockopt(session_ptr->m_socket, SOL_SOCKET, SO_DONTLINGER, (char*)&dont_linger, sizeof(dont_linger));

		m_fn_get_acceptex_sock_addr(session_ptr->m_recv_msg_ptr, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, 
			(sockaddr**)&local_addr_ptr, (LPINT)&local_addr_size, (sockaddr**)&remote_addr_ptr, (LPINT)&remote_addr_size);

		session_ptr->_open(m_completion_port, remote_addr_ptr->sin_addr.S_un.S_addr, remote_addr_ptr->sin_port);
	}

} // namespace nexus

#include "ncluster_server.h"
#include "ncluster_session.h"
#include "nwinsock_init.h"
#include "nlog_mt.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace nexus {

	ncluster_server::ncluster_server() : m_status(TSVRS_NONE), m_sock_listen(0), m_sessions_ptr(NULL), m_terminate_accept(0), m_terminate_close(0), 
		m_port(0), m_max_session(0), m_reuse_port(false), m_nagle(false)
	{
		ZeroMemory(&m_address, sizeof(m_address));
	}

	ncluster_server::~ncluster_server()
	{
		_free_memory();
	}

	bool ncluster_server::startup(uint16 port, uint32 max_session, uint32 max_send_buffer, bool reuse_port, bool nagle)
	{
		if (0 == max_session || 0 == max_send_buffer)
		{
			NLOG_NERROR(_T("startup param error!, port = %u, max_session = %u, max_send_buffer = %u, reuse_port = %u, nagle = %u"), 
				port, max_session, max_send_buffer, reuse_port, nagle);
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

		m_port			= port;						// �˿�
		m_max_session	= max_session;				// ���������
		m_reuse_port	= reuse_port;				// �Ƿ����ö˿�
		m_nagle			= nagle;					// �Ƿ���nagle�㷨

		m_sessions_ptr = new ncluster_session[m_max_session];

		if (!m_sessions_ptr)
		{
			NLOG_NERROR(_T("alloc ncluster_session memory error!"));
			return false;
		}

		for (uint32 i = 0; i < m_max_session; ++i)
		{
			if (m_sessions_ptr[i]._init(uint16(i), this, NULL, max_send_buffer))
			{
				_return_session(&(m_sessions_ptr[i]));
			}
		}

		// ����socket
		m_sock_listen = ::socket(AF_INET, SOCK_STREAM, 0);

		// ʹ�÷�������ʽ
		u_long arg = 1;
		int result = ::ioctlsocket(m_sock_listen, FIONBIO, &arg);

		if (SOCKET_ERROR == result)
		{
			NLOG_NERROR(_T("set socket option error, error_code = %u"), ::WSAGetLastError());
			_free_memory();
			return false;
		}

		// ��ر�
		linger lin;
		lin.l_onoff		= 1;
		lin.l_linger	= 0;
		::setsockopt(m_sock_listen, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin));

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
			arg = 1;
			::setsockopt(m_sock_listen, IPPROTO_TCP, TCP_NODELAY, (char*)&arg, sizeof(arg));
		}

		ZeroMemory(&m_address, sizeof(m_address));
		m_address.sin_addr.s_addr	= htonl(INADDR_ANY);
		m_address.sin_family		= AF_INET;
		m_address.sin_port			= htons(m_port);

		result = ::bind(m_sock_listen, (sockaddr*)&m_address, sizeof(m_address));

		if (0 != result)
		{
			NLOG_NERROR(_T("bind error, error_code = %u"), ::WSAGetLastError());
			_free_memory();
			return false;
		}

		result = ::listen(m_sock_listen, 5);

		if (0 != result)
		{
			NLOG_NERROR(_T("listen error, error_code = %u"), ::WSAGetLastError());
			_free_memory();
			return false;
		}

		// ����accpet�߳�
		m_threads.create_thread(boost::bind(&ncluster_server::_thread_accept, this));

		// �������ӹر��߳�
		m_threads.create_thread(boost::bind(&ncluster_server::_thread_close, this));

		// open״̬
		m_status = TSVRS_OPEN;

		return true;
	}

	void ncluster_server::shutdown()
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
			m_sessions_ptr[i]._closing();
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
						m_sessions_ptr[i]._closing();			// ��һ��Ͷ�ݵ�close����ʧ�ܣ�����Ͷ��
					}
				}
			}

			::Sleep(100);

			if (exit)
			{
				break;
			}
		}
		// �ر��߳�
		::InterlockedExchange((LONG*)&m_terminate_accept, 1);
		::InterlockedExchange((LONG*)&m_terminate_close, 1);

		// �ȴ������̵߳��˳�
		m_threads.join_all();

		m_status = TSVRS_CLOSE;
		_free_memory();
	}

	void ncluster_server::_free_memory()
	{
		if (m_sessions_ptr)
		{
			delete []m_sessions_ptr;
			m_sessions_ptr = NULL;
		}

		if (m_sock_listen)
		{
			::closesocket(m_sock_listen);
			m_sock_listen = 0;
		}
	}

	void ncluster_server::_thread_accept()
	{
		fd_set fdread;
		timeval block_time;
		block_time.tv_sec	= 0;
		block_time.tv_usec	= 200 * 1000;			// 200 ms

		ncluster_session* session_ptr = NULL;
		int add_len = sizeof(m_address);

		while (0 == m_terminate_accept)
		{
			// "select" will change fdread, so this must put in the loop
			FD_ZERO(&fdread);
			FD_SET(m_sock_listen, &fdread);

			// ��������
			if (1 == select(0, &fdread, NULL, NULL, &block_time))
			{
				if (m_available_sessions.empty())
				{
					::Sleep(200);
					continue;
				}

				session_ptr = _get_available_session();

				if (!session_ptr)
				{
					::Sleep(200);
					continue;
				}

				session_ptr->m_socket = ::accept(m_sock_listen, (sockaddr*)&(m_address), &add_len);

				if (INVALID_SOCKET ==  session_ptr->m_socket)
				{	
					session_ptr->_set_status(TSS_CLOSE);
					_return_session(session_ptr);
					continue;
				}

				session_ptr->_open();
			}
		}
	}

	void ncluster_server::_thread_close()
	{
		ncluster_session* session_ptr = NULL;

		while (0 == m_terminate_close || !m_closing_sessions.empty())
		{
			if (m_closing_sessions.empty())
			{
				::Sleep(200);
				continue;
			}

			session_ptr = _get_closing_session();

			if (!session_ptr)
			{
				::Sleep(200);
				continue;
			}

			session_ptr->_close();
		}
	}

} // namespace nexus

#include "ntcp_client.h"
#include "nguard.h"
#include "nwinsock_init.h"
#include "nlzo_wrap.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

/**
	in http://tangentsoft.net/wskfaq/newbie.html#howclose 

	The proper sequence for closing a TCP connection is:
	1. Finish sending data. 
	2. Call shutdown() with the how parameter set to 1. 
	3. Loop on recv() until it returns 0. 
	4. Call closesocket(). 

	in MSDN:
	To assure that all data is sent and received on a connected socket 
	before it is closed, an application should use shutdown to close connection 
	before calling closesocket. For example, to initiate a graceful disconnect:
	1. Call WSAAsyncSelect to register for FD_CLOSE notification. 
	2. Call shutdown with how=SD_SEND. 
	3. When FD_CLOSE received, call recv until zero returned, or SOCKET_ERROR. 
	4. Call closesocket. 
*/

namespace nexus {

	const int MAX_RECV_SIZE		= 48 * 1024;	// 48K
	const int MAX_SEND_SIZE		= 8 * 1024;		// 8K * 2
	const int NET_BLOCK_TIME	= 150 * 1000;	// 150 ms
	const int MAX_SEND_MSG_SIZE	= 512;			// 512�ֽ�

	ntcp_client::ntcp_client() : m_status(ECS_NONE), m_socket(0), m_ip_addresss(""), m_port(0), m_time_out(0), m_nagle(false), m_send_index(0), m_recv_buffer_ptr(NULL),
		m_terminate(0), m_terminate_workder(0), m_connecting(0), m_closing(0), m_send_event(NULL)
	{
		m_send_buffer_ptr[0] = NULL;
		m_send_buffer_ptr[1] = NULL;

		m_write_wr[0]	= 0;
		m_write_wr[1]	= 0;
	}

	ntcp_client::~ntcp_client()
	{
		_free_memory();
	}

	bool ntcp_client::startup(uint32 time_out_sec, bool nagle)
	{
		{
			nguard<nmutex> scoped_lock(m_send_mutex);

			if (ECS_NONE != m_status)
			{
				return false;
			}

			m_status = ECS_INIT;
		}

		m_time_out		= time_out_sec;
		m_nagle			= nagle;

		// �Զ�����
		m_send_event = ::CreateEvent(NULL, FALSE, TRUE, NULL);

		// ���뻺������Դ
		m_send_buffer_ptr[0] = new char[MAX_SEND_SIZE];

		if (!m_send_buffer_ptr[0])
		{
			// log
			return false;
		}

		m_send_buffer_ptr[1] = new char[MAX_SEND_SIZE];

		if (!m_send_buffer_ptr[1])
		{
			// log
			_free_memory();
			return false;
		}

		m_recv_buffer_ptr = new char[MAX_RECV_SIZE];

		if (!m_recv_buffer_ptr)
		{
			// log
			_free_memory();
			return false;
		}

		// ���������߳�
		m_thread_worker.create_thread(boost::bind(&ntcp_client::_thread_worker, this));

		m_status = ECS_OPEN;
		return true;
	}

	void ntcp_client::shutdown()
	{
		if(m_status == ECS_NONE)
		{
			return; // û�п���������²���Ҫ�ر�
		}

		{
			nguard<nmutex> scoped_lock(m_send_mutex);

			if (0 != m_terminate_workder)
			{
				return;
			}

			close();
			::InterlockedExchange((LONG*)&m_terminate_workder, 1);
		}

		while (true)
		{
			if (ECS_CLOSE == m_status)
			{
				break;
			}
			else
			{
				::Sleep(200);
			}
		}

		// �ȴ��߳��˳�
		m_thread_worker.join_all();

		// �ͷ���Դ
		_free_memory();

		m_status = ECS_NONE;
	}

	bool ntcp_client::connect_to(const std::string& ip_addresss, uint16 port)
	{
		nguard<nmutex> scoped_lock(m_send_mutex);

		if (0 != m_connecting || ECS_OPEN != m_status || 1 == m_terminate_workder)
		{
			return false;
		}

		m_ip_addresss	= ip_addresss;
		m_port			= port;

		// ����socket
		m_socket = ::socket(AF_INET, SOCK_STREAM, 0);

		if (INVALID_SOCKET == m_socket)
		{
			return false;
		}

		// ʹ�÷�������ʽ, linger��Ϊ
		DWORD arg = 1;
		int result = ::ioctlsocket(m_socket, FIONBIO, &arg);

		if (SOCKET_ERROR == result)
		{
			::closesocket(m_socket);
			m_socket = 0;
			return false;
		}

		// disables the Nagle algorithm for send coalescing
		if (!m_nagle)
		{
			BOOL nodelay = TRUE;
			::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));
		}

		// set addr reusable
		DWORD reuse_addr = TRUE;
		::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr, sizeof(reuse_addr));

		// hard shut down
		linger lin;
		lin.l_linger	= 0;
		lin.l_onoff		= 1;
		::setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin));

		m_connecting = 1;

		return true;
	}

	// �첽�ر�����
	void ntcp_client::close()
	{
		m_closing = 1;
	}

	bool ntcp_client::send(const void* msg_ptr, uint32 size)
	{
		// �ж������Ƿ����ɹ�����Ϣ��С�ĺϷ���
		if (ECS_CONNECTED != m_status || 0 == size || size > MAX_SEND_MSG_SIZE)
		{
			return false;
		}

		DWORD index = 0;
		// ��û�����
		nguard<nmutex> scoped_lock(m_send_mutex);

		if (ECS_CONNECTED != m_status)
		{
			return false;
		}

		index = (m_send_index + 1) % 2;

		if (m_write_wr[index] + size >= MAX_SEND_SIZE || !m_send_buffer_ptr[index])
		{
			return false;
		}

		// �������ݣ�1. ��Ϣ����
		*reinterpret_cast<WORD*>(m_send_buffer_ptr[index] + m_write_wr[index]) = WORD(size);
		m_write_wr[index] += sizeof(WORD);
		// �������ݣ�2. ��Ϣ����
		memcpy(m_send_buffer_ptr[index] + m_write_wr[index], msg_ptr, size);
		m_write_wr[index] += size;

		// ����send�¼�
		::SetEvent(m_send_event);
		return true;
	}

	void ntcp_client::_free_memory()
	{
		// �ͷŻ�������Դ
		if (m_send_buffer_ptr[0])
		{
			delete []m_send_buffer_ptr[0];
			m_send_buffer_ptr[0] = NULL;
		}

		if (m_send_buffer_ptr[1])
		{
			delete []m_send_buffer_ptr[1];
			m_send_buffer_ptr[1] = NULL;
		}

		if (m_recv_buffer_ptr)
		{
			delete []m_recv_buffer_ptr;
			m_recv_buffer_ptr = NULL;
		}

		// �ر�handle
		if (m_send_event)
		{
			::CloseHandle(m_send_event);
			m_send_event = NULL;
		}
	}

	// �����ر�socket
	bool ntcp_client::_graceful_close_socket(SOCKET& socket, int retry_times)
	{
		if (0 == socket)
		{
			return true;
		}

		::shutdown(socket, SD_SEND);

		int retry = 0;
		fd_set fdread;
		timeval block_time;							// ��������ʱ��
		block_time.tv_sec	= 0;
		block_time.tv_usec	= NET_BLOCK_TIME;		// 150 ms 

		while (retry++ < retry_times)
		{
			FD_ZERO(&fdread);
			FD_SET(socket, &fdread);

			if (1 == select(0, &fdread, NULL, NULL, &block_time))
			{
				CHAR temp;
				int result = ::recv(socket, &temp, 1, 0);

				// receive FIN, ok
				if (0 == result)
				{
					::closesocket(socket);
					socket = 0;
					return true;
				}

				// network down
				if (SOCKET_ERROR == result)
				{
					break;
				}
			}
		}

		::closesocket(socket);
		// a hard shutdown
		socket = 0;
		return false;
	}

	// �����߳�
	void ntcp_client::_thread_worker()
	{
		while (0 == m_terminate_workder || 1 == m_connecting || 1 == m_closing)
		{
			if (1 == m_connecting)
			{
				_handle_connect();
				m_connecting = 0;		// connect���������(��ȷ&����)���ڻص���������Ͷ��connect����᲻�ɹ�
			}

			if (1 == m_closing)
			{
				_handle_close();
				m_closing = 0;			// close���������(��ȷ&����)���ڻص���������Ͷ��close����᲻�ɹ�
			}

			::Sleep(200);
		}

		m_status = ECS_CLOSE;
	}

	// ���ݷ����߳�
	void ntcp_client::_thread_send()
	{
		fd_set fdwrite;
		timeval block_time;							// ���÷�������ʱ��
		block_time.tv_sec	= 0;
		block_time.tv_usec	= NET_BLOCK_TIME;		// 150 ms

		char*	msg_ptr = NULL;
		DWORD	msg_size = 0;
		int		result = 0;

		while (0 == m_terminate)
		{
			for (;;)								// ע���ڲ���break
			{
				// ���Ҫ���͵����ݻ�����
				{
					nguard<nmutex> scoped_lock(m_send_mutex);
					msg_ptr		= m_send_buffer_ptr[m_send_index];
					msg_size	= m_write_wr[m_send_index];

					if (0 == msg_size)
					{
						m_send_index = (m_send_index + 1) % 2;
						msg_ptr		 = m_send_buffer_ptr[m_send_index];
						msg_size	 = m_write_wr[m_send_index];
					}

					m_write_wr[m_send_index] = 0;
				}

				if (0 == msg_size)
				{
					break;
				}

				// send ��ʱ����һ�ν����ݷ���
				while (0 < msg_size && 0 == m_terminate)
				{
					result = ::send(m_socket, msg_ptr, msg_size, 0);

					if (SOCKET_ERROR == result)
					{
						DWORD last_error = WSAGetLastError();

						switch (last_error)
						{
						case WSAEWOULDBLOCK:
						case WSAENOBUFS:
							{
								// no buffer space is available within the transport 
								// system to hold the data to be transmitted
								// �ȴ����ݿ��Է��͵�ʱ��
								FD_ZERO(&fdwrite);
								FD_SET(m_socket, &fdwrite);
								select(0, NULL, &fdwrite, NULL, &block_time);
							}
							break;
						default:
							{
								// ����
								close();
								return;
							}
							break;
						}
					}
					else
					{
						// ���ݷ��ͳɹ������㻺����ƫ��
						msg_ptr += result;
						msg_size -= result;
					}
				}

				{
					nguard<nmutex> scoped_lock(m_send_mutex);
					m_send_index = (m_send_index + 1) % 2;
				}
			}

			::WaitForSingleObject(m_send_event, 120);
		}
	}

	// ���ݽ����߳�
	void ntcp_client::_thread_recv()
	{
		fd_set fdread;
		timeval block_time;								// ��������ʱ��
		block_time.tv_sec	= 0;
		block_time.tv_usec	= NET_BLOCK_TIME;			// 150 ms

		char*	msg_ptr = NULL;
		DWORD	msg_size = 0;
		DWORD	temp_size = 0;
		int		result = 0;
		bool	lzo = false;
		char*	buffer = new char[MAX_RECV_SIZE + 1];

		if (!buffer)
		{
			return;
		}

		nlzo_wrap minilzo;

		if (!minilzo.init())
		{
			return;
		}

		// ѭ���������
		while (0 == m_terminate)
		{
			// ���Ƚ�������ΪDWORD����Ϣ������Ϣ
			temp_size = sizeof(DWORD);
			msg_ptr = (char*)&msg_size;

			// recv ��ʱ����һ�ν����ݽ������
			while (0 < temp_size)
			{
				// ��ȡ�����ݵĴ�С
				FD_ZERO(&fdread);
				FD_SET(m_socket, &fdread);

				if (1 == select(0, &fdread, NULL, NULL, &block_time))
				{
					result = ::recv(m_socket, msg_ptr, temp_size, 0);

					if (0 == result)
					{
						// FIN
						close();
						return;
					}

					if (SOCKET_ERROR == result)
					{
						// �������
						close();
						return;
					}
					else
					{
						temp_size -= result;
						msg_ptr += result;
					}
				}

				if (1 == m_terminate)
				{
					return;
				}
			}

			// ��鳤�ȵĺϷ���
			if (0 == msg_size || msg_size > MAX_RECV_SIZE)
			{
				close();
				return;
			}

			if (SOCKET_ERROR != result)
			{
				msg_ptr = &(buffer[0]);
				lzo = ((msg_size & 0x00000001) == 1);
				msg_size >>= 1;
				temp_size = msg_size;

				while (0 < temp_size)
				{
					FD_ZERO(&fdread);
					FD_SET(m_socket, &fdread);

					if (1 == select(0, &fdread, NULL, NULL, &block_time))
					{
						result = ::recv(m_socket, msg_ptr, temp_size, 0);

						if (0 == result)
						{
							// FIN
							close();
							return;
						}

						if (SOCKET_ERROR == result)
						{
							// �������
							close();
							return;
						}
						else
						{
							temp_size -= result;
							msg_ptr += result;
						}

						if (1 == m_terminate)
						{
							return;
						}
					}
				}

				if (!lzo)
				{
				memcpy(m_recv_buffer_ptr, &(buffer[0]), msg_size);
				}
				else
				{
					uint32 new_size = 0;
					// !!!���������Ʒ��͸��ͻ��˵����ݽ�ѹ��Ҫ����MAX_RECV_SIZE

					if (!minilzo.decompress(&(buffer[0]), msg_size, m_recv_buffer_ptr, new_size))
					{
						close();
						return;
					}

					msg_size = new_size;
				}
				
				// ����Ϊ��С��Ϣ��λ���ص�����
				DWORD	ptr = 0;

				while (ptr < msg_size)
				{
					temp_size = static_cast<DWORD>(*reinterpret_cast<WORD*>(m_recv_buffer_ptr + ptr));
					ptr += sizeof(WORD);

					if (ptr + temp_size <= msg_size)
					{
						on_datarecv(m_recv_buffer_ptr + ptr, temp_size);
					}
					else
					{
						break;
					}

					ptr += temp_size;
				}
			}
		}

		minilzo.destroy();
		delete []buffer;
		buffer = NULL;
	}

	void ntcp_client::_handle_connect()
	{
		sockaddr_in address;
		ZeroMemory(&address, sizeof(address));
		address.sin_addr.s_addr	= inet_addr(m_ip_addresss.c_str());
		address.sin_family		= AF_INET;
		address.sin_port		= htons(m_port);

		int	result = 0;
		DWORD error_code = 0;
		uint32 error = ECS_Connected;

		// ���IP�Ƿ�Ϸ�
		if (INADDR_NONE == address.sin_addr.s_addr)
		{
			// log
			on_connect(ECS_Error);
			::closesocket(m_socket);
			m_socket = 0;
			return;
		}

		DWORD time_now = ::timeGetTime();

		fd_set fdwrite;
		timeval block_time;						// ��������ʱ��
		block_time.tv_sec	= 0;
		block_time.tv_usec	= NET_BLOCK_TIME;

		// ���ӵ�������
		// For connection-oriented, nonblocking sockets, it is often not possible 
		// to complete the connection immediately. In such a case, "connect"
		// returns the error WSAEWOULDBLOCK. However, the operation proceeds.
		result = ::connect(m_socket, (sockaddr*)&(address), sizeof(address));

		if (SOCKET_ERROR == result)
		{
			error_code = ::WSAGetLastError();

			if (WSAEWOULDBLOCK == error_code)
			{
				// ��ͣ����, ֱ����ʱΪֹ
				while (true)
				{
					FD_ZERO(&fdwrite);
					FD_SET(m_socket, &fdwrite);

					if (1 == select(0, NULL, &fdwrite, NULL, &block_time))
					{
						// ���ӳɹ�
						break;
					}

					if (::timeGetTime() - time_now >= m_time_out * 1000)
					{
						error = ECS_ConnectTimeout;
						break;
					}

					if (1 == m_closing)
					{
						error = ECS_ConnectAborted;
						break;
					}
				}
			}
			else
			{
				error = ECS_Error;
			}
		}

		if (ECS_Connected != error)
		{
			on_connect(error);
			::closesocket(m_socket);
			m_socket = 0;
			return;
		}

		// �������ӳɹ�״̬
		m_status = ECS_CONNECTED;
		on_connect(ECS_Connected);

		// ���ö�д�߳�ѭ����־λ
		::InterlockedExchange((LONG*)&m_terminate, 0);

		// ���������߳�
		m_threads.create_thread(boost::bind(&ntcp_client::_thread_recv, this));

		// ���������߳�
		m_threads.create_thread(boost::bind(&ntcp_client::_thread_send, this));
	}

	void ntcp_client::_handle_close()
	{
		if (0 == m_closing || ECS_CONNECTED != m_status)
		{
			return;
		}
		// ������д�߳�
		::InterlockedExchange((LONG*)&m_terminate, 1);
		// �ȴ��߳̽���,һֱ����ȥ
		m_threads.join_all();

		// �����ر�socket
		_graceful_close_socket(m_socket, 20);		// ���� 20 ��

		// ���ö�дָ�룬�����������ͷţ������൱�����Ͷ�ݵķ�����������
		m_write_wr[0]	= 0;
		m_write_wr[1]	= 0;

		// ����״̬�ص�����
		m_status = ECS_OPEN;
		on_disconnect();
	}

} // namespace nexus

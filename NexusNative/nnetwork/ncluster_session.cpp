#include "ncluster_session.h"
#include "ncluster_server.h"
#include "ncluster_client.h"
#include "nguard.h"
#include "nlog_mt.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace nexus {

	const int MSG_HEADER_SIZE	= sizeof(DWORD);		// ��Ϣͷ�Ĵ�С
	const int MAX_POOL_SIZE		= 12 * 1024 * 1024;		// �ڴ�ص�Ĭ�ϴ�С

	ncluster_session::ncluster_session() : m_session_id(0), m_server_ptr(NULL), m_client_ptr(NULL), m_socket(0), m_status(TSS_NONE),  
		m_send_index(0), m_max_send_buffer(0), m_send_event(NULL), m_terminate(0), m_pool(MAX_POOL_SIZE)
	{
		ZeroMemory(&m_address, sizeof(m_address));
		m_write_wr[0]	= 0;
		m_write_wr[1]	= 0;
		m_buffer[0]		= NULL;
		m_buffer[1]		= NULL;
	}

	ncluster_session::~ncluster_session()
	{
		_free_memory();
		m_session_id		= 0;
		m_server_ptr		= NULL;
		m_client_ptr		= NULL;
		m_socket			= 0;
		m_status			= TSS_NONE;
		m_send_index			= 0;
		m_max_send_buffer		= 0;
		m_write_wr[0]			= 0;
		m_write_wr[1]			= 0;
		m_buffer[0]				= NULL;
		m_buffer[1]				= NULL;
	}

	bool ncluster_session::send(uint32 session_id, nmessage* msg_ptr, uint32 size)
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

	bool ncluster_session::send_msg(uint32 session_id, const void* msg_ptr, uint32 size)
		{
		if (session_id != m_session_id || !msg_ptr || 0 == size || TSS_OPEN != m_status)
			{
				return false;
			}

		// �ж������Ƿ��洦
		uint32 index = 0;
		// ��û�����
		nguard<nfast_mutex> scoped_lock(m_mutex);

		if (TSS_OPEN != m_status)
			{
			return false;
		}

		index = (m_send_index + 1) % 2;

		if (m_write_wr[index] + size >= m_max_send_buffer)
		{
			return false;
		}

		// �������ݣ�1. ��Ϣ����
		*reinterpret_cast<WORD*>(m_buffer[index] + MSG_HEADER_SIZE + m_write_wr[index]) = WORD(size);
		m_write_wr[index] += sizeof(WORD);
		// �������ݣ�2. ��Ϣ����
		memcpy(m_buffer[index] + MSG_HEADER_SIZE + m_write_wr[index], msg_ptr, size);
		m_write_wr[index] += size;

		// ����send�¼�
		::SetEvent(m_send_event);
		return true;
	}

	bool ncluster_session::_init(uint16 index, ncluster_server* server_ptr, ncluster_client* client_ptr, uint32 max_send_buffer)
	{
		if (0XFFFF == index || 0 == max_send_buffer)
		{
			NLOG_NERROR(_T("ncluster_session init param error, index = %u, max_send_buffers = %u"), index, max_send_buffer);
			return false;
		}

		*reinterpret_cast<uint16*>(&m_session_id) = index;
		m_server_ptr			= server_ptr;
		m_client_ptr			= client_ptr;
		m_max_send_buffer		= max_send_buffer;

		// alloc memory
		m_buffer[0] = new char[m_max_send_buffer + MSG_HEADER_SIZE + 1];

		if (!m_buffer[0])
		{
			NLOG_NERROR(_T("alloc send buffer memory error!"));
			return false;
		}

		m_buffer[1] = new char[m_max_send_buffer + MSG_HEADER_SIZE + 1];

		if (!m_buffer[1])
		{
			NLOG_NERROR(_T("alloc send buffer memory error!"));
			_free_memory();
			return false;
		}

		m_status = TSS_INIT;

		return true;
	}

	void ncluster_session::_free_memory()
	{
		m_status = TSS_NONE;

		// �ͷ�session new���ڴ�����
		if (m_buffer[0])
		{
			delete []m_buffer[0];
			m_buffer[0] = NULL;
		}

		if (m_buffer[1])
		{
			delete []m_buffer[1];
			m_buffer[1] = NULL;
		}

		// �ر�handle
		if (m_send_event)
		{
			::CloseHandle(m_send_event);
			m_send_event = NULL;
		}
	}

	void ncluster_session::_pre_accept()
	{
		m_write_wr[0]		= 0;
		m_write_wr[1]		= 0;
		m_send_index		= 0;
		m_status = TSS_ACCEPTEX;
	}

	void ncluster_session::_open()
	{
		// ����״̬
		m_status = TSS_OPEN;
		// �ص�����
		if (m_server_ptr)
		{
			m_server_ptr->on_connect(m_session_id, 0, 0);
		}
		else if (m_client_ptr)
		{
			m_client_ptr->on_connect(m_session_id, 0, 0);
		}
		// �Զ�����
		m_send_event = ::CreateEvent(NULL, FALSE, TRUE, NULL);
		// �����߳�������־
		::InterlockedExchange((LONG*)&m_terminate, 0);
		// ������д�߳�
		m_threads.create_thread(boost::bind(&ncluster_session::_thread_recv, this));
		m_threads.create_thread(boost::bind(&ncluster_session::_thread_send, this));
	}

	void ncluster_session::_release()
	{
		// �����ж�
		if (TSS_SHUT != m_status)
		{
			return;
		}
		// ע���̰߳�ȫ
		{
			nguard<nfast_mutex> scoped_lock(m_mutex);

			if (TSS_SHUT != m_status)
			{
				return;
			}

		// �ر�handle
		if (m_send_event)
		{
			::CloseHandle(m_send_event);
			m_send_event = NULL;
		}
		// ����session״̬
		m_status = TSS_CLOSE;
		// �ı�m_client_id
		++(*(reinterpret_cast<uint16*>(&m_session_id) + 1));
		}
		// ��session�黹�����ӹ����
		if (m_server_ptr)
		{
			m_server_ptr->_return_session(this);
		}
		else if (m_client_ptr)
		{
			m_client_ptr->_return_session(this);
		}
	}

	void ncluster_session::_closing()
	{
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSS_OPEN != m_status)
			{
				return;
			}

			m_status = TSS_SHUT;
		}

		if (m_server_ptr)
		{
			m_server_ptr->_closing_session(this);
		}
		else if (m_client_ptr)
		{
			m_client_ptr->_closing_session(this);
		}
	}

	void ncluster_session::_close()
	{
		if (TSS_SHUT != m_status)
		{
				return;
			}

		::shutdown(m_socket, SD_SEND);

		int retry = 0;
		int result = 0;
		fd_set fdread;
		timeval block_time;							// ��������ʱ��
		block_time.tv_sec	= 0;
		block_time.tv_usec	= 150 * 1000;			// 150 ms 

		while (retry++ < 10)
		{
			FD_ZERO(&fdread);
			FD_SET(m_socket, &fdread);

			if (1 == select(0, &fdread, NULL, NULL, &block_time))
			{
				CHAR temp;
				result = ::recv(m_socket, &temp, 1, 0);

				// receive FIN, ok
				if (0 == result)
				{
					break;
				}

				// network down
				if (SOCKET_ERROR == result)
				{
					break;
				}
			}
		}

		::closesocket(m_socket);
		m_socket = 0;

		// �ص��رպ���
		if (m_server_ptr)
		{
			m_server_ptr->on_disconnect(m_session_id);
		}
		else if (m_client_ptr)
		{
			m_client_ptr->on_disconnect(m_session_id);
		}

		// �رն�д�߳�
		::InterlockedExchange((LONG*)&m_terminate, 1);

		// �ȴ������̵߳��˳�
		m_threads.join_all();

		// �ͷ��ڴ����Դ���黹������Դ
		_release();
	}

	void ncluster_session::_thread_recv()
	{
		fd_set fdread;
		timeval block_time;					// ��������ʱ��
		block_time.tv_sec	= 0;
		block_time.tv_usec	= 200 * 1000;	// 200 ms

		int result		= 0;
		DWORD msg_size	= 0;
		DWORD temp_size	= 0;
		DWORD ptr		= 0;
		char* temp_ptr	= NULL;
		nmessage* nmsg_ptr = NULL;

		char* buffer = new char[m_max_send_buffer + 1];

		if (!buffer)
		{
			return;
		}

		// ѭ���������
		while (0 == m_terminate)
		{
			// 1. ����DWORD����Ϣ����
			temp_size = sizeof(DWORD);
			temp_ptr = (char*)&msg_size;

			while (0 < temp_size)
			{
				FD_ZERO(&fdread);
				FD_SET(m_socket, &fdread);

				if (1 == select(0, &fdread, NULL, NULL, &block_time))
				{
					result = ::recv(m_socket, temp_ptr, temp_size, 0);

					if (0 == result || SOCKET_ERROR == result)
					{
						_closing();
						return;
					}

					temp_size -= result;
					temp_ptr += result;
				}

				if (1 == m_terminate)
				{
					_closing();
					return;
				}
			}

			if (SOCKET_ERROR != result)
			{
				temp_size = msg_size;
				temp_ptr = (char*)&(buffer[0]);

				while (0 < temp_size)
				{
					FD_ZERO(&fdread);
					FD_SET(m_socket, &fdread);

					if (1 == select(0, &fdread, NULL, NULL, &block_time))
					{
						result = ::recv(m_socket, temp_ptr, temp_size, 0);

						if (0 == result || SOCKET_ERROR == result)
						{
							_closing();
							return;
						}

						temp_size -= result;
						temp_ptr += result;
					}

					if (1 == m_terminate)
					{
						_closing();
						return;
					}
				}

				// ������Ϣ���ص�����
				temp_size = msg_size;
				temp_ptr = (char*)&(buffer[0]);

				// ����Ϊ��С��Ϣ��λ���ص�����
				ptr = 0;

				while (ptr < msg_size)
				{
					temp_size = DWORD(*(WORD*)(temp_ptr + ptr));
					ptr += sizeof(WORD);

					if (ptr + temp_size <= msg_size)
					{
						// ����nmessage
						nmsg_ptr = reinterpret_cast<nmessage*>(m_pool.alloc(sizeof(nmessage) + temp_size));

						if (NULL == nmsg_ptr)
						{
							_closing();
							return;
						}

						nmsg_ptr->pool_ptr		= &m_pool;
						nmsg_ptr->next_ptr		= NULL;
						nmsg_ptr->reference		= 1;
						nmsg_ptr->size			= temp_size;
						nmsg_ptr->session_id	= 0;
						nmsg_ptr->server_id		= 0;

						// ��������
						memcpy(&(nmsg_ptr->session_id), temp_ptr + ptr, temp_size);

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
			}
		}

		delete []buffer;
		buffer = NULL;
	}

	void ncluster_session::_thread_send()
	{
		fd_set fdwrite;
		timeval block_time;					// ���÷�������ʱ��
		block_time.tv_sec	= 0;
		block_time.tv_usec	= 200 * 1000;	// 200 ms

		int result		= 0;
		char*	msg_ptr = NULL;
		DWORD	msg_size = 0;

		while (0 == m_terminate)
		{
			// ע���ڲ���break
			for(;;)
			{
				// ���Ҫ���͵����ݻ�����
				{
					nguard<nfast_mutex> scoped_lock(m_mutex);
					msg_ptr		= m_buffer[m_send_index];
					msg_size	= m_write_wr[m_send_index];

					if (0 == msg_size)
				{
						m_send_index = (m_send_index + 1) % 2;
						msg_ptr		= m_buffer[m_send_index];
						msg_size	= m_write_wr[m_send_index];
					}

					m_write_wr[m_send_index] = 0;
				}

				if (0 == msg_size)
				{
					break;
				}

				// ��ֵ�����Ϣ����
				*((DWORD*)m_buffer[m_send_index]) = msg_size;
				msg_size += sizeof(uint32);

				// send ���ܲ���һ�ν����ݷ���
				while (0 < msg_size && 0 == m_terminate)
				{
					result = ::send(m_socket, msg_ptr, msg_size, 0);

					if (SOCKET_ERROR == result)
					{
						DWORD last_error = ::WSAGetLastError();

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
								_closing();
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
					nguard<nfast_mutex> scoped_lock(m_mutex);
					m_send_index = (m_send_index + 1) % 2;
			}
			}

			::WaitForSingleObject(m_send_event, 120);
		}
	}

} // namespace nexus

/**
 *	nexus network - ntcp_server
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NTCP_SERVER_H_
#define _NNETWORK_NTCP_SERVER_H_

#include "ncommon.h"
#include "nnetwork_def.h"
#include "ntcp_session.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	ntcp_server��������������ͨ�ŵײ�
	 */
	class nNET_API ntcp_server : private nnoncopyable
	{
	private:
		friend class ntcp_session;

	public:
		ntcp_server();
		virtual ~ntcp_server();

		/** 
		 * ��������������ָ���˿ڵ������ַ
		 */
		bool startup(const ntcp_server_config& param);

		/** 
		 * �رշ��������ú�������ִ���̲߳���ǿ�ƹرգ����������߳��йر�
		 */
		void shutdown();

		/** 
		 * ��ָ�����ӷ������ݣ��ڲ��������ü����ķ�ʽ��������
		 * ���ͬһnmessage��send���Ҫ�������е����ô����ٵ���send, server�ڲ����ͷ�nmessage�ڴ�����
		 * @param session_id	����id
		 * @param msg_ptr		������Ϣ�ĵ�ַ
		 * @param size			�������ݵ�ʵ�ʳ���
		 */
		NINLINE bool send(uint32 session_id, nmessage* msg_ptr, uint32 size)
		{
			if ((session_id & 0X0000FFFF) >= m_ntcp_server_param.max_session)
			{
				msg_ptr->dec_reference();
				return false;
			}

			return m_sessions_ptr[session_id & 0X0000FFFF].send(session_id, msg_ptr, size);
		}

		/** 
		 * �ر�ָ������
		 * @param session_id	����id
		 */
		NINLINE void close(uint32 session_id)
		{
			if ((session_id & 0X0000FFFF) >= m_ntcp_server_param.max_session)
			{
				return;
			}

			return m_sessions_ptr[session_id & 0X0000FFFF].close(session_id);
		}

		/** 
		 * �����ӽ����ص�����, ע���̰߳�ȫ
		 * note: ������Ҫ���������߼�����װһ����ϢͶ�ݵ�ͳһ���߼���Ϣ����
		 * @param session_id	����id
		 * @param address		����ip��ַ
		 * @param port			���Ӷ˿�
		 */
		virtual void on_connect(uint32 session_id, uint32 address, uint32 port) {}

		/** 
		 * ��Ϣ���ջص�����, ע���̰߳�ȫ
		 * note: recv nmessage �����ü�����Ϊ0�����ûص�������ײ㲻�����ø��ڴ�
		 * ������Ҫ���������߼���ת����Ϣ����ϢͶ�ݵ�ͳһ���߼���Ϣ����
		 * @param session_id	����id
		 * @param msg_ptr		��Ϣ��ַ
		 * @param port			��Ϣ��ʵ�ʳ���
		 */
		virtual void on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size) {}

		/** 
		 * ���ӹرջص�����, ע���̰߳�ȫ
		 * note: ������Ҫ���������߼�����װһ����ϢͶ�ݵ�ͳһ���߼��̶߳���
		 * @param session_id	����id
		 */
		virtual void on_disconnect(uint32 session_id) {}

	private:
		// �ͷŸ���������ڴ���Դ
		void _free_memory();
		// iocp�����̸߳�������ɰ�
		void _worker_thread(int32 thread_index);
		// �첽accpet����
		void _async_accept();
		// �첽accpet������Ӧ�ص�
		void _handle_accept(ntcp_session* session_ptr, DWORD last_error, nsafe_mem_pool* pool_ptr);
		// ����ɶ˿�Ͷ��һ��close�ص�����
		NINLINE void _post_close_call_back(uint32 session_id)
		{
			if (0XFFFFFFFF == session_id)
			{
				return;
			}

			::PostQueuedCompletionStatus(m_completion_port, 0, (ULONG_PTR)session_id, 0);
		}

		NINLINE void _handle_close(uint32 session_id)
		{
			if ((session_id & 0X0000FFFF) >= m_ntcp_server_param.max_session)
			{
				return;
			}

			// �ص��ϲ�رպ���
			on_disconnect(session_id);
			// ��session�黹�����ӹ����
			_return_session(&(m_sessions_ptr[session_id & 0X0000FFFF]));
			_async_accept();
		}

		// ȡ��һ�����õĻỰ
		NINLINE ntcp_session* _get_available_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_OPEN != m_status || m_available_sessions.empty())
			{
				return NULL;
			}

			ntcp_session* temp_ptr = m_available_sessions.front();
			m_available_sessions.pop_front();
			temp_ptr->_pre_accept();
			return temp_ptr;
		}
		// �黹һ���رյĻỰ
		NINLINE void _return_session(ntcp_session* session_ptr)
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_available_sessions.push_back(session_ptr);
		}

	private:
		SOCKET						m_sock_listen;				// ����socket
		HANDLE						m_completion_port;			// ��������ɶ˿�

		LPFN_ACCEPTEX				m_fn_acceptex;
		LPFN_GETACCEPTEXSOCKADDRS	m_fn_get_acceptex_sock_addr;
		volatile LONG				m_acceptexs;				// �ų���AcceptEx����

		nfast_mutex					m_mutex;					// �������ӳ��̰߳�ȫ��server״̬�ı�
		ntcp_session*				m_sessions_ptr;
		std::list<ntcp_session*>	m_available_sessions;

		boost::thread_group			m_threads;					// �̳߳�

		ntcp_server_config			m_ntcp_server_param;
		LONG						m_max_acceptex;

		nsafe_mem_pool*				m_pools_ptr;				// �ڴ��

		TCPServerStatus				m_status;
	};

} // namespace nexus

#endif _NNETWORK_NTCP_SERVER_H_

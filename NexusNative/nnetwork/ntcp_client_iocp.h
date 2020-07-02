/**
 *	nexus network - ntcp_client_iocp
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NTCP_CLIENT_IOCP_H_
#define _NNETWORK_NTCP_CLIENT_IOCP_H_

#include "ncommon.h"
#include "nnetwork_def.h"
#include "ntcp_session_client.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	/**
	 *	ntcp_client_iocp ѹ�����Կͻ���ͨ�ŵײ�
	 */
	class nNET_API ntcp_client_iocp : private nnoncopyable
	{
	private:
		friend class ntcp_session_client;

	public:
		ntcp_client_iocp();
		virtual ~ntcp_client_iocp();

		/** 
		 * �����ͻ��ˣ���ʼ����Դ
		 */
		bool startup(uint32 max_session = 1000, uint32 max_recv_buffer = 48 * 1024, uint32 max_send_buffer = 32, uint32 max_send_msg_size = 512, uint32 time_out_sec = 60, bool nagle = false);

		/** 
		 * �رտͻ��ˣ��ú�������ִ���̲߳���ǿ�ƹرգ����������߳��йر�
		 */
		void shutdown();

		/** 
		 * �첽�����ӷ�����
		 * @param ip_addresss	Զ��ip��ַ
		 * @param port			Զ�̶˿�
		 * @return				����id,0XFFFFFFFF��ʾ��������Ͷ��ʧ��
		 */
		uint32 connect_to(const std::string& ip_addresss, uint16 port);

		/** 
		 * ��ָ�����ӷ������ݣ��ڲ�nmessage�������ڲ�����nmessage
		 * @param session_id	����id
		 * @param msg_ptr		������Ϣ�ĵ�ַ
		 * @param size			�������ݵ�ʵ�ʳ���
		 */
		NINLINE bool send(uint32 session_id, nmessage* msg_ptr, uint32 size)
		{
			if ((session_id & 0X0000FFFF) >= m_max_session)
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
			if ((session_id & 0X0000FFFF) >= m_max_session)
			{
				return;
			}

			return m_sessions_ptr[session_id & 0X0000FFFF].close(session_id);
		}

		/** 
		 * �����ӽ����ص�����, ע���̰߳�ȫ
		 * note: ������Ҫ���������߼�����װһ����ϢͶ�ݵ�ͳһ���߼���Ϣ����
		 * @param session_id	����id
		 * @param param1		����1��EConnectCode
		 * @param param2		����2
		 */
		virtual void on_connect(uint32 session_id, uint32 param1, uint32 param2) {}

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
			if ((session_id & 0X0000FFFF) >= m_max_session)
			{
				return;
			}

			// �ص��ϲ�رպ���
			on_disconnect(session_id);
			// ��session�黹�����ӹ����
			_return_session(&(m_sessions_ptr[session_id & 0X0000FFFF]));
		}

		// ȡ��һ�����õĻỰ
		NINLINE ntcp_session_client* _get_available_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (TSVRS_OPEN != m_status || m_available_sessions.empty())
			{
				return NULL;
			}

			ntcp_session_client* temp_ptr = m_available_sessions.front();
			m_available_sessions.pop_front();
			temp_ptr->_pre_connect();
			return temp_ptr;
		}
		// �黹һ���رյĻỰ
		NINLINE void _return_session(ntcp_session_client* session_ptr)
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_available_sessions.push_back(session_ptr);
		}

	private:
		HANDLE						m_completion_port;			// �ͻ�����ɶ˿�

		nfast_mutex					m_mutex;					// �������ӳ��̰߳�ȫ��״̬�ı�
		ntcp_session_client*		m_sessions_ptr;
		std::list<ntcp_session_client*>	m_available_sessions;

		boost::thread_group			m_threads;					// �̳߳�
		uint32						m_thread_num;				// �����߳���Ŀ

		nsafe_mem_pool*				m_pools_ptr;				// �ڴ��
		TCPServerStatus				m_status;

		uint32						m_max_session;				// ���������
		uint32						m_max_recv_buffer;			// recv����󳤶�
		bool						m_nagle;					// �Ƿ���nagle�㷨
	};

} // namespace nexus

#endif _NNETWORK_NTCP_CLIENT_IOCP_H_

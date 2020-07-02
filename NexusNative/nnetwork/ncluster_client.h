/**
 *	nexus network - ncluster_server
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NCLUSTER_CLIENT_H_
#define _NNETWORK_NCLUSTER_CLIENT_H_

#include "ncommon.h"
#include "nnetwork_def.h"
#include "ncluster_session.h"

#include <string>
#include <boost/thread/thread.hpp>

namespace nexus {

	/**
 	 *	ncluster_client�������ͻ��˶˴���ײ�
	 *	@remark ncluster_client ����������ͨ�ſͻ��ˣ�Ϊÿ�����ӿ��ٶ�д�̣߳������ڴ�����������ͨѶ
	 */
	class nNET_API ncluster_client : private nnoncopyable
	{
	private:
		friend class ncluster_session;

	public:
		ncluster_client();
		virtual ~ncluster_client();

		/** 
		 * ����cluster�ͻ��ˣ���ʼ����Դ
		 */
		bool startup(uint32 max_session = 4, uint32 max_send_buffer = 2 * 1024 * 1024, uint32 time_out_sec = 20, bool nagle = true);

		/** 
		 * �رտͻ����������ӣ��ú�������ִ���̲߳���ǿ�ƹرգ����������߳��йر�
		 */
		void shutdown();

		/** 
		 * �첽�����ӷ�����
		 * @param ip_addresss	Զ��ip��ַ
		 * @param port			Զ�̶˿�
		 * @param time_out_sec	���ӳ�ʱ���ã���λs
		 * @return				����id,0XFFFFFFFF��ʾ��������Ͷ��ʧ��
		 */
		uint32 connect_to(const std::string& ip_addresss, uint16 port);

		/** 
		 * ��ָ�����ӷ������ݣ��ڲ��������ü����ķ�ʽ��������
		 * ���ͬһnmessage��send���Ҫ�������е����ô����ٵ���send, server�ڲ����ͷ�nmessage�ڴ�����
		 * note: gatewayʹ�ýӿ�
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
		 * ��ָ�����ӷ������ݣ��ڲ��Ὠ��һ�ݿ������˺������غ�msgָ����ڴ���԰�ȫ�ͷ�
		 * note: ��Ϸ�߼�������ʹ�ýӿ�
		 * @param session_id	����id
		 * @param msg_ptr		��Ϣ�ĵ�ַ
		 * @param size			�������ݵ�ʵ�ʳ���
		 */
		NINLINE bool send_msg(uint32 session_id, const void* msg_ptr, uint32 size)
		{
			if ((session_id & 0X0000FFFF) >= m_max_session)
			{
				return false;
			}

			return m_sessions_ptr[session_id & 0X0000FFFF].send_msg(session_id, msg_ptr, size);
		}

		/** 
		 * �ر�ָ������
		 * @param session_id	����id
		 */
		void close(uint32 session_id)
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
		// ʵ���첽connect�߳�
		void _thread_connect();
		// ���ӹر��߳�
		void _thread_close();
		// connect ����ͳһ����
		void _handle_connect(uint32 error_code, ncluster_session* session_ptr);

		// ȡ��һ�����õĻỰ
		NINLINE ncluster_session* _get_available_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (m_available_sessions.empty())
			{
				return NULL;
			}

			ncluster_session* temp_ptr = m_available_sessions.front();
			m_available_sessions.pop_front();
			temp_ptr->_pre_accept();
			return temp_ptr;
		}
		// �黹һ���رյĻỰ
		NINLINE void _return_session(ncluster_session* session_ptr)
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_available_sessions.push_back(session_ptr);
		}

		// ȡ��һ��close����Ự
		NINLINE ncluster_session* _get_closing_session()
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (m_closing_sessions.empty())
			{
				return NULL;
			}

			ncluster_session* temp_ptr = m_closing_sessions.front();
			m_closing_sessions.pop_front();
			return temp_ptr;
		}

		// Ͷ��һ��close����
		NINLINE void _closing_session(ncluster_session* session_ptr)
		{
			nguard<nfast_mutex> guard(m_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_closing_sessions.push_back(session_ptr);
		}

		// ȡ��һ��connnecting����Ự
		NINLINE ncluster_session* _get_connnecting_session()
		{
			nguard<nfast_mutex> guard(m_connect_mutex);

			if (m_connecting_sessions.empty())
			{
				return NULL;
			}

			ncluster_session* temp_ptr = m_connecting_sessions.front();
			m_connecting_sessions.pop_front();
			return temp_ptr;
		}

		// ��connect����session���뵽����
		NINLINE void _connecting_session(ncluster_session* session_ptr)
		{
			nguard<nfast_mutex> guard(m_connect_mutex);

			if (!session_ptr)
			{
				return;
			}

			m_connecting_sessions.push_back(session_ptr);
		}

	private:
		TCPServerStatus				m_status;					// ����server��״̬ö��

		nfast_mutex					m_mutex;					// �������ӳ��̰߳�ȫ��server״̬�ı䣬���ú͹رչ���һ������cluster������Ƶ���Ͽ�����
		ncluster_session*			m_sessions_ptr;				// ���ӳ�
		std::list<ncluster_session*>m_available_sessions;
		std::list<ncluster_session*>m_closing_sessions;

		nfast_mutex					m_connect_mutex;			// Ͷ���첽����������
		std::list<ncluster_session*>m_connecting_sessions;		// �������ӵ�session����

		boost::thread_group			m_threads;					// �̳߳�

		volatile LONG				m_terminate_connect;		// 1 �ر�connect�߳�
		volatile LONG				m_terminate_close;			// 1 �ر�close�߳�

		uint32						m_max_session;				// ���������
		uint32						m_time_out_sec;				// ��ʱ���õ�λs
		bool						m_nagle;					// �Ƿ���nagle�㷨
	};

} // namespace nexus

#endif _NNETWORK_NCLUSTER_CLIENT_H_
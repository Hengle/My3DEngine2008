/**
 *	nexus network - ncluster_session
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NCLUSTER_SESSION_H_
#define _NNETWORK_NCLUSTER_SESSION_H_

#include "ncommon.h"
#include "nnetwork_def.h"

#include <boost/thread/thread.hpp>

namespace nexus {

	class ncluster_server;
	class ncluster_client;

	/**
	 *	ncluster_session���������紫��Ự
	 */
	class nNET_API ncluster_session : private nnoncopyable
	{
	private:
		friend class ncluster_server;
		friend class ncluster_client;

	public:
		ncluster_session();
		virtual ~ncluster_session();

		/** 
		 * ��ָ�����ӷ������ݣ��ڲ��������ü����ķ�ʽ��������, 
		 * ���ͬһnmessage��send���Ҫ�������е����ô����ٵ���send, server�ڲ����ͷ�nmessage�ڴ�����
		 * note: gatewayʹ�ýӿ�
		 * @param session_id	����id
		 * @param msg_ptr		������Ϣ�ĵ�ַ
		 * @param size			�������ݵ�ʵ�ʳ���
		 */
		bool send(uint32 session_id, nmessage* msg_ptr, uint32 size);

		/** 
		 * ��ָ�����ӷ������ݣ��ڲ��Ὠ��һ�ݿ������˺������غ�msgָ����ڴ���԰�ȫ�ͷ�
		 * note: ��Ϸ�߼�������ʹ�ýӿ�
		 * @param session_id	����id
		 * @param msg_ptr		������Ϣ�ĵ�ַ
		 * @param size			�������ݵ�ʵ�ʳ���
		 */
		bool send_msg(uint32 session_id, const void* msg_ptr, uint32 size);

		/** 
		 * �ر�ָ������
		 * @param session_id	����id
		 */
		NINLINE void close(uint32 session_id)
		{
			if (session_id != m_session_id)
			{
				return;
			}

			_closing();
		}

	private:
		// ���ӳ�ʼ�������ò�����������Դ, �������Ϳͻ��˹��ø�sessionͨ�����ݵĲ�������server_ptr��client_ptr����ֻ��һ���ǿ�
		bool _init(uint16 index, ncluster_server* server_ptr, ncluster_client* client_ptr, uint32 max_send_buffer);
		// �ͷ�����������ڴ���Դ��������pool������Դ��pool��Դ��closeʱ�黹
		void _free_memory();
		// ������Ͷ��acceptexǰ��connectǰ���ݳ�ʼ��
		void _pre_accept();
		// ���Ӵ򿪻ص�������������д�߳�
		void _open();
		// �黹����pool��Դ
		void _release();
		// �ر�����
		void _closing();
		// �ر�ָ������
		void _close();

		// ����session״̬
		NINLINE void _set_status(TCPSessionStatus status) { m_status = status; }
		// ���session״̬
		NINLINE TCPSessionStatus _get_status() const { return m_status; }
		// ���session id
		NINLINE uint32 _get_client_id() const { return m_session_id; }

		// �������߳�
		void _thread_recv();
		// д�����߳�
		void _thread_send();

	private:
		uint32				m_session_id;					// session_idһ������| ��2�ֽ� ��ʾindex | ��2�ֽڱ�ʾguid |��0XFFFFFFFFΪ�Ƿ���client_id
		ncluster_server*	m_server_ptr;					// �ϲ�serverָ�룬���ڻص���Ϣ
		ncluster_client*	m_client_ptr;					// �ϲ�clientָ�룬���ڻص���Ϣ
		SOCKET				m_socket;						// session��socket��Դ
		sockaddr_in			m_address;						// ������connect�ķ�������ַ

		TCPSessionStatus	m_status;						// session״̬

		nfast_mutex				m_mutex;						// ����send��������session״̬
		uint32					m_write_wr[2];					// ���ͻ�����дָ��
		char*					m_buffer[2];					// ���ͻ�����
		uint32					m_send_index;					// ��ǰsend�Ļ���������
		uint32					m_max_send_buffer;				// ���ͻ���������󳤶�
		HANDLE				m_send_event;					// �����¼�

		volatile LONG			m_terminate;					// 1 �رն�д�߳�
		nsafe_mem_pool		m_pool;							// recv�����ص������ڴ�
		boost::thread_group	m_threads;
	};

} // namespace nexus

#endif _NNETWORK_NCLUSTER_SESSION_H_

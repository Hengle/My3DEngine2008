/**
 *	nexus network - ntcp_client �������ͻ���ͨѶ�ײ�
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NTCP_CLIENT_H_
#define _NNETWORK_NTCP_CLIENT_H_

#include "ncommon.h"
#include "nmutex.h"

#include <string>
#include <boost/thread/thread.hpp>

namespace nexus {

	/** 
	 * �ͻ���on_connect�ص���������ö�� 
	 */
	enum EClientConnect
	{
		ECS_Connected			= 0,		// �Ѿ����Ӻ�
		ECS_ConnectTimeout		= 1,		// ���ӳ�ʱ
		ECS_ConnectAborted		= 2,		// ��������ȡ��
		ECS_Error				= 3,		// ��������
		ECS_Max,
	};

	/**
	 *	ntcp_server�������ͻ��˶�ͨ�ŵײ�
	 */
	class nNET_API ntcp_client : private nnoncopyable
	{
	private:
		enum EClientStatus
		{
			ECS_NONE			= 0,		// ��
			ECS_INIT			= 1,		// ��ʼ��ʼ��
			ECS_OPEN			= 2,		// ����connect
			ECS_CONNECTED		= 3,		// ���ӳɹ�
			ECS_CLOSE			= 4,		// �ر�
			ECS_MAX,
		};

	public:
		ntcp_client();
		virtual ~ntcp_client();

		/** 
		 * �����ͻ��ˣ���ʼ����Դ
		 * @param time_out_sec	���ӳ�ʱ���ã���λs
		 * @param nagle			true��Nagle�㷨��false�ر�Nagle�㷨
		 */
		bool startup(uint32 time_out_sec = 20, bool nagle = true);

		/** 
		 * �رտͻ��ˣ��ú�������ִ���̲߳���ǿ�ƹرգ����������߳��йر�
		 */
		void shutdown();

		/** 
		 * �첽���ӷ�����������״��ͨ���ص�����on_connect����
		 * @param ip_addresss	Զ��ip��ַ
		 * @param port			Զ�̶˿�
		 * @return				true�첽��������Ͷ�ݳɹ���falseͶ��ʧ��
		 */
		bool connect_to(const std::string& ip_addresss, uint16 port);

		/** 
		 * �ر�������������ӣ��رճɹ���ص�on_disconnect����
		 */
		void close();

		/** 
		 * ��������������ݣ��ڲ��Ὠ��һ�ݿ������˺������غ�msgָ����ڴ���԰�ȫ�ͷ�
		 * note: ���ܵĹ������������װ
		 * @param msg_ptr	���ݵ��׵�ַ
		 * @param size		���ݵĳ���
		 */
		bool send(const void* msg_ptr, uint32 size);

		/** 
		 * �ͻ�����������״̬�ı�ص�����, ע���̰߳�ȫ
		 * note: ������Ҫ���������߼�������ֻ��װһ����ϢͶ�ݵ�ͳһ���߼���Ϣ����
		 * @param error_code	���������EClientConnect
		 */
		virtual void on_connect(uint32 error_code) {}

		/** 
		 * �ͻ��˽���������Ϣ�ص��������ⲿ��Ҫ����һ�ݿ��������ؿ���msg_ptr���ͷ�, ע���̰߳�ȫ
		 * note: ������Ҫ���������߼�������ֻ��װһ����ϢͶ�ݵ�ͳһ���߼���Ϣ����
		 * @param msg_ptr	���ݵ��׵�ַ
		 * @param size		���ݵĳ���
		 */
		virtual void on_datarecv(const void* msg_ptr, uint32 size) {}

		/** 
		 * �ͻ����ӹرջص�������ע���̰߳�ȫ
		 * note: ������Ҫ���������߼�����װһ����ϢͶ�ݵ�ͳһ���߼��̶߳���
		 */
		virtual void on_disconnect() {}

	private:
		// �ͷ��ڴ���Դ
		void _free_memory();
		// �����ر�socket
		bool _graceful_close_socket(SOCKET& socket, int retry_times);
		// �����̣߳�����ʵ���첽���ӣ��첽�ر�
		void _thread_worker();
		// ���ݷ����߳�
		void _thread_send();
		// ���ݽ����߳�
		void _thread_recv();

		// ����connect
		void _handle_connect();
		// ����close
		void _handle_close();

	private:
		EClientStatus				m_status;					// client״̬
		SOCKET						m_socket;					// socket
		std::string					m_ip_addresss;				// Զ��ip��ַ
		uint16						m_port;						// Զ�̶˿�
		uint32						m_time_out;					// ��ʱ���ã���λs
		bool						m_nagle;					// �Ƿ���nagle�㷨

		boost::thread_group			m_threads;					// �̳߳أ���д�߳�
		boost::thread_group			m_thread_worker;			// �����̳߳أ�Ŀǰֻ��һ��

		char*						m_send_buffer_ptr[2];		// ���ͻ�����
		DWORD						m_write_wr[2];				// ���ͻ�����дָ��

		nmutex						m_send_mutex;				// ���ͻ�������ȫ����������client״̬�ı�
		DWORD						m_send_index;				// ��ǰsend�Ļ���������

		char*						m_recv_buffer_ptr;			// ���ջ�����

		volatile LONG				m_terminate;				// 1 ��ʾǿ���˳���д�߳�, 0��ʾ���˳�
		volatile LONG				m_terminate_workder;		// 1 ��ʾǿ���˳������߳�, 0��ʾ���˳�

		volatile LONG				m_connecting;				// 1 ��ʾ��������, 0��ʾ����
		volatile LONG				m_closing;					// 1 ��ʾ�ر�����, 0��ʾ����

		HANDLE						m_send_event;				// �����¼�
	};

} // namespace nexus

#endif _NNETWORK_NTCP_CLIENT_H_

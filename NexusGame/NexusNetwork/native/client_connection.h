#pragma once
#include "nnetwork.h"
#include "msg_queue.h"
#include "msg_listener.h"

namespace nexus
{
	enum EClientSessionStatus
	{
		ECSS_None				= 0,
		ECSS_Connected			= 1,
		ECSS_Disconnect			= (1<<5),
	};


	struct client_msg;
	struct msg_listener;

	/**
	*	�ͻ������ݴ�����
	*/
	class client_connection 
		: public nexus::ntcp_client/*, public singleton<client_connection>*/
	{
	public:
		client_connection();
		virtual ~client_connection();

		// ������յ�����Ϣ����������Ϣ������
		// ֻ����Update��ʱ��Ŵ���Ϣ������ȡ������������Ϣ
		void update(float delta_time);

		// �����Ϣ������
		void add_listener(msg_listener_ptr listener)
		{
			if (!listener)
				return;

			m_msg_listeners.push_back( listener );
		}

		// �Ƴ���Ϣ������
		void remove_listener(msg_listener_ptr listener)
		{
			if (!listener)
				return;

			if (m_msg_listeners.empty())
				return;
			m_msg_listeners.erase( std::find(m_msg_listeners.begin(), m_msg_listeners.end(), listener) );
		}

		void clear_listeners()
		{
			m_msg_listeners.clear();
		}

		virtual void on_connect(uint32 error_code);
		virtual void on_datarecv(const void* msg_ptr, uint32 size);
		virtual void on_disconnect();

		// Session״̬
		void set_status(unsigned int  status)	{ m_status = status; }
		unsigned int get_status() const		{ return m_status; }

	private:
		// �ͻ������������ʹ�����Ϣ
		void on_handle_msg(client_msg* msg_ptr);

		// on_datarecvʱ�����
		bool push_msg(const void* msg_ptr, uint32 size)
		{
			return m_msg_queue.enqueue(msg_ptr, size);
		}


	private:
		// Session״̬
		unsigned int 				m_status;

		// ��Ϣ���Ͷ���
		msg_queue					m_msg_queue;
	
		// ��Ϣ������,�ⲿ��������������
		typedef std::vector<msg_listener_ptr> msg_listener_list;
		msg_listener_list			m_msg_listeners;
	};

}

#pragma once

namespace nexus
{
	/** 
	* ��Ϣ�������ӿ�
	*/
	struct msg_listener
	{
		msg_listener() {};

		// ��Ϣ������
		// param message_id: ������Ϣ�ɣ�
		// param msg_ptr: ������Ϣ�ṹ��ָ��
		virtual void on_msg(uint16 message_id, const void* msg_ptr) = 0;

		virtual void on_connected() = 0;

		virtual void on_disconnected() = 0;

		virtual void on_connect_timeout() = 0;

		virtual void on_connect_aborted() = 0;

		virtual void on_connect_error() = 0;

		DISALLOW_COPY_AND_ASSIGN(msg_listener);
	};

	typedef boost::shared_ptr<msg_listener> msg_listener_ptr;
}

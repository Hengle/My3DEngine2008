#pragma once
#include "native/msg_listener.h"
#include "native/client_define.h"


namespace NexusEngine
{
	/**
	* ��Ϣ��������CLI��װ
	*/
	public interface class INetMessageListener
	{
	public:
		void OnNetMessage(System::Int16 messageID, System::IntPtr message);
		void OnConnected();
		void OnDisconnected();
		void OnConnectTimeout();
		void OnConnectAborted();
		void OnConnectError();
	};


	/**
	* ������Ϸ���õ�������Ϣ������
	*/
	class GameMessageListener : public nexus::msg_listener
	{
	private:
		GameMessageListener() {}

	public:
		explicit GameMessageListener(INetMessageListener^ listener)
			: m_clrObj(listener)
		{}

		virtual void on_msg(nexus::uint16 message_id, const void* msg_ptr)
		{
			// ����Ϣָ���׵�Manage��
			m_clrObj->OnNetMessage(message_id, System::IntPtr(const_cast<void*>(msg_ptr)));
		}

		virtual void on_connected()
		{
			m_clrObj->OnConnected();
		}

		virtual void on_disconnected()
		{
			m_clrObj->OnDisconnected();
		}

		virtual void on_connect_timeout()
		{
			m_clrObj->OnConnectTimeout();
		}

		virtual void on_connect_aborted()
		{
			m_clrObj->OnConnectAborted();
		}

		virtual void on_connect_error()
		{
			m_clrObj->OnConnectError();
		}

	private:
		gcroot<INetMessageListener^>			m_clrObj;
	};
}

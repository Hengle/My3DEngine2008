// NexusNetwork.h

#pragma once
#include "native/client_connection.h"
#include "GameMessageListener.h"

using namespace System;

namespace NexusEngine {

	public enum class ClientSessionStatus : System::Int32
	{
		None		=	nexus::ECSS_None,
		Connected	=	nexus::ECSS_Connected,
		Disconnect	=	nexus::ECSS_Disconnect,
	};


	/**
	* ��װnative class client_connection
	* ʹ������ 
	* NClientConnection connection = new NClientConnection
	* connection->AddListener(...);
	* connection->Startup(...);
	* connection->Connect(...);
	* while(game)
	* {
	*	connection->SendMessage(...);
	*	connection->Update(...);
	* }
	* connection->Close();
	* connection->ClearListeners();
	* connection->Shutdown();
	*/
	public ref class NClientConnection sealed
	{
	public:
		NClientConnection();
		virtual ~NClientConnection();

		// -- native pointer
		property nexus::client_connection*  NativePtr
		{
			nexus::client_connection*  get()
			{
				return m_NativeObj;
			}
		}

		// ��ǰSession״̬
		property ClientSessionStatus SessionStatus
		{
			ClientSessionStatus get()
			{
				return static_cast<enum class ClientSessionStatus>(m_NativeObj->get_status());
			}
		}

		//-- ��װ�ӿ�
		void Update(float DeltaTime)
		{
			m_NativeObj->update(DeltaTime);
		}

		// ��Ϸ�����������Ϣ
		void AddListener(INetMessageListener^ listener)
		{
			m_NativeObj->add_listener( 
				nexus::msg_listener_ptr(new GameMessageListener(listener))
				);
		}

		void ClearListeners()
		{
			m_NativeObj->clear_listeners();
		}

		// �����ͻ��ˣ���ʼ����Դ
		bool Startup(UInt32 timeout, bool enableNagle)
		{
			return m_NativeObj->startup(timeout,enableNagle);
		}

		// �رտͻ��ˣ��ú�������ִ���̲߳���ǿ�ƹرգ����������߳��йر�
		void Shutdown()
		{
			m_NativeObj->shutdown();
		}
		
		// �첽���ӷ�����������״��ͨ���ص�����on_connect����
		bool Connect(System::String^ ipAddress, UINT16 port)
		{
			pin_ptr<const wchar_t> szIP = PtrToStringChars(ipAddress);		
			return m_NativeObj->connect_to(nexus::conv_string(szIP), port);
		}
	
		// �ر�������������ӣ��رճɹ���ص�on_disconnect����
		void Close()
		{
			m_NativeObj->close();
		}

		// ������Ϣ�ṹ�����ת��
		bool SendMessage(const void* message, UINT32 size)
		{
			return m_NativeObj->send(message, size);
		}

	private:
		// �Լ�����Native�����������
		nexus::client_connection*  m_NativeObj;

	};
}

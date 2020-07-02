#ifndef _NEXUS_SERVER_CALL_BACK_H_
#define _NEXUS_SERVER_CALL_BACK_H_
#include <boost/function.hpp>
#include "net_packet.h"

namespace nexus
{
	/** ��������״̬ö�� */
	enum EConnectionState
	{
		ECS_Invalid,		// ��Ч״̬
		ECS_Connecting,		// ����������
		ECS_Connected,		// �Ѿ����Ӻ�
		ECS_ConnectTimeout,	// ���ӳ�ʱ
		ECS_Disconnected,	// �Ѿ��Ͽ�����
		ECS_Error,			// ��������
	};

	typedef boost::function<void (enum EConnectionState)> conn_state_call_back;

	// tcp connection
	typedef boost::function<void (nnet_packet_ptr)>	recv_handler;	
}//namespace nexus

#endif // _NEXUS_SERVER_CALL_BACK_H_
#include "nmsg_dispatch.h"
#include "nlan_client.h"
#include "nwan_server.h"
#include "nlan_session.h"
#include "nwan_session.h"
#include "nnetwork_def.h"
#include "nprotocol_base.h"
#include "nlog_mt.h"
#include "nenums.h"

#include "nmsg_cluster.h"
#include "nmsg_world_gateway.h"
#include "nmsg_login.h"
#include "nmsg_char.h"
#include "nmsg_chat.h"

#include "nmsg_world_login.h"

namespace nexus {

	nmsg_dispatch::nmsg_dispatch() : m_server_id(0)
	{
	}

	nmsg_dispatch::~nmsg_dispatch()
	{

	}

	bool nmsg_dispatch::init()
	{
		// ע��Ĭ����Ϣ������
		for(uint16 i = 0; i < Msg_Max; ++i)
		{
			_register_lan2wan_default(i);
			_register_wan2lan_default(i);
		}

		// �������ڲ���Ϣ
		_register_lan2wan_msg_handler(Msg_Reply_Login,		_T("Msg_Reply_Login"),		EWSS_Connected, &nmsg_dispatch::_handle_reply_login);
		_register_lan2wan_msg_handler(Msg_G2W_Server_Info,	_T("Msg_G2W_Server_Info"),	EWSS_Connected, &nmsg_dispatch::_handle_world_info);
		_register_lan2wan_msg_handler(Msg_G2W_Broadcast,	_T("Msg_G2W_Broadcast"),	EWSS_Connected, &nmsg_dispatch::_handle_ngateway_broadcast);

		// �ͻ�����Ϣ
		_register_wan2lan_msg_handler(C2S_LoginProof,		_T("C2S_LoginProof"),		EWSS_Connected,	&nmsg_dispatch::_handle_ngateway_to_nlogin);
		_register_wan2lan_msg_handler(C2S_CharLeaveGame,	_T("C2S_CharLeaveGame"),	EWSS_Connected | EWSS_Logined, &nmsg_dispatch::handle_disconnect);

		_register_wan2lan_msg_handler(C2S_ChatMessage,		_T("C2S_ChatMessage"),		EWSS_Logined,	&nmsg_dispatch::_handle_c2s_chat_message);
		_register_lan2wan_msg_handler(S2C_ChatMessage,		_T("S2C_ChatMessage"),		EWSS_Logined,	&nmsg_dispatch::_handle_s2c_chat_message);


		_register_lan2wan_msg_handler(S2C_LoginProofResult,	_T("S2C_LoginProofResult"),	EWSS_Connected, &nmsg_dispatch::_handle_player_login);

		return true;
	}

	void nmsg_dispatch::destroy()
	{

	}

	// ѹ���ϴ����ݰ�ȫ��У����
	void nmsg_dispatch::do_lan2wan_dispatch(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));

		// У��msg_id
		if (packet->msg_id >= Msg_Max)
		{
			// ע�����msg���ͷ�
			msg_ptr->free();
			// lan �ڲ�����������Ϣ�󲻶Ͽ�����������ʱ���ᷢ����log error
			NLOG_ERROR(_T("error msg id, session_id = %u"), session_id);
			return;
		}

		// ע���޸���Ϣ����- uint64 client_id
		(this->*(m_l2w_msg_handler[packet->msg_id].handler))(session_id, msg_ptr, size);
		// ͳ����Ϣ����
		::InterlockedIncrement((LPLONG)&(m_l2w_msg_handler[packet->msg_id].handle_count));
	}

	// ѹ����С�����ݲ���ȫ��У���
	void nmsg_dispatch::do_wan2lan_dispatch(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		const nmsg_base* packet = reinterpret_cast<const nmsg_base*>(&(msg_ptr->session_id));

		// У��msg_id
		if (packet->msg_id >= Msg_Max)
		{
			// ע�����msg���ͷ�
			msg_ptr->free();
			// �ر�����
			g_wan_server.close(session_id);
			// log close ��������ʱ�������log��ϢҪ��debug�´�ӡ����������±�֤��������������
			NLOG_DEBUG(_T("error msg id, session_id = %u"), session_id);
			return;
		}

		// У��ngateway���server id, ת��ʱ��Ҫ��װ��id
		if (0 == m_server_id)
		{
			// ע�����msg���ͷ�
			msg_ptr->free();
			// �ر�����
			g_wan_server.close(session_id);
			return;
		}

		// У����Ϣ����
		// TODO: D.Zhao ���Բ�У�飬�ɾ����߼�������У��

		// У��״̬
		nwan_session* session_ptr = g_wan_server.get_session(session_id);

		if (!session_ptr)
		{
			// ע�����msg���ͷ�
			msg_ptr->free();
			// �ر�����
			g_wan_server.close(session_id);
			NLOG_WARNING(_T("get session error!, session_id = %u"), session_id);
			return;
		}

		if (0 == (m_w2l_msg_handler[packet->msg_id].status & session_ptr->get_status()))
		{
			// ע�����msg���ͷ�
			msg_ptr->free();
			// �ر�����
			g_wan_server.close(session_id);
			NLOG_WARNING(_T("session status error!, session_id = %u, msg_status = %u, session_status = %u, msg id = %u"), session_id, 
				m_w2l_msg_handler[packet->msg_id].status, session_ptr->get_status(), packet->msg_id);
			return;
		}

		// У�鷢��Ƶ��
		// TODO: D.Zhao, ������ƶ�����Ҫ!!!

		// ���server id, ע����Ϣ���ȵı仯, - uint64 client_id
		msg_ptr->server_id = m_server_id;
		(this->*(m_w2l_msg_handler[packet->msg_id].handler))(session_id, msg_ptr, size + sizeof(uint64));
		// ͳ����Ϣ����
		::InterlockedIncrement((LPLONG)&(m_w2l_msg_handler[packet->msg_id].handle_count));
	}

	void nmsg_dispatch::handle_disconnect(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		if (msg_ptr)
		{
			msg_ptr->free();
		}

		// ����session״̬�ж��Ƿ񷢸�login����world
		nwan_session* session_ptr = g_wan_server.get_session(session_id);

		if (!session_ptr)
		{
			NLOG_WARNING(_T("get session error!, session_id = %u"), session_id);
			return;
		}

		tagMsg_Player_Logout request;
		request.client_id = MAKE_PAIR64(session_id, m_server_id);

		// TODO: D. Zhao ����login��������session״̬��Ҫ�ḻ�������˳�����������
		if (EWSS_Connected == session_ptr->get_status())
		{
			g_lan_client.send_nlogin_msg(&request, sizeof(request));
		}
		else if (EWSS_Logined == session_ptr->get_status())
		{
			// world or & bg
			g_lan_client.send_nworld_msg(&request, sizeof(request));
		}
		else
		{
			NLOG_ERROR(_T("state error when handle disconnect!, session_id = %u"), session_id);
		}

		session_ptr->set_status(EWSS_Closed);
	}

	void nmsg_dispatch::_register_lan2wan_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, handler_type handler)
	{
		m_l2w_msg_handler[msg_id].msg_name		= msg_name;
		m_l2w_msg_handler[msg_id].status		= status;
		m_l2w_msg_handler[msg_id].handle_count	= 0;
		m_l2w_msg_handler[msg_id].handler		= handler;
	}

	void nmsg_dispatch::_register_wan2lan_msg_handler(uint16 msg_id, const tstring& msg_name, uint32 status, handler_type handler)
	{
		m_w2l_msg_handler[msg_id].msg_name		= msg_name;
		m_w2l_msg_handler[msg_id].status		= status;
		m_w2l_msg_handler[msg_id].handle_count	= 0;
		m_w2l_msg_handler[msg_id].handler		= handler;
	}

	void nmsg_dispatch::_register_lan2wan_default(uint16 msg_id)
	{
		m_l2w_msg_handler[msg_id].msg_name		= _T("l2w_default");
		m_l2w_msg_handler[msg_id].status		= EWSS_Logined;
		m_l2w_msg_handler[msg_id].handle_count	= 0;
		m_l2w_msg_handler[msg_id].handler		= &nmsg_dispatch::_handle_ncluster_to_ngateway;
	}

	void nmsg_dispatch::_register_wan2lan_default(uint16 msg_id)
	{
		m_w2l_msg_handler[msg_id].msg_name		= _T("w2l_default");
		m_w2l_msg_handler[msg_id].status		= EWSS_Logined;
		m_w2l_msg_handler[msg_id].handle_count	= 0;
		m_w2l_msg_handler[msg_id].handler		= &nmsg_dispatch::_handle_ngateway_to_nworld;
	}

	void nmsg_dispatch::_handle_ngateway_to_nworld(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		if (false == g_lan_client.send_nworld(msg_ptr, size))
		{
			NLOG_NERROR(_T("send nworld error, may be buffer overflow or connect close!"));
		}
	}

	void nmsg_dispatch::_handle_ngateway_to_nlogin(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		if (false == g_lan_client.send_nlogin(msg_ptr, size))
		{
			NLOG_NERROR(_T("send nworld error, may be buffer overflow or connect close!"));
		}
	}

	void nmsg_dispatch::_handle_ngateway_to_nworld_or_nbg(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{

	}

	void nmsg_dispatch::_handle_ncluster_to_ngateway(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		if (false == g_wan_server.send(msg_ptr->session_id, msg_ptr, size - sizeof(uint64)))
		{
			// ��������ʱ�����
			NLOG_DEBUG(_T("send client error, may be buffer overflow or connect close!"));
			// �ߵ�����������Ŀͻ���
			g_wan_server.close(msg_ptr->session_id);
		}
	}

	void nmsg_dispatch::_handle_ngateway_broadcast(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		tagMsg_G2W_Broadcast* packet = reinterpret_cast<tagMsg_G2W_Broadcast*>(&(msg_ptr->session_id));
		// �޸�msg_id
		packet->msg_id = packet->get_original_msg_id();
		uint16 num = packet->get_client_num();

		// ����session�㲥
		if (0 == num)
		{
			g_wan_server.broadcast(msg_ptr, packet->get_msg_size() - sizeof(uint64));
			return;
	}

		msg_ptr->reference = uint32(num);

		for (uint16 i = 0; i < num; ++i)
		{
			if (false == g_wan_server.send(packet->get_client_id(i), msg_ptr, packet->get_msg_size() - sizeof(uint64)))
			{
				// ��������ʱ�����
				NLOG_DEBUG(_T("send client error, may be buffer overflow or connect close!"));
				// �ߵ�����������Ŀͻ���
				g_wan_server.close(msg_ptr->session_id);
			}
		}
	}

	void nmsg_dispatch::_handle_reply_login(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		const tagMsg_Reply_Login* packet = reinterpret_cast<const tagMsg_Reply_Login*>(&(msg_ptr->session_id));
		nlan_session* session_ptr = g_lan_client.get_session(session_id);

		if (!session_ptr)
		{
			msg_ptr->free();
			NLOG_ERROR(_T("get session error, session_id = %u"), session_id);
			return;
		}

		if (EError_Success == packet->error_code)
		{
			session_ptr->set_status(ELSS_Logined);
			NLOG_INFO(_T("login server sucess, server_type = %u"), session_ptr->get_type());
		}
		else
		{
			NLOG_ERROR(_T("login server errror, server_type = %u"), session_ptr->get_type());
		}

		// ע����Ϣ���ͷ�
		msg_ptr->free();
	}

	void nmsg_dispatch::_handle_world_info(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		const tagMsg_G2W_Server_Info* packet = reinterpret_cast<const tagMsg_G2W_Server_Info*>(&(msg_ptr->session_id));
		m_server_id = packet->server_id;

		if (false == g_lan_client.connect_to_nlogin(packet->login_ip, packet->login_port))
		{
			NLOG_ERROR(_T("request connect to nlogin server errror, ip = %u, port = %u"), packet->login_ip, packet->login_port);
		}

		if (false == g_lan_client.connect_to_nbg(packet->bg_ip, packet->bg_port))
		{
			NLOG_ERROR(_T("request connect to nbg server errror, ip = %u, port = %u"), packet->bg_ip, packet->bg_port);
		}

		// ע����Ϣ���ͷ�
		msg_ptr->free();
	}

	void nmsg_dispatch::_handle_player_login(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		// �޸�session״̬
		nwan_session* session_ptr = g_wan_server.get_session(msg_ptr->session_id);

		if (!session_ptr)
		{
			// ע�����msg���ͷ�
			msg_ptr->free();
			// �ر�����
			g_wan_server.close(session_id);
			NLOG_WARNING(_T("get session error!, session_id = %u"), session_id);
			return;
		}

		session_ptr->set_status(EWSS_Logined);

		if (false == g_wan_server.send(msg_ptr->session_id, msg_ptr, size - sizeof(uint64)))
		{
			// ��������ʱ�����
			NLOG_DEBUG(_T("send client error, may be buffer overflow or connect close!"));
			// �ߵ�����������Ŀͻ���
			g_wan_server.close(msg_ptr->session_id);
		}
	}

	void nmsg_dispatch::_handle_c2s_chat_message(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		tagC2S_ChatMessage* packet = reinterpret_cast<tagC2S_ChatMessage*>(&(msg_ptr->session_id));
		// ���ʱ���
		if (ECT_Channel == packet->type)
		{
			tagTimeStamp* data_ptr = reinterpret_cast<tagTimeStamp*>(packet->msg3.msg);
			data_ptr->time_stamp1 = ::timeGetTime();
		}

		_handle_ngateway_to_nworld(session_id, msg_ptr, size);
	}

	void nmsg_dispatch::_handle_s2c_chat_message(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		tagS2C_ChatMessage* packet = reinterpret_cast<tagS2C_ChatMessage*>(&(msg_ptr->session_id));
		// ���ʱ���
		if (ECT_Channel == packet->type)
		{
			tagTimeStamp* data_ptr = reinterpret_cast<tagTimeStamp*>(packet->msg2.msg);
			data_ptr->time_stamp2 = ::timeGetTime();
		}

		_handle_ncluster_to_ngateway(session_id, msg_ptr, size);
	}


	nmsg_dispatch g_msg_dispatch;

} // namespace nexus

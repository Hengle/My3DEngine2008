#include "nlan_client.h"
#include "nlan_session.h"
#include "nlog_mt.h"
#include "nenums.h"
#include "ncfg.h"
#include "ninit_flag.h"

#include "nmsg_cluster.h"
#include "nmsg_world_login.h"

namespace nexus {

	const int32 RETRY_TIME = 10 * 1000;		// 10s

	nlan_client::nlan_client() : m_session_ptrs(NULL), m_max_session(0), m_nlogin_session_id(0XFFFFFFFF), m_nbg_session_id(0XFFFFFFFF), 
		m_retry_nlogin_elapse(0), m_retry_nbg_elapse(0)
	{
	}

	nlan_client::~nlan_client()
	{
		destroy();
	}

	bool nlan_client::init(uint32 max_session)
	{
		if (0 == max_session || max_session >= 0xFF)
		{
			NLOG_ERROR(_T("invaild max session num, max_session = %u"), max_session);
			return false;
		}

		m_max_session = max_session;

		m_session_ptrs = new nlan_session[m_max_session];

		if (!m_session_ptrs)
		{
			NLOG_ERROR(_T("allocate sesssion memory error!"));
			return false;
		}

		return true;
	}

	void nlan_client::destroy()
	{
		if (m_session_ptrs)
		{
			delete []m_session_ptrs;
			m_session_ptrs = NULL;
		}
	}

	void nlan_client::update(uint32 elapse)
	{
		// update��������
		if (m_retry_nlogin_elapse > 0)
		{
			m_retry_nlogin_elapse -= elapse;

			if (m_retry_nlogin_elapse <= 0)
			{
				if (false == connect_to_nlogin())
				{
					NLOG_ERROR(_T("%s, connect nlogin request error!"), __TFUNCTION__);
					m_retry_nlogin_elapse = RETRY_TIME;
				}
			}
		}

		// Ŀǰֻ����һ��bg�����
		// TODO: D. Zhao ����ж��bg�Ͽ��������ӵģ�����Ҫ�����̰߳�ȫ�����Ӷ���
		if (m_retry_nbg_elapse > 0)
		{
			m_retry_nbg_elapse -= elapse;

			if (m_retry_nbg_elapse <= 0)
			{
				if (false == connect_to_nbg())
				{
					NLOG_ERROR(_T("%s, connect nbg request error!"), __TFUNCTION__);
					m_retry_nbg_elapse = RETRY_TIME;
				}
			}
		}

		// update session
		for (uint32 i = 0; i < m_max_session; ++i)
		{
			if (m_session_ptrs[i].is_in_used())
			{
				m_session_ptrs[i].update(elapse);
			}
		}
	}

	void nlan_client::on_connect(uint32 session_id, uint32 param1, uint32 param2)
	{
		// ע������
		if (m_nlogin_session_id == session_id)
		{
			_handle_connect_nlogin(param1);
		}
		else
		{
			_handle_connect_nbg(param1);
		}
	}

	// ע���̰߳�ȫ, ���е��ú���������������ʱ��nlan_client���ӵ���login��bg���Ƿ������ڲ���Ϣ�����طŵ�player session��
	void nlan_client::on_datarecv(uint32 session_id, nmessage* msg_ptr, uint32 size)
	{
		nlan_session* session_ptr = get_session(session_id);

		if (!session_ptr)
		{
			msg_ptr->free();
			NLOG_ERROR(_T("get session error, session_id = %u"), session_id);
			return;
		}

		if (false == session_ptr->push_msg(msg_ptr))
		{
			msg_ptr->free();
			NLOG_ERROR(_T("nlan_session msg queue overflow!, session_id = %u"), session_id);
			return;
		}
	}

	void nlan_client::on_disconnect(uint32 session_id)
	{
		// ע������
		if (m_nlogin_session_id == session_id)
		{
			_handle_close_nlogin();
		}
		else
		{
			_handle_close_nbg();
		}
	}

	// world��bg�ṩ��ͬ�Ľӿڲ�ͬ�Ĺ���
	bool nlan_client::send_nlogin(const void* msg_ptr, uint32 size)
	{
#ifndef _NBG
		if (false == g_lan_client.send_msg(m_nlogin_session_id, msg_ptr, size))
		{
			NLOG_NERROR(_T("send login messgae error, maybe send buffer overflow or connect close! session_id = %u"), m_nlogin_session_id);
			return false;
		}
#endif
		return true;
	}

	bool nlan_client::send_nbg(const void* msg_ptr, uint32 size)
	{
#ifndef _NBG
		if (false == g_lan_client.send_msg(m_nbg_session_id, msg_ptr, size))
		{
			NLOG_NERROR(_T("send bg messgae error, maybe send buffer overflow or connect close! session_id = %u"), m_nbg_session_id);
			return false;
		}
#endif
		return true;
	}

	nlan_session* nlan_client::get_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return NULL;
		}

		// У��id, �����log����������ʱ�����
		if (session_id != m_session_ptrs[index].get_sesssion_id())
		{
			return NULL;
		}

		return &(m_session_ptrs[index]);
	}

	bool nlan_client::connect_to_nlogin()
	{
		std::string ip = sncfg.get_string(ECI_NLoginIp);
		uint32 value = sncfg.get_uint32(ECI_NLoginPort);

		// ���ط������ӵ�session_id,id�ȵ���֤�����ʹ��
		m_nlogin_session_id = g_lan_client.connect_to(ip, uint16(value));

		if (0XFFFFFFFF == m_nlogin_session_id)
		{
			NLOG_ERROR(_T("connect to nlogin error!, ip = %s, port = %u"), ip.c_str(), value);
			m_retry_nlogin_elapse = RETRY_TIME;
			return false;
		}

		return true;
	}

	bool nlan_client::connect_to_nbg()
	{
		// TODO: D. Zhao ���nbg������
		g_init_flag.set_init_flag(EIF_Bg);
		return true;

		std::string ip = sncfg.get_string(ECI_NBgIp);
		uint32 value = sncfg.get_uint32(ECI_NBgPort);

		// ���ط������ӵ�session_id,id�ȵ���֤�����ʹ��
		m_nbg_session_id = g_lan_client.connect_to(ip, uint16(value));

		if (0XFFFFFFFF == m_nbg_session_id)
		{
			NLOG_ERROR(_T("connect to nworld error!, ip = %s, port = %u"), ip.c_str(), value);
			m_retry_nbg_elapse = RETRY_TIME;
			return false;
		}

		return true;
	}

	bool nlan_client::_register_session(uint32 session_id, uint32 server_type, const std::string& address, uint16 port)
	{
		// ע������
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return false;
		}

		// �ж��Ƿ�����ʹ��, ͬһsession��on_connect��on_disconnect�ײ㱣֤����
		if (m_session_ptrs[index].is_in_used())
		{
			NLOG_ERROR(_T("this session id is in use!, index = %u, session_id = %u"), index, session_id);
			return false;
		}

		return m_session_ptrs[index].init(session_id, server_type, address, port);
	}

	void nlan_client::_unregister_session(uint32 session_id)
	{
		uint32 index = session_id & 0X0000FFFF;

		if (index >= m_max_session)
		{
			NLOG_ERROR(_T("invaild session id, index = %u, session_id = %u"), index, session_id);
			return;
		}

		m_session_ptrs[index].destroy();
	}

	void nlan_client::_handle_connect_nlogin(uint32 param)
	{
		if (ECC_Connected != param)
		{
			// ����update��������
			m_retry_nlogin_elapse = RETRY_TIME;
			m_nlogin_session_id = 0XFFFFFFFF;
			NLOG_ERROR(_T("connect login error, error_code = %u, retry connect login 10s later"), param);
			return;
		}

		// ע������
		_register_session(m_nlogin_session_id, EST_Login, sncfg.get_string(ECI_NLoginIp), sncfg.get_uint32(ECI_NLoginPort));

		// ����login��Ϣ
		tstring world_name = sncfg.get_tstring(ECI_WorldName);
		uint32 min_len = uint32(world_name.length());

		if (min_len > MAX_SERVER_NAME_LEN)
		{
			min_len = MAX_SERVER_NAME_LEN;
		}

		tagMsg_W2L_World_Request_Login request;
		request.server_id	= sncfg.get_uint32(ECI_ServerId);
		memcpy(request.world_name, world_name.c_str(), sizeof(nchar) * min_len);
		send_msg(m_nlogin_session_id, &request, sizeof(request));
	}

	void nlan_client::_handle_connect_nbg(uint32 param)
	{
		if (ECC_Connected != param)
		{
			// ����update��������,ip��portʹ��m_retry_bg_ip��m_retry_bg_port
			m_retry_nlogin_elapse = RETRY_TIME;
			m_nbg_session_id = 0XFFFFFFFF;
			return;
		}

		// ע������
		_register_session(m_nbg_session_id, EST_Bg, sncfg.get_string(ECI_NBgIp), sncfg.get_uint32(ECI_NBgPort));
		// ����login��Ϣ
		tagMsg_Request_Login request;
		request.server_type = EST_World;
		request.server_id	= sncfg.get_uint32(ECI_ServerId);
		send_msg(m_nlogin_session_id, &request, sizeof(request));
	}

	void nlan_client::_handle_close_nlogin()
	{
		_unregister_session(m_nlogin_session_id);
		m_nlogin_session_id = 0XFFFFFFFF;
		NLOG_ERROR(_T("disconnect with nlogin server, 10s later reconnect!"));
	}

	void nlan_client::_handle_close_nbg()
	{
		_unregister_session(m_nbg_session_id);
		m_nbg_session_id = 0XFFFFFFFF;
		NLOG_ERROR(_T("disconnect with nlogin server, 10s later reconnect!"));
	}

	nlan_client g_lan_client;

} // namespace nexus

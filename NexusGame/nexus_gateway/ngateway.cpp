#include "ngateway.h"
#include "nlog_mt.h"
#include "ncfg.h"
#include "nmsg_dispatch.h"
#include "nlan_client.h"
#include "nwan_server.h"

#include <signal.h>
#include <iostream>

namespace nexus {

	const uint32 TICK_TIME = 200;

	volatile LONG ngateway::m_terminate = 0;

	ngateway::ngateway()
	{
	}

	ngateway::~ngateway()
	{
	}

	bool ngateway::init()
	{
		NLOG_STRING(_T("nexus ngateway startup!"));
		// log ��ʼ��
		if (false == NLOG.init(_T("ngateway"), ELT_Debug, ELT_Debug))
		{
			std::cout << "init log error!" <<std::endl;
			return false;
		}

		NLOG_STRING(_T("nexus ngateway log init success!"));

		// config ��ʼ��
		if (false == sncfg.init(_T("ngateway.cfg"), ECI_Max))
		{
			NLOG_ERROR(_T("%s, sncfg init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway config init success!"));

		// ��Ϣע��
		if (false == g_msg_dispatch.init())
		{
			NLOG_ERROR(_T("%s, nmsg_dispatch init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway message dispatch init success!"));

		uint32 value = sncfg.get_uint32(ECI_MaxLanSession);
		// nlan_client ��ʼ��,ע���ȳ�ʼ����startup
		if (false == g_lan_client.init(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway LAN client init success!"));

		value = sncfg.get_uint32(ECI_MaxSession);
		// nwan_server ��ʼ��,ע���ȳ�ʼ����startup
		if (false == g_wan_server.init(value))
		{
			NLOG_ERROR(_T("%s, nwan_server init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway WAN server init success!"));

		value = sncfg.get_uint32(ECI_MaxLanSession);
		// LAN����
		if (false == g_lan_client.startup(value))
		{
			NLOG_ERROR(_T("%s, nlan_client init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway LAN client startup success!"));

		// WAN����
		ntcp_server_config param;
		param.port = uint16(sncfg.get_uint32(ECI_WanPort));
		param.max_session = sncfg.get_uint32(ECI_MaxSession);
		param.minilzo = (sncfg.get_int32(ECI_Minilzo) == 1) ? true : false;

		if (false == g_wan_server.startup(param))
		{
			NLOG_ERROR(_T("%s, nwan_server init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway WAN server startup success!"));

		// ��ʼ�����������Ϣ

		// ����update�߳�
		m_terminate = 0;
		m_update_thread_ptr.reset(new boost::thread(boost::bind(&ngateway::_update_thread, this)));

		NLOG_STRING(_T("nexus ngateway update thread init success!"));

		// ͨ�������ļ�ngatewayֻ����nworld, nlogin��nbg����nworld������Ӧip��port��������
		if (false == g_lan_client.connect_to_nworld())
		{
			NLOG_ERROR(_T("%s, connect nworld request error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus ngateway connect to world ..."));
		return true;
	}

	void ngateway::destroy()
	{
		// update�߳��˳�
		m_terminate = 1;
		m_update_thread_ptr->join();
		NLOG_STRING(_T("nexus ngateway update thread close!"));
		// WAN�ر�
		g_wan_server.shutdown();
		NLOG_STRING(_T("nexus ngateway WAN server shutdown!"));
		// LAN�ر�
		g_lan_client.shutdown();
		NLOG_STRING(_T("nexus ngateway LAN client shutdown!"));
		// wan session ����
		g_wan_server.destroy();
		NLOG_STRING(_T("nexus ngateway WAN server destroy!"));
		// lan session ����
		g_lan_client.destroy();
		NLOG_STRING(_T("nexus ngateway LAN client destroy!"));
		// ��Ϣ�ɷ�
		g_msg_dispatch.destroy();
		NLOG_STRING(_T("nexus ngateway message dispatch destroy!"));
		// �����ļ��˳�
		sncfg.destroy();
		NLOG_STRING(_T("nexus ngateway config destroy!"));
		// log�˳�
		NLOG.destroy();
	}

	bool ngateway::run()
	{
		_hook_signals();

		while (0 == m_terminate)
		{

			::Sleep(200);
		}

		_hook_signals();
		return true;
	}

	void ngateway::stop()
	{
		m_terminate = 1;
	}

	void ngateway::_update_thread()
	{
		::timeBeginPeriod(1);
		uint32 elapse = 0;
		uint32 cur_time = 0;
		uint32 pre_time = ::timeGetTime();

		while (0 == m_terminate)
		{
			cur_time = ::timeGetTime();
			elapse = cur_time - pre_time;
			pre_time = cur_time;

			g_wan_server.update(elapse);
			g_lan_client.update(elapse);

			cur_time = ::timeGetTime();

			if (cur_time - pre_time < TICK_TIME)
			{
				::Sleep(TICK_TIME - cur_time + pre_time);
			}
		}

		::timeEndPeriod(1);
	}

	void ngateway::_on_signal(int s)
	{
		switch (s)
		{
		case SIGINT:
		case SIGTERM:
		case SIGBREAK:
			{
				stop();
			}
			break;
		}

		signal(s, _on_signal);
	}

	void ngateway::_hook_signals()
	{
		signal(SIGINT, _on_signal);
		signal(SIGTERM, _on_signal);
		signal(SIGBREAK, _on_signal);
	}

	void ngateway::_unhook_signals()
	{
		signal(SIGINT, 0);
		signal(SIGTERM, 0);
		signal(SIGBREAK, 0);
	}

} // namespace nexus

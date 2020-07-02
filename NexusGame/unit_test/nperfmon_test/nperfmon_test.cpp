#include "nperfmon_test.h"
#include "nclient.h"
#include "nworld_session.h"
#include "nmsg_dispatch.h"
#include "ncommands.h"
#include "nmap_mgr.h"
#include "nlog_mt.h"
#include "ncfg.h"
#include "nutil.h"

#include <signal.h>
#include <iostream>

namespace nexus {

	const uint32 TICK_TIME = 100;

	volatile LONG nperfmon_test::m_terminate = 0;

#ifdef _UNICODE
	#define tcout wcout
	#define tcin wcin
#else
	#define tcout cout
	#define tcin cin
#endif

	nperfmon_test::nperfmon_test()
	{
	}

	nperfmon_test::~nperfmon_test()
	{
	}

	bool nperfmon_test::init()
	{
		srand(NULL);
		NLOG_STRING(_T("nexus nperfmon_test startup!"));
		// log ��ʼ��
		if (false == NLOG.init(_T("nperfmon_test"), ELT_Debug, ELT_Debug))
		{
			std::tcout << _T("init log error!") <<std::endl;
			return false;
		}

		NLOG_STRING(_T("nexus nperfmon_test log init success!"));

		// config ��ʼ��
		if (false == sncfg.init(_T("nperfmon_test.cfg"), ECI_Max))
		{
			NLOG_ERROR(_T("%s, sncfg init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nperfmon_test config init success!"));

		// nutil�ĳ�ʼ��
		s_util.init();
		NLOG_STRING(_T("nexus nperfmon_test nutil init success!"));

		if (false == _init_game())
		{
			NLOG_ERROR(_T("%s, game init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nperfmon_test game init success!"));

		// ��Ϣע��
		if (false == scommands.init())
		{
			NLOG_ERROR(_T("%s, scommands init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nperfmon_test scommands init success!"));

		// cmdע��
		if (false == g_msg_dispatch.init())
		{
			NLOG_ERROR(_T("%s, nmsg_dispatch init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nperfmon_test message dispatch init success!"));

		uint32 max_session = sncfg.get_uint32(ECI_MaxSession);
		// LAN����
		if (false == g_client.startup(max_session))
		{
			NLOG_ERROR(_T("%s, nclient init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nperfmon_test nclient startup success!"));

		// ��ʼ�����������Ϣ

		// ����update�߳�
		m_terminate = 0;
		m_threads.create_thread(boost::bind(&nperfmon_test::_update_thread, this));
		m_threads.create_thread(boost::bind(&nperfmon_test::_cmd_line_thread, this));

		NLOG_STRING(_T("nexus nperfmon_test update thread init success!"));
		return true;
	}

	void nperfmon_test::destroy()
	{
		// update�߳��˳�
		m_terminate = 1;
		m_threads.join_all();
		NLOG_STRING(_T("nexus nperfmon_test update thread close!"));
		// client�ر�
		g_client.shutdown();
		NLOG_STRING(_T("nexus nperfmon_test LAN server shutdown!"));
		// ��Ϣ�ɷ�
		g_msg_dispatch.destroy();
		NLOG_STRING(_T("nexus nperfmon_test message dispatch destroy!"));
		// nutil������
		s_util.destroy();
		NLOG_STRING(_T("nexus nperfmon_test nutil destroy!"));
		// �����ļ��˳�
		sncfg.destroy();
		NLOG_STRING(_T("nexus nperfmon_test config destroy!"));
		// log�˳�
		NLOG.destroy();
	}

	bool nperfmon_test::run()
	{
		_hook_signals();

		while (0 == m_terminate)
		{

			::Sleep(200);
		}

		_hook_signals();
		return true;
	}

	void nperfmon_test::stop()
	{
		m_terminate = 1;
	}

	bool nperfmon_test::_init_game()
	{
		if (false == g_map_mgr.init())
		{
			NLOG_ERROR(_T("%s, nmap_mgr init error!"), __TFUNCTION__);
			return false;
		}

		NLOG_STRING(_T("nexus nperfmon_test nmap_mgr init success!"));
		return true;
	}

	void nperfmon_test::_update_thread()
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

			m_time_stamp += float(elapse) / 1000.0f;
			g_world_session.update(elapse);

			cur_time = ::timeGetTime();

			if (cur_time - pre_time < TICK_TIME)
			{
				::Sleep(TICK_TIME - cur_time + pre_time);
			}
		}

		::timeEndPeriod(1);
	}

	void nperfmon_test::_cmd_line_thread()
	{
		nchar command_buf[256];
		nmessage* msg_ptr = NULL;

		while (0 == m_terminate)
		{
			std::tcout.flush();

			std::tcin.getline(command_buf, sizeof(command_buf));
			nchar* command_str = command_buf;

			if (command_str != NULL)
			{
				for(uint32 i = 0; command_str[i]; ++i)
				{
					if (command_str[i] == _T('\r') || command_str[i] == _T('\n'))
					{
						command_str[i] = 0;
						break;
					}
				}

				if (0 == (*command_str))
				{
					std::tcout << _T("nexus>");
					continue;
				}

				msg_ptr = g_client.make_command_msg(command_str);

				if (msg_ptr)
				{
					g_world_session.push_cmd(msg_ptr);
				}
				else
				{
					std::tcout << _T("nexus>");
				}
			}
		}
	}

	void nperfmon_test::_on_signal(int s)
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

	void nperfmon_test::_hook_signals()
	{
		signal(SIGINT, _on_signal);
		signal(SIGTERM, _on_signal);
		signal(SIGBREAK, _on_signal);
	}

	void nperfmon_test::_unhook_signals()
	{
		signal(SIGINT, 0);
		signal(SIGTERM, 0);
		signal(SIGBREAK, 0);
	}

} // namespace nexus

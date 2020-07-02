#include "StdAfx.h"
#include "WxApp.h"
#include "WxMainFrame.h"
#include "WxEngineConfigDlg.h"

nexus::nengine g_engine;

IMPLEMENT_APP(WxApp);

WxApp::WxApp(void)
{
}

WxApp::~WxApp(void)
{
}

bool WxApp::OnInit()
{
	g_engine.init_core();
	
	//-- ����config
	engine_config cfg;
	WxEngineConfigDlg cfgDlg(NULL);
	
	if( wxID_OK != cfgDlg.ShowModal() )
	{
		g_engine.close();
		return false;
	}

	cfgDlg.get_config(cfg);
	//-- ����������
	WxMainFrame* pFrame = new WxMainFrame;

	pFrame->create_frame(cfg.width, cfg.height);
	pFrame->SetClientSize(cfg.width, cfg.height);

	nTRACE(_T("create main frame succeeded.\r\n"));

	//-- ��ʼ������
	cfg.window_handle = pFrame->GetHandle();

	g_engine.init_modules(cfg);

	pFrame->create_main_level();

	//--
	pFrame->Show(true);
	SetTopWindow(pFrame);
	return true;
}
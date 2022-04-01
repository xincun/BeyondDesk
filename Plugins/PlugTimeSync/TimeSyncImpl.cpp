#include "stdafx.h"
#include "TimeSyncIntf.h"
#include "TimeSyncWnd.h"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"
#include <string>
#include "resource.h"

using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("TimeSync");
	gPlugInfo.Type = PLUG_TYPE_TIMETOOL;
	gPlugInfo.Desc = _T("时间同步插件");
	gPlugInfo.Order =102;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CTimeSyncPlug;
	p->OnCreate();

	return p;
}

CTimeSyncPlug::CTimeSyncPlug() : m_pWnd(NULL)
{
	m_sCaption = _T("时间同步");
	m_sHint = _T("和世界标准时间精准同步");
	m_sIconFile = _T("icon.png");
}

CTimeSyncPlug::~CTimeSyncPlug()
{

}

void CTimeSyncPlug::OnCreate()
{
	//Window::ShowMessage(_T("TimeSync Plugin Created..."));
}

void CTimeSyncPlug::OnDestroy()
{

}

void CTimeSyncPlug::OnUnload(void *pSender)
{
	//Window::ShowMessage(L"TimeSync Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CTimeSyncPlug::OnLoad(void *pSender)
{
	
}

void CTimeSyncPlug::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CTimeSyncPlug::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void CTimeSyncPlug::OnClick(void *pSender /*= NULL*/)
{
	if (!m_pWnd || !IsWindow(*m_pWnd))
	{
		m_pWnd = new CTimeSyncWnd(0);
		if (m_pWnd == NULL) return;

		//gUnify->GetPaintManager()->GetPaintWindow()
		//m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
		m_pWnd->Create(0, _T("时间同步"), UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标
		m_pWnd->CenterWindow();
		m_pWnd->ShowWindow();
	}
	else
	{
		if (IsWindow(*m_pWnd))
		{
			SetForegroundWindow(*m_pWnd);
		}
	}
}

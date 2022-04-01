#include "stdafx.h"
#include "DateCalcIntf.h"
#include "DateCalcWnd.h"
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

	gPlugInfo.Name = _T("DateCalc");
	gPlugInfo.Type = PLUG_TYPE_TIMETOOL;
	gPlugInfo.Desc = _T("日期计算插件");
	gPlugInfo.Order = 100;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CDateCalc;
	p->OnCreate();

	return p;
}

CDateCalc::CDateCalc() : m_pWnd(NULL)
{
	//对于DateCalc无用
	m_sCaption = _T("日期计算");
	m_sHint = _T("计算日期");
	m_sIconFile = _T("icon.png");
}

CDateCalc::~CDateCalc()
{

}

void CDateCalc::OnCreate()
{
	//Window::ShowMessage(_T("DateCalc Plugin Created..."));
}

void CDateCalc::OnDestroy()
{
	
	//Window::ShowMessage(L"DateCalc Plugin Destroyed...");
}

void CDateCalc::OnUnload(void *pSender)
{
	//Window::ShowMessage(L"DateCalc Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CDateCalc::OnLoad(void *pSender)
{
	
}

void CDateCalc::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CDateCalc::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void CDateCalc::OnClick(void *pSender /*= NULL*/)
{
	//Window::ShowMessage(L"clicked");

	if (!m_pWnd || !IsWindow(*m_pWnd))
	{
		m_pWnd = new CDateCalcWnd(0);

		if (m_pWnd == NULL) return;

		m_pWnd->m_pHisObj = this;
		//gUnify->GetPaintManager()->GetPaintWindow()
		//m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
		m_pWnd->Create(0, _T("日期计算"), UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标
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

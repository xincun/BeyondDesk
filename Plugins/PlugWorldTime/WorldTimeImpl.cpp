#include "stdafx.h"
#include "WorldTimeIntf.h"
#include "WorldTimeWnd.h"


using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("WorldTime");
	gPlugInfo.Type = PLUG_TYPE_TIMETOOL;
	gPlugInfo.Desc = _T("世界时间");
	gPlugInfo.Order = 101;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CWorldTime(_T("世界时间"), _T("查看选定时区的世界各个主要地区时间"), _T("icon.png"));
	p->OnCreate();

	return p;
}

CWorldTime::CWorldTime(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile) : m_sURL(_T("")), m_pWnd(NULL)
{
	m_sCaption = pstrCaption;//;
	m_sHint = pstrHint;// ;
	m_sIconFile = pstrIconFile;// ;
}

CWorldTime::~CWorldTime()
{

}

void CWorldTime::OnCreate()
{
	//Window::ShowMessage(_T("WorldTime Plugin Created..."));
}

void CWorldTime::OnDestroy()
{
	
}

void CWorldTime::OnUnload(void *pSender)
{
	
	//Window::ShowMessage(L"WorldTime Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CWorldTime::OnLoad(void *pSender)
{
	
}

void CWorldTime::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CWorldTime::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void CWorldTime::OnClick(void *pSender /*= NULL*/)
{
	//Window::ShowMessage(L"clicked");

	if (!m_pWnd || !IsWindow(*m_pWnd))
	{
		m_pWnd = new CWorldTimeWnd(0);
		if (m_pWnd == NULL) return;

		//gUnify->GetPaintManager()->GetPaintWindow()
		//m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
		m_pWnd->Create(0, m_sCaption.c_str(), UI_WNDSTYLE_FRAME, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标, UI_WNDSTYLE_FRAME可以改变大小，UI_WNDSTYLE_DIALOG无法改变大小
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

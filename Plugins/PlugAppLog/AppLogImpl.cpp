#include "stdafx.h"
#include "AppLogIntf.h"
#include "AppLogWnd.h"
#include <ShellAPI.h>

using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("AppLog");
	gPlugInfo.Type = PLUG_TYPE_NORMALTOOL;
	gPlugInfo.Desc = _T("任务日志");
	gPlugInfo.Order = 6;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	TString sPath = gUnify->App()->Path()->DBPath;
	sPath += _T("app.log");
	IPlugBase *p = new CAppLog(sPath.c_str(), _T("任务日志"), _T("查看定时任务执行日志"), _T("icon.png"));
	p->OnCreate();

	return p;
}

CAppLog::CAppLog(LPCTSTR pstrURL, LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile) : m_sURL(_T("")), m_pWnd(NULL)
{
	m_sURL = pstrURL;

	m_sCaption = pstrCaption;//;
	m_sHint = pstrHint;// ;
	m_sIconFile = pstrIconFile;// ;
}

CAppLog::~CAppLog()
{

}

void CAppLog::OnCreate()
{
	//Window::ShowMessage(_T("AppLog Plugin Created..."));
}

void CAppLog::OnDestroy()
{
	
}

void CAppLog::OnUnload(void *pSender)
{
	
	//Window::ShowMessage(L"AppLog Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CAppLog::OnLoad(void *pSender)
{
	
}

void CAppLog::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CAppLog::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void CAppLog::OnClick(void *pSender /*= NULL*/)
{
	ShellExecute(NULL, _T("open"), _T("notepad.exe"), m_sURL.c_str(), NULL, SW_SHOWNORMAL);
	//Window::ShowMessage(L"clicked");

	//if (!m_pWnd || !IsWindow(*m_pWnd))
	//{
	//	m_pWnd = new CAppLogWnd(0);
	//	if (m_pWnd == NULL) return;

	//	//gUnify->GetPaintManager()->GetPaintWindow()
	//	//m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
	//	m_pWnd->Create(0, m_sCaption.c_str(), UI_WNDSTYLE_FRAME, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标, UI_WNDSTYLE_FRAME可以改变大小，UI_WNDSTYLE_DIALOG无法改变大小
	//	m_pWnd->Navigate(m_sURL.c_str());
	//	m_pWnd->CenterWindow();
	//	m_pWnd->ShowWindow();
	//	SetForegroundWindow(*m_pWnd);
	//}
	//else
	//{
	//	if (IsWindow(*m_pWnd))
	//	{
	//		
	//			SetForegroundWindow(*m_pWnd);
	//		
	//	}
	//}
}

#include "stdafx.h"
#include "ScrRecIntf.h"
#include "ScrRecWnd.h"


using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("ScrRec");
	gPlugInfo.Type = PLUG_TYPE_MEDIATOOL;
	gPlugInfo.Desc = _T("媒体工具");

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CScrRec(_T("屏幕录像"), _T("录制屏幕动作"), _T("icon.png"));
	p->OnCreate();

	return p;
}


CScrRec::CScrRec(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile) : m_pWnd(NULL)
{
	m_sCaption = pstrCaption;//;
	m_sHint = pstrHint;// ;
	m_sIconFile = pstrIconFile;// ;
}

CScrRec::~CScrRec()
{

}

void CScrRec::OnCreate()
{
	//Window::ShowMessage(_T("ScrRec Plugin Created..."));
}

void CScrRec::OnDestroy()
{
	
}

void CScrRec::OnUnload(void *pSender)
{
	//Window::ShowMessage(L"ScrRec Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CScrRec::OnLoad(void *pSender)
{
	
}

void CScrRec::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CScrRec::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void CScrRec::OnClick(void *pSender /*= NULL*/)
{
	//Window::ShowMessage(L"clicked");

	if (!m_pWnd || !IsWindow(*m_pWnd))
	{
		m_pWnd = new CScrRecWnd(0);
		if (m_pWnd == NULL) return;

		//gUnify->GetPaintManager()->GetPaintWindow()
		//m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
		m_pWnd->Create(gUnify->App()->m_MainWnd, m_sCaption.c_str(), UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标
		m_pWnd->ShowWindow(false);

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

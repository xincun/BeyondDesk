#include "stdafx.h"
#include "MicRecIntf.h"
#include "MicRecWnd.h"


using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("MicRec");
	gPlugInfo.Type = PLUG_TYPE_MEDIATOOL;
	gPlugInfo.Desc = _T("媒体工具");

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CMicRec(_T("麦克风录音"), _T("用设定的麦克风录音"), _T("icon.png"));
	p->OnCreate();

	return p;
}


CMicRec::CMicRec(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile) : m_pWnd(NULL)
{
	m_sCaption = pstrCaption;//;
	m_sHint = pstrHint;// ;
	m_sIconFile = pstrIconFile;// ;
}

CMicRec::~CMicRec()
{

}

void CMicRec::OnCreate()
{
	//Window::ShowMessage(_T("MicRec Plugin Created..."));
}

void CMicRec::OnDestroy()
{
	
}

void CMicRec::OnUnload(void *pSender)
{
	//Window::ShowMessage(L"MicRec Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CMicRec::OnLoad(void *pSender)
{
	
}

void CMicRec::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CMicRec::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void CMicRec::OnClick(void *pSender /*= NULL*/)
{
	//Window::ShowMessage(L"clicked");

	if (!m_pWnd || !IsWindow(*m_pWnd))
	{
		m_pWnd = new CMicRecWnd(0);
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

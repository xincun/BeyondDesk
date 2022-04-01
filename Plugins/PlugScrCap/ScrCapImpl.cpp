#include "stdafx.h"
#include "ScrCapIntf.h"

#define  SCRCAP_DLL_NAME  _T("ScreenCap.dll")
typedef void (FAR __stdcall *ProcScrCap)(LPCSTR pszID, HWND h);

HINSTANCE g_hScrCap = NULL;;

using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("ScrCap");
	gPlugInfo.Type = PLUG_TYPE_MEDIATOOL;
	gPlugInfo.Desc = _T("媒体工具");

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CScrCap(_T("屏幕截图"), _T("截取屏幕选定区域"), _T("icon.png"));
	p->OnCreate();

	return p;
}


CScrCap::CScrCap(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile) : m_pWnd(NULL)
{
	m_sCaption = pstrCaption;//;
	m_sHint = pstrHint;// ;
	m_sIconFile = pstrIconFile;// ;

	m_bCanSet = true;
}

CScrCap::~CScrCap()
{

}

void CScrCap::OnCreate()
{
	//Window::ShowMessage(_T("ScrCap Plugin Created..."));
}

void CScrCap::OnDestroy()
{
	
}

void CScrCap::OnUnload(void *pSender)
{
	//Window::ShowMessage(L"ScrCap Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CScrCap::OnLoad(void *pSender)
{
	
}

void CScrCap::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	OnClick(pSender);
}

TString CScrCap::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void  CScrCap::OnSet(void* pSender) {

};

void CScrCap::OnClick(void *pSender /*= NULL*/)
{
	if (!g_hScrCap)
	{
		g_hScrCap = LoadLibrary(SCRCAP_DLL_NAME);
		if (!g_hScrCap) return;
	}

	ProcScrCap p = (ProcScrCap)::GetProcAddress(g_hScrCap, "CaptureScreen"); //
	p("id3232esfsdsuibian", gUnify->App()->m_MainWnd);
}

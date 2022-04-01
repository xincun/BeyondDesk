#include "stdafx.h"
#include "BootExport.h"
#include "BootWnd.h"
#include "AppPath.h"
#include "Event.h"
#include "SimpleTimer.h"
#include "CmdLine.h"


IPlugBase *gPlugObject = NULL;
IUnify *gUnify;
TPlugInfo gPlugInfo;

class CBoot : public IBootPlug
{
public:
	CBoot()
	{
		
	}
	virtual ~CBoot() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("Boot Plugin Created..."));
		/*int x = StaticTest::getN();
		int y = StaticTest::getn();*/
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"Boot Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"Boot Plugin Unloaded...");
		//gUnify->Core()->Timer()->StopTimer(this, 1);

		
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"Boot Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		return;
		CBootWnd *m_pAboutWnd = new CBootWnd(0);
		//if (m_pAboutWnd == NULL) return;

		////gUnify->GetPaintManager()->GetPaintWindow()
		////m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
		m_pAboutWnd->Create(0, _T("关于"), UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标
		m_pAboutWnd->CenterWindow();
		m_pAboutWnd->ShowModal();
		m_pAboutWnd = NULL;
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"Boot Plugin Invoked...");
		
	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("Boot");
	gPlugInfo.Type = PLUG_TYPE_BOOT;
	gPlugInfo.Desc = _T("启动操作插件");
	gPlugInfo.Order = -1;
	gPlugInfo.Persistent = true;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CBoot;
	p->OnCreate();
	gPlugObject = p;

	return p;
}

extern "C" PLUG_API void  FreePlugIntf()
{
	if (gPlugObject)
	{
		gPlugObject->OnDestroy();
		delete gPlugObject;
	}
}
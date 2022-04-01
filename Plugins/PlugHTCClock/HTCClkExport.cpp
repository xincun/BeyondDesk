#include "stdafx.h"
#include "HTCClkExport.h"
#include "HTCClkWnd.hpp"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;

class CHTCClk : public IToolPlug
{
public:
	CHTCClk()
	{
		m_sCaption = _T("HTC时钟");
		m_sHint = _T("HTC手机样式的时钟");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}

	virtual ~CHTCClk() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("HTCClk Plugin Created..."));
		
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"HTCClk Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"HTCClk Plugin Unloaded...");
		//CDeskWnd::Clear(_T("CHTCClkWnd")); //duilib自身清理
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"HTCClk Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

		CDeskWnd::RestoreFromDB<CHTCClkWnd>(_T("CHTCClkWnd"), this);
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"HTCClk Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		CDeskWnd::CreateNew<CHTCClkWnd>(this);

	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("HTCClock");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("桌面时钟插件");
	gPlugInfo.Order = 6;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CHTCClk;
	p->OnCreate();

	return p;
}
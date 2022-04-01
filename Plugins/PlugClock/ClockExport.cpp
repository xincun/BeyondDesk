#include "stdafx.h"
#include "ClockExport.h"
#include "ClockWnd.hpp"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;

class CClock : public IToolPlug
{
public:
	CClock()
	{
		m_sCaption = _T("模拟时钟");
		m_sHint = _T("显示表盘和指针的时钟");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}
	virtual ~CClock() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("Clock Plugin Created..."));
		
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"Clock Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"Clock Plugin Unloaded...");
		CDeskWnd::Clear(_T("CClockWnd"));
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"Clock Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

		CDeskWnd::RestoreFromDB<CClockWnd>(_T("CClockWnd"), this);
	
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"Clock Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		CDeskWnd::CreateNew<CClockWnd>(this);
		
	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("Clock");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("模拟时钟插件");
	gPlugInfo.Order = 3;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CClock;
	p->OnCreate();

	return p;
}
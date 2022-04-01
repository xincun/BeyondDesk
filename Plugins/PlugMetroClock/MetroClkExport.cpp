#include "stdafx.h"
#include "MetroClkExport.h"
#include "MetroClkWnd.hpp"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;

class CMetroClk : public IToolPlug
{
public:
	CMetroClk()
	{
		m_sCaption = _T("磁贴时钟");
		m_sHint = _T("WIN10磁贴样式的时钟");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}

	virtual ~CMetroClk() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("MetroClk Plugin Created..."));
		
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"MetroClk Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"MetroClk Plugin Unloaded...");
		//CDeskWnd::Clear(_T("CMetroClkWnd")); //duilib自身清理
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"MetroClk Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

		CDeskWnd::RestoreFromDB<CMetroClkWnd>(_T("CMetroClkWnd"), this);
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"MetroClk Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		CDeskWnd::CreateNew<CMetroClkWnd>(this);

	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("MetroClock");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("Metro时钟插件");
	gPlugInfo.Order = 7;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CMetroClk;
	p->OnCreate();

	return p;
}
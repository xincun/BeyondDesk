#include "stdafx.h"
#include "NoteExport.h"
#include "NoteWnd.hpp"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;

class CNote : public IToolPlug
{
public:
	CNote()
	{
		m_sCaption = _T("桌面便签");
		m_sHint = _T("桌面显示便签");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}

	virtual ~CNote() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("Note Plugin Created..."));
		
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"Note Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"Note Plugin Unloaded...");
		//CDeskWnd::Clear(_T("CNoteWnd")); //duilib自身清理
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"Note Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

		CDeskWnd::RestoreFromDB<CNoteWnd>(_T("CNoteWnd"), this);
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"Note Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		CDeskWnd::CreateNew<CNoteWnd>(this);

	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("Note");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("便签插件");
	gPlugInfo.Order = 1;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CNote;
	p->OnCreate();

	return p;
}
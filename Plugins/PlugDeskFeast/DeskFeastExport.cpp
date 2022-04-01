#include "stdafx.h"
#include "DeskFeastExport.h"
#include "DeskFeastWnd.h"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;

class CDeskFeast : public IToolPlug
{
	//bool m_nCount = 0;
public:
	CDeskFeast()
	{
		m_sCaption = _T("最近节日");
		m_sHint = _T("桌面显示最近的节日");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}

	virtual ~CDeskFeast() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("DeskFeast Plugin Created..."));
		
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"DeskFeast Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"DeskFeast Plugin Unloaded...");
		//CDeskWnd::Clear(_T("CDeskFeastWnd")); //duilib自身清理
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"DeskFeast Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

		CDeskWnd::RestoreFromDB<CDeskFeastWnd>(_T("CDeskFeastWnd"), this);
		
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"DeskFeast Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		vector<CString> secList = gUnify->DB()->PlugDB()->GetSections();
		for (CString item : secList) {
			if (item.Find(L"CDeskFeastWnd") == 0)
			{
				MessageBox(NULL, _T("只能有一个桌面节日!"), _T("提示"), MB_OK);
				return;
			}
		}
		
		CDeskWnd::CreateNew<CDeskFeastWnd>(this);

		
	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("DeskFeast");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("最近节日插件");
	gPlugInfo.Order = 1;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CDeskFeast;
	p->OnCreate();

	return p;
}
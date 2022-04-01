#include "stdafx.h"
#include "DeskCountExport.h"
#include "DeskCountWnd.h"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;

class CDeskCount : public IToolPlug
{
	//bool m_nCount = 0;
public:
	CDeskCount()
	{
		m_sCaption = _T("日期倒计时");
		m_sHint = _T("桌面显示日期倒计时");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}

	virtual ~CDeskCount() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("DeskCount Plugin Created..."));
		
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"DeskCount Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"DeskCount Plugin Unloaded...");
		//CDeskWnd::Clear(_T("CDeskCountWnd")); //duilib自身清理
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"DeskCount Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

		 CDeskWnd::RestoreFromDB<CDeskCountWnd>(_T("CDeskCountWnd"), this);
		
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"DeskCount Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		/*vector<CString> secList = gUnify->DB()->PlugDB()->GetSections();
		for (CString item : secList) {
			if (item.Find(L"CDeskCountWnd") == 0)
			{
				MessageBox(NULL, _T("只能有一个桌面日程!"), _T("提示"), MB_OK);
				return;
			}
		}*/
		
		CDeskWnd::CreateNew<CDeskCountWnd>(this);
		
	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("DeskCount");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("日期倒计时插件");
	gPlugInfo.Order = 1;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CDeskCount;
	p->OnCreate();

	return p;
}
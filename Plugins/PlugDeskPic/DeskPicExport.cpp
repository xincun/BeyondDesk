#include "stdafx.h"
#include "DeskPicExport.h"
#include "DeskPicWnd.h"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;

class CDeskPic : public IToolPlug
{
	//bool m_nCount = 0;
public:
	CDeskPic()
	{
		m_sCaption = _T("桌面相框");
		m_sHint = _T("桌面上动态展示图片");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}

	virtual ~CDeskPic() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("DeskSch Plugin Created..."));
		
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"DeskSch Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"DeskSch Plugin Unloaded...");
		//CDeskWnd::Clear(_T("CDeskPicWnd")); //duilib自身清理
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"DeskSch Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

CDeskWnd::RestoreFromDB<CDeskPicWnd>(_T("CDeskPicWnd"), this);
		
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"DeskSch Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		/*vector<CString> secList = gUnify->DB()->PlugDB()->GetSections();
		for (CString item : secList) {
			if (item.Find(L"CDeskPicWnd") == 0)
			{
				MessageBox(NULL, _T("只能有一个桌面相框!"), _T("提示"), MB_OK);
				return;
			}
		}*/
		
		 CDeskWnd::CreateNew<CDeskPicWnd>(this);
		
	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("DeskPic");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("桌面相框插件");
	gPlugInfo.Order = 0;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CDeskPic;
	p->OnCreate();

	return p;
}
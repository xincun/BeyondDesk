#include "stdafx.h"
#include "DeskCountTimeExport.h"
#include "DeskCountTimeWnd.h"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;

class CDeskCountTime : public IToolPlug
{
	//bool m_nCount = 0;
public:
	CDeskCountTime()
	{
		m_sCaption = _T("ʱ�䵹��ʱ");
		m_sHint = _T("������ʾ����ʱʱ��");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}

	virtual ~CDeskCountTime() {};

	virtual void OnCreate()
	{
		//Window::ShowMessage(_T("DeskCountTime Plugin Created..."));
		
	};

	virtual void OnDestroy()
	{
		//Window::ShowMessage(L"DeskCountTime Plugin Destroyed...");
	};

	virtual void OnUnload(void *pSender)
	{
		//Window::ShowMessage(L"DeskCountTime Plugin Unloaded...");
		//CDeskWnd::Clear(_T("CDeskCountTimeWnd")); //duilib��������
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"DeskCountTime Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

		 CDeskWnd::RestoreFromDB<CDeskCountTimeWnd>(_T("CDeskCountTimeWnd"), this);
		
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"DeskCountTime Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		/*vector<CString> secList = gUnify->DB()->PlugDB()->GetSections();
		for (CString item : secList) {
			if (item.Find(L"CDeskCountTimeWnd") == 0)
			{
				MessageBox(NULL, _T("ֻ����һ�������ճ�!"), _T("��ʾ"), MB_OK);
				return;
			}
		}*/
		
		CDeskWnd::CreateNew<CDeskCountTimeWnd>(this);
	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("DeskCountTime");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("ʱ�䵹��ʱ���");
	gPlugInfo.Order = 1;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CDeskCountTime;
	p->OnCreate();

	return p;
}
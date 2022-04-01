#include "stdafx.h"
#include "_PlugExport.h"
#include "DigiClockWnd.hpp"
#include "LogHelper.h"
#include "Event.h"

IUnify *gUnify;
TPlugInfo gPlugInfo;



class CDigiClock : public IToolPlug
{
public:
	CDigiClock()
	{
		m_sCaption = _T("����ʱ��");
		m_sHint = _T("��ʾҺ�����ֵ�ʱ��");
		m_sIconFile = _T("icon.png");

		m_bCanNew = true;
	}
	virtual ~CDigiClock() {};

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
		CDeskWnd::Clear(_T("CDigiClockWnd"));
	};

	virtual void OnLoad(void *pSender)
	{
		//Window::ShowMessage(L"Clock Plugin Loaded...");

		//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
		//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());
		
		//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

		 CDeskWnd::RestoreFromDB<CDigiClockWnd>(_T("CDigiClockWnd"), this);
		
	};

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"")
	{
		//Window::ShowMessage(L"Clock Plugin Invoked...");
	};

	virtual void OnNew(void* pSender = NULL) {
		CDeskWnd* p = CDeskWnd::CreateNew<CDigiClockWnd>(this);
		
	};
};

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("DigiClock");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("����ʱ�Ӳ��");
	gPlugInfo.Order = 4;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CDigiClock;
	p->OnCreate();

	return p;
}
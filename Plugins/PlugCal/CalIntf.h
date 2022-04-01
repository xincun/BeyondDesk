#ifndef CalExport_h
#define CalExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
extern "C" PLUG_API IPlugBase*  GetPlugIntf2();
//extern "C" PLUG_API void  FreePlugIntf(IPlugBase* p);


class CCalUI;
class CTabCal : public IUIPlug
{
	CCalUI *m_pCalUI = NULL;
public:
	virtual ~CTabCal();

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);

	virtual CControlUI* GetUI();
	virtual void Init(CPaintManagerUI* pManager);

	virtual void SetEnabled(bool val);

	//CCalendarUI* GetCalendar();
};



class CTodayUI;
class CUITodayPlug : public IUIPlug
{
	CTodayUI *m_pUI = NULL;
public:
	virtual CControlUI* GetUI();
	virtual void Init(CPaintManagerUI* pManager);
};

class CDeskCalWnd;
class CDeskCal : public IToolPlug
{
	CDeskCalWnd* m_pWnd;
public:
	CDeskCal(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile);

	virtual ~CDeskCal();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);

	virtual void OnNew(void* pSender = NULL);
	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
};

#endif
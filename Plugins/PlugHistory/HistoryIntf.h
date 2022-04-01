#ifndef HistoryExport_h
#define HistoryExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú
extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
extern "C" PLUG_API IPlugBase*  GetPlugIntf2();

class CHistoryWnd;
class CHistory : public IToolPlug
{
	typedef pair<TString, TString> HisItem;
	vector<HisItem> HisList;
	CHistoryWnd *m_pWnd;
	void OnTimer(void *pOwner, UINT nTimerID);
public:
	CHistory();

	virtual ~CHistory();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);;

	TString GetHis(const TString& sDate);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
};


class CHisUI;
class CUIHisPlug: public IUIPlug
{
	CHisUI *m_pUI = NULL;
public:
	virtual CControlUI* GetUI();
	virtual void Init(CPaintManagerUI* pManager);
};

#endif
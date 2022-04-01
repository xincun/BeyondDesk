#ifndef CartExport_h
#define CartExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú
extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();

class CCart : public IToolPlug
{
	void OnTimer(void *pOwner, UINT nTimerID);
public:
	CCart();

	virtual ~CCart();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);;

	TString GetHis(const TString& sDate);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
	virtual void OnNew(void* pSender = NULL);
};



#endif
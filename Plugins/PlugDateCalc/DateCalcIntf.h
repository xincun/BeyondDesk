#ifndef DateCalcExport_h
#define DateCalcExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú
extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();

class CDateCalcWnd;
class CDateCalc : public IToolPlug
{
	CDateCalcWnd* m_pWnd;
public:
	CDateCalc();

	virtual ~CDateCalc();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
};

#endif
#ifndef WorldTimeExport_h
#define WorldTimeExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
//extern "C" PLUG_API void  FreePlugIntf(IPlugBase* p);

class CWorldTimeWnd;
class CWorldTime : public IToolPlug
{
	TString m_sURL;
	CWorldTimeWnd* m_pWnd;
public:
	CWorldTime(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile);

	virtual ~CWorldTime();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
};

#endif
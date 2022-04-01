#ifndef CamRecExport_h
#define CamRecExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
//extern "C" PLUG_API void  FreePlugIntf(IPlugBase* p);

class CCamRecWnd;
class CCamRec : public IToolPlug
{
	CCamRecWnd* m_pWnd;
public:
	CCamRec(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile);

	virtual ~CCamRec();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
};

#endif
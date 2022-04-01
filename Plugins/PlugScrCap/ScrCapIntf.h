#ifndef ScrCapExport_h
#define ScrCapExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
//extern "C" PLUG_API void  FreePlugIntf(IPlugBase* p);

class CScrCapWnd;
class CScrCap : public IToolPlug
{
	CScrCapWnd* m_pWnd;
public:
	CScrCap(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile);

	virtual ~CScrCap();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
	virtual void OnSet(void* pSender = NULL);
};

#endif
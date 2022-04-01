#ifndef MicRecExport_h
#define MicRecExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
//extern "C" PLUG_API void  FreePlugIntf(IPlugBase* p);

class CMicRecWnd;
class CMicRec : public IToolPlug
{
	CMicRecWnd* m_pWnd;
public:
	CMicRec(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile);

	virtual ~CMicRec();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
};

#endif
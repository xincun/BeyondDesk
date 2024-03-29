#ifndef AppLogExport_h
#define AppLogExport_h

#include "stdafx.h"

//�����ӿ�

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
//extern "C" PLUG_API void  FreePlugIntf(IPlugBase* p);

class CAppLogWnd;
class CAppLog : public IToolPlug
{
	TString m_sURL;
	CAppLogWnd* m_pWnd;
public:
	CAppLog(LPCTSTR pstrURL, LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile);

	virtual ~CAppLog();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
	virtual void OnClick(void *pSender = NULL);
};

#endif
#ifndef WeatherExport_h
#define WeatherExport_h

#include "stdafx.h"

//µ¥¶À½Ó¿Ú
extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
extern "C" PLUG_API IPlugBase*  GetPlugIntf2();

class CDeskWeather : public IToolPlug
{
public:
	CDeskWeather();

	virtual ~CDeskWeather();;

	virtual void OnCreate();;

	virtual void OnDestroy();;

	virtual void OnUnload(void *pSender);;

	virtual void OnLoad(void *pSender);;

	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");;

	virtual void OnNew(void* pSender = NULL);;
};

class CMainUI;
class CUIMainPlug : public IUIPlug
{
	CMainUI *m_pUI = NULL;
public:
	virtual CControlUI* GetUI();
	virtual void Init(CPaintManagerUI* pManager);
	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = L"", const TString &eParam = L"");
};

#endif
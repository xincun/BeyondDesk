#include "stdafx.h"
#include "WeatherExport.h"
#include "DeskWeather.hpp"
#include "AppPath.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"
#include "IniFile.h"


IUnify *gUnify;
TPlugInfo gPlugInfo;



extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;
	
	gPlugInfo.Name = _T("Weather");
	gPlugInfo.Type = PLUG_TYPE_BOOT;
	gPlugInfo.Type |= PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("天气插件");
	gPlugInfo.PlugIntfCount = 2;
	gPlugInfo.Order = 2;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf2()
{
	IPlugBase *p = new CUIMainPlug;
	p->OnCreate();

	return p;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CDeskWeather;
	p->OnCreate();

	return p;
}

CDeskWeather::CDeskWeather()
{
	m_sCaption = _T("桌面天气");
	m_sHint = _T("桌面上显示天气信息");
	m_sIconFile = _T("icon.png");

	m_bCanNew = true;
}

CDeskWeather::~CDeskWeather()
{

}

void CDeskWeather::OnCreate()
{
	//Window::ShowMessage(_T("HTCClk Plugin Created..."));
	//LogHelper::init_log();
}

void CDeskWeather::OnDestroy()
{
	//Window::ShowMessage(L"HTCClk Plugin Destroyed...");
}

void CDeskWeather::OnUnload(void *pSender)
{
	//Window::ShowMessage(L"HTCClk Plugin Unloaded...");
	//CDeskWnd::Clear(_T("CDeskWeather")); //duilib自身清理
}

void CDeskWeather::OnLoad(void *pSender)
{
	//Window::ShowMessage(L"HTCClk Plugin Loaded...");

	//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
	//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());

	//gUnify->Core()->Event()->PostEvent(9999, this,  "wparam", "lparam");

	CDeskWnd::RestoreFromDB<CDeskWeatherWnd>(_T("CDeskWeatherWnd"), this);
}

void CDeskWeather::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	//Window::ShowMessage(L"HTCClk Plugin Invoked...");
}

void CDeskWeather::OnNew(void* pSender /*= NULL*/)
{
	CDeskWnd::CreateNew<CDeskWeatherWnd>(this);
}



#include "stdafx.h"
#include "CalIntf.h"


using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("Cal");
	gPlugInfo.Type = PLUG_TYPE_BACKGROUND;
	gPlugInfo.Type |= PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("日历插件");
	gPlugInfo.PlugIntfCount = 3;
	gPlugInfo.Order = 1;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CTabCal();
	p->OnCreate();

	return p;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf2()
{
	IPlugBase *p = new CUITodayPlug();
	p->OnCreate();

	return p;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf3()
{
	IPlugBase *p = new CDeskCal(_T("桌面日历"), _T("桌面日历"), _T("icon.png"));
	p->OnCreate();

	return p;
}



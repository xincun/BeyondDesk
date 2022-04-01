#include "stdafx.h"
#include "Frame.h"


bool Frame::Init(LPCTSTR szCmdLine, LPCTSTR szMainWndClass, LPCTSTR szMainTitle, bool checkOneInstacne /*= true*/)
{
	CreateUnify();

#ifdef ENABLE_SINGLETON_APP
	if (checkOneInstacne)
	    if (!App::InsureOneInstanceAndActiveMainWnd(szMainTitle, szMainWndClass, szMainTitle)) return false;
#endif

	gUnify->App()->CmdLine()->SetCmdLine(szCmdLine);

#ifdef ENABLE_GDIP
	GDIPHelper::Initialize();
#endif

#ifdef ENABLE_PLUG
	gUnify->PlugManager()->LoadPlugs((gUnify->App()->Path()->ExePath + _T("Plugins")).c_str());
#endif

	return true;
}

bool Frame::Clear()
{
#ifdef ENABLE_PLUG
	gUnify->PlugManager()->UnLoadPlugs();
#endif

#ifdef ENABLE_GDIP
	GDIPHelper::Shutdown();
#endif

	delete gUnify;

	return true;
}

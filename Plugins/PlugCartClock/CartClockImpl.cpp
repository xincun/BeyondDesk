#include "stdafx.h"
#include "CartClockIntf.h"
#include "CartWnd.hpp"
#include "AppPath.h"
#include "Event.h"

#include "SimpleTimer.h"
#include "CmdLine.h"
#include <string>
#include "resource.h"

using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("Cart");
	gPlugInfo.Type = PLUG_TYPE_DESKWIDGET;
	gPlugInfo.Desc = _T("卡通时钟插件");
	gPlugInfo.Order = 5;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CCart;
	p->OnCreate();

	return p;
}

void CCart::OnTimer(void *pOwner, UINT nTimerID)
{
//	LOGCON("on timer...");
}

CCart::CCart()
{
	m_sCaption = _T("卡通时钟");
	m_sHint = _T("显示卡通形象的时钟");
	m_sIconFile = _T("icon.png");

	m_bCanNew = true;
}

CCart::~CCart()
{

}

void CCart::OnCreate()
{
	//Window::ShowMessage(_T("Cart Plugin Created..."));
}

void CCart::OnDestroy()
{

	//Window::ShowMessage(L"Cart Plugin Destroyed...");
}

void CCart::OnUnload(void *pSender)
{
	CDeskWnd::Clear(_T("CCartWnd"));
	//Window::ShowMessage(L"Cart Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CCart::OnNew(void* pSender /*= NULL*/)
{
	CDeskWnd::CreateNew<CCartWnd>(this);
}

void CCart::OnLoad(void *pSender)
{
	 CDeskWnd::RestoreFromDB<CCartWnd>(_T("CCartWnd"), this);
}

void CCart::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CCart::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void CCart::OnClick(void *pSender /*= NULL*/)
{
	//Window::ShowMessage(L"clicked");
	return;

	
}

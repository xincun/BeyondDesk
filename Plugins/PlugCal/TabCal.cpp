#include "stdafx.h"
#include "CalIntf.h"
#include "UICal.h"
#include <atlstr.h>

CTabCal::~CTabCal()
{

}

void CTabCal::OnCreate()
{

}

void CTabCal::OnDestroy()
{

}

void CTabCal::OnUnload(void *pSender)
{

}

void CTabCal::OnLoad(void *pSender)
{

}

void CTabCal::SetEnabled(bool val)
{
	m_pCalUI->SetEnabled(val);
}

void CTabCal::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	if (wParam == 3)
	{
		//桌面日程获取状态
		m_pCalUI->UpdateTodaySchCount();
	}
}

TString CTabCal::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return _T("");
}

void CTabCal::OnClick(void *pSender /*= NULL*/)
{

}

CControlUI* CTabCal::GetUI()
{
	CCalUI* p = new CCalUI;
	m_pCalUI = p; 
	gHolidayGetter.GetHoliday();
	return p;
}

void CTabCal::Init(CPaintManagerUI* pManager)
{
	if (!m_pCalUI) return;

	m_pCalUI->Create(pManager);
	m_pCalUI->InitWindow();
}
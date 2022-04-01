#include "stdafx.h"
#include "WeatherExport.h"
#include "UIMain.h"
#include <atlstr.h>

CControlUI* CUIMainPlug::GetUI()
{
	CMainUI* p = new CMainUI;
	m_pUI = p;
	return p;
}

void CUIMainPlug::Init(CPaintManagerUI* pManager)
{
	if (!m_pUI) return;

	m_pUI->Create(pManager);
	m_pUI->InitWindow();
}

void CUIMainPlug::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	if (wParam == 0) {
		m_pUI->OnConfig();
	}

	
}
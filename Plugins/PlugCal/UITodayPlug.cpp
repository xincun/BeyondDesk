#include "stdafx.h"
#include "CalIntf.h"
#include "UIToday.h"
#include <atlstr.h>

CControlUI* CUITodayPlug::GetUI()
{
	CTodayUI* p = new CTodayUI;
	m_pUI = p;
	return p;
}

void CUITodayPlug::Init(CPaintManagerUI* pManager)
{
	if (!m_pUI) return;

	m_pUI->Create(pManager);
	m_pUI->InitWindow();
}
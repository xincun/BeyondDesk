#include "stdafx.h"
#include "HistoryIntf.h"
#include "UIHis.h"
#include <atlstr.h>

CControlUI* CUIHisPlug::GetUI()
{
	CHisUI* p = new CHisUI;
	m_pUI = p;
	return p;
}

void CUIHisPlug::Init(CPaintManagerUI* pManager)
{
	if (!m_pUI) return;

	m_pUI->Create(pManager);
	m_pUI->InitWindow();
}
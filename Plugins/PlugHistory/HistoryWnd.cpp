#include "stdafx.h"
#include "resource.h"
#include "HistoryWnd.h"
#include <shellapi.h>

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CHistoryWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CHistoryWnd::CHistoryWnd(HWND hMainWnd) : m_pHisObj(NULL)
{
	m_hMainWnd = hMainWnd;
}

CHistoryWnd::~CHistoryWnd(void)
{
	
}

void CHistoryWnd::OnFinalMessage( HWND hWnd)
{
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);
	
	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CHistoryWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CHistoryWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CHistoryWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("PlugWndHistory.xml")).c_str();
	//return _T("XML\\PlugWndHistory.xml");
}

UILIB_RESOURCETYPE CHistoryWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CHistoryWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CHistoryWnd::GetWindowClassName( void ) const
{
	return _T("UIHistoryWnd");
}

void CHistoryWnd::OnClick( TNotifyUI &msg )
{
	__super::OnClick(msg);

	if (msg.pSender->GetName() == _T("btnToday"))
	{
		GotoToday();

	}
	else if (msg.pSender->GetName() == _T("btnLastDay"))
	{
		//if (m_pCombDay->GetCurSel() == 0)
		//{
		//	/*if (m_pCombMon->GetCurSel() == 0)
		//	{
		//	m_pCombMon->SelectItem(11);
		//	}
		//	else
		//	m_pCombMon->SelectItem(m_pCombMon->GetCurSel() - 1);*/
		//}
		if (m_pCombDay->GetCurSel() > 0)
		{
			m_pCombDay->SelectItem(m_pCombDay->GetCurSel() - 1);
		}
	}
	else if (msg.pSender->GetName() == _T("btnNextDay"))
	{
		int n = 0;
		for (int i = 0; i < m_pCombDay->GetCount() - 1; ++i)
		{
			if (m_pCombDay->GetItemAt(i)->IsVisible()) n++;
		}

		if (m_pCombDay->GetCurSel() < n - 1)
		{
			m_pCombDay->SelectItem(m_pCombDay->GetCurSel() + 1);
		}

	}
	else if (msg.pSender->GetName() == _T("btnLastMon"))
	{
		if (m_pCombMon->GetCurSel() > 0)
		{
			m_pCombMon->SelectItem(m_pCombMon->GetCurSel() - 1);
		}

	}
	else if (msg.pSender->GetName() == _T("btnNextMon"))
	{
		if (m_pCombMon->GetCurSel() < 11)
		{
			m_pCombMon->SelectItem(m_pCombMon->GetCurSel() + 1);
		}

	}
}

void CHistoryWnd::OnSelectChanged( TNotifyUI &msg )
{

}

void CHistoryWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CHistoryWnd::GotoToday()
{
	CTime Now = CTime::GetCurrentTime();
	m_pCombMon = FindControl<CComboUI>("cmbMonth");
	m_pCombDay = FindControl<CComboUI>("cmbDay");

	m_pCombMon->SelectItem(Now.GetMonth() - 1);
	m_pCombDay->SelectItem(Now.GetDay() - 1);
}

void CHistoryWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

void CHistoryWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE, FALSE);

	GotoToday();
}

LRESULT CHistoryWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	/*演示键盘消息的处理*/
	/*TCHAR press_char = (TCHAR)wParam;
	if(press_char == VK_BACK)
	{
	MessageBox(NULL, _T("按下了回退键"), _T("DUILIB DEMO"), MB_OK);
	}
	else
	{
	bHandled = FALSE;
	}*/
	return 0;
}

LRESULT CHistoryWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CHistoryWnd::OnItemSelect( TNotifyUI &msg )
{
	if (msg.pSender == m_pCombMon)
	{
		if (m_pCombMon->GetCurSel() == 1)
		{
			if (m_pCombDay->GetCurSel() == 29)
				m_pCombDay->SelectItem(28);
			if (m_pCombDay->GetCurSel() == 30)
				m_pCombDay->SelectItem(29);
			m_pCombDay->GetItemAt(29)->SetVisible(false);
			m_pCombDay->GetItemAt(30)->SetVisible(false);
		}
		else if (m_pCombMon->GetCurSel() == 3 ||
			m_pCombMon->GetCurSel() == 5 ||
			m_pCombMon->GetCurSel() == 8 ||
			m_pCombMon->GetCurSel() == 10
			)
		{
			if (m_pCombDay->GetCurSel() == 30)
				m_pCombDay->SelectItem(29);
			m_pCombDay->GetItemAt(30)->SetVisible(false);
		}
		ChangeHis();
	}
	else if (msg.pSender == m_pCombDay)
	{
		ChangeHis();
	}
}

void CHistoryWnd::ChangeHis()
{
	CString sDate;
	sDate.Format(_T("%02d%02d"), m_pCombMon->GetCurSel() + 1, m_pCombDay->GetCurSel() + 1);

	TString sHis = m_pHisObj->GetHis(sDate.GetBuffer());

	CRichEditUI* pEdit = FindControl<CRichEditUI>("edtHis");
	pEdit->SetText(sHis.c_str());
}

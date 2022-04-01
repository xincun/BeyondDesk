#include "stdafx.h"
#include "resource.h"
#include "ConfigWnd.h"
#include "../../Unify/Unify.h"
#include "DeskWnd.h"
#include <future>

#include "DeskCountTimeWnd.h"
//////////////////////////////////////////////////////////////////////////
///

CConfigWnd::CConfigWnd() : m_pColorComboTitle(NULL), m_pColorComboCount(NULL),  m_pMainUI(NULL)
{
	
}

DuiLib::CDuiString CConfigWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("Res\\PlugCfgDeskCountTime.xml")).c_str();
}


void CConfigWnd::OnClick( TNotifyUI &msg )
{
	return __super::OnClick(msg);
}

void CConfigWnd::OnTimer(void* pOwner, UINT uiID)
{
	//DrawPreview();
}

void CConfigWnd::HideDeskAboutCtrls()
{
	CVerticalLayoutUI* pBack = static_cast<CVerticalLayoutUI*>(m_PaintManager->FindControl(_T("pnlTab")));
	pBack->SetVisible(false);
    pBack = static_cast<CVerticalLayoutUI*>(m_PaintManager->FindControl(_T("pnlBack")));
	pBack->SetVisible(false);
	pBack = static_cast<CVerticalLayoutUI*>(m_PaintManager->FindControl(_T("pnlFontColor")));
	pBack->SetVisible(false);
}

void CConfigWnd::Notify(TNotifyUI &msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		

		/*std::async(std::launch::async, [&](){
			Sleep(100);
			DrawPreview();
		});*/

		return;
	}

	return __super::Notify(msg);
}

void CConfigWnd::Repaint()
{
	InvalidateRect(m_hWnd, NULL, true);
	::UpdateWindow(m_hWnd);
}

void CConfigWnd::OnOK()
{
	SYSTEMTIME endDate = static_cast<CDateTimeUI*>(m_PaintManager->FindControl(_T("DateEnd")))->GetTime();
	SYSTEMTIME st = {};
	::SendMessage(m_hTimeEndCtrl, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
	CTime EndDateTime = CTime(endDate.wYear, endDate.wMonth, endDate.wDay, st.wHour, st.wMinute, st.wSecond);
	CTime Now = CTime::GetCurrentTime();

	if (EndDateTime <= Now) {
		MessageBox(this->m_hWnd, _T("结束日期必须大于当前日期！"), _T("警告"), MB_OK);

		return;
	}

	__super::OnOK();
}

void CConfigWnd::OnApply()
{
	SYSTEMTIME endDate = static_cast<CDateTimeUI*>(m_PaintManager->FindControl(_T("DateEnd")))->GetTime();
	SYSTEMTIME st = {};
	::SendMessage(m_hTimeEndCtrl, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
	CTime EndDateTime = CTime(endDate.wYear, endDate.wMonth, endDate.wDay, st.wHour, st.wMinute, st.wSecond);
	CTime Now = CTime::GetCurrentTime();

	if (EndDateTime <= Now) {
		MessageBox(this->m_hWnd, _T("结束日期必须大于当前日期！"), _T("警告"), MB_OK);

		return;
	}

	__super::OnApply();
}

void CConfigWnd::InitWindow()
{
	TryGetControls();
	m_pColorComboTitle->SetSelColor(RGB(255, 255, 255));
	m_pColorComboCount->SetSelColor(RGB(255, 255, 255));


	__super::InitWindow();

	m_bInited = true;
}

LRESULT CConfigWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NOTIFY: {
		if (((LPNMHDR)lParam)->hwndFrom == m_hTimeEndCtrl)
		{
			FindControl<CButtonUI>("btnApply")->SetEnabled(true);
		}
		break;
	}
	default:
		break;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

static LRESULT CALLBACK	 _WndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

CControlUI* CConfigWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	if (_tcsicmp(pstrClass, _T("WndDateEnd")) == 0)
	{
		CWndHostUI  *pUI = new CWndHostUI;
		//CDateTimeCtrl *dtTime = new CDateTimeCtrl;

		m_hTimeEndCtrl = CreateWindow(_T("SysDateTimePick32"), _T("win32"), WS_VISIBLE | WS_CHILD | WS_MAXIMIZEBOX | WS_TABSTOP | WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_CONTROLPARENT,
			0, 0, 0, 0, this->m_hWnd, NULL, NULL, NULL);

		//m_TimeCtrl = static_cast<CDateTimeCtrl*>(CDateTimeCtrl::FromHandle(hWnd));
		/*m_pFont= new CFont();
		m_pFont->CreateFont( 18, 0, 0, 0, 100, FALSE, FALSE, 0,
		GB2312_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_MODERN, _T("微软雅黑") ) ;
		m_TimeCtrl->SetFont(m_pFont);*/
		//m_TimeCtrl->SetFormat(_T("HH:mm"));

		HFONT hFont = CreateFont(18, 0, 0, 0, 100, FALSE, FALSE, 0,
			GB2312_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_MODERN, _T("微软雅黑"));

		//HGDIOBJ hOldFont = ::SelectObject(GetDC(m_hWnd), hFont);

		::SendMessage(m_hTimeEndCtrl, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
		::SendMessage(m_hTimeEndCtrl, DTM_SETFORMAT, 0, (LPARAM)(_T("HH:mm:ss")));

		//::SelectObject(GetDC(m_hWnd), hOldFont);

		//DeleteObject(hFont)

		pUI->Attach(m_hTimeEndCtrl);

		//CButton *pBtn = new CButton;    // 记得释放内存
		//pBtn->Create(_T("MFC"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, CRect(0, 0, 0, 0), CWnd::FromHandle(this->m_hWnd), 0);
		//pUI->Attach(pBtn->GetSafeHwnd());

		return pUI;
	}

	return NULL;
}

void CConfigWnd::SaveCfg()
{
	SYSTEMTIME endDate = static_cast<CDateTimeUI*>(m_PaintManager->FindControl(_T("DateEnd")))->GetTime();
	SYSTEMTIME st = {};
	::SendMessage(m_hTimeEndCtrl, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
	CTime EndDateTime = CTime(endDate.wYear, endDate.wMonth, endDate.wDay, st.wHour, st.wMinute, st.wSecond);
	CTime Now = CTime::GetCurrentTime();


	__super::SaveCfg();

	TString sSection = m_sSection;// m_pDeskWnd->GetDBSection();

	/*gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Prov"), m_pProvinceCmb->GetItemAt(m_pProvinceCmb->GetCurSel())->GetText().GetData());
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("City"), m_pCityCmb->GetItemAt(m_pCityCmb->GetCurSel())->GetText().GetData());
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Region"), m_pCountyCmb->GetItemAt(m_pCountyCmb->GetCurSel())->GetText().GetData());*/

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Back"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());

	/*pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbFresh")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Fresh"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());*/

	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("FontColorTitle"), m_pColorComboTitle->GetSelColor());
	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("m_pColorComboCount"), m_pColorComboCount->GetSelColor());

	auto SetParamByCheckCtrl = [&](LPCTSTR sName) {
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(sName));
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), sName, pChk->GetCheck());
	};

	auto SetParamBySpinEditCtrl = [&](LPCTSTR szCtrlName) {
		CDuiString sText;
		sText = static_cast<CSpinEditUI*>(m_PaintManager->FindControl(szCtrlName))->GetText();
		gUnify->DB()->PlugDB()->SetString(sSection.c_str(), szCtrlName , sText.GetData());
	};
	auto SetParamByEditCtrl = [&](LPCTSTR szCtrlName) {
		CDuiString sText;
		sText = static_cast<CEditUI*>(m_PaintManager->FindControl(szCtrlName))->GetText();
		gUnify->DB()->PlugDB()->SetString(sSection.c_str(), szCtrlName, sText.GetData());
	};

	//SetParamByEditCtrl(L"edtFormat");
	SetParamByEditCtrl(L"edtTitle");
	SetParamByCheckCtrl(L"chkTitle");
	//SetParamByCheckCtrl(L"chkDetail");
	/*SetParamByCheckCtrl(L"chkDay");

	SetParamByCheckCtrl(L"chkHour");
	SetParamByCheckCtrl(L"chkMin");
	SetParamByCheckCtrl(L"chkSec");*/
	
	//CString sDate = EndDateTime.Format(L"%Y-%m-%d");
	CString sDate = EndDateTime.Format(L"%Y-%m-%d %H:%M:%S");
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), L"DateEnd", sDate);
}

void CConfigWnd::LoadCfg()
{
	__super::LoadCfg();

	TString sSection = m_sSection;// m_pDeskWnd->GetDBSection();

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	for (int i = 0; i < pComb->GetCount(); ++i)
	{
		if (pComb->GetItemAt(i)->GetUserData().GetData() == gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Back"), _T("black")))
		{
			pComb->SelectItem(i);
		}
	}

	TryGetControls();

	m_pColorComboTitle->SetSelColor(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("FontColorTitle"), RGB(255, 255, 255)));
	m_pColorComboCount->SetSelColor(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("m_pColorComboCount"), RGB(255, 255, 255)));

	auto SetCheckCtrlByParam = [&](LPCTSTR szCtrlName) {
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(szCtrlName));
		pChk->SetCheck(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), szCtrlName, true));
	};

	auto SetSpinEditCtrlByParam = [&](LPCTSTR szCtrlName)
	{
		CSpinEditUI* pEdt = static_cast<CSpinEditUI*>(m_PaintManager->FindControl(szCtrlName));
		CString s = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), szCtrlName, L"");
		if (!s.IsEmpty())
			pEdt->SetText(s.GetBuffer());
	};
	auto SetEditCtrlByParam = [&](LPCTSTR szCtrlName) {
		CEditUI* pEdt = static_cast<CEditUI*>(m_PaintManager->FindControl(szCtrlName));
		CString s = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), szCtrlName, L"");
		if (!s.IsEmpty())
			pEdt->SetText(s.GetBuffer());
	};

	//SetEditCtrlByParam(L"edtFormat");
	SetEditCtrlByParam(L"edtTitle");
	SetCheckCtrlByParam(L"chkTitle");
	//SetCheckCtrlByParam(L"chkDetail");
	/*SetCheckCtrlByParam(L"chkDay");

	SetCheckCtrlByParam(L"chkHour");
	SetCheckCtrlByParam(L"chkMin");
	SetCheckCtrlByParam(L"chkSec");*/

	SYSTEMTIME sysTime;
	CTime Now = CTime::GetCurrentTime();
	CString sDate = Now.Format(L"%Y-%m-%d %H:%M:%S");
	CTime EndDateTime = Util::Time::StrToTime(gUnify->DB()->PlugDB()->GetString(sSection.c_str(), L"DateEnd", sDate));
	EndDateTime.GetAsSystemTime(sysTime);
	::SendMessage(m_hTimeEndCtrl, DTM_SETSYSTEMTIME, 0, LPARAM(&sysTime));
	static_cast<CDateTimeUI*>(m_PaintManager->FindControl(_T("DateEnd")))->SetTime(&sysTime);
}

void CConfigWnd::TryGetControls()
{
	//m_pProvinceCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbProvince")));
	//m_pCityCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbCity")));
	//m_pCountyCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbCounty")));
	m_pColorComboTitle = static_cast<CColorComboUI*>(m_PaintManager->FindControl(_T("cmbColorTitle"))); //保证获取到
	m_pColorComboCount = static_cast<CColorComboUI*>(m_PaintManager->FindControl(_T("cmbColorCount")));
}


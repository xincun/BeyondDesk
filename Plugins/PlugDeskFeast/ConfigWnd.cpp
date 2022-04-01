#include "stdafx.h"
#include "resource.h"
#include "ConfigWnd.h"
#include "../../Unify/Unify.h"
#include "DeskWnd.h"
#include <future>
#include "DeskFeastWnd.h"
//////////////////////////////////////////////////////////////////////////
///

CConfigWnd::CConfigWnd() : m_pColorCombo(NULL), m_pMainUI(NULL)
{
	
}

DuiLib::CDuiString CConfigWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("Res\\PlugCfgDeskFeast.xml")).c_str();
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



void CConfigWnd::InitWindow()
{
	TryGetControls();
	m_pColorCombo->SetSelColor(RGB(255, 255, 255));

	__super::InitWindow();

	m_bInited = true;
}

LRESULT CConfigWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//switch (uMsg)
	//{
	//case WM_PAINT:
	////case WM_NCPAINT:
	//{
	//	DrawPreview();
	//	break;
	//}
	//default:
	//	break;
	//}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

static LRESULT CALLBACK	 _WndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

CControlUI* CConfigWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	return NULL;
}

void CConfigWnd::SaveCfg()
{
	__super::SaveCfg();

	TString sSection = m_sSection;// m_pDeskWnd->GetDBSection();

	/*gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Prov"), m_pProvinceCmb->GetItemAt(m_pProvinceCmb->GetCurSel())->GetText().GetData());
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("City"), m_pCityCmb->GetItemAt(m_pCityCmb->GetCurSel())->GetText().GetData());
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Region"), m_pCountyCmb->GetItemAt(m_pCountyCmb->GetCurSel())->GetText().GetData());*/

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Back"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());

	/*pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbFresh")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Fresh"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());*/

	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("FontColor"), m_pColorCombo->GetSelColor());

	auto SetParamByCheckCtrl = [&](LPCTSTR sName) {
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(sName));
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), sName, pChk->GetCheck());
	};

	auto SetParamBySpinEditCtrl = [&](LPCTSTR szCtrlName) {
		CDuiString sText;
		sText = static_cast<CSpinEditUI*>(m_PaintManager->FindControl(szCtrlName))->GetText();
		gUnify->DB()->PlugDB()->SetString(sSection.c_str(), szCtrlName , sText.GetData());
	};


	SetParamBySpinEditCtrl(L"edtNum");
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

	m_pColorCombo->SetSelColor(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("FontColor"), RGB(255, 255, 255)));

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


	SetSpinEditCtrlByParam(L"edtNum");
}

void CConfigWnd::TryGetControls()
{
	//m_pProvinceCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbProvince")));
	//m_pCityCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbCity")));
	//m_pCountyCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbCounty")));
	m_pColorCombo = static_cast<CColorComboUI*>(m_PaintManager->FindControl(_T("cmbColor"))); //保证获取到
}


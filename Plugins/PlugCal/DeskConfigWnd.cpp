#include "stdafx.h"
#include "resource.h"
#include "DeskConfigWnd.h"
#include "../../Unify/Unify.h"
#include "DeskWnd.h"
#include <future>

#include "DeskCalWnd.h"
//////////////////////////////////////////////////////////////////////////
///

CDeskConfigWnd::CDeskConfigWnd() : m_pColorCombo(NULL)
{
	
}

DuiLib::CDuiString CDeskConfigWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("CfgDeskCal.xml")).c_str();
}


void CDeskConfigWnd::OnClick( TNotifyUI &msg )
{
	return __super::OnClick(msg);
}

void CDeskConfigWnd::OnTimer(void* pOwner, UINT uiID)
{
	//DrawPreview();
}

void CDeskConfigWnd::HideDeskAboutCtrls()
{
	CVerticalLayoutUI* pBack = static_cast<CVerticalLayoutUI*>(m_PaintManager->FindControl(_T("pnlTab")));
	pBack->SetVisible(false);
    pBack = static_cast<CVerticalLayoutUI*>(m_PaintManager->FindControl(_T("pnlBack")));
	pBack->SetVisible(false);
	pBack = static_cast<CVerticalLayoutUI*>(m_PaintManager->FindControl(_T("pnlFontColor")));
	pBack->SetVisible(false);
}

void CDeskConfigWnd::Notify(TNotifyUI &msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		

		/*std::async(std::launch::async, [&](){
			Sleep(100);
			DrawPreview();
		});*/

	}
	
	return __super::Notify(msg);
}

void CDeskConfigWnd::Repaint()
{
	InvalidateRect(m_hWnd, NULL, true);
	::UpdateWindow(m_hWnd);
}



void CDeskConfigWnd::InitWindow()
{
	TryGetControls();
	//m_pColorCombo->SetSelColor(RGB(255, 255, 255));

	__super::InitWindow();



	m_bInited = true;
}

LRESULT CDeskConfigWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

CControlUI* CDeskConfigWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	return NULL;
}

void CDeskConfigWnd::SaveCfg()
{
	__super::SaveCfg();

	TString sSection = m_sSection;// m_pDeskWnd->GetDBSection();

	///*gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Prov"), m_pProvinceCmb->GetItemAt(m_pProvinceCmb->GetCurSel())->GetText().GetData());
	//gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("City"), m_pCityCmb->GetItemAt(m_pCityCmb->GetCurSel())->GetText().GetData());
	//gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Region"), m_pCountyCmb->GetItemAt(m_pCountyCmb->GetCurSel())->GetText().GetData());*/

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Back"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());

	///*pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbFresh")));
	//gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Fresh"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());*/

	//gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("FontColor"), m_pColorCombo->GetSelColor());

	//auto SetParamByCheckCtrl = [&](LPCTSTR sName) {
	//	CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(sName));
	//	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), sName, pChk->GetCheck());
	//};

	//auto SetParamBySpinEditCtrl = [&](LPCTSTR szCtrlName) {
	//	CDuiString sText;
	//	sText = static_cast<CSpinEditUI*>(m_PaintManager->FindControl(szCtrlName))->GetText();
	//	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), szCtrlName , sText.GetData());
	//};

	//SetParamByCheckCtrl(L"chkGL");
	//SetParamByCheckCtrl(L"chkNL");
	//SetParamByCheckCtrl(L"chkFeast");

	//SetParamByCheckCtrl(L"chkTask");
	//SetParamByCheckCtrl(L"chkTaskNo");
	//SetParamByCheckCtrl(L"chkTodo");
	//SetParamByCheckCtrl(L"chkTodoNo");
	//SetParamByCheckCtrl(L"chkSch");
	//SetParamByCheckCtrl(L"chkSchNo");

	//SetParamBySpinEditCtrl(L"edtTaskNum");
	//SetParamBySpinEditCtrl(L"edtTaskDay");
	//SetParamBySpinEditCtrl(L"edtTodoNum");
	////SetParamBySpinEditCtrl(L"edtTodoDay");
	//SetParamBySpinEditCtrl(L"edtSchNum");
	//SetParamBySpinEditCtrl(L"edtSchDay");
}

void CDeskConfigWnd::LoadCfg()
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

	//m_pColorCombo->SetSelColor(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("FontColor"), RGB(255, 255, 255)));

	//auto SetCheckCtrlByParam = [&](LPCTSTR szCtrlName) {
	//	CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(szCtrlName));
	//	pChk->SetCheck(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), szCtrlName, true));
	//};

	//auto SetSpinEditCtrlByParam = [&](LPCTSTR szCtrlName)
	//{
	//	CSpinEditUI* pEdt = static_cast<CSpinEditUI*>(m_PaintManager->FindControl(szCtrlName));
	//	CString s = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), szCtrlName, L"");
	//	if (!s.IsEmpty())
	//		pEdt->SetText(s.GetBuffer());
	//};

	//SetCheckCtrlByParam(L"chkGL");
	//SetCheckCtrlByParam(L"chkNL");
	//SetCheckCtrlByParam(L"chkFeast");

	//SetCheckCtrlByParam(L"chkTask");
	//SetCheckCtrlByParam(L"chkTaskNo");
	//SetCheckCtrlByParam(L"chkTodo");
	//SetCheckCtrlByParam(L"chkTodoNo");
	//SetCheckCtrlByParam(L"chkSch");
	//SetCheckCtrlByParam(L"chkSchNo");

	//SetSpinEditCtrlByParam(L"edtTaskNum");
	//SetSpinEditCtrlByParam(L"edtTaskDay");
	//SetSpinEditCtrlByParam(L"edtTodoNum");
	////SetSpinEditCtrlByParam(L"edtTodoDay");
	//SetSpinEditCtrlByParam(L"edtSchNum");
	//SetSpinEditCtrlByParam(L"edtSchDay");
}

void CDeskConfigWnd::TryGetControls()
{
	//m_pProvinceCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbProvince")));
	//m_pCityCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbCity")));
	//m_pCountyCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbCounty")));
	//m_pColorCombo = static_cast<CColorComboUI*>(m_PaintManager->FindControl(_T("cmbColor"))); //保证获取到
}


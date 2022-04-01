#include "stdafx.h"
#include "resource.h"
#include "ConfigWnd.h"
#include "../../Unify/Unify.h"
#include "DeskWnd.h"
#include <future>
#include "UIMain.h"
//////////////////////////////////////////////////////////////////////////
///

CConfigWnd::CConfigWnd() : m_pColorCombo(NULL), m_pMainUI(NULL)
{
	
}

DuiLib::CDuiString CConfigWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("Res\\PlugCfgDeskWeather.xml")).c_str();
}


void CConfigWnd::OnClick( TNotifyUI &msg )
{
	return __super::OnClick(msg);
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
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.pSender->GetName() == _T("cmbProvince"))
		{
			OnSelectProvince();
			//OnModifyPersonInfo(msg.pSender);
		}
		else if (msg.pSender->GetName() == _T("cmbCity"))
		{
			OnSelectCity();
			//OnModifyPersonInfo(msg.pSender);
		}

		
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnApply") && m_pDeskWnd == NULL) //另一种进入设置，不是桌面窗口进入的
		{
			SaveCfg();
            
			if (m_pMainUI)
				m_pMainUI->ApplyCfg();

			FindControl<CButtonUI>("btnApply")->SetEnabled(false);

			return; //返回不处理
		}
		else if (msg.pSender->GetName() == _T("btnOK") && m_pDeskWnd == NULL)
		{
			if (FindControl<CButtonUI>("btnApply")->IsEnabled()) //否则已经应用了
			{
				SaveCfg();

				if (m_pMainUI)
					m_pMainUI->ApplyCfg();
			}

			Close();

			return;
		}
		
	}

	return __super::Notify(msg);
}

void CConfigWnd::Repaint()
{
	InvalidateRect(m_hWnd, NULL, true);
	::UpdateWindow(m_hWnd);
}

bool CConfigWnd::LoadLocationJson()
{
	if (m_bInitLocationJson)
		return true;

	wstring strFilePath =gPlugInfo.Path + _T("Res\\Region.dat");
	HANDLE hFile = CreateFile(strFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}

	DWORD dwByteRead = 0;
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	char* pBuf = new char[dwFileSize + 1];
	if (NULL != pBuf)
	{
		memset(pBuf, 0, dwFileSize + 1);
		ReadFile(hFile, pBuf, dwFileSize, &dwByteRead, NULL);
	}
	CloseHandle(hFile);

	bool bRet = false;
	Json::Reader reader;
	bRet = reader.parse(pBuf, m_oLocationJsonRoot);
	if (bRet)
	{
		m_bInitLocationJson = true;
	}

	if (NULL != pBuf)
	{
		delete[] pBuf;
		pBuf = NULL;
	}

	return bRet;
}

void CConfigWnd::InitLocationCmb()
{
	if (!LoadLocationJson())
	{
		return;
	}

	int iProvinceCurIndex = 0;
	int iProvinceCnt = 0;
	for (Json::Value::iterator it = m_oLocationJsonRoot.begin(); it != m_oLocationJsonRoot.end(); it++)
	{
		TString strProvinceId = String::UTF8ToUnicode((char*)it.memberName());
		TString strProvinceValue = String::UTF8ToUnicode((*it)["region_name"].asString().c_str());

		CListLabelElementUI *pItem = new CListLabelElementUI();
		pItem->SetText(strProvinceValue.c_str());
		pItem->SetUserData(strProvinceId.c_str());
		m_pProvinceCmb->Add(pItem);
		
		iProvinceCnt++;

		if (strProvinceValue == m_strProvince)
		{
			iProvinceCurIndex = iProvinceCnt - 1;
		}
	}

	m_pProvinceCmb->SelectItem(iProvinceCurIndex);
}

void CConfigWnd::OnSelectProvince()
{
	int iCurSel = m_pProvinceCmb->GetCurSel();
	if (iCurSel < 0)
		return;

	CListLabelElementUI* pItem = (CListLabelElementUI*)m_pProvinceCmb->GetItemAt(iCurSel);
	CDuiString strUserData = pItem->GetUserData();
	string pProvinceId = String::UnicodeToUTF8(strUserData.GetData());


	//清空市
	m_pCityCmb->RemoveAll();
	int iCurCityIndex = 0;
	int iCityCnt = 0;
	for (Json::Value::iterator it = m_oLocationJsonRoot[pProvinceId.c_str()]["city"].begin(); it != m_oLocationJsonRoot[pProvinceId.c_str()]["city"].end(); it++)
	{
		wstring strCityId = String::UTF8ToUnicode((char*)it.memberName());
		wstring strCityValue = String::UTF8ToUnicode((*it)["region_name"].asString().c_str());
		CListLabelElementUI *pItemCity = new CListLabelElementUI();
		pItemCity->SetText(strCityValue.c_str());
		pItemCity->SetUserData(strCityId.c_str());
		m_pCityCmb->Add(pItemCity);
		iCityCnt++;

		if (strCityValue == m_strCity)
		{
			iCurCityIndex = iCityCnt - 1;
		}
	}

	m_pCityCmb->SelectItem(iCurCityIndex);

}

void CConfigWnd::OnSelectCity()
{
	int iCurProvinceSel = m_pProvinceCmb->GetCurSel();
	int iCurCitySel = m_pCityCmb->GetCurSel();

	if (iCurProvinceSel < 0 || iCurCitySel < 0)
		return;

	//省	
	CListLabelElementUI* pItem = (CListLabelElementUI*)m_pProvinceCmb->GetItemAt(iCurProvinceSel);
	CDuiString strUserData = pItem->GetUserData();
	string pProvinceId = String::UnicodeToUTF8(strUserData.GetData());

	//市
	pItem = (CListLabelElementUI*)m_pCityCmb->GetItemAt(iCurCitySel);
	strUserData = pItem->GetUserData();
	string pCityId = String::UnicodeToUTF8(strUserData.GetData()).c_str();

	//清空区
	m_pCountyCmb->RemoveAll();

	int iCurCountyIndex = 0;
	int iCountyCnt = 0;
	for (Json::Value::iterator it = m_oLocationJsonRoot[pProvinceId.c_str()]["city"][pCityId.c_str()]["county"].begin();
		it != m_oLocationJsonRoot[pProvinceId.c_str()]["city"][pCityId.c_str()]["county"].end(); it++)
	{
		wstring strCountyId = String::UTF8ToUnicode((char*)it.memberName());
		wstring strCountyValue = String::UTF8ToUnicode((*it)["region_name"].asString().c_str());
		CListLabelElementUI *pItemCounty = new CListLabelElementUI();
		pItemCounty->SetText(strCountyValue.c_str());
		pItemCounty->SetUserData(strCountyId.c_str());
		m_pCountyCmb->Add(pItemCounty);
		iCountyCnt++;

		if (strCountyValue == m_strCounty)
		{
			iCurCountyIndex = iCountyCnt - 1;
		}
	}

	m_pCountyCmb->SelectItem(iCurCountyIndex);
}

void CConfigWnd::InitWindow()
{
	TryGetControls();
	m_pColorCombo->SetSelColor(RGB(255, 255, 255));

	InitLocationCmb();

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

	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Prov"), m_pProvinceCmb->GetItemAt(m_pProvinceCmb->GetCurSel())->GetText().GetData());
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("City"), m_pCityCmb->GetItemAt(m_pCityCmb->GetCurSel())->GetText().GetData());
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Region"), m_pCountyCmb->GetItemAt(m_pCountyCmb->GetCurSel())->GetText().GetData());

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Back"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());

	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbFresh")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Fresh"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());

	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("FontColor"), m_pColorCombo->GetSelColor());
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

	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbFresh")));
	for (int i = 0; i < pComb->GetCount(); ++i)
	{
		if (pComb->GetItemAt(i)->GetUserData().GetData() == gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Fresh"), _T("30")))
		{
			pComb->SelectItem(i);
		}
	}

	TryGetControls();

	m_pColorCombo->SetSelColor(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("FontColor"), RGB(255, 255, 255)));

	for (int i = 0; i < m_pProvinceCmb->GetCount(); ++i)
	{
		if (m_pProvinceCmb->GetItemAt(i)->GetText() == gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Prov"), _T("北京")).GetBuffer())
		{
			m_pProvinceCmb->SelectItem(i);
		}
	}
	for (int i = 0; i < m_pCityCmb->GetCount(); ++i)
	{
		if (m_pCityCmb->GetItemAt(i)->GetText() == gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("City"), _T("北京")).GetBuffer())
		{
			m_pCityCmb->SelectItem(i);
		}
	}
	for (int i = 0; i < m_pCountyCmb->GetCount(); ++i)
	{
		if (m_pCountyCmb->GetItemAt(i)->GetText() == gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Region"), _T("朝阳区")).GetBuffer())
		{
			m_pCountyCmb->SelectItem(i);
		}
	}
}

void CConfigWnd::TryGetControls()
{
	m_pProvinceCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbProvince")));
	m_pCityCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbCity")));
	m_pCountyCmb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbCounty")));
	m_pColorCombo = static_cast<CColorComboUI*>(m_PaintManager->FindControl(_T("cmbColor"))); //保证获取到
}


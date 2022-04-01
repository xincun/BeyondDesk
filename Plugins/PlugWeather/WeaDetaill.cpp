#include "stdafx.h"
#include "resource.h"
#include "../../Unify/Unify.h"
#include "WeaDetail.h"
//////////////////////////////////////////////////////////////////////////
///

CWeaDetailWnd* CWeaDetailWnd::g_pWeaDetailWnd = NULL;

CWeaDetailWnd::CWeaDetailWnd() 
{

}

void CWeaDetailWnd::SetDetailsShow()
{
	CWeatherParser &WeaParser = *m_pWeaParser;
	CWeatherGetter &WeaGetter = *m_pWeaGetter;
	CPaintManagerUI &m_pm = *m_PaintManager;

	CString s;
	CControlUI* pImg = NULL; 
	CLabelUI* pLbl = NULL;

	 auto SetWeaCtrl = [&](int n){
		if (n <= 0) return;

		CDuiString sName;

		sName.Format(_T("lbl%d"), n);
		CLabelUI* pLbl = static_cast<CLabelUI*>(m_pm.FindControl(sName));
		pLbl->SetText(WeaParser.Future[n - 1][_T("Week")].GetBuffer());

		sName.Format(_T("lbl%dWea"), n);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(sName));
		CString s = WeaParser.Future[n - 1][_T("Wea")];
		pLbl->SetText(s.GetBuffer());

		sName.Format(_T("lbl%dTemp"), n);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(sName));
		s.Format(_T("%s~%s"), WeaParser.Future[n - 1][_T("LowTemp")], WeaParser.Future[n - 1][_T("HighTemp")]);
		pLbl->SetText(s.GetBuffer());

		sName.Format(_T("img%d"), n);
		CControlUI* pImg = static_cast<CLabelUI*>(m_pm.FindControl(sName));
		s.Format(_T("Res/%s"), WeaParser.Future[n - 1][_T("Img")]);
		pImg->SetBkImage(s.GetBuffer());
	};

	SetWeaCtrl(1);
	SetWeaCtrl(2);
	SetWeaCtrl(3);
	SetWeaCtrl(4);

	return;

	//指数
	for (auto it = WeaParser.NowIndex.begin(); it != WeaParser.NowIndex.end(); ++it)
	{
		if ((*it)["Name"] == _T("穿衣指数"))
		{
			s.Format(_T("穿衣指数：%s"), (*it)[_T("Index")]);
			pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblCY")));
			pLbl->SetText(s.GetBuffer());

			CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("lblCYDesc")));
			s = (*it)[_T("Detail")];
			pEdit->SetText(s.GetBuffer());
			pEdit->SetToolTip(s.GetBuffer());

			break;
		}
	}
	for (auto it = WeaParser.NowIndex.begin(); it != WeaParser.NowIndex.end(); ++it)
	{
		if ((*it)["Name"] == _T("晾晒指数"))
		{
			s.Format(_T("晾晒指数：%s"), (*it)[_T("Index")]);
			pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblLS")));
			pLbl->SetText(s.GetBuffer());

			CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("lblLSDesc")));
			s = (*it)[_T("Detail")];
			pEdit->SetText(s.GetBuffer());
			pEdit->SetToolTip(s.GetBuffer());

			break;
		}
	}
	for (auto it = WeaParser.NowIndex.begin(); it != WeaParser.NowIndex.end(); ++it)
	{
		if ((*it)["Name"] == _T("运动指数"))
		{
			s.Format(_T("运动指数：%s"), (*it)[_T("Index")]);
			pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblYD")));
			pLbl->SetText(s.GetBuffer());

			CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("lblYDDesc")));
			s = (*it)[_T("Detail")];
			pEdit->SetText(s.GetBuffer());
			pEdit->SetToolTip(s.GetBuffer());

			break;
		}
	}
	for (auto it = WeaParser.NowIndex.begin(); it != WeaParser.NowIndex.end(); ++it)
	{
		if ((*it)["Name"] == _T("感冒指数"))
		{
			s.Format(_T("感冒指数：%s"), (*it)[_T("Index")]);
			pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblGM")));
			pLbl->SetText(s.GetBuffer());

			CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("lblGMDesc")));
			s = (*it)[_T("Detail")];
			pEdit->SetText(s.GetBuffer());
			pEdit->SetToolTip(s.GetBuffer());

			break;
		}
	}
	for (auto it = WeaParser.NowIndex.begin(); it != WeaParser.NowIndex.end(); ++it)
	{
		if ((*it)["Name"] == _T("辐射指数"))
		{
			s.Format(_T("辐射指数：%s"), (*it)[_T("Index")]);
			pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblFS")));
			pLbl->SetText(s.GetBuffer());

			CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("lblFSDesc")));
			s = (*it)[_T("Detail")];
			pEdit->SetText(s.GetBuffer());
			pEdit->SetToolTip(s.GetBuffer());

			break;
		}
	}
	for (auto it = WeaParser.NowIndex.begin(); it != WeaParser.NowIndex.end(); ++it)
	{
		if ((*it)["Name"] == _T("洗车指数"))
		{
			s.Format(_T("洗车指数：%s"), (*it)[_T("Index")]);
			pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblXC")));
			pLbl->SetText(s.GetBuffer());

			CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("lblXCDesc")));
			s = (*it)[_T("Detail")];
			pEdit->SetText(s.GetBuffer());
			pEdit->SetToolTip(s.GetBuffer());

			break;
		}
	}
}

DuiLib::CDuiString CWeaDetailWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("\\Res");
#else
	return _T("\\Res");
#endif
}

LPCTSTR CWeaDetailWnd::GetWindowClassName(void) const
{
	return _T("UIWeaDeatail");
}

DuiLib::CDuiString CWeaDetailWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("Res\\WndWeaDetail.xml")).c_str();
}


void CWeaDetailWnd::OnClick( TNotifyUI &msg )
{
	return __super::OnClick(msg);
}

void CWeaDetailWnd::OnTimer(void* pOwner, UINT uiID)
{
	
}

void CWeaDetailWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);

	delete this;
}

void CWeaDetailWnd::Notify(TNotifyUI &msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{

		return;
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{


		return;
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender == m_pOpt1 || msg.pSender == m_pOpt2 || msg.pSender == m_pOpt3)
		{
			if (m_pOpt1->IsSelected())
			{
				m_pTab->SelectItem(0);
			}
			else if (m_pOpt2->IsSelected())
			{
				m_pTab->SelectItem(1);
			}
			else if (m_pOpt3->IsSelected())
			{
				m_pTab->SelectItem(2);
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
	{

	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("closebtn"))
		{
			this->Close(IDOK);
			g_pWeaDetailWnd = NULL;
			return;
		}
	}
		
	return __super::Notify(msg);
}

void CWeaDetailWnd::InitWindow()
{
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE);

	//gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));

	m_pTab = static_cast<CTabLayoutUI*>(m_PaintManager->FindControl(_T("tabMain")));
	m_pOpt1 = static_cast<COptionUI*>(m_PaintManager->FindControl(_T("opt1")));
	m_pOpt2 = static_cast<COptionUI*>(m_PaintManager->FindControl(_T("opt2")));
	m_pOpt3 = static_cast<COptionUI*>(m_PaintManager->FindControl(_T("opt3")));
	if (m_pOpt1)
		m_pOpt1->Selected(true);
	else if (m_pOpt2)
		m_pOpt2->Selected(true);
	else if (m_pOpt3)
		m_pOpt3->Selected(true);

	m_bInited = true;


}

LRESULT CWeaDetailWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

CControlUI* CWeaDetailWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	return NULL;
}

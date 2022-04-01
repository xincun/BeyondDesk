#include "stdafx.h"
#include "resource.h"
#include "ConfigWnd.h"
#include "../../Unify/Unify.h"
#include "Img/ContourBitmap.h"
#include "DeskWnd.h"
//////////////////////////////////////////////////////////////////////////
///

DuiLib::CDuiString CConfigWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("PlugCfgDigit.xml")).c_str();
	//return _T("XML\\PlugCfgDigit.xml");
}


void CConfigWnd::OnClick( TNotifyUI &msg )
{
	return __super::OnClick(msg);
}

void CConfigWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		

		
	}
	else
	if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		/*if (msg.pSender->GetName() == _T("opt1"))
		{
			::ShowWindow(m_hWndPre, SW_SHOW);
			if (m_bInited)
				DrawPreview();
		}
		else if (msg.pSender->GetName() == _T("opt2"))
		{
			::ShowWindow(m_hWndPre, SW_HIDE);
		}*/
	}

	return __super::Notify(msg);
}

void CConfigWnd::InitWindow()
{
	__super::InitWindow();

	m_pOpt1->SetVisible(false);
	m_pOpt2->Selected(true);
	m_pTab->SelectItem(1);
}

LRESULT CConfigWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

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

	
}

void CConfigWnd::LoadCfg()
{
	__super::LoadCfg();

	
}


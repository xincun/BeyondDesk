#include "stdafx.h"
#include "resource.h"
#include "ConfigWnd.h"
#include "../../Unify/Unify.h"
#include "DeskWnd.h"
#include <future>

//////////////////////////////////////////////////////////////////////////
///

DuiLib::CDuiString CConfigWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("Res\\PlugCfgNote.xml")).c_str();
}


void CConfigWnd::OnClick( TNotifyUI &msg )
{
	return __super::OnClick(msg);
}

void CConfigWnd::OnTimer(void* pOwner, UINT uiID)
{
	//DrawPreview();
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

	
}

void CConfigWnd::LoadCfg()
{
	__super::LoadCfg();

	
}


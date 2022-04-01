#include "stdafx.h"
#include "resource.h"
#include "BootWnd.h"
#include <shellapi.h>

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CBootWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CBootWnd::CBootWnd(HWND hMainWnd)
{
	m_hMainWnd = hMainWnd;
}

CBootWnd::~CBootWnd(void)
{
	
}

void CBootWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CBootWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CBootWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Plugins\\Boot\\Res\\");
#else
	return _T("Plugins\\Boot\\Res\\");
#endif
	
}

DuiLib::CDuiString CBootWnd::GetSkinFile()
{
	//return _T("BootWnd.xml");
	return _T("MsgBox.xml");
}

UILIB_RESOURCETYPE CBootWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CBootWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CBootWnd::GetWindowClassName( void ) const
{
	return _T("UIBootWnd");
}

void CBootWnd::OnClick( TNotifyUI &msg )
{
	if( msg.pSender->GetName() == _T("closebtn") ) 
	{   //OutputDebugString("AboutWndClose....");
		Close();
	}else if( msg.pSender == m_pMinBtn ) 
	{ 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		return; 
	}else if( msg.pSender == m_pMaxBtn ) 
	{ 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
		return; 
	}else if( msg.pSender == m_pRestoreBtn ) 
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		return;
	}
	else if( msg.pSender->GetName() == _T("quitbtn") ) 
	{
		Close();
	}
	else if( msg.pSender->GetName() == _T("btnClose") ) 
	{
        Close();
	}
	else if( msg.pSender->GetName() == _T("btnMail") ) 
	{
		CTextUI *pTxt = static_cast<CTextUI*>(m_PaintManager->FindControl(_T("btnMail")));

		if (pTxt)
		{ 
			TString s = _T("mailto:");
			s += pTxt->GetText();
            ShellExecute(0, _T("open"), s.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
	}
}

void CBootWnd::OnSelectChanged( TNotifyUI &msg )
{
	
}

void CBootWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CBootWnd::Notify( TNotifyUI &msg )
{
	return WindowImplBase::Notify(msg);
}

LRESULT CBootWnd::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBootWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		Close();
		bHandled = TRUE;
		return 0;
	}
	//BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	/*if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}*/
	bHandled = false;
	return lRes;
}

void CBootWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));

	InitControl();
}

LRESULT CBootWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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

LRESULT CBootWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}

void CBootWnd::OnItemSelect( TNotifyUI &msg )
{
	
}

void CBootWnd::InitControl()
{
	//CLabelUI *pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblRemind")));
	//if (pLbl != NULL)
	//{
	//	//pLbl->SetText(p->RemindTitle);

	//	UINT uStyle = pLbl->GetTextStyle();  
	//	uStyle &= (~DT_SINGLELINE);
	//	uStyle |= (DT_WORDBREAK);  

	//	if (pLbl->GetText().GetLength() > 25)
	//	{
	//		uStyle &= (~DT_CENTER);  
	//	}
	//	else
	//	{
	//		uStyle |= (DT_CENTER);  
	//	}

	//	pLbl->SetTextStyle(uStyle);
	//}
	
}

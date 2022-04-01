#include "stdafx.h"
#include "../resource.h"
#include "DlgTipExit.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CTipExitWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()

CTipExitWnd::CTipExitWnd()
{
	
}

CTipExitWnd::~CTipExitWnd(void)
{
	
}

void CTipExitWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	
	delete this;
}

DuiLib::CDuiString CTipExitWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("");
#else
	return _T("");
#endif
	
}

DuiLib::CDuiString CTipExitWnd::GetSkinFile()
{
	return _T("XML\\DlgTipExit.xml");
}

UILIB_RESOURCETYPE CTipExitWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CTipExitWnd::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CTipExitWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CTipExitWnd::GetWindowClassName( void ) const
{
	return _T("UITipExitWnd");
}

void CTipExitWnd::OnClick( TNotifyUI &msg )
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
	else if (msg.pSender->GetName() == _T("btnOK"))
	{
		gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkCloseToTray"), FindControl<COptionUI>("chkCloseToTray")->IsSelected());
		gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkCloseToExit"), FindControl<COptionUI>("chkCloseToExit")->IsSelected());
		gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("TipExit"), !FindControl<CCheckBoxUI>("chkNoTip")->IsSelected());

		Close(IDOK);
	}
	else if (msg.pSender->GetName() == _T("btnCancel"))
	{


		Close(IDCANCEL);
	}
}

void CTipExitWnd::Notify( TNotifyUI &msg )
{
	return WindowImplBase::Notify(msg);
}

LRESULT CTipExitWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager->FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager->FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager->FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager->FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}*/
	bHandled = false;
	return lRes;
}

void CTipExitWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));

	FindControl<COptionUI>("chkCloseToTray")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkCloseToTray"), true));
	FindControl<COptionUI>("chkCloseToExit")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkCloseToExit"), false));

	InitControl();
}

LRESULT CTipExitWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}

void CTipExitWnd::InitControl()
{
	//CLabelUI *pLbl = static_cast<CLabelUI*>(m_PaintManager->FindControl(_T("lblRemind")));
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

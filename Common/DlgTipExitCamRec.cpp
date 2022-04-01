#include "stdafx.h"
#include "../resource.h"
#include "DlgTipExitCamRec.h"
#include "IniFile.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CCamRecTipExitWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()

CCamRecTipExitWnd::CCamRecTipExitWnd()
{
	
}

CCamRecTipExitWnd::~CCamRecTipExitWnd()
{
	
}

void CCamRecTipExitWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	
	delete this;
}

DuiLib::CDuiString CCamRecTipExitWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("");
#else
	return _T("");
#endif
	
}

DuiLib::CDuiString CCamRecTipExitWnd::GetSkinFile()
{
	return _T("XML\\DlgTipExitCamRec.xml");
}

UILIB_RESOURCETYPE CCamRecTipExitWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CCamRecTipExitWnd::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CCamRecTipExitWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CCamRecTipExitWnd::GetWindowClassName( void ) const
{
	return _T("UITipExitWnd");
}

void CCamRecTipExitWnd::OnClick( TNotifyUI &msg )
{
	if( msg.pSender->GetName() == _T("closebtn") ) 
	{   //OutputDebugString("AboutWndClose....");
		Close(IDCANCEL);
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
		Close(IDCANCEL);
	}
	else if( msg.pSender->GetName() == _T("btnClose") ) 
	{
		Close(IDCANCEL);
	}
	else if (msg.pSender->GetName() == _T("btnOK"))
	{
		/*gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkClosePre"), FindControl<COptionUI>("chkClosePre")->IsSelected());
		gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkCloseRec"), FindControl<COptionUI>("chkCloseRec")->IsSelected());
		gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("TipExitCamRec"), !FindControl<CCheckBoxUI>("chkNoTip")->IsSelected());*/

		if (FindControl<COptionUI>("chkClosePre")->IsSelected())
			Close(IDYES);
		else
			Close(IDNO);
	}
	else if (msg.pSender->GetName() == _T("btnCancel"))
	{
		Close(IDCANCEL);
	}
}

void CCamRecTipExitWnd::Notify( TNotifyUI &msg )
{
	return WindowImplBase::Notify(msg);
}

LRESULT CCamRecTipExitWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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

void CCamRecTipExitWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));
	FindControl<COptionUI>("chkClosePre")->Selected(TRUE);
	/*(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkClosePre"), true));
	FindControl<COptionUI>("chkCloseRec")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkCloseRec"), false));*/

	InitControl();
}

LRESULT CCamRecTipExitWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}

void CCamRecTipExitWnd::InitControl()
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

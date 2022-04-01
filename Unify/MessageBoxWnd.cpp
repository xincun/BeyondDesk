#include "stdafx.h"
#include "resource.h"
#include "MessageboxWnd.h"
#include <shellapi.h>

//////////////////////////////////////////////////////////////////////////
///
CMsgBoxWnd* CMsgBoxWnd::g_pMsgBoxWnd = NULL;

DUI_BEGIN_MESSAGE_MAP(CMsgBoxWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()

CMsgBoxWnd::CMsgBoxWnd(HWND hMainWnd, LPCTSTR lpText, LPCTSTR lpCation, UINT uType):
m_hMainWnd(hMainWnd),
m_lpText(lpText),
m_lpCaiton(lpCation),
m_uType(uType)
{
	
}

UINT CMsgBoxWnd::CreateInstance(HWND hMainWnd, LPCTSTR lpText, LPCTSTR lpCation, UINT uType)
{
	if (g_pMsgBoxWnd == NULL)
	{
		g_pMsgBoxWnd = new CMsgBoxWnd(hMainWnd, lpText, lpCation, uType);
		if (g_pMsgBoxWnd == NULL) return IDCANCEL;

		g_pMsgBoxWnd->Create(hMainWnd, lpCation, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);

		g_pMsgBoxWnd->CenterWindow();
	
		return g_pMsgBoxWnd->ShowModal();
	}

	//SetWindowPos(g_pMsgBoxWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetForegroundWindow(g_pMsgBoxWnd->GetHWND());
	return IDCANCEL;
}

CMsgBoxWnd::~CMsgBoxWnd(void)
{
	g_pMsgBoxWnd = NULL;
}

void CMsgBoxWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	
	delete this;
}

DuiLib::CDuiString CMsgBoxWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CMsgBoxWnd::GetSkinFile()
{
	return _T("MsgBox\\MsgBox.xml");
}

UILIB_RESOURCETYPE CMsgBoxWnd::GetResourceType() const
{
	#ifdef _DEBUG
	    return UILIB_FILE;
	#else
	     return UILIB_FILE;// UILIB_ZIP;
	#endif
}

DuiLib::CDuiString CMsgBoxWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CMsgBoxWnd::GetWindowClassName( void ) const
{
	return _T("UIMessageboxWnd");
}

void CMsgBoxWnd::OnClick( TNotifyUI &msg )
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
	else if( msg.pSender->GetName() == _T("btnOK") ) 
	{
		Close(IDOK);
	}
	else if( msg.pSender->GetName() == _T("btnCancel") ) 
	{
        Close(IDCANCEL);
	}
	else if (msg.pSender->GetName() == _T("btnYes"))
	{
		Close(IDYES);
	}
	else if (msg.pSender->GetName() == _T("btnNo"))
	{
		Close(IDNO);
	}
}

void CMsgBoxWnd::Notify( TNotifyUI &msg )
{
	return WindowImplBase::Notify(msg);
}

LRESULT CMsgBoxWnd::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMsgBoxWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}*/
	bHandled = false;
	return lRes;
}

void CMsgBoxWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));

	InitControl();

	SetFlags(m_uType);
}

LRESULT CMsgBoxWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}

void CMsgBoxWnd::InitControl()
{
	
}

void CMsgBoxWnd::SetFlags(UINT utype)
{
	const int BUTTON_WIDTH = 64;

	CControlUI *pImg = FindControl<CControlUI>(_T("imgMsg"));
	CLabelUI *pLbl = FindControl<CLabelUI>(_T("lblMsg"));
	pLbl->SetText(m_lpText);
	if (pLbl != NULL) //换行支持
	{
		UINT uStyle = pLbl->GetTextStyle();
		uStyle &= (~DT_SINGLELINE);
		uStyle |= (DT_WORDBREAK);

		if (Window::GetTextSize(m_hWnd, pLbl->GetText()).cx > 200)
		{
			uStyle &= (~DT_CENTER);
			//uStyle &= (DT_LEFT);
		}
		else
		{
			uStyle |= (DT_CENTER);
		}

		pLbl->SetTextStyle(uStyle);
	}

	CTextUI *pTxt = FindControl<CTextUI>(_T("txtTitle"));
	if (m_lpCaiton && _tcscmp(m_lpCaiton, _T("")) != 0)
	    pTxt->SetText(m_lpCaiton);
	CButtonUI *pBtnOK = FindControl<CButtonUI>(_T("btnOK"));
	CButtonUI *pBtnCancel = FindControl<CButtonUI>(_T("btnCancel"));
	CButtonUI *pBtnYes = FindControl<CButtonUI>(_T("btnYes"));
	CButtonUI *pBtnNo = FindControl<CButtonUI>(_T("btnNo"));

	RECT rcWnd;
	GetWindowRect(this->m_hWnd, &rcWnd);

	wstring sImg = _T(""); 
	/*if ((utype & MB_ICONINFORMATION) == MB_ICONINFORMATION)
		sImg = _T("MsgBox\\sys_info.png");
	else if ((utype &  MB_ICONWARNING )== MB_ICONWARNING)
		sImg =_T("MsgBox\\sys_warning.png");
	else if ((utype &  MB_ICONQUESTION) == MB_ICONQUESTION)
	    sImg =  _T("MsgBox\\sys_confirm.png");
	else if ((utype &  MB_ICONERROR) == MB_ICONERROR)
		sImg =  _T("MsgBox\\sys_error.png");
	else if ((utype &  MB_ICONSTOP) == MB_ICONSTOP)
	    sImg = _T("MsgBox\\sys_stop.png");*/
	
	
	if (_tcscmp(sImg.c_str(), _T("")) != 0)
	{
		FindControl<CControlUI>(_T("ctrlRerserve"))->SetVisible(false);
		FindControl<CControlUI>(_T("imgMsg"))->SetVisible(true);
		pImg->SetBkImage(sImg.c_str());
		pImg->SetVisible(true);
	}
	else
	{
		FindControl<CControlUI>(_T("ctrlRerserve"))->SetVisible(true);
		FindControl<CControlUI>(_T("imgMsg"))->SetVisible(false);
		pImg->SetFixedWidth(1);
		RECT rc;
		rc.left = 10;
		rc.top = 10;
		rc.bottom = 30;
		rc.right = rcWnd.right - rc.left * 2;

		pLbl->SetPos(rc);
	}

	if ((utype &  MB_YESNOCANCEL) == MB_YESNOCANCEL)
	{
		pBtnOK->SetVisible(false);

		RECT rc;
		rc.left = (rcWnd.right - rcWnd.left - BUTTON_WIDTH * 3 - 10) / 2;
		rc.top = 2;
		rc.bottom = 30;
		rc.right = rc.left + BUTTON_WIDTH;
		pBtnYes->SetPos(rc);

		rc.left += BUTTON_WIDTH + 5;
		rc.right = rc.left + BUTTON_WIDTH + 5;
		pBtnNo->SetPos(rc);

		rc.left += BUTTON_WIDTH + 10;
		rc.right = rc.left + BUTTON_WIDTH + 10;
		pBtnCancel->SetPos(rc);
	}
	else if ((utype &  MB_OKCANCEL) == MB_OKCANCEL)
	{
		pBtnYes->SetVisible(false);
		pBtnNo->SetVisible(false);

		RECT rc;
		rc.left = (rcWnd.right - rcWnd.left - BUTTON_WIDTH * 2 - 10) / 2;
		rc.top = 2;
		rc.bottom = 30;
		rc.right = rc.left + BUTTON_WIDTH;
		pBtnOK->SetPos(rc);

		rc.left += BUTTON_WIDTH + 10;
		rc.right = rc.left + BUTTON_WIDTH + 10;
		pBtnCancel->SetPos(rc);
	}
	else if ((utype &  MB_YESNO) == MB_YESNO)
	{
		pBtnOK->SetVisible(false);
		pBtnCancel->SetVisible(false);

		RECT rc;
		rc.left = (rcWnd.right - rcWnd.left - BUTTON_WIDTH * 2 - 10) / 2;
		rc.top = 2;
		rc.bottom = 30;
		rc.right = rc.left + BUTTON_WIDTH;
		pBtnYes->SetPos(rc);

		rc.left += BUTTON_WIDTH + 10;
		rc.right = rc.left + BUTTON_WIDTH + 10;
		pBtnNo->SetPos(rc);
	}
	else
	{
		pBtnYes->SetVisible(false);
		pBtnNo->SetVisible(false);
		pBtnCancel->SetVisible(false);

		RECT rc;
		rc.left = (rcWnd.right - rcWnd.left - BUTTON_WIDTH) / 2;
		rc.top = 2;
		rc.bottom = 30;
		rc.right = rc.left + BUTTON_WIDTH;
		pBtnOK->SetPos(rc);
	}
}

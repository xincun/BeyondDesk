#include "stdafx.h"
#include "resource.h"
#include "AboutWnd.h"
#include "Reg/Reg.h"
//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CAboutWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()

CAboutWnd::CAboutWnd()
{
	
}

CAboutWnd::~CAboutWnd(void)
{
	
}

void CAboutWnd::OnFinalMessage( HWND hWnd)
{
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

DuiLib::CDuiString CAboutWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("");
#else
	return _T("");
#endif
	
}

DuiLib::CDuiString CAboutWnd::GetSkinFile()
{
	return _T("XML\\DlgAbout.xml");
}

UILIB_RESOURCETYPE CAboutWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CAboutWnd::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CAboutWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CAboutWnd::GetWindowClassName( void ) const
{
	return _T("UIAboutWnd");
}

void CAboutWnd::OnClick( TNotifyUI &msg )
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
	else if( msg.pSender->GetName() == _T("btnOK") ) 
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
			CString s = _T("mailto:");
			s += pTxt->GetText();
            ShellExecute(0, _T("open"), s, NULL, NULL, SW_SHOWNORMAL);
		}
	}
	else if (msg.pSender->GetName() == _T("btnWX"))
	{
		CTextUI *pTxt = static_cast<CTextUI*>(m_PaintManager->FindControl(_T("btnWX")));

		if (pTxt)
		{
			CString s = _T("mailto:");
			s += pTxt->GetText();
			ShellExecute(0, _T("open"), s, NULL, NULL, SW_SHOWNORMAL);
		}
	}
	else if (msg.pSender->GetName() == _T("btnReg")) {
		CReg::showRegWnd();
	}
	else if (msg.pSender->GetName() == _T("btnSite"))
	{
		CTextUI *pTxt = static_cast<CTextUI*>(m_PaintManager->FindControl(_T("btnSite")));

		if (pTxt)
		{
			Net::OpenURLInDefaultBrowser(pTxt->GetText().GetData());
		}
	}
}

void CAboutWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		/*CWaterEffectUI* pWater = FindControl<CWaterEffectUI>("water");

		CVerticalLayoutUI* pVert = FindControl<CVerticalLayoutUI>("vertBack");
		HBITMAP hBmp = pVert->GetBKImageObj();
		Gdiplus::Bitmap *pBmp = Util::Image::BitmapFromHBITMAP(hBmp);
		Util::Image::SaveBmpToPngFile(pBmp, (gUnify->App()->Path()->TempPath +  _T("BKImage.png")).c_str());
	
		pWater->Init((gUnify->App()->Path()->TempPath + _T("BKImage.png")).c_str());*/

		/*CWaterEffectUI* pWater = new CWaterEffectUI;
		pWater->SetManager(m_PaintManager, NULL);
		pWater->Init(m_hWnd, CRect(0, 0, 300, 300), L"D:\[Work]\desktop\BeyondDesk\RESOURCE\[Res]\Back\Blue2.png");*/
	}

	return WindowImplBase::Notify(msg);
}

LRESULT CAboutWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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

void CAboutWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));

	CDuiString s;
	// if (CReg::isReg()){
	// 	s.Format(_T("版本：V%s  注册版"), APP_VER);
 //
	// 	CButtonUI* reg = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnReg")));
	// 	reg->SetVisible(false);
 //    }
	// else {
	// 	s.Format(_T("版本：V%s  30天试用版"), APP_VER);
	// }

	s.Format(_T("版本：V%s"), APP_VER);

	CButtonUI* reg = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnReg")));
	reg->SetVisible(false);

	CLabelUI* pLbl = static_cast<CLabelUI*>(m_PaintManager->FindControl(_T("lblVer")));
	pLbl->SetText(s);
}

LRESULT CAboutWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}

#include "stdafx.h"
#include "resource.h"
#include "VolWnd.h"
#include "DLLInvoke.h"
#include "DuiHelper.h"
#include "../resource.h"

CVolWnd* CVolWnd::g_pVolWnd = nullptr;

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CVolWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()


CVolWnd::CVolWnd()
{
	
}

void CVolWnd::OnFinalMessage( HWND hWnd)
{
	//gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

DuiLib::CDuiString CVolWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("");
#else
	return _T("");
#endif
	
}

DuiLib::CDuiString CVolWnd::GetSkinFile()
{
	return _T("XML\\VolWnd.xml");
}

UILIB_RESOURCETYPE CVolWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CVolWnd::GetResourceID() const
{
	return   MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CVolWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CVolWnd::GetWindowClassName( void ) const
{
	return _T("UIVolumeWnd");
}

void CVolWnd::OnClick( TNotifyUI &msg )
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
	
}

void CVolWnd::Notify( TNotifyUI &msg )
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

LRESULT CVolWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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

void CVolWnd::InitWindow()
{
	//gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	
}

LRESULT CVolWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}

#include "stdafx.h"
#include "PalletDialog.h"
#include "../resource.h"

CPalletDialog::CPalletDialog(DWORD dwBKColor, LPCTSTR pstrBKImage) :
//m_pSkinPickerWnd(pSkinPickerWnd),
m_InitBKColor(dwBKColor),
m_InitBKImage(pstrBKImage),
	m_pBtnCancel(NULL),
	m_pBtnOk(NULL),
	m_pPallet(NULL),
	m_SelectedColor(0)
{
}

CPalletDialog::~CPalletDialog()
{

}

DuiLib::CDuiString CPalletDialog::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("");
#else
	return _T("");
#endif

}

DuiLib::CDuiString CPalletDialog::GetSkinFile()
{
	return _T("XML\\DlgPallet.xml");
}

UILIB_RESOURCETYPE CPalletDialog::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CPalletDialog::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CPalletDialog::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CPalletDialog::GetWindowClassName() const 
{ 
	return _T("UIPalletDialog");
}

UINT CPalletDialog::GetClassStyle() const 
{ 
	return CS_DBLCLKS;
}


void CPalletDialog::OnFinalMessage(HWND /*hWnd*/) 
{ 

	return;
}

void CPalletDialog::InitWindow()
{
	 m_pBtnCancel = static_cast<CButtonUI*>(m_PaintManager->FindControl(kPalletCancel));
	 m_pBtnOk = static_cast<CButtonUI*>(m_PaintManager->FindControl(kPalletOk));
	 m_pPallet = static_cast<CColorPaletteUI*>(m_PaintManager->FindControl(kPalletPallet));

	 /*SetBkColor(m_pSkinPickerWnd->GetBkColor());
	 SetBkImage(m_pSkinPickerWnd->GetBkImage());*/
	SetBkColor(m_InitBKColor);
	SetBkImage(m_InitBKImage);
}

CControlUI* CPalletDialog::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	if (_tcsicmp(pstrClass, kPalletInterface) == 0)
		return	new CColorPaletteUI();

	return NULL;
}

DWORD CPalletDialog::GetBkColor() const
{
	CControlUI* background = m_PaintManager->FindControl(L"vertBack");
	if (background != NULL)
		return background->GetBkColor();
	return 0;
}

void CPalletDialog::SetBkColor(DWORD dwBackColor)
{
	CControlUI* background = m_PaintManager->FindControl(L"vertBack");
	if (background != NULL)
	{
		background->SetBkImage(_T(""));
		background->SetBkColor(dwBackColor);
		background->NeedUpdate();
	}
	if (m_pPallet)
	{
		m_pPallet->SetSelectColor(dwBackColor);
	}
}

void CPalletDialog::SetBkImage(CDuiString strBkImage)
{
	CControlUI* background = m_PaintManager->FindControl(L"vertBack");
	if (background != NULL)
		background->SetBkImage(strBkImage.GetData());
}

LPCTSTR CPalletDialog::GetBkImage()
{
	CControlUI* background = m_PaintManager->FindControl(L"vertBack");
	if (background != NULL)
		return background->GetBkImage();
	return _T("");
}

void CPalletDialog::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == _T("closebtn")) 
		{
			Close(IDCANCEL);
			return; 
		}
		if( msg.pSender->GetName() == _T("okbtn")) 
		{
			if (m_pPallet)
			{
				m_SelectedColor = m_pPallet->GetSelectColor();
				Close(IDOK);
				return; 
			} 
			else
			{
				Close(IDCANCEL);
				return; 
			}
			
		}
	}

}

DWORD CPalletDialog::GetSelectColor()
{
	return m_SelectedColor;
}

LRESULT CPalletDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
	}
	return lRes;
}

LRESULT CPalletDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}


#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "../resource.h"
#include "HkEditWnd.h"
#include "GlobalDefine.h"
#include "UIHotKeyProxy.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CHkEditWnd, WindowImplBase)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
    DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CHkEditWnd::CHkEditWnd()
{
	m_hTimeCtrl = NULL;
	
	m_nHotkeyTypeID = L"";
}

CHkEditWnd::~CHkEditWnd(void)
{
	
}

void CHkEditWnd::OnFinalMessage( HWND hWnd)
{
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);
    __super::OnFinalMessage(hWnd);
 //   delete m_pFont;
    //delete this;
}

DuiLib::CDuiString CHkEditWnd::GetSkinFolder()
{
#ifdef _DEBUG
    return _T("");
#else
    return _T("");
#endif
}

DuiLib::CDuiString CHkEditWnd::GetSkinFile()
{
	return _T("XML\\DlgHkEdit.xml");
	//return _T("DlgHotkeyEdit.xml");
}

UILIB_RESOURCETYPE CHkEditWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CHkEditWnd::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CHkEditWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CHkEditWnd::GetWindowClassName( void ) const
{
    return _T("UIHotkeyEditWnd");
}

void CHkEditWnd::OnClick( TNotifyUI &msg )
{
    if( msg.pSender == m_pCloseBtn ) 
    { 
        Close();
        //return; 
    }
    else if( msg.pSender == m_pMinBtn ) 
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
    else if( msg.pSender->GetName() == _T("btnOK") ) 
    {
		OnOK();

    }
    else if( msg.pSender->GetName() == _T("btnCancel") ) 
    {
        Close(IDCANCEL);
    }
	
	
	
}

void CHkEditWnd::OnOK()
{
	if (CheckInput())
	{
		SaveHK();

		Close(IDOK);
	}
}

void CHkEditWnd::OnSelectChanged(TNotifyUI &msg)
{
	
    
}

void CHkEditWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CHkEditWnd::Notify( TNotifyUI &msg )
{
	
	

    return WindowImplBase::Notify(msg);
}



LRESULT CHkEditWnd::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT CHkEditWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
    return lRes;
}

void CHkEditWnd::InitWindow()
{
    m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE);
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

    m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
    m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
    m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
    m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));
    
    
    //static_cast<COptionUI*>(m_PaintManager->FindControl(_T("optShut1")))->Selected(TRUE);
    

    InitControl();
}

void CHkEditWnd::InitControl()
{
	if (Modifiers.Find(L"Ctrl") >= 0) static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(L"chkCtrl"))->SetCheck(true);
	if (Modifiers.Find(L"Alt") >= 0) static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(L"chkAlt"))->SetCheck(true);
	if (Modifiers.Find(L"Shift") >= 0) static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(L"chkShift"))->SetCheck(true);
	if (Modifiers.Find(L"Win") >= 0) static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(L"chkWin"))->SetCheck(true);
	
	CComboUI *pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(L"cmbKey"));
	pComb->SelectItem(0);
	for (int i = 0; i < pComb->GetCount(); i++)
	{
		CListLabelElementUI *pListE = static_cast<CListLabelElementUI*>(pComb->GetItemAt(i));

		/*if (IsUserData)
		{
			if (lstrcmp(pListE->GetUserData().GetData(), p->GetParam(szParamName).c_str()) == 0)
			{
				pComb->SelectItem(i);
				break;
			}
		}
		else*/
		{
			if (Key ==  pListE->GetTag())
			{
				pComb->SelectItem(i);
				break;
			}
		}
	}
}

LRESULT CHkEditWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
   
    return 0;
}

LRESULT CHkEditWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
    if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
    {
        return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
    }

    return 0;	  
}

LRESULT CHkEditWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//LOGCON_WM(uMsg);
	if (uMsg == WM_CLOSE)
	{
		//CAVCapDLL::StopPreview();
		bHandled = FALSE;
	}

	return 0;
}

void CHkEditWnd::OnItemSelect(TNotifyUI &msg)
{
	
}



CControlUI* CHkEditWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
    

    return NULL;
}

LRESULT CHkEditWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		OnOK();
	}
	else if (wParam == VK_ESCAPE)
	{
		if (m_bEscCloseSelf)
		{
			Close();
			return TRUE;
		}
	}
}

BOOL CHkEditWnd::CheckInput()
{
	if (!(FindControl<CCheckBoxUI>("chkCtrl")->IsSelected() ||
		FindControl<CCheckBoxUI>("chkAlt")->IsSelected() ||
		FindControl<CCheckBoxUI>("chkShift")->IsSelected() ||
		FindControl<CCheckBoxUI>("chkWin")->IsSelected() ||
		FindControl<CComboUI>("cmbKey")->GetCurSel() > 0))
	{
		MessageBox(this->m_hWnd, _T("没有选择任何一个热键！"), _T("警告"), MB_OK);
		
		return FALSE;
	}
	else
	{
		WORD wVirtualKeyCode = FindControl<CComboUI>("cmbKey")->GetItemAt(FindControl<CComboUI>("cmbKey")->GetCurSel())->GetTag();
		WORD wModifiers = 0;
		if (FindControl<CCheckBoxUI>("chkCtrl")->IsSelected()) wModifiers |= MOD_CONTROL;
		if (FindControl<CCheckBoxUI>("chkAlt")->IsSelected()) wModifiers |= MOD_ALT;
		if (FindControl<CCheckBoxUI>("chkShift")->IsSelected()) wModifiers |= MOD_SHIFT;
		if (FindControl<CCheckBoxUI>("chkWin")->IsSelected()) wModifiers |= MOD_WIN;

		WORD wVirtualKeyCodeOld = 0;
		WORD wModifiersOld = 0;
		wVirtualKeyCodeOld = Key;
			if (Modifiers.Find(L"Ctrl") >= 0) wModifiersOld |= MOD_CONTROL;
			if (Modifiers.Find(L"Alt") >= 0) wModifiersOld |= MOD_ALT;
			if (Modifiers.Find(L"Shift") >= 0) wModifiersOld |= MOD_SHIFT;
			if (Modifiers.Find(L"Win") >= 0) wModifiersOld |= MOD_WIN;
		

		UIHotkeyProxy::GetInstance()->HotKey()->Stop();

		bool bNew = true;
		if (wModifiers == wModifiersOld && wVirtualKeyCode == wVirtualKeyCodeOld)
			bNew = false;
		else
			bNew = true;

		int nCount = UIHotkeyProxy::GetInstance()->HotKey()->TryRegisterHotKey(wModifiers, wVirtualKeyCode);
		bool b = false;
		if (bNew)
			b = nCount == 1;
		if (b)
		{
			MessageBox(this->m_hWnd, _T("热键重复，请重新选择！"), _T("警告"), MB_OK);
			static_cast<CTabLayoutUI*>(m_PaintManager->FindControl(_T("switch")))->SelectItem(2);
			UIHotkeyProxy::GetInstance()->HotKey()->Start();
			return FALSE;
		}
		UIHotkeyProxy::GetInstance()->HotKey()->Start();
	}

    return TRUE;
}

void CHkEditWnd::SaveHK()
{
	Modifiers = L"";
	Key = 0;

	if (static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(L"chkCtrl"))->GetCheck())
		Modifiers += L"Ctrl+";
	if (static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(L"chkAlt"))->GetCheck())
		Modifiers += L"Alt+";
	if (static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(L"chkShift"))->GetCheck())
		Modifiers += L"Shift+";
	if (static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(L"chkWin"))->GetCheck())
		Modifiers += L"Win+";

	CComboUI *pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbKey")));
	CListLabelElementUI *pElement = static_cast<CListLabelElementUI*>(pComb->GetItemAt(pComb->GetCurSel()));
	Key = pElement->GetTag();
	Modifiers += pElement->GetText();

	/*if (isAdd)
	    pHotkeyMgr->NotifyTaskerUpdate(utAdd, pTasker);
	else
		pHotkeyMgr->NotifyTaskerUpdate(utEdit, pTasker, m_Tasker);*/
}

#include "stdafx.h"
#include "resource.h"
#include "ConfigWnd.h"
#include "Plugins/Common/DeskWnd.h"
#include "HkEditWnd.h"


//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CConfigWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()

CConfigWnd::CConfigWnd()
{
	gUnify->Core()->Event()->AddEventListen(EVENT_DESK_WND_CLOSE, this, &CConfigWnd::OnEvent);
}

CConfigWnd::~CConfigWnd(void)
{
	gUnify->Core()->Event()->RemoveEventListen(EVENT_DESK_WND_CLOSE, this, &CConfigWnd::OnEvent);

	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);
}

void CConfigWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	
	delete this;
}

DuiLib::CDuiString CConfigWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("");
#else
	return _T("");
#endif
	
}

DuiLib::CDuiString CConfigWnd::GetSkinFile()
{
	return _T("XML\\DlgConfig.xml");
}

UILIB_RESOURCETYPE CConfigWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CConfigWnd::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CConfigWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CConfigWnd::GetWindowClassName( void ) const
{
	return _T("UIConfigWnd");
}

void CConfigWnd::OnClick( TNotifyUI &msg )
{
    if( msg.pSender == m_pMinBtn ) 
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
	else if (msg.pSender == m_pCloseBtn)
	{
		Close();
		return;
	}
	else if( msg.pSender->GetName() == _T("btnCancel") ) 
	{
        Close();
	}
	else if (msg.pSender->GetName() == _T("btnOK"))
	{
		SaveCfg();


		Close();
	}
	else if (msg.pSender->GetName() == _T("optFuncCal"))
	{
		/*if (FindControl<COptionUI>("optFuncCal")->IsSelected()) {
			if (MessageBox(this->m_hWnd, _T("关闭日历将清除所有的日历提醒，您确定要继续关闭吗？"), _T(""), MB_OKCANCEL) == IDCANCEL) {
				FindControl<COptionUI>("optFuncCal")->Selected(false);
			}
			else
				FindControl<COptionUI>("optFuncCal")->Selected(true);
		}*/
	}
	else if (msg.pSender->GetName() == _T("optFuncToday"))
	{
		if (FindControl<COptionUI>("optFuncToday")->IsSelected()) {
			FindControl<CLabelUI>("lblTodayWeather")->SetVisible(false);
			FindControl<COptionUI>("optFuncTodayWeather")->SetVisible(false);
		}
		else {
			FindControl<CLabelUI>("lblTodayWeather")->SetVisible(true);
			FindControl<COptionUI>("optFuncTodayWeather")->SetVisible(true);
		}
	}
	else if (msg.pSender->GetName() == _T("btnCloseDeskWnd")) {
		if (MessageBox(this->m_hWnd, _T("确定要关闭吗？"), _T(""), MB_OKCANCEL) == IDOK)
		{
			//此方式会引起二次弹出对话框要求关闭，崩溃
			/*HWND h = HWND(msg.pSender->GetTag());
			CDuiString* strPost = new CDuiString(_T("mnuClose"));
			::PostMessage(h, WM_MENUCLICK, (WPARAM)(strPost), 0);*/

			TDeskWindow* item = (TDeskWindow*)(msg.pSender->GetTag());
			item->WndObject->SelfClose();
			//EVENT_DESK_WND_CLOSE接替处理
			/*CListContainerElementUI* pItem = (CListContainerElementUI*)msg.pSender->GetParent()->GetParent();
			CListUI *pList = FindControl<CListUI>("listDeskWnd");
			pList->RemoveAt(pItem->GetIndex());*/
		}
	}
	/*else if (msg.pSender->GetName() == _T("btnTrayClockFontColor")) {
		CButtonUI* pBtn = static_cast<CButtonUI*>(msg.pSender);
		
		COLORREF clr = Window::SelectColor(pBtn->GetTextColor(), m_hWnd);
		if (clr >= 0)
			pBtn->SetTextColor(RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)));
	}*/
	else if (msg.pSender->GetName() == _T("btnTrayClockFontColorClear")) {
		CColorComboUI* pColor = static_cast<CColorComboUI*>(FindControl("cmbTrayClockFontColor"));
		pColor->SetSelColor(4210884338);
		pColor->Invalidate();
		m_PaintManager->NeedUpdate();
		::UpdateWindow(m_hWnd);
	}
	/*else if (msg.pSender->GetName() == _T("btnTrayClockBackColor")) {
		CButtonUI* pBtn = static_cast<CButtonUI*>(msg.pSender);

		COLORREF clr = Window::SelectColor(pBtn->GetTextColor(), m_hWnd);
		if (clr >= 0)
			pBtn->SetTextColor(RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)));
	}*/
	else if (msg.pSender->GetName() == _T("btnTrayClockBackColorClear")) {
		CColorComboUI* pColor = static_cast<CColorComboUI*>(FindControl("cmbTrayClockBackColor"));
		pColor->SetSelColor(4210884338);
		pColor->Invalidate();
		m_PaintManager->NeedUpdate();
		::UpdateWindow(m_hWnd);
	}
	else if (msg.pSender->GetName() == _T("btnFmtDesc")) {
		CString s = gUnify->App()->Path()->ExePath.c_str();
		s += L"TrayClockFormat.txt";

		

	

		ShellExecute(m_hWnd, _T("open"), s.GetBuffer(), L"", L"", SW_SHOWNORMAL);
	}
	else if (msg.pSender->GetName() == _T("btnHKEdit")) {
		CDuiString sID = msg.pSender->GetUserData();

		OnHKEdit( sID.GetData(), msg.pSender->GetUserData2().GetData(), msg.pSender->GetTag());

	}
}

void CConfigWnd::OnHKEdit(LPCTSTR sID, LPCTSTR sModifiers, int nKey)
{
	CHkEditWnd* pWnd = NewPopWindow<CHkEditWnd>(L"编辑热键", m_hWnd);
	pWnd->CenterWindow();
	pWnd->Modifiers = sModifiers;
	pWnd->Key = nKey;
	pWnd->SetHotkeyTypeID(sID);
	pWnd->InitControl();
	if (pWnd->ShowModal() == IDOK) {
		CString sKey = pWnd->GetHotkeyTypeID().c_str();

		gUnify->DB()->SysConfig()->SetString(L"Hotkey", sKey, pWnd->Modifiers);
		sKey += L"_key";
		gUnify->DB()->SysConfig()->SetInt(L"Hotkey", sKey, pWnd->Key);

		delete pWnd;

		gUnify->Core()->Event()->PostEvent(EVENT_CFG_FRESH);

		LoadHK();
	}
}

void CConfigWnd::OnSelectChanged(TNotifyUI &msg)
{
	if (msg.pSender->GetName() == _T("chkShow")) {
		TDeskWindow* item = (TDeskWindow*)(msg.pSender->GetTag());
		HWND h = item->WndObject->GetHWND();
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(msg.pSender);
		if (pChk->GetCheck())
			::ShowWindow(h, SW_SHOW);
		else
			::ShowWindow(h, SW_HIDE);

		item->WndObject->SaveStat();
	}
	else if (msg.pSender->GetName() == _T("tabTrayClockText")) {
		CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(FindControl("tabTrayClock"));
		pTab->SelectItem(0);
	}
	else if (msg.pSender->GetName() == _T("tabTrayClockFmt")) {
		CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(FindControl("tabTrayClock"));
		pTab->SelectItem(1);
	}
	else if (msg.pSender->GetName() == _T("optTrayClockFmt1")) {
		CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(FindControl("tabTrayClockFmtShow"));
		pTab->SelectItem(0);
	}
	else if (msg.pSender->GetName() == _T("optTrayClockFmt2")) {
		CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(FindControl("tabTrayClockFmtShow"));
		pTab->SelectItem(1);
	}
	else if (msg.pSender->GetName() == _T("chkTrayClockYear4")) {
		CCheckBoxUI* pChk4 = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockYear4"));
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockYear"));
		if (pChk4->GetCheck())
			pChk->SetCheck(false);
	}
	else if (msg.pSender->GetName() == _T("chkTrayClockYear")) {
		CCheckBoxUI* pChk4 = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockYear4"));
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockYear"));
		if (pChk->GetCheck())
			pChk4->SetCheck(false);
	}
	else if (msg.pSender->GetName() == _T("chkTrayClockWea")) {
		CCheckBoxUI* pChk1 = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockWea"));
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockTmp"));
	
		pChk->SetVisible(pChk1->GetCheck());
	}
	else if (msg.pSender->GetName() == _T("chkTrayClockFontBold")) {
		CCheckBoxUI* pChk4 = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockFontBold"));
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockFontItalic"));
		if (pChk->GetCheck())
			pChk4->SetCheck(false);
	}
	else if (msg.pSender->GetName() == _T("chkTrayClockFontItalic")) {
		CCheckBoxUI* pChk1 = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockFontBold"));
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(FindControl("chkTrayClockFontItalic"));

		pChk->SetVisible(pChk1->GetCheck());
	}
}

void CConfigWnd::SaveCfg()
{

	auto SetParamByComboCtrl = [&](LPCTSTR szParamName, LPCTSTR szCtrlName, bool IsUserData) {
		CDuiString s;
		s = L"";
		CComboUI *pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(szCtrlName));
		/*s = pComb->GetText();*/
		CListLabelElementUI *pElement = static_cast<CListLabelElementUI*>(pComb->GetItemAt(pComb->GetCurSel()));

		if (pElement)
		{
			if (IsUserData)
				s = pElement->GetUserData();
			else
				s = pElement->GetText();
			
			gUnify->DB()->SysConfig()->SetString(_T("Base"), szParamName, s.GetData());
		}
	};

	gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkAutoRun"), FindControl<CCheckBoxUI>("chkAutoRun")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkRestoreStat"), FindControl<CCheckBoxUI>("chkRestoreStat")->GetCheck());

	gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkStartToTray"), FindControl<CCheckBoxUI>("chkStartToTray")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkMinToTray"), FindControl<CCheckBoxUI>("chkMinToTray")->GetCheck());

	gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkCloseToTray"), FindControl<COptionUI>("chkCloseToTray")->IsSelected());
	gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkCloseToExit"), FindControl<COptionUI>("chkCloseToExit")->IsSelected());

	//gUnify->DB()->PlugDB()->SetBool(_T("UIMainWeather"), _T("Enabled"), FindControl<CCheckBoxUI>("chkMainWea")->GetCheck());
	//gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkTabAnimation"), FindControl<CCheckBoxUI>("chkTabAnima")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("chkUpdate"), FindControl<CCheckBoxUI>("chkUpdate")->GetCheck());

	SetParamByComboCtrl(L"cmbUpdate", L"cmbUpdate", true);

	// gUnify->DB()->SysConfig()->SetBool(_T("Func"), _T("optFuncToday"), FindControl<COptionUI>("optFuncToday")->IsSelected());
	// gUnify->DB()->SysConfig()->SetBool(_T("Func"), _T("optFuncTodayWeather"), FindControl<COptionUI>("optFuncTodayWeather")->IsSelected());
	// gUnify->DB()->SysConfig()->SetBool(_T("Func"), _T("optFuncTask"), FindControl<COptionUI>("optFuncTask")->IsSelected());
	// gUnify->DB()->SysConfig()->SetBool(_T("Func"), _T("optFuncCal"), FindControl<COptionUI>("optFuncCal")->IsSelected());
	// gUnify->DB()->SysConfig()->SetBool(_T("Func"), _T("optFuncTodo"), FindControl<COptionUI>("optFuncTodo")->IsSelected());
	// gUnify->DB()->SysConfig()->SetBool(_T("Func"), _T("optFuncHotkeys"), FindControl<COptionUI>("optFuncHotkeys")->IsSelected());
	// gUnify->DB()->SysConfig()->SetBool(_T("Func"), _T("optFuncOption"), FindControl<COptionUI>("optFuncOption")->IsSelected());

	gUnify->DB()->SysConfig()->SetBool(_T("Net"), _T("chkProxy"), FindControl<CCheckBoxUI>("chkProxy")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("Net"), _T("chkProxyAuth"), FindControl<CCheckBoxUI>("chkProxyAuth")->GetCheck());
	SetParamByComboCtrl(L"cmbProxyType", L"cmbProxyType", true);
	gUnify->DB()->SysConfig()->SetString(_T("Net"), _T("edtProxyAddr"), FindControl<CEditUI>("edtProxyAddr")->GetText().GetData());
	gUnify->DB()->SysConfig()->SetString(_T("Net"), _T("edtProxyPort"), FindControl<CEditUI>("edtProxyPort")->GetText().GetData());
	gUnify->DB()->SysConfig()->SetString(_T("Net"), _T("edtProxyUser"), FindControl<CEditUI>("edtProxyUser")->GetText().GetData());
	gUnify->DB()->SysConfig()->SetString(_T("Net"), _T("edtProxyPass"), FindControl<CEditUI>("edtProxyPass")->GetText().GetData());

	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClock"), FindControl<CCheckBoxUI>("chkTrayClock")->GetCheck());
	SetParamByComboCtrl(L"cmbTrayClockFont", L"cmbTrayClockFont", false);
	gUnify->DB()->SysConfig()->SetString(_T("TrayClock"), _T("edtTrayClockFontSize"), FindControl<CSpinEditUI>("edtTrayClockFontSize")->GetText().GetData());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockFontBold"), FindControl<CCheckBoxUI>("chkTrayClockFontBold")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockFontItalic"), FindControl<CCheckBoxUI>("chkTrayClockFontItalic")->GetCheck());
	gUnify->DB()->SysConfig()->SetString(_T("TrayClock"), _T("edtTrayClockLineSpace"), FindControl<CSpinEditUI>("edtTrayClockLineSpace")->GetText().GetData());
	gUnify->DB()->SysConfig()->SetInt(_T("TrayClock"), _T("cmbTrayClockFontColor"), FindControl<CColorComboUI>("cmbTrayClockFontColor")->GetSelColor());
	gUnify->DB()->SysConfig()->SetInt(_T("TrayClock"), _T("cmbTrayClockBackColor"), FindControl<CColorComboUI>("cmbTrayClockBackColor")->GetSelColor());

	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockWea"), FindControl<CCheckBoxUI>("chkTrayClockWea")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockTmp"), FindControl<CCheckBoxUI>("chkTrayClockTmp")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("optTrayClockFmt1"), FindControl<COptionUI>("optTrayClockFmt1")->IsSelected());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("optTrayClockFmt2"), FindControl<COptionUI>("optTrayClockFmt2")->IsSelected());

	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClock24"), FindControl<CCheckBoxUI>("chkTrayClock24")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockAP"), FindControl<CCheckBoxUI>("chkTrayClockAP")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockHour"), FindControl<CCheckBoxUI>("chkTrayClockHour")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockMin"), FindControl<CCheckBoxUI>("chkTrayClockMin")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockSec"), FindControl<CCheckBoxUI>("chkTrayClockSec")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockNewLine"), FindControl<CCheckBoxUI>("chkTrayClockNewLine")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockYear4"), FindControl<CCheckBoxUI>("chkTrayClockYear4")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockYear"), FindControl<CCheckBoxUI>("chkTrayClockYear")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockMon"), FindControl<CCheckBoxUI>("chkTrayClockMon")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockDay"), FindControl<CCheckBoxUI>("chkTrayClockDay")->GetCheck());
	gUnify->DB()->SysConfig()->SetBool(_T("TrayClock"), _T("chkTrayClockWeek"), FindControl<CCheckBoxUI>("chkTrayClockWeek")->GetCheck());

	gUnify->DB()->SysConfig()->SetString(_T("TrayClock"), _T("edtTrayClockFmt2"), FindControl<CEditUI>("edtTrayClockFmt2")->GetText().GetData());

	SaveHK();

	gUnify->Core()->Event()->PostEvent(EVENT_CFG_FRESH);

	//apply config
	TString s = _T("\"") + Path::GetAppPath() + _T("\" -m");
	Registry::Reg_SetAutoRun(_T("BeyondDesk"), s.c_str(), FindControl<CCheckBoxUI>("chkAutoRun")->GetCheck());
}

void CConfigWnd::OnEvent(CEvent* e)
{
	if (e->EventID == EVENT_DESK_WND_CLOSE) {
		CListUI *pList = FindControl<CListUI>("listDeskWnd");
		pList->RemoveAll();
		LoadDeskWindows();
	}
}

void CConfigWnd::SetIndex(unsigned int index)
{
	m_pTree->SelectItem(index);
	//m_pTab->SelectItem(index);
}

void CConfigWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (m_pTree->GetCurSel() == 5) {
			THREAD_BEGIN
			TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Weather"));
			pPlugInfo->GetPlugIntf(1)->OnInvoke(NULL);
			THREAD_END
		}
		else
		m_pTab->SelectItem(m_pTree->GetCurSel());
	}
	else if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		if (!m_bInited)
		{
			OnInitWindow();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMACTIVATE)
	{
		CListContainerElementUI* pItem = static_cast<CListContainerElementUI*>(msg.pSender);
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		OnHKEdit(pBtn->GetUserData().GetData(), pBtn->GetUserData2().GetData(), pBtn->GetTag());
	}
	
	return WindowImplBase::Notify(msg);
}

LRESULT CConfigWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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

void CConfigWnd::InitWindow()
{
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE, FALSE);

	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));
	m_pTree = static_cast<CTreeViewUI*>(m_PaintManager->FindControl(_T("tvCat")));
	m_pTab = static_cast<CTabLayoutUI*>(m_PaintManager->FindControl(_T("tabMain")));

	InitControl();

	LoadCfg();

}

void CConfigWnd::LoadCfg()
{
	auto SetCmoboCtrlByParam = [&](LPCTSTR szCtrlName, LPCTSTR szParamName, bool IsUserData) {
		CComboUI *pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(szCtrlName));
		pComb->SelectItem(0);
		CString s = gUnify->DB()->SysConfig()->GetString(_T("Base"), szParamName, L"1");

		for (int i = 0; i < pComb->GetCount(); i++)
		{
			CListLabelElementUI *pListE = static_cast<CListLabelElementUI*>(pComb->GetItemAt(i));

			if (IsUserData)
			{

				if (lstrcmp(pListE->GetUserData().GetData(), s.GetBuffer()) == 0)
				{
					pComb->SelectItem(i);
					break;
				}
			}
			else
			{
				if (lstrcmp(pListE->GetText().GetData(), s.GetBuffer()) == 0)
				{
					pComb->SelectItem(i);
					break;
				}
			}
		}
	};

	FindControl<CCheckBoxUI>("chkAutoRun")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkAutoRun"), true));
	FindControl<CCheckBoxUI>("chkRestoreStat")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkRestoreStat"), true));

	FindControl<CCheckBoxUI>("chkStartToTray")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkStartToTray"), false));
	FindControl<CCheckBoxUI>("chkMinToTray")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkMinToTray"), false));

	FindControl<COptionUI>("chkCloseToTray")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkCloseToTray"), true));
	FindControl<COptionUI>("chkCloseToExit")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkCloseToExit"), false));

	//FindControl<CCheckBoxUI>("chkMainWea")->Selected(gUnify->DB()->PlugDB()->GetBool(_T("UIMainWeather"), _T("Enabled"), true));
	//FindControl<CCheckBoxUI>("chkTabAnima")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkTabAnimation"), true));

	FindControl<CCheckBoxUI>("chkUpdate")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkUpdate"), true));

	SetCmoboCtrlByParam(L"cmbUpdate", L"cmbUpdate", true);

	// FindControl<COptionUI>("optFuncToday")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncToday"), true));
	// if (!gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncToday"), true)) {
	// 	FindControl<CLabelUI>("lblTodayWeather")->SetVisible(false);
	// 	FindControl<COptionUI>("optFuncTodayWeather")->SetVisible(false);
	// }
	// FindControl<COptionUI>("optFuncTodayWeather")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTodayWeather"), true));
	// FindControl<COptionUI>("optFuncTask")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTask"), true));
	// FindControl<COptionUI>("optFuncCal")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncCal"), false));
	// FindControl<COptionUI>("optFuncTodo")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTodo"), true));
	// FindControl<COptionUI>("optFuncHotkeys")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncHotkeys"), true));
	// FindControl<COptionUI>("optFuncOption")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncOption"), true));

	FindControl<CCheckBoxUI>("chkProxy")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Net"), _T("chkProxy"), false));
	FindControl<CCheckBoxUI>("chkProxyAuth")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Net"), _T("chkProxyAuth"), false));
	SetCmoboCtrlByParam(L"cmbProxyType", L"cmbProxyType", true);
	FindControl<CEditUI>("edtProxyAddr")->SetText(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyAddr")));
	FindControl<CEditUI>("edtProxyPort")->SetText(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyPort")));
	FindControl<CEditUI>("edtProxyUser")->SetText(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyUser")));
	FindControl<CEditUI>("edtProxyPass")->SetText(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyPass")));

	LoadDeskWindows();

	CComboUI *pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbTrayClockFont")));
	
	vector<TString> vecFonts;
	System::GetInstalledFonts(vecFonts);
	for (auto item : vecFonts) {
		CListLabelElementUI *pListE = new CListLabelElementUI;
		pListE->SetText(item.c_str());
		//pListE->SetUserData(it->c_str());
		pListE->SetToolTip(item.c_str());
		pComb->Add(pListE);
	}

	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(L"cmbTrayClockFont"));
	pComb->SelectItem(0);
	CString s = gUnify->DB()->SysConfig()->GetString(_T("Base"), L"cmbTrayClockFont", L"Tahoma");

	for (int i = 0; i < pComb->GetCount(); i++)
	{
		CListLabelElementUI *pListE = static_cast<CListLabelElementUI*>(pComb->GetItemAt(i));

		if (lstrcmp(pListE->GetText().GetData(), s.GetBuffer()) == 0)
		{
			pComb->SelectItem(i);
			break;
		}
	}

	FindControl<CCheckBoxUI>("chkTrayClock")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClock"), false));
	FindControl<CSpinEditUI>("edtTrayClockFontSize")->SetText(gUnify->DB()->SysConfig()->GetString(_T("TrayClock"), _T("edtTrayClockFontSize"), _T("10")));
	FindControl<CCheckBoxUI>("chkTrayClockFontBold")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockFontBold"), false));
	FindControl<CCheckBoxUI>("chkTrayClockFontItalic")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockFontItalic"), false));
	FindControl<CSpinEditUI>("edtTrayClockLineSpace")->SetText(gUnify->DB()->SysConfig()->GetString(_T("TrayClock"), _T("edtTrayClockLineSpace"), _T("4")));
	FindControl<CColorComboUI>("cmbTrayClockFontColor")->SetSelColor(gUnify->DB()->SysConfig()->GetInt(_T("TrayClock"), _T("cmbTrayClockFontColor"), 4210884338));
	FindControl<CColorComboUI>("cmbTrayClockBackColor")->SetSelColor(gUnify->DB()->SysConfig()->GetInt(_T("TrayClock"), _T("cmbTrayClockBackColor"), 4210884338));

	FindControl<CCheckBoxUI>("chkTrayClockWea")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockWea"), false));
	FindControl<CCheckBoxUI>("chkTrayClockTmp")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockTmp"), false));
	FindControl<COptionUI>("optTrayClockFmt1")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("optTrayClockFmt1"), true));
	FindControl<COptionUI>("optTrayClockFmt2")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("optTrayClockFmt2"), false));
	FindControl<CCheckBoxUI>("chkTrayClock24")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClock24"), true));
	FindControl<CCheckBoxUI>("chkTrayClockAP")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockAP"), false));
	FindControl<CCheckBoxUI>("chkTrayClockHour")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockHour"), true));
	FindControl<CCheckBoxUI>("chkTrayClockMin")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockMin"), true));
	FindControl<CCheckBoxUI>("chkTrayClockSec")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockSec"), true));
	FindControl<CCheckBoxUI>("chkTrayClockNewLine")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockNewLine"), true));
	FindControl<CCheckBoxUI>("chkTrayClockYear4")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockYear4"), false));
	FindControl<CCheckBoxUI>("chkTrayClockYear")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockYear"), false));
	FindControl<CCheckBoxUI>("chkTrayClockMon")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockMon"), true));
	FindControl<CCheckBoxUI>("chkTrayClockDay")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockDay"), true));
	FindControl<CCheckBoxUI>("chkTrayClockWeek")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockWeek"), true));

	FindControl<CEditUI>("edtTrayClockFmt2")->SetText(gUnify->DB()->SysConfig()->GetString(_T("TrayClock"), _T("edtTrayClockFmt2"), _T("H:nn:ss\\nmm/dd ddd")));

	LoadHK();
}

void CConfigWnd::LoadDeskWindows()
{
	CListUI *pList = FindControl<CListUI>("listDeskWnd");

	for (TDeskWindow& item : gUnify->App()->Status()->DeskWindows) {
		CDialogBuilder builder;
		CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIDeskWndListItem.xml"), 0, this, m_PaintManager, NULL));
		if (!pItem) continue;

		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
		CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(pVert->GetItemAt(0));
		pChk->SetCheck(::IsWindowVisible(item.WndObject->GetHWND()));
		pChk->SetTag(int(&item));
		/*CControlUI* pImg = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		TString sPath = gUnify->App()->Path()->pu
		sPath = Path::GetParetnDir(sPath.c_str());
		sPath += pPlug->GetIconFile();
		if (!PathFileExists(sPath.c_str()))
		{
			sPath = gUnify->App()->Path()->PlugPath;
			sPath += _T("Common\\");
			sPath += pPlug->GetIconFile();
		}
		pImg->SetBkImage()*/
		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(0));
		pLbl->SetText(item.PlugObject->GetCaption().c_str());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(0));
		RECT rc;
		HWND h = item.WndObject->GetHWND();
		::GetWindowRect(h, &rc);
		TString s = _T("X:") + String::IntToStr(rc.left) + _T("  Y:") + String::IntToStr(rc.top);
		pLbl->SetText(s.c_str());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetTag(int(&item));

		pList->Add(pItem);
	}
}

void CConfigWnd::LoadHK()
{
	FindControl<CCheckBoxUI>("chkHotkey")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Hotkey"), _T("chkHotkey"), true));

	CListUI *pList = FindControl<CListUI>("listhk");
	pList->RemoveAll();

	//for (int i=) 
	{
		CDialogBuilder builder;
		CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"today", L"Ctrl+Shift+D");
		int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"today_key", 68);
		pItem->SetUserData(L"today");
		pItem->SetTag(nKey);

		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
		CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(L"今日页面");

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(sKeyDesc);

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetUserData(L"today");
		pBtn->SetTag(nKey);
		pBtn->SetUserData2(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
		pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"today_enabled", true));

		pList->Add(pItem);
	}
	// {
	// 	CDialogBuilder builder;
	// 	CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
	// 	CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"task", L"Ctrl+Shift+T");
	// 	int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"task_key", 84);
	// 	pItem->SetTag(nKey);
	//
	// 	pItem->SetUserData(L"task");
	// 	
	// 	CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
	// 	CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(L"定时页面");
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
	// 	pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
	// 	CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
	// 	pBtn->SetUserData(L"task");
	// 	pBtn->SetTag(nKey);
	// 	pBtn->SetUserData2(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
	// 	COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
	// 	pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"task_enabled", true));
	//
	// 	pList->Add(pItem);
	// }
	{
		CDialogBuilder builder;
		CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));

		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"todo", L"Ctrl+Shift+B");
		int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"todo_key", 66);
		pItem->SetTag(nKey);

		pItem->SetUserData(L"todo");


		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
		CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(L"待办页面");

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetUserData(L"todo");
		pBtn->SetTag(nKey);
		pBtn->SetUserData2(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
		pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"todo_enabled", true));

		pList->Add(pItem);
	}
	// {
	// 	CDialogBuilder builder;
	// 	CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
	//
	// 	CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"cal", L"Ctrl+Shift+C");
	// 	int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"cal_key", 67);
	// 	pItem->SetTag(nKey);
	//
	// 	pItem->SetUserData(L"cal");
	//
	//
	// 	CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
	// 	CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(L"日历页面");
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
	// 	pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
	// 	CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
	// 	pBtn->SetUserData(L"cal");
	// 	pBtn->SetTag(nKey);
	// 	pBtn->SetUserData2(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
	// 	COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
	// 	pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"cal_enabled", true));
	//
	// 	pList->Add(pItem);
	// }
	// {
	// 	CDialogBuilder builder;
	// 	CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
	//
	// 	CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"hotkey", L"Ctrl+Shift+H");
	// 	int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"hotkey_key", 72);
	// 	pItem->SetTag(nKey);
	//
	// 	pItem->SetUserData(L"hotkey");
	//
	//
	// 	CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
	// 	CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(L"热键页面");
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
	// 	pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
	// 	CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
	// 	pBtn->SetUserData(L"hotkey");
	// 	pBtn->SetTag(nKey);
	// 	pBtn->SetUserData2(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
	// 	COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
	// 	pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"hotkey_enabled", true));
	//
	// 	pList->Add(pItem);
	// }
	{
		CDialogBuilder builder;
		CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));

		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"toolbox", L"Ctrl+Shift+X");
		int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"toolbox_key", 88);
		pItem->SetTag(nKey);

		pItem->SetUserData(L"toolbox");


		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
		CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(L"工具箱页面");

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetUserData(L"toolbox");
		pBtn->SetTag(nKey);
		pBtn->SetUserData2(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
		pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"toolbox_enabled", true));

		pList->Add(pItem);
	}
	// {
	// 	CDialogBuilder builder;
	// 	CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
	//
	// 	CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_remind", L"Ctrl+Shift+R");
	// 	int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_remind_key", 82);
	// 	pItem->SetTag(nKey);
	// 	pItem->SetUserData(L"new_remind");
	//
	//
	// 	CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
	// 	CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(L"新建提醒");
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
	// 	pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
	// 	CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
	// 	pBtn->SetUserData(L"new_remind");
	// 	pBtn->SetTag(nKey);
	// 	pBtn->SetUserData2(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
	// 	COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
	// 	pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_remind_enabled", true));
	//
	// 	pList->Add(pItem);
	// }
	// {
	// 	CDialogBuilder builder;
	// 	CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
	//
	// 	CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"quick_remind", L"Ctrl+Shift+Q");
	// 	int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"quick_remind_key", 81);
	// 	pItem->SetTag(nKey);
	// 	pItem->SetUserData(L"quick_remind");
	//
	// 	CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
	// 	CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(L"快速提醒");
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
	// 	pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
	// 	CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
	// 	pBtn->SetUserData(L"quick_remind");
	// 	pBtn->SetTag(nKey);
	// 	pBtn->SetUserData2(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
	// 	COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
	// 	pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"quick_remind_enabled", true));
	//
	// 	pList->Add(pItem);
	// }
	{
		CDialogBuilder builder;
		CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
	
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_todo", L"Ctrl+Shift+I");
		int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_todo_key", 73);
		pItem->SetTag(nKey);
		pItem->SetUserData(L"new_todo");
	
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
		CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(L"新建待办");
	
		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(sKeyDesc.GetBuffer());
	
		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetUserData(L"new_todo");
		pBtn->SetTag(nKey);
		pBtn->SetUserData2(sKeyDesc.GetBuffer());
	
		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
		pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_todo_enabled", true));
	
		pList->Add(pItem);
	}

	// {
	// 	CDialogBuilder builder;
	// 	CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
	//
	// 	CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_sch", L"Ctrl+Shift+S");
	// 	int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_sch_key", 83);
	// 	pItem->SetTag(nKey);
	// 	pItem->SetUserData(L"new_sch");
	//
	// 	CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
	// 	CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(L"新建日程");
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
	// 	pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
	// 	CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
	// 	pBtn->SetUserData(L"new_sch");
	// 	pBtn->SetTag(nKey);
	// 	pBtn->SetUserData2(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
	// 	COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
	// 	pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_sch_enabled", true));
	//
	// 	pList->Add(pItem);
	// }
	{
		CDialogBuilder builder;
		CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));

		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_note", L"Ctrl+Shift+N");
		int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_note_key", 78);
		pItem->SetTag(nKey);
		pItem->SetUserData(L"new_note");

		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
		CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(L"新建便签");

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetUserData(L"new_note");
		pBtn->SetTag(nKey);
		pBtn->SetUserData2(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
		pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_note_enabled", true));

		pList->Add(pItem);
	}
	{
		CDialogBuilder builder;
		CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));

		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"main", L"Ctrl+Shift+M");
		int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"main_key", 77);
		pItem->SetTag(nKey);
		pItem->SetUserData(L"main");

		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
		CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(L"显示/隐藏主窗口");

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetUserData(L"main");
		pBtn->SetTag(nKey);
		pBtn->SetUserData2(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
		pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"main_enabled", true));

		pList->Add(pItem);
	}
	// {
	// 	CDialogBuilder builder;
	// 	CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));
	//
	// 	CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"mute", L"Ctrl+Shift+U");
	// 	int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"mute_key", 85);
	// 	pItem->SetTag(nKey);
	// 	pItem->SetUserData(L"mute");
	//
	// 	CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
	// 	CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(L"静音模式");
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
	// 	pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
	// 	pLbl->SetText(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
	// 	CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
	// 	pBtn->SetUserData(L"mute");
	// 	pBtn->SetTag(nKey);
	// 	pBtn->SetUserData2(sKeyDesc.GetBuffer());
	//
	// 	pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
	// 	COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
	// 	pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"mute_enabled", true));
	//
	// 	pList->Add(pItem);
	// }
	{
		CDialogBuilder builder;
		CListContainerElementUI *pItem = static_cast<CListContainerElementUI*>(builder.Create(_T("XML\\UIHkListItem.xml"), 0, this, m_PaintManager, NULL));

		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"option", L"Ctrl+Shift+O");
		int nKey = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"option_key", 79);
		pItem->SetTag(nKey);
		pItem->SetUserData(L"option");

		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(0));
		CLabelUI* pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(L"选项");

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));
		pLbl->SetText(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(2));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetUserData(L"option");
		pBtn->SetTag(nKey);
		pBtn->SetUserData2(sKeyDesc.GetBuffer());

		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
		pOpt->Selected(gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"option_enabled", true));

		pList->Add(pItem);
	}
}

void CConfigWnd::SaveHK()
{
	gUnify->DB()->SysConfig()->SetBool(_T("Hotkey"), _T("chkHotkey"), FindControl<CCheckBoxUI>("chkHotkey")->GetCheck());

	CListUI *pList = FindControl<CListUI>("listhk");

	for (int i = 0; i < pList->GetCount(); i++) {
		CListContainerElementUI *pItem = (CListContainerElementUI *)pList->GetItemAt(i);
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(1));
		CLabelUI*pLbl = static_cast<CLabelUI*>(pVert->GetItemAt(1));

		gUnify->DB()->SysConfig()->SetString(L"Hotkey", pItem->GetUserData().GetData(), pLbl->GetText().GetData());

		CString sKey = pItem->GetUserData().GetData();
		sKey += L"_key";
		gUnify->DB()->SysConfig()->SetInt(L"Hotkey", sKey, pItem->GetTag());

		sKey = pItem->GetUserData().GetData();
		sKey += L"_enabled";
		pVert = static_cast<CVerticalLayoutUI*>(pItem->GetItemAt(3));
		COptionUI* pOpt = static_cast<COptionUI*>(pVert->GetItemAt(0));
		gUnify->DB()->SysConfig()->SetInt(L"Hotkey", sKey, pOpt->IsSelected());
	}
}

LRESULT CConfigWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}

void CConfigWnd::InitControl()
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

void CConfigWnd::OnInitWindow()
{
	if (m_bInited) return;
	m_bInited = true;

	
}

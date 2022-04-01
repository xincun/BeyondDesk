
#include "stdafx.h"
#include "UIMainProxy.h"
#include "MainFrame.h"
#include "DLLInvoke.h"
#include "ConfigWnd.h"
#include "DlgTipExit.h"
#include "SKin/SkinPickerDialog.h"
#include "../[Common]/ZipWrapper/include/ZHead.h"
#include "TrayClockProxy.h"
#include "UIHotKeyProxy.h"
#include "VolWnd.h"
#include "Reg/Reg.h"

DUI_BEGIN_MESSAGE_MAP(UIMainProxy, UIProxy)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_HEADERCLICK, OnHeaderClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE, OnItemActivate)
DUI_END_MESSAGE_MAP()

void UIMainProxy::Notify(TNotifyUI& msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		m_bShowed = true;

		gUnify->Core()->Event()->PostEvent(EVENT_MAIN_SHOW_DONE);
	}
	else
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("setbtn"))
		{
			
			CMenuWnd* pMenu = new CMenuWnd(); 
			DuiLib::CDuiPoint point;
			GetCursorPos(&point);

			//此处mute动态，不能在main->ApplyCfg里设，因为hotkey会动态改变，不走ApplyCfg
			bool bMute = gUnify->DB()->SysConfig()->GetBool(L"Stat", L"MuteMode", false);
			m_CheckList[_T("mnuMute")] = bMute;
			pMenu->Init(NULL, _T("XML\\MenuMainDrop.xml"), point, m_pPaintManager, &m_CheckList, &m_VisibleList, &m_TextList);
			//CMenuElementUI* pItem = new CMenuElementUI;
			//pItem->SetText(L"xx");
			//pMenu->m_pLayout->Add(pItem);
		}
		else if (msg.pSender->GetName() == _T("btnAbout"))
		{
		//	ShowAboutWnd();
			return;
		}
	}
	else if (msg.sType == DUI_MSGTYPE_HEADWIDTHCHANGED)
	{
		SaveStat();
	}

	UIProxy::Notify(msg); //not forget
}

void UIMainProxy::SaveStat()
{
	if (!m_bShowed) return;
	if (!m_hWnd) return;
	if (IsIconic(m_hWnd)) return;

	if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkRestoreStat"), true))
	{
		gUnify->DB()->SysConfig()->SetBool(_T("Stat"),  _T("Zoomed"), IsZoomed(m_hWnd));

		if (IsZoomed(m_hWnd)) return; //最大化恢复时不回复大小位置，形成干扰
		
		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		//::MapWindowRect(m_hWnd, GetDesktopWindow(), &rc);

		gUnify->DB()->SysConfig()->SetInt(_T("Stat"), _T("Left"), rc.left);
		gUnify->DB()->SysConfig()->SetInt(_T("Stat"), _T("Top"), rc.top);
		gUnify->DB()->SysConfig()->SetInt(_T("Stat"), _T("Width"), rc.right - rc.left);
		gUnify->DB()->SysConfig()->SetInt(_T("Stat"), _T("Height"), rc.bottom - rc.top);

		/*if (m_pTaskList)
			m_pTaskList->SaveStat();*/
	}
}

//void InsurePosition(HWND hWnd, int& x, int& y, int& w, int& h)
//{
//	int n = Window::GetMonitorNumbers();
//	//if (n == 1)
//	{
//		RECT rcDesk = Window::GetWorkRect(hWnd);
//
//		if (w > rcDesk.right) w = rcDesk.right;
//		if (h > rcDesk.bottom) h = rcDesk.bottom;
//		if (x > rcDesk.right || x < 0) x = (rcDesk.right - rcDesk.left) / 2;
//		if (y > rcDesk.bottom || y < 0) y = (rcDesk.bottom - rcDesk.top) / 2;
//	}
//
//}

void UIMainProxy::LoadStat()
{
	if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("FirstRun"), true))
	{
		if (m_pProxyedWnd)
			m_pProxyedWnd->CenterWindow();
	}
	else
	if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkRestoreStat"), true))
	{
		if (m_pProxyedWnd)
			m_pProxyedWnd->CenterWindow();

		RECT rc;
		::GetWindowRect(m_hWnd, &rc);

		int x = gUnify->DB()->SysConfig()->GetInt(_T("Stat"), _T("Left"), rc.left);
		int y = gUnify->DB()->SysConfig()->GetInt(_T("Stat"), _T("Top"), rc.top);
		int w = gUnify->DB()->SysConfig()->GetInt(_T("Stat"), _T("Width"), rc.right - rc.left);
		int h = gUnify->DB()->SysConfig()->GetInt(_T("Stat"), _T("Height"), rc.bottom - rc.top);

		Window::InsurePosition(m_hWnd, x, y, w, h);

		::MoveWindow(m_hWnd, x, y, w, h, false);

		/*if (m_pTaskList)
			m_pTaskList->LoadStat();*/

		if (gUnify->DB()->SysConfig()->GetBool(_T("Stat"), _T("Zoomed"), false)) //最大化，恢复，前面恢复大小位置的代码不要去掉，否则从最大化恢复到正常时会有问题
		{
			//ShowWindow(m_hWnd, SW_MAXIMIZE);
			::PostMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
	}
	else
	{
		if (m_pProxyedWnd)
			m_pProxyedWnd->CenterWindow();
	}
}

void UIMainProxy::ApplyCfg()
{
	
    //m_pTabMain->SetAnimationEnabled(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkTabAnimation"), false));
	m_pTabMain->SetAnimationEnabled(false);

	TString s = _T("\"") + Path::GetAppPath() + _T("\" -m");
	Registry::Reg_SetAutoRun(_T("BeyondDesk"), s.c_str(), gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkAutoRun"), true));

	vector<int> vecShow;

	auto SetPlugEnabled = [&](LPCTSTR szPlugName) {

		CString s = _T("optFunc");
		s += szPlugName;
		bool bEnabled = gUnify->DB()->SysConfig()->GetBool(_T("Func"), s, true);

		s = _T("ctl");
		s += szPlugName;
		CControlUI *pCtrl = static_cast<CControlUI*>(m_pPaintManager->FindControl(s));
		if (pCtrl) {
			pCtrl->SetVisible(bEnabled);
		}

		s = _T("opt");
		s += szPlugName;
		COptionUI *pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(s));
		if (pOpt == nullptr) return;
		int nTag = 0;
		nTag = pOpt->GetTag();
		if (bEnabled) vecShow.push_back(nTag);
		pOpt->SetVisible(bEnabled);
		TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(szPlugName);
		if (pPlugInfo) {
			pPlugInfo->GetPlugIntf()->SetEnabled(bEnabled);
		}
	};

	//对应插件功能，先隐藏，禁用，下次启动时不加载
	/*FindControl<COptionUI>("optFuncToday")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncToday"), true));
	FindControl<COptionUI>("optFuncTodayWeather")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTodayWeather"), true));
	FindControl<COptionUI>("optFuncTask")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTask"), true));
	FindControl<COptionUI>("optFuncCal")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncCal"), true));
	FindControl<COptionUI>("optFuncTodo")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTodo"), true));
	FindControl<COptionUI>("optFuncHotkey")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncHotkey"), true));*/

	SetPlugEnabled(_T("Today"));
	//SetPlugEnabled(_T("Task"));
	SetPlugEnabled(_T("Todo"));
	SetPlugEnabled(_T("Cal"));
	//SetPlugEnabled(_T("Hotkeys"));
	m_pPaintManager->FindControl(L"optSet")->SetVisible(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncOption"), true));

	int n = 5;
	if (vecShow.size() > 0) {
		n = vecShow.at(0);
	}
	//设置为第一项可见
	COptionUI *pOpt = nullptr;
	switch (n)
	{
	case 0:
		pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optToday")));
		pOpt->Selected(true);
		m_pTabMain->SelectItem(n);
		break;
	case 1:
		pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optTask")));
		pOpt->Selected(true);
		m_pTabMain->SelectItem(n);
		break;
	case 2:
		pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optTodo")));
		pOpt->Selected(true);
		m_pTabMain->SelectItem(n);
		break;
	case 3:
		pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optCal")));
		pOpt->Selected(true);
		m_pTabMain->SelectItem(n);
		break;
	case 4:
		pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optHotkeys")));
		pOpt->Selected(true);
		m_pTabMain->SelectItem(n);
		break;
	case 5:
		pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optTools")));
		pOpt->Selected(true);
		m_pTabMain->SelectItem(n);
		break;
	default:
		break;
	}

	//m_TextList[L"mnu"]
	m_VisibleList[_T("mnuReg")] = false;
	m_VisibleList[_T("mnuMute")] = gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTask"), true);
	m_VisibleList[_T("mnuQuickRemind")] = gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTask"), true);
	m_VisibleList[_T("menuTimerShut")] = gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTask"), true);
	m_VisibleList[_T("mnuNewRemind")] = gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTask"), true);
	m_VisibleList[_T("mnuNewTodo")] = gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTodo"), true);
	m_VisibleList[_T("mnuAddSch")] = gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncCal"), false);

	CString sMenu;
	sMenu.Format(L"主窗口    %14s", gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"main", L"Ctrl+Shift+M"));
	m_TextList[_T("mnuMain")] = sMenu.GetBuffer();

	//sMenu = L"自定义分钟   " + gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"quick_remind", L"Ctrl+Shift+Q");
	sMenu.Format(L"自定义分钟    %14s", gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"quick_remind", L"Ctrl+Shift+Q"));
	m_TextList[_T("mnuQuickCustom")] = sMenu.GetBuffer();

	//sMenu = L"新提醒   " + gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_remind", L"Ctrl+Shift+R");
	sMenu.Format(L"新提醒    %14s", gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_remind", L"Ctrl+Shift+R"));
	m_TextList[_T("mnuNewRemind")] = sMenu.GetBuffer();

	//sMenu = L"新待办   " + gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_todo", L"Ctrl+Shift+I");
	sMenu.Format(L"新待办    %14s", gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_todo", L"Ctrl+Shift+I"));
	m_TextList[_T("mnuNewTodo")] = sMenu.GetBuffer();

	//sMenu = L"新日程   " + gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_sch", L"Ctrl+Shift+S");
	sMenu.Format(L"新日程    %14s", gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_sch", L"Ctrl+Shift+S"));
	m_TextList[_T("mnuAddSch")] = sMenu.GetBuffer();

	//sMenu = L"新便签   " + gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_note", L"Ctrl+Shift+N");
	sMenu.Format(L"新便签    %14s", gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_note", L"Ctrl+Shift+N"));
	m_TextList[_T("mnuNewNote")] = sMenu.GetBuffer();

	// sMenu = L"静音模式         " + gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"mute", L"Ctrl+Shift+U");
	 sMenu.Format(L"静音模式    %12s", gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"mute", L"Ctrl+Shift+U"));
	m_TextList[_T("mnuMute")] = sMenu.GetBuffer();

	//sMenu = L"选项             " + gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"option", L"Ctrl+Shift+O");
	sMenu.Format(L"选项    %18s", gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"option", L"Ctrl+Shift+O"));
	m_TextList[_T("mnuSet")] = sMenu.GetBuffer();

	TrayClockProxy::GetInstance()->RefreshConfig();

	UIHotkeyProxy::GetInstance()->ResetHotkeys();
}

CControlUI* UIMainProxy::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	/*if (_tcsicmp(pstrClass, _T("TaskListUI")) == 0)
	{
		m_pTaskList = new  CTaskListUI;

		return m_pTaskList;
	}*/
	TString sClass = pstrClass;
	if (sClass.substr(0, 8) == _T("Plug_UI_"))
	{
		TString sPlugName;
		UINT nIndex = 0;
		sClass = sClass.substr(8, sClass.length());
		if (sClass.rfind('_') == TString::npos)
		{
			sPlugName = sClass;
		}
		else
		{
			sPlugName = sClass.substr(0, sClass.rfind('_'));
			TString sPlugIndex = sClass.substr(sClass.rfind('_')+1, sClass.length());
			nIndex = String::StrToInt(sPlugIndex.c_str());
		}
		
		return m_TabPlugHelper.CreateUI(sPlugName.c_str(), nIndex);
	}

	return NULL;
}

UIMainProxy::UIMainProxy()
{
	m_TrayFlag = FALSE;
	pAboutWnd = NULL;
	
	gUnify->Core()->Event()->AddEventListen(EVENT_CFG_FRESH, this, &UIMainProxy::OnEvent);
}

UIMainProxy::~UIMainProxy()
{
	gUnify->Core()->Event()->RemoveEventListen(EVENT_CFG_FRESH, this, &UIMainProxy::OnEvent);

	gUnify->UI()->RemoveSkinChangeObserver(m_pPaintManager);
}

void UIMainProxy::CheckUpdate(bool rightNow /*= false*/)
{
	CString sUpdateFile;
	sUpdateFile.Format(L"%s%s", gUnify->App()->Path()->ExePath.c_str(), L"BeyondDeskUp.exe");
	CString sUpdateFileTemp;
	sUpdateFileTemp.Format(L"%s%s", gUnify->App()->Path()->ExePath.c_str(), L"BeyondDeskUpgrade.exe"); //xp下，仍然需要vcruntime140.dll，ucrtbase.dll,api-ms...

	if (!PathFileExists(sUpdateFileTemp) || Util::File::GetFileModifyTime(sUpdateFile) > Util::File::GetFileModifyTime(sUpdateFileTemp)) {
		::CopyFile(sUpdateFile, sUpdateFileTemp, FALSE);
	}
	CString param;
	if (rightNow)
		param.Format(L"-ver=%s -checknow -mainexe=%s", APP_VER, gUnify->App()->Path()->ExeFullPath.c_str());
	else
	    param.Format(L"-ver=%s -mainexe=%s", APP_VER, gUnify->App()->Path()->ExeFullPath.c_str());
	::ShellExecute(gUnify->App()->m_MainWnd, L"open", sUpdateFileTemp, param,
		Util::Path::GetParetnDir(sUpdateFileTemp).c_str(), SW_SHOW);
}

void UIMainProxy::ExitApp()
{
	gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("FirstRun"), false);

	LOGCON("结束程序");

	::PostMessage(m_pPaintManager->GetPaintWindow(), WM_CLOSE, 0, 0);

	TrayClockProxy::GetInstance()->Unload();
}

void UIMainProxy::OnInit()
{
	LOGCON("on init");
	
	m_TabPlugHelper.SetManager(m_pPaintManager);
	UIHotkeyProxy::GetInstance()->SetPaintManger(m_pPaintManager);

	LOGCON("m_TabPlugHelper.InitPlugs();");
	m_TabPlugHelper.InitPlugs();

	//m_pTaskList->Create(m_pPaintManager);
	//m_pTaskList->InitWindow();

	gUnify->UI()->AddSkinChangeObserver(m_pPaintManager);

	m_pCloseBtn = static_cast<CButtonUI*>(m_pPaintManager->FindControl(_T("closebtnMain")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pPaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pPaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pPaintManager->FindControl(_T("minbtn")));

	m_pTabMain = static_cast<CAnimationTabLayoutUI*>(m_pPaintManager->FindControl(_T("tabMain")));

	LOGCON("ApplyCfg");
	ApplyCfg();
	
	LOGCON("LoadStat");
	LoadStat();

	/*TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
	if (pPlugInfo)
	{
		TString sCount = pPlugInfo->GetPlugIntf()->OnGet(this);

		CButtonUI *pBtn = static_cast<CButtonUI*>(m_pPaintManager->FindControl(_T("btnTodayTask")));
		if (sCount == _T("0"))
		{
			CDuiString s = _T("今日无任务");
			pBtn->SetText(s);
		}
		else
		{
			CDuiString s;
			s.Format(_T("今日 %s 个任务"), sCount.c_str());
			pBtn->SetText(s);
		}
	}*/
	//LOGAPP("UIMainProxy Init done");
	//SetProcessWorkingSetSize(::GetCurrentProcess(), -1, -1);

	CDuiString s;
	s.Format(_T("%s V%s"), MAIN_TITLE, APP_VER);
	CTextUI* pTitle = static_cast<CTextUI*>(m_pPaintManager->FindControl(_T("txtTitleMain")));
	pTitle->SetText(s);

	bool bMute = gUnify->DB()->SysConfig()->GetBool(L"Stat", L"MuteMode", false);
	if (bMute) {
		CTextUI* pStat = static_cast<CTextUI*>(m_pPaintManager->FindControl(_T("txtStat")));
		pStat->SetText(L"静音模式");
	}

	CheckUpdate();
}



void UIMainProxy::ToTray()
{
	::ShowWindow(m_hWnd, SW_HIDE);
}

void UIMainProxy::TryShowFirstHideToTrayHint()
{
	if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("FirstRun"), true))
	{
		gUnify->DB()->SysConfig()->SetBool(_T("Base"), _T("FirstRun"), false);

		TString s = MAIN_TITLE;
		s += _T("已经隐藏到托盘，您可以点击托盘图标再次打开！");
		gUnify->App()->Tray()->ShowBalloonHint(MAIN_TITLE, s.c_str());
	}
}

void UIMainProxy::ShowAboutWnd()
{
	if (pAboutWnd != NULL) return;

	pAboutWnd = new CAboutWnd();
	if (pAboutWnd == NULL) return;

	TString sTitle = _T("关于");
	sTitle += MAIN_TITLE;
	pAboutWnd->Create(this->m_hWnd, sTitle.c_str(), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);

	//skin_changed_observer_.AddReceiver(pChatDialog);
	pAboutWnd->CenterWindow();
	pAboutWnd->ShowModal();//OutputDebugString("AboutWndDestr");
	pAboutWnd = NULL;
}

LRESULT UIMainProxy::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_MOVE:
	{
		SaveStat();

		break;
	}
	case WM_SIZE:
	{
		BOOL bHandled = true;

		SaveStat();

		break;
	}
	case WM_EXITSIZEMOVE:
	{
		gUnify->Core()->Event()->PostEvent(EVENT_MAIN_SHOW_DONE); //让界面上的节日等列表信息自动适应

		break;
	}
	//case WM_COPYDATA:
	//{
	//	//这里的消息应该是以 '\0' 结尾的字符串
	//	COPYDATASTRUCT* pCDS = (COPYDATASTRUCT*)lParam;
	//	if (pCDS->cbData > 0)
	//	{
	//		wchar_t recvData[256] = { 0 };
	//		lstrcpyn(recvData, (wchar_t *)pCDS->lpData, pCDS->cbData);

	//		int h = _wtoi(recvData);
	//		TrayClockProxy::GetInstance()->SetTrayClockMsgWnd(HWND(h));
	//	}
	//	break;
	//}
	case WM_SHOWWINDOW:
	{
		// CMainFrameWnd* pMain = static_cast<CMainFrameWnd*>(m_pProxyedWnd);
		// if (pMain->getCanCheckReg()) {
		// 	if (wParam == 1) {
		// 		if (!CReg::isReg() && CReg::getRemainDays() == 0) {
		// 			CReg::showWaitWnd();
		// 		}
		// 	}
		// }

		break;
	}
	/*case WM_SYSCOMMAND:
	{
		switch (UINT(wParam))
		{
		case SC_MAXIMIZE:
		{
			if (!CReg::isReg() && CReg::getRemainDays() == 0 ) {
				CReg::showWaitWnd();
			}

			break;
		}
		case SC_RESTORE:
		{
			if (!CReg::isReg() && CReg::getRemainDays() == 0 ) {
				CReg::showWaitWnd();
			}

			break;
		}

		}
	}*/
	case WM_MENUCLICK:
	{
		CDuiString* sName = (CDuiString*)(wParam);

		if (*sName == _T("mnuSet")) {
			OnUISet();
		}
		else if (*sName == _T("mnuReg"))
			CReg::showRegWnd();
		else if (*sName == _T("mnuAbout"))
			ShowAboutWnd();
		else if (*sName == _T("mnuMain"))
			ShowWindow(m_hWnd, SW_SHOWNORMAL);
		else if (*sName == _T("mnuExit"))
			ExitApp();
		else if (*sName == _T("mnuWeb")) {
			Net::OpenURLInDefaultBrowser(URL_HOME);
		}
		else if (*sName == _T("mnuHelp"))
			::Net::OpenURLInDefaultBrowser(URL_FAQ);
		else if (*sName == _T("mnuUpdate")) {
			CheckUpdate(true);
		}
		else if (*sName == _T("mnuMute")) {
			bool bChecked = (bool)lParam;

			gUnify->DB()->SysConfig()->SetBool(L"Stat", L"MuteMode", bChecked);

			CTextUI* pStat = static_cast<CTextUI*>(m_pPaintManager->FindControl(_T("txtStat")));
			if (bChecked) {

				pStat->SetText(L"静音模式");
			}
			else
				pStat->SetText(L"");
		}	
		else if (*sName == _T("mnuExportData")) {
			TString sTemp = Util::Path::GetSysTempDirW();
			sTemp += L"user"; //先复制到临时目录，防止正在使用
			Util::File::CopyDir(gUnify->App()->Path()->UserPath.c_str(), sTemp.c_str());

			TCHAR sBakFile[MAX_PATH] = {};
			TString strBakFile;
			File::ShellBrowseSaveFile(sBakFile, L"BeyondDesk备份文件(*.ktb)\0*.ktb\0");
            
			if (_tcsicmp(sBakFile, L"") == 0) break;

			strBakFile = sBakFile;
			if (Path::GetFileExtension(strBakFile.c_str()) == L"")
				strBakFile += L".ktb";
            
			::DeleteFile(strBakFile.c_str());
			DWORD  dwRet = ZipDirectory(sTemp.c_str(), strBakFile.c_str());
			//UnZipToDirectory("D:\\user.zip", "D:\\user");
			if (dwRet == 0) {
				MessageBox(this->m_hWnd, _T("备份成功！"), _T("提示"), MB_OK);
			}
			
		}
		else if (*sName == _T("mnuImportData")) {
			if (MessageBox(this->m_hWnd, _T("恢复数据将重启您的软件并覆盖您的数据，您确定要继续吗?"), _T("提示"), MB_OKCANCEL) == IDCANCEL) break;

			TCHAR sBakFile[MAX_PATH] = {};
			TString strBakFile;
			File::ShellBrowseOpenFile(sBakFile, L"BeyondDesk备份文件(*.ktb)\0*.ktb\0");

			if (_tcsicmp(sBakFile, L"") == 0) break;

			strBakFile = sBakFile;
			if (Path::GetFileExtension(strBakFile.c_str()) == L"")
				strBakFile += L".ktb";

			TString sTemp = Util::Path::GetSysTempDirW();
			sTemp += L"BeyondDeskData"; //先复制到临时目录，防止正在使用
			File::DeleteDir(sTemp.c_str());

			DWORD  dwRet = UnZipToDirectory(strBakFile.c_str(), sTemp.c_str());
			//if (dwRet == 0) 
			{
				TString sParam = _T("-restore ");
				sParam += sTemp;
				sParam += L" ";
				sParam += gUnify->App()->Path()->UserPath;
				ShellExecute(NULL, _T("open"), gUnify->App()->Path()->ExeFullPath.c_str(), sParam.c_str(), gUnify->App()->Path()->ExePath.c_str(), SW_SHOW);
				ExitApp();
			}
		}
		else if (*sName == _T("mnuOpenData")) {
			ShellExecute(NULL, L"open", L"Explorer.exe", gUnify->App()->Path()->UserPath.c_str(), NULL, SW_SHOW);
		}
		else if (*sName == _T("mnuNewRemind")) {
			TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
			pPlugInfo->GetPlugIntf()->OnInvoke(this, 4, _T("ttRemind"));
		}
		/*"mnuShutdown"
			"mnuReboot" t
			"mnuLogoff" t
			"mnuStandby"
			"mnuHibernate
			"mnuLock" tex*/

		else if (*sName == _T("ttShutdown") ||
			*sName == _T("ttReboot") ||
			*sName == _T("ttLogoff") || 
			*sName == _T("ttStandby") || 
			*sName == _T("ttHibernate") || 
			*sName == _T("ttLock") ) {
			TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
			pPlugInfo->GetPlugIntf()->OnInvoke(this, 4, (*sName).GetData());
		}
		else if (*sName == _T("mnuNewNote")) {
			TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Note"));
			pPlugInfo->GetPlugIntf()->OnNew();
		}
		else if (*sName == _T("mnuQuick5") ||
			*sName == _T("mnuQuick15") ||
			*sName == _T("mnuQuick30") ||
			*sName == _T("mnuQuickCustom") ) {
			TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
			pPlugInfo->GetPlugIntf()->OnInvoke(this, 5, (*sName).GetData());
		}

		break;
	}
	default:
	{

	}
	}

	return -1;	  
	//return __super::HandleMessage(uMsg, wParam, lParam);
}

void UIMainProxy::OnEvent(CEvent* e)
{
	if (e->EventID == EVENT_CFG_FRESH)
	{
		ApplyCfg();
	}
	/*if (e->EventID == EVENT_SKIN_CHANGE)
	{
		if (e->wParam == "alpha")
		{
			m_pPaintManager->SetTransparent(String::AStrToInt(e->lParam));
		}
		else if (e->wParam == "color")
		{
			FindControl<CVerticalLayoutUI>("vertBack")->SetBkImage(_T(""));
			FindControl<CVerticalLayoutUI>("vertBack")->SetBkColor(String::AStrToInt64(e->lParam));
		}
		else if (e->wParam == "bkimage")
		{
			FindControl<CVerticalLayoutUI>("vertBack")->SetBkImage(String::AnsiToUnicode(e->lParam.c_str()).c_str());
		}
	}*/
}

void UIMainProxy::OnUISet(unsigned int index)
{
	CConfigWnd* pWnd = NewPopWindow<CConfigWnd>(L"选项", m_hWnd);
	pWnd->CenterWindow();
	pWnd->SetIndex(index);
	pWnd->ShowModal();
}

void UIMainProxy::OnClick(TNotifyUI& msg)
{
	if (msg.pSender == m_pCloseBtn) {
		//询问选项没关 或 第一次运行
		if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("TipExit"), true))
		{
			//弹窗对话框，modal，保存选项
			CTipExitWnd *pWnd = NewPopWindow<CTipExitWnd>(L"", m_hWnd);
			pWnd->CenterWindow();
			if (pWnd->ShowModal() == IDCANCEL)
			    return;
		}
	    
		ToTray();

		if (!gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkCloseToTray"), true))
			ExitApp();
		else
		{
			SetProcessWorkingSetSize(::GetCurrentProcess(), -1, -1);
			TryShowFirstHideToTrayHint();
		}

		return;
	}
	else if (msg.pSender == m_pMinBtn)
	{
		if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkMinToTray"), false))
		{
			/*RECT rc;
			::GetWindowRect(m_hWnd, &rc);*/

			ToTray();

			TryShowFirstHideToTrayHint();
		}
		else
		{
			::PostMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}

		SetProcessWorkingSetSize(::GetCurrentProcess(), -1, -1);

		return;
	}
	else if (msg.pSender == m_pMaxBtn) {
		::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
	}
	else if (msg.pSender == m_pRestoreBtn) {
		::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0); return;
	}
	else if (msg.pSender->GetName() == _T("btnHelp"))
	{
	    ShowAboutWnd();
		return;
	}
	else if (msg.pSender->GetName() == _T("btnSkin"))
	{
		CDuiString sPath = CPaintManagerUI::GetInstancePath();
		sPath += _T("Res\\XML\\DlgSkinPicker.xml");
		CSkinPickerDialog *pSkinPicker = new CSkinPickerDialog();
		pSkinPicker->Create(m_hWnd, _T("皮肤与窗口调整"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
		pSkinPicker->CenterWindow();
		pSkinPicker->ShowModal();
		delete	pSkinPicker;
		return;
	}
	else if (msg.pSender->GetName() == _T("optSet"))
	{
		OnUISet();
	}
	else if (msg.pSender->GetName() == _T("optAbout"))
	{
		ShowAboutWnd();
		return;
	}
	else if (msg.pSender->GetName() == _T("btnTodayTask"))
	{
		COptionUI *pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optTask")));
		pOpt->Selected(true);

		m_pTabMain->SelectItem(1);

		TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
		pPlugInfo->GetPlugIntf()->OnInvoke(this);
	}
	else if (msg.pSender->GetName() == _T("btnTodayTodo"))
	{
		COptionUI *pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optTodo")));
		pOpt->Selected(true);

		m_pTabMain->SelectItem(2);

		/*TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Todo"));
		pPlugInfo->GetPlugIntf()->OnInvoke(this);*/
	}
	else if (msg.pSender->GetName() == _T("btnTodaySch"))
	{
		COptionUI *pOpt = static_cast<COptionUI*>(m_pPaintManager->FindControl(_T("optCal")));
		pOpt->Selected(true);

		m_pTabMain->SelectItem(3);

		/*TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Todo"));
		pPlugInfo->GetPlugIntf()->OnInvoke(this);*/
	}
	else if (msg.pSender->GetName() == _T("btnTest1"))
	{
		/*CAVCapDLL::Init();

		wchar_t szDevs[1024];
		
		CAVCapDLL::Get("GetVideoDevices", szDevs);
		LOGCON(L"GetVideoDevices: %s", szDevs);

		memset(szDevs, 0, 1024);
		CAVCapDLL::Get("GetVideoResolutions", szDevs);
		LOGCON(L"GetVideoResolutions: %s", szDevs);

		memset(szDevs, 0, 1024);
		CAVCapDLL::Get("GetVideoCompressors", szDevs);
		LOGCON(L"GetVideoCompressors: %s", szDevs);

		memset(szDevs, 0, 1024);
		CAVCapDLL::Get("GetAudioDevices", szDevs);
		LOGCON(L"GetAudioDevices: %s", szDevs);

		memset(szDevs, 0, 1024);
		CAVCapDLL::Get("GetAudioCompressors", szDevs);
		LOGCON(L"GetAudioCompressors: %s", szDevs);*/

		return;
	}
	else if (msg.pSender->GetName() == _T("btnTest2"))
	{
		//CAVCapDLL::Clear();
		return;
	}
	else if (msg.pSender->GetName() == _T("btnMangeDeskWnd"))
	{
		OnUISet(2);
		return;
	}
}

void UIMainProxy::OnSelectChanged(TNotifyUI &msg)
{
	if (msg.pSender->GetName() == _T("optToday"))
	{
		m_pTabMain->SelectItem(0);
	}
	else if (msg.pSender->GetName() == _T("optTask"))
	{
		m_pTabMain->SelectItem(1);
	}
	
	else if (msg.pSender->GetName() == _T("optTodo"))
	{
		m_pTabMain->SelectItem(2);
	}
	else if (msg.pSender->GetName() == _T("optCal"))
	{
		m_pTabMain->SelectItem(3);
	}
	else if (msg.pSender->GetName() == _T("optHotkeys"))
	{
		m_pTabMain->SelectItem(4);
	}
	else if (msg.pSender->GetName() == _T("optTools"))
	{
		m_pTabMain->SelectItem(5);
	}
	else if (msg.pSender->GetName() == _T("optMgr"))
	{
		m_pTabMain->SelectItem(6);
	}
}

void UIMainProxy::OnItemClick(TNotifyUI &msg)
{

}

void UIMainProxy::OnItemActivate(TNotifyUI& msg)
{

}

void UIMainProxy::OnHeaderClick(TNotifyUI& msg)
{

}

//void UIMainProxy::OnSkinChange(CSkinChangeType sct, LPCTSTR szParam)
//{
//	switch (sct)
//	{
//	case sctBack:
//		FindControl<CVerticalLayoutUI>("vertBack")->SetBkImage(szParam);
//		break;
//	case sctAlpha:
//		m_pPaintManager->SetTransparent(String::StrToInt(szParam));
//		break;
//	case sctColor:
//		FindControl<CVerticalLayoutUI>("vertBack")->SetBkImage(_T(""));
//		FindControl<CVerticalLayoutUI>("vertBack")->SetBkColor(String::StrToInt64(szParam));
//		break;
//	default:
//		break;
//	}
//}
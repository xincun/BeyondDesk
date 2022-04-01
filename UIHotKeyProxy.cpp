
#include "stdafx.h"
#include "UIHotKeyProxy.h"
#include "../MainFrame.h"
#include "Control/UIMenu.h"
#include "ConfigWnd.h"

UIHotkeyProxy* UIHotkeyProxy::_instance = NULL;

void HotkeyHandler(const char * param)
{
	if (strcmp(param, "today") == 0) {
		static_cast<COptionUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"optToday"))->Selected(true);
		ShowWindow(gUnify->App()->m_MainWnd, SW_SHOW);
		CTabLayoutUI* m_pTab = static_cast<CTabLayoutUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"tabMain"));
		m_pTab->SelectItem(0);
	}
	else if (strcmp(param, "task") == 0) {
		static_cast<COptionUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"optTask"))->Selected(true);
		ShowWindow(gUnify->App()->m_MainWnd, SW_SHOW);
		CTabLayoutUI* m_pTab = static_cast<CTabLayoutUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"tabMain"));
		m_pTab->SelectItem(1);
	}
	else if (strcmp(param, "todo") == 0) {
		static_cast<COptionUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"optTodo"))->Selected(true);
		ShowWindow(gUnify->App()->m_MainWnd, SW_SHOW);
		CTabLayoutUI* m_pTab = static_cast<CTabLayoutUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"tabMain"));
		m_pTab->SelectItem(2);
	}
	else if (strcmp(param, "cal") == 0) {
		static_cast<COptionUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"optCal"))->Selected(true);
		ShowWindow(gUnify->App()->m_MainWnd, SW_SHOW);
		CTabLayoutUI* m_pTab = static_cast<CTabLayoutUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"tabMain"));
		m_pTab->SelectItem(3);
	}
	else if (strcmp(param, "hotkey") == 0) {
		static_cast<COptionUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"optHotkeys"))->Selected(true);
		ShowWindow(gUnify->App()->m_MainWnd, SW_SHOW);
		CTabLayoutUI* m_pTab = static_cast<CTabLayoutUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"tabMain"));
		m_pTab->SelectItem(4);
	}
	else if (strcmp(param, "toolbox") == 0) {
		static_cast<COptionUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"optTools"))->Selected(true);
		ShowWindow(gUnify->App()->m_MainWnd, SW_SHOW);
		CTabLayoutUI* m_pTab = static_cast<CTabLayoutUI*>(UIHotkeyProxy::GetInstance()->GetPaintManger()->FindControl(L"tabMain"));
		m_pTab->SelectItem(5);
	}
	else if (strcmp(param, "new_remind") == 0) {
		/*TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
		pPlugInfo->GetPlugIntf()->OnInvoke(UIHotkeyProxy::GetInstance(), 4, _T("ttRemind"));*/
		CDuiString* strPost = new CDuiString(_T("mnuNewRemind"));
		::PostMessage(gUnify->App()->m_MainWnd, WM_MENUCLICK, (WPARAM)(strPost), 0);
	}
	else if (strcmp(param, "quick_remind") == 0) {
		/*TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
		pPlugInfo->GetPlugIntf()->OnInvoke(UIHotkeyProxy::GetInstance(), 5);*/
		CDuiString* strPost = new CDuiString(_T("mnuQuickCustom"));
		::PostMessage(gUnify->App()->m_MainWnd, WM_MENUCLICK, (WPARAM)(strPost), 0);
	}
	else if (strcmp(param, "new_todo") == 0) {
		/*TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
		pPlugInfo->GetPlugIntf()->OnInvoke(UIHotkeyProxy::GetInstance(), 4);*/
		CDuiString* strPost = new CDuiString(_T("mnuNewTodo"));
		::PostMessage(gUnify->App()->m_MainWnd, WM_MENUCLICK, (WPARAM)(strPost), 0);
	}
	else if (strcmp(param, "new_sch") == 0) {
		
		//TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));

		//CTime Now = CTime::GetCurrentTime();
		//CString sNow = Now.Format(_T(" %Y-%m-%d"));//%03d毫秒
		////sDate.Format(_T("%.04d-%.02d-%.02d"), dt.GetYear(), dt.GetMonth(), dt.GetDay());

		//pPlugInfo->GetPlugIntf()->OnInvoke(UIHotkeyProxy::GetInstance(), 1, sNow.GetBuffer()); //新增sid为空
		CDuiString* strPost = new CDuiString(_T("mnuAddSch"));
		::PostMessage(gUnify->App()->m_MainWnd, WM_MENUCLICK, (WPARAM)(strPost), 0);
	}
	else if (strcmp(param, "new_note") == 0) {
		/*TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Note"));
		pPlugInfo->GetPlugIntf()->OnNew();*/
		//上面方式会引起便签卡住，可能是线程里回调此方法，然后操作UI的关系
		CDuiString* strPost = new CDuiString(_T("mnuNewNote"));
		::PostMessage(gUnify->App()->m_MainWnd, WM_MENUCLICK, (WPARAM)(strPost), 0);
	}
	else if (strcmp(param, "main") == 0) {
		if (IsWindowVisible(gUnify->App()->m_MainWnd))
		    ShowWindow(gUnify->App()->m_MainWnd, SW_HIDE);
		else
			ShowWindow(gUnify->App()->m_MainWnd, SW_SHOW);
	}
	else if (strcmp(param, "mute") == 0) {
		bool bMute = gUnify->DB()->SysConfig()->GetBool(L"Stat", L"MuteMode", false);
		bMute = !bMute;
		CDuiString* strPost = new CDuiString(_T("mnuMute"));
		::PostMessage(gUnify->App()->m_MainWnd, WM_MENUCLICK, (WPARAM)(strPost), (LPARAM)bMute);
	}
	else if (strcmp(param, "option") == 0) {
		CDuiString* strPost = new CDuiString(_T("mnuSet"));
		::PostMessage(gUnify->App()->m_MainWnd, WM_MENUCLICK, (WPARAM)(strPost), 0);
	}
}

void UIHotkeyProxy::ResetHotkeys()
{
	mHotKey.Stop();
	mHotKey.ClearHotkeys();

	if (!gUnify->DB()->SysConfig()->GetBool(_T("Hotkey"), _T("chkHotkey"), true)) return;

	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"today_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"today_key", 68);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"today", L"Ctrl+Shift+D");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "today");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"task_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"task_key", 84);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"task", L"Ctrl+Shift+T");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "task");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"todo_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"todo_key", 66);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"todo", L"Ctrl+Shift+B");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "todo");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"cal_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"cal_key", 67);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"cal", L"Ctrl+Shift+C");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "cal");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"hotkey_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"hotkey_key", 72);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"hotkey", L"Ctrl+Shift+H");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "hotkey");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"toolbox_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"toolbox_key", 88);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"toolbox", L"Ctrl+Shift+X");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "toolbox");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_remind_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_remind_key", 82);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_remind", L"Ctrl+Shift+R");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "new_remind");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"quick_remind_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"quick_remind_key", 81);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"quick_remind", L"Ctrl+Shift+Q");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "quick_remind");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_todo_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_todo_key", 73);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_todo", L"Ctrl+Shift+I");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "new_todo");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_sch_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_sch_key", 83);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_sch", L"Ctrl+Shift+S");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "new_sch");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_note_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"new_note_key", 83);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"new_note", L"Ctrl+Shift+N");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "new_note");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"main_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"main_key", 77);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"main", L"Ctrl+Shift+M");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "main");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"mute_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"mute_key", 85);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"mute", L"Ctrl+Shift+U");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "mute");
		}
	}
	if (gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"option_enabled", true))
	{
		WORD wVirtualKeyCode = gUnify->DB()->SysConfig()->GetInt(L"Hotkey", L"option_key", 79);
		WORD wModifiers = 0;
		CString sKeyDesc = gUnify->DB()->SysConfig()->GetString(L"Hotkey", L"option", L"Ctrl+Shift+O");
		if (sKeyDesc.Find(L"Ctrl") >= 0) wModifiers |= MOD_CONTROL;
		if (sKeyDesc.Find(L"Alt") >= 0) wModifiers |= MOD_ALT;
		if (sKeyDesc.Find(L"Shift") >= 0) wModifiers |= MOD_SHIFT;
		if (sKeyDesc.Find(L"Win") >= 0) wModifiers |= MOD_WIN;

		if (wVirtualKeyCode != 0 || wModifiers != 0)
		{
			mHotKey.InsertHotkey(wModifiers, wVirtualKeyCode, HotkeyHandler, "option");
		}
	}

	mHotKey.Start();
}

UIHotkeyProxy::UIHotkeyProxy()
{

}

//************************************
//分离主窗口tray操作和消息相应逻辑，每一种逻辑可以从UIProxy派生相应类来处理
//Author: LX.g
//************************************

#ifndef UI_HK_PROXY
#define UI_HK_PROXY
#endif

#ifdef ENABLE_TRAY

#pragma once

#include "stdafx.h"
#include "Core/UIProxy.h"
#include "../[Common]/HotKey/include/HotKeyExport.h"

//struct HotKeyItem {
//	CString Modifiers;
//	WORD Key;
//	string id;
//};

class UIHotkeyProxy
{
	CHotKey mHotKey;
	UIHotkeyProxy();
	static UIHotkeyProxy* _instance;
	CPaintManagerUI* m_pPaintManger = NULL;
public:
	static UIHotkeyProxy* GetInstance() {
		if (!_instance) {
			_instance = new UIHotkeyProxy();
		}
		return _instance;
	}

	
	void ResetHotkeys();
	CHotKey* HotKey() { return &mHotKey; }

	DuiLib::CPaintManagerUI* GetPaintManger() const { return m_pPaintManger; }
	void SetPaintManger(DuiLib::CPaintManagerUI* val) { m_pPaintManger = val; }
};

#endif
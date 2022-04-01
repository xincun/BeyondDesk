//************************************
//分离主窗口tray操作和消息相应逻辑，每一种逻辑可以从UIProxy派生相应类来处理
//Author: LX.g
//************************************

#ifndef UI_TRAY_PROXY
#define UI_TRAY_PROXY
#endif

#ifdef ENABLE_TRAY

#pragma once

#include "stdafx.h"
#include "Core/UIProxy.h"

class UITrayProxy: public UIProxy
{
	TMenuCheckList* m_CheckList = nullptr;
	TMenuTextList* m_TextList = nullptr;
	TMenuVisibleList* m_VisibleList;

	void MinToTray();
	virtual void OnInit() {};
	TString GetTip();
	CString GetCurrentDT();

	
public:
	UITrayProxy();
	virtual void Notify(TNotifyUI& msg);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnContextMenu(int x=0);

	void SetCheckList(DuiLib::TMenuCheckList* val) { m_CheckList = val; }
	void SetVisibleList(DuiLib::TMenuVisibleList* val) { m_VisibleList = val; }
	void SetTextList(DuiLib::TMenuTextList* val) { m_TextList = val; }
};

#endif

void ShowTip();

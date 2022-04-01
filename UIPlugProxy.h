//************************************
//分离主窗口操作和消息相应逻辑，每一种逻辑可以从UIProxy派生相应类来处理
//目标：支持分离窗口逻辑，为方便原有项目移植，支持原有逻辑，支持DUI_DECLARE_MESSAGE_MAP
//换一个Proxy即换一种逻辑，主Proxy逻辑换掉，XML换掉就相当于一个新程序
//Author: LX.g
//************************************

#pragma once

#include "stdafx.h"
#include "Core/UIProxy.h"
#include "GlobalDefine.h"

class UIPlugProxy : public UIProxy, public IDialogBuilderCallback
{
	
	bool m_bInited = false;
	bool m_bShowed = false;

	void OnClick(TNotifyUI& msg);
	virtual void OnInit();
	void OnEvent(CEvent* e);
	void OnItemActivate(TNotifyUI& msg);
	void OnHeaderClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI &msg);
	void OnItemClick(TNotifyUI &msg);
	DUI_DECLARE_MESSAGE_MAP()
public:
	

	void TryGetTodayHis();


	UIPlugProxy();
	~UIPlugProxy();

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
};

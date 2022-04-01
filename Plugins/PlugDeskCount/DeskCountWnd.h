#pragma once

#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"
#include "IniFile.h"
#include "EventDefine.h"



using namespace DuiLib;

class CDeskCountWnd : public CDeskWnd, public IDialogBuilderCallback, public INotifyUI/*, public CNotifyPump*/
{
	vector<TString> mTodoList;
	vector<TString> mTaskList;
	vector<TString> mSchList;

	
protected:
	

public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CDeskCountWnd");
	};

	virtual UINT GetClassStyle() const
	{
		return CS_DBLCLKS;
	}

	virtual void  Notify(TNotifyUI& msg);

	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
	{
		return NULL;
	}

	CDeskCountWnd() {}

	~CDeskCountWnd();;

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnEvent(CEvent* e);

	CString GetCurrentDT();
	void Update();
	UINT UpdateTodo();
	UINT UpdateTask();
	UINT UpdateSch();

	virtual void ApplyCfg();;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	virtual void SaveStat(); //保存窗口状态，设置
	virtual void LoadStat(LPCTSTR pstrSection); //加载

	void OnTimer(void *pOwner, UINT nTimerID);

	virtual void OnConfig();


	/*virtual void OnBeforeContextMenu() {
		m_VisbileList[_T("mnuSet")] = false;
	};*/
	
};
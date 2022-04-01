#pragma once

#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"
#include "IniFile.h"
#include "EventDefine.h"
#include "ThreadHelper.h"

void DrawFile();


using namespace DuiLib;

class CDeskPicWnd : public CDeskWnd, public IDialogBuilderCallback, public INotifyUI/*, public CNotifyPump*/
{
	ThreadHelper<CDeskPicWnd> mThdHelper;
	vector<TString> mFilesList;
	vector<int> mPlaySequence;
	int m_nPlayPosition = 0;
	bool m_bPaused = false;
	/*int m_nOriginalWidth = 0;
	int m_nOriginalHeight = 0;*/
	CDuiRect m_OriginalRect;
	bool m_bOriginalTop = false;
	bool m_bFullScreen = false;
	bool m__bRecruseing = false;
	TString m_sFileName = L"";
	TString m_sTmpFile = L"";

	void DrawPicByNumber(int number);
	void DrawFile();
protected:
	

public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CDeskPicWnd");
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

	CDeskPicWnd() {}

	~CDeskPicWnd();;

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnEvent(CEvent* e);

	
	virtual void OnContextMenu();
	CString GetCurrentDT();
	void Update();
	UINT UpdateTodo();
	UINT UpdateTask();
	UINT UpdateSch();

	virtual void ApplyCfg();

	void ThdRecruseDirFiles();
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	virtual void SaveStat(); //保存窗口状态，设置
	virtual void LoadStat(LPCTSTR pstrSection); //加载

	void OnTimer(void *pOwner, UINT nTimerID);

	void SetPlayBarPos();

	virtual void OnConfig();


	/*virtual void OnBeforeContextMenu() {
		m_VisbileList[_T("mnuSet")] = false;
	};*/
	
};
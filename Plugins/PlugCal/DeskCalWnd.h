#pragma once

#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"
#include "IniFile.h"

using namespace DuiLib;

class CDeskCalWnd : public CDeskWnd, public IDialogBuilderCallback, public INotifyUI/*, public CNotifyPump*/
{
	CCalendarUI* m_pCal;
	vector<DuiLib::CSchItem> m_CalSchList;
	void FreshCalSchList();
	void OnCalMonthChange();

	template <typename T>
	inline  T * FindControl(LPCTSTR pszCtrlName)
	{
		CControlUI* pUI = m_pm.FindControl(pszCtrlName);
		if (pUI)
			return static_cast<T*>(pUI);
		return NULL;
	};

	template <typename T>
	inline T * FindControl(LPCSTR pszCtrlName)
	{
		USES_CONVERSION;


		CControlUI* pUI = m_pm.FindControl(A2T(pszCtrlName));
		if (pUI)
			return static_cast<T*>(pUI);
		return NULL;
	};
protected:
	

public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CDeskCalWnd");
	};

	virtual UINT GetClassStyle() const
	{
		return CS_DBLCLKS;
	}

	virtual void  Notify(TNotifyUI& msg);

	void GotoToday();

	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
	{
		return NULL;
	}

	CDeskCalWnd() {}

	~CDeskCalWnd();;

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	int GetTodaySchCount();
	void UpdateTodaySchCount();
	void OnEvent(CEvent* param);

	void FreshCalDisplay();

	CString GetCurrentDT();
	void Update();

	virtual void ApplyCfg();;
	void OnAddSch();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	
	virtual void SaveStat(); //保存窗口状态，设置
	virtual void LoadStat(LPCTSTR pstrSection); //加载

	void OnTimer(void *pOwner, UINT nTimerID)
	{
		//LOGCON("on HTCClk timer...");

	}

	virtual void OnConfig();


	/*virtual void OnBeforeContextMenu() {
		m_VisbileList[_T("mnuSet")] = false;
	};*/
	
};
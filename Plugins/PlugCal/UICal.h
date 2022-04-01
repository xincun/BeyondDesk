#pragma once

#include "stdafx.h"
#include "Control/UICalendar.h"

void OnAddSch();

class CCalUI : 
	public CContainerUI, 
	public IDialogBuilderCallback, 
	public INotifyUI,
	public IMessageFilterUI
{
	HWND m_hWnd;
	bool m_bShowed = false;

	CCalendarUI* m_pCal;
	CPaintManagerUI* m_pPaintManager;

	void OnCalMonthChange();

	void OnItemActivate(TNotifyUI& msg);
	void OnHeaderClick(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI &msg);
	void OnItemClick(TNotifyUI &msg);
	vector<DuiLib::CSchItem> m_CalSchList;
	void FreshCalSchList();
	int GetTodaySchCount();
	
	template <typename T>
	inline  T * FindControl(LPCTSTR pszCtrlName)
	{
		CControlUI* pUI = m_pPaintManager->FindControl(pszCtrlName);
		if (pUI)
			return static_cast<T*>(pUI);
		return NULL;
	};

	template <typename T>
	inline T * FindControl(LPCSTR pszCtrlName)
	{
		USES_CONVERSION;


		CControlUI* pUI = m_pPaintManager->FindControl(A2T(pszCtrlName));
		if (pUI)
			return static_cast<T*>(pUI);
		return NULL;
	};

	void OnEvent(CEvent* param);
	void OnAddSch();
	//COleDateTime m_SelDate;
public:
	CCalUI();
	~CCalUI();
	
	void InitWindow();
	void UpdateTodaySchCount();

	void SetEnabled(bool val);

	void Create(CPaintManagerUI* pManager);
	void OnCalEvent(LPCTSTR event, LPCTSTR param);
	virtual void Notify(TNotifyUI& msg);

	void GotoToday();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	

	//DUI_DECLARE_MESSAGE_MAP()
};


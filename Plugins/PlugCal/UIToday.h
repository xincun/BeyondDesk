#pragma once

#include "stdafx.h"
#include "Control/UICalendar.h"
#include "Event.h"

class CTodayUI : 
	public CContainerUI, 
	public IDialogBuilderCallback, 
	public INotifyUI,
	public IMessageFilterUI
{
	HWND m_hWnd;
	bool m_bShowed = false;

	CCalendarUI* m_pCal;

	CPaintManagerUI* m_pPaintManager;

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

	void SetTodayHisUI();
	//COleDateTime m_SelDate;

	//void OnEvent(CEvent* p);

	CString m_sOldDT;

	void SetDayCtrls();
	void SetFeastListUI();
	void OnTimerDay(void* pOwner, UINT uiTimerID);

	CString GetCurrentDT();
public:
	CTodayUI();
	~CTodayUI();

	
	void OnEvent(CEvent* p);
	void InitWindow();

	void Create(CPaintManagerUI* pManager);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//DUI_DECLARE_MESSAGE_MAP()
};


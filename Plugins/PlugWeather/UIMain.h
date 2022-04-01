#pragma once

#include "stdafx.h"
#include "Control/UICalendar.h"
#include "Event.h"
#include "../Common/DeskWnd.h"
#include "WeatherGetter.h"
#include "WeatherParser.h"
#include "WeaDetail.h"

class CMainUI : 
	public CContainerUI, 
	public IDialogBuilderCallback, 
	public INotifyUI,
	public IMessageFilterUI
{
	CWeatherGetter WeaGetter;
	CWeatherParser WeaParser;

	HWND m_hWnd;
	bool m_bShowed = false;
	TString m_sSection = _T("UIMainWeather");

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

	//TString m_sTodayWeatherDesc;
	bool OnGetWeather(void* pOwner, const CString& sWeaData);
public:
	CMainUI();
	~CMainUI();

	void OnEvent(CEvent* p);
	void InitWindow();

	void Create(CPaintManagerUI* pManager);
	virtual void Notify(TNotifyUI& msg);

	void OnConfig();
	//TString GetTodayWeatherDesc();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	virtual void ApplyCfg();
	//virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//DUI_DECLARE_MESSAGE_MAP()
};


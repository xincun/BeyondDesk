#pragma once
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeatherParser.h"
#include "WeatherGetter.h"

class CWeaDetailWnd : public WindowImplBase
{
	static CWeaDetailWnd* g_pWeaDetailWnd;

	CWeaDetailWnd();
	CWeatherParser* m_pWeaParser = NULL;
	CWeatherGetter* m_pWeaGetter = NULL;
	
	
public:
	HWND m_hParent;

	static CWeaDetailWnd* GetWnd(HWND hParent)
	{
		if (!g_pWeaDetailWnd)
		{
			g_pWeaDetailWnd = new CWeaDetailWnd();
			g_pWeaDetailWnd->m_hParent = hParent;
			g_pWeaDetailWnd->Create(hParent, L"", UI_WNDSTYLE_DIALOG | WS_POPUP, WS_EX_TOOLWINDOW, 0, 0, 0);
		}

		return g_pWeaDetailWnd;
	}

	static void FreeWnd(){
		delete g_pWeaDetailWnd;
		g_pWeaDetailWnd = NULL;
	}

	void SetWeatherParser(CWeatherParser* val) { m_pWeaParser = val; }
	void SetWeatherGetter(CWeatherGetter* val) { m_pWeaGetter = val; }

	void SetDetailsShow();
	/*void DockToParent()
	{
		RECT rcMon = Window::GetMonitorRect();

		CDuiRect rcParent;
		::GetWindowRect(m_hParent, &rcParent);

		CDuiRect rc;
		::GetWindowRect(g_pWeaDetailWnd->m_hWnd, &rc);

	    

		int n = 1;
	}*/

	virtual CDuiString GetSkinFolder();
	virtual LPCTSTR GetWindowClassName(void) const;

	virtual CDuiString GetSkinFile();

	virtual void Notify( TNotifyUI &msg );

	virtual void InitWindow();
	
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	virtual void OnClick(TNotifyUI& msg);

	void OnTimer(void* pOwner, UINT uiID);

	virtual void OnFinalMessage(HWND hWnd);
private:
	bool m_bInited = false;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;

	CTabLayoutUI* m_pTab;
	COptionUI* m_pOpt1;
	COptionUI* m_pOpt2;
	COptionUI* m_pOpt3;
};

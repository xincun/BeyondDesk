//************************************
//主窗口View
//可以用UIProxy来分离操作和消息相应逻辑，每一种逻辑可以从UIPraoy派生相应类来处理
//例如：UITrayProxy和UIMainProxy就是两种逻辑处理
//Author: LX.g
//************************************

#pragma once

#include <shellapi.h>
#include "stdafx.h"

#ifdef ENABLE_TRAY
#include "UITrayProxy.h"
#endif
#include "UIMainProxy.h"
#include "UIPlugProxy.h"


class CMainFrameWnd : public WindowImplBase/*, public CWebBrowserEventHandler*/
{
#ifdef ENABLE_TRAY
	UITrayProxy m_TrayProxy;
#endif
	UIPlugProxy m_PlugProxy;
	UIMainProxy m_MainProxy;

	//HICON m_hIcon;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;

	//CWebBrowserUI *m_pWB;
	//bool OnWebkitNavigate(const wstring &sURL);
public:
	CMainFrameWnd();
	~CMainFrameWnd();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	UINT GetClassStyle() const;
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual LPCTSTR CMainFrameWnd::GetResourceID() const;
	virtual void Notify( TNotifyUI &msg );
	void OnClick(TNotifyUI& msg);
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();
	//virtual LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	//virtual CDuiString GetZIPFileName() const;
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	DuiLib::CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	bool getCanCheckReg() const { return m_canCheckReg; }
	void setCanCheckReg(bool val) { m_canCheckReg = val; }
	//virtual void BeforeNavigate2(IDispatch *pDisp, VARIANT *&url, VARIANT *&Flags, VARIANT *&TargetFrameName, VARIANT *&PostData, VARIANT *&Headers, VARIANT_BOOL *&Cancel);
	//virtual void NewWindow3(IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
	//virtual HRESULT STDMETHODCALLTYPE GetHostInfo(/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
	//DUI_DECLARE_MESSAGE_MAP()
private:
	//void OnTimer(void * pOwner, UINT iTimerID);
	bool m_canCheckReg = true;
};
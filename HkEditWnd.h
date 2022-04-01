#pragma once
//////////////////////////////////////////////////////////////////////////
#include "GlobalDefine.h"

class CHkEditWnd : public WindowImplBase
{

	//CDateTimeCtrl *m_TimeCtrl; //no mfc try
	//CFont *m_pFont;
	HWND m_hTimeCtrl;
	TString m_nHotkeyTypeID;
	HWND m_hMainWnd;
	BOOL CheckInput();
	void SaveHK();
	
	
	HWND m_hScrRecPre = NULL;
	bool m_bCanAVAction = false;
public:
	WORD Key = 0;
	CString Modifiers;

	CHkEditWnd();
	~CHkEditWnd(void);

	void SetHotkeyTypeID(const TString& id) { m_nHotkeyTypeID = id; };
	TString GetHotkeyTypeID() { return m_nHotkeyTypeID; };

	void InitControl();
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );

	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();
	//virtual LRESULT OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	DuiLib::CDuiString GetZIPFileName() const;
	//virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);

	void OnOK();

	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemClick( TNotifyUI &msg );
	virtual void OnItemSelect( TNotifyUI &msg );
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	//CComboUI* m_pComboHotkeyType;
};

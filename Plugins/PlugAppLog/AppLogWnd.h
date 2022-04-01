#pragma once
//////////////////////////////////////////////////////////////////////////

#include "AppLogIntf.h"

class CAppLogWnd : public WindowImplBase
{
private:
	HWND m_hMainWnd;
public:
	CAppLogWnd(HWND hMainWnd);
	~CAppLogWnd(void);
	CAppLog* m_pHisObj;

	virtual void OnFinalMessage( HWND );
	virtual bool UseCustomRes();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );

	virtual void InitWindow();
	//virtual LRESULT OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Navigate(LPCTSTR pstrURL);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged( TNotifyUI &msg );
	virtual void OnItemClick( TNotifyUI &msg );
	virtual void OnItemSelect(TNotifyUI &msg);
private:
};

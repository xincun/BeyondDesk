#pragma once
//////////////////////////////////////////////////////////////////////////

#include "HistoryIntf.h"

class CHistoryWnd : public WindowImplBase
{
private:
	HWND m_hMainWnd;
public:
	CHistoryWnd(HWND hMainWnd);
	~CHistoryWnd(void);
	CHistory* m_pHisObj;

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

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged( TNotifyUI &msg );
	virtual void OnItemClick( TNotifyUI &msg );

	void GotoToday();

	virtual void OnItemSelect(TNotifyUI &msg);

	void ChangeHis();

private:

	CComboUI* m_pCombMon;
	CComboUI* m_pCombDay;
};

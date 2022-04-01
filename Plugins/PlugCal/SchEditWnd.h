#pragma once
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GlobalDefine.h"

class CSchEditWnd : public WindowImplBase
{
private:
	HWND m_hMainWnd;
	UINT m_nCountSec = 5;

	vector <DuiLib::CSchItem>* m_pSchList = nullptr;
	TString m_sDate = L"";
public:
	CSchEditWnd(HWND hMainWnd);
	~CSchEditWnd(void);

	//std::vector<DuiLib::CSchItem>* GetList() const { return m_pList; }
	void SetSchList(std::vector<DuiLib::CSchItem>* val) { m_pSchList = val; }

	void SetDate(TString val) { m_sDate = val; }

	void InitControl();
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual LRESULT OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();
	//virtual LRESULT OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);

	void OnItemEdit();

	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemClick( TNotifyUI &msg );
	virtual void OnItemSelect( TNotifyUI &msg );
	
private:
	void OnEvent(CEvent* param);
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	virtual void OnTimer(void* pOwner, UINT uiTimerId);	// 定时器到时回调函数
	/*CPage1 m_Page1;
	CPage2 m_Page2;*/
};

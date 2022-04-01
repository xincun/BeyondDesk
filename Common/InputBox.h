#pragma once
//////////////////////////////////////////////////////////////////////////
#include "DuiHelper.h"

class CInputBox : public WindowImplBase
{
public:
	static CInputBox* CreateInputBox(LPCTSTR szTitle, HWND hParent)
	{
		CInputBox* pWnd = DuiHelper::NewPopWindow<CInputBox>(szTitle, hParent);
		pWnd->CenterWindow();
		
		return pWnd;
	}

	CInputBox();
	~CInputBox(void);

	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetPassword(bool b);
	const DuiLib::CDuiString& GetInput() const {
		return m_sInput; }
	//void SetInput(const DuiLib::CDuiString& val) { m_sInput = val; }
	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CDuiString m_sInput;
	//bool m_bPassword;
	
};

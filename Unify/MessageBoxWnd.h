#pragma once
//////////////////////////////////////////////////////////////////////////

class CMsgBoxWnd : public WindowImplBase
{
	static CMsgBoxWnd* g_pMsgBoxWnd;
private:
	HWND m_hMainWnd;
	LPCTSTR m_lpText, m_lpCaiton;
	UINT m_uType;
	void SetFlags(UINT utype);
	CMsgBoxWnd(HWND hMainWnd, LPCTSTR lpText, LPCTSTR lpCation, UINT uType);
public:
	static UINT CreateInstance(HWND hMainWnd, LPCTSTR lpText, LPCTSTR lpCation, UINT uType);
	~CMsgBoxWnd(void);

	void InitControl();
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual LRESULT OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();
	//virtual LRESULT OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
};

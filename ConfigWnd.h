#pragma once
//////////////////////////////////////////////////////////////////////////

class CConfigWnd : public WindowImplBase
{
	bool	m_bInited = false;

	void OnInitWindow();
public:
	CConfigWnd();
	~CConfigWnd(void);

	void SetIndex(unsigned int index);
	void OnHKEdit(LPCTSTR sID, LPCTSTR sModifiers, int nKey);
	void SaveCfg();
	void OnEvent(CEvent* e);

	void InitControl();
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();

	void LoadCfg();

	void LoadDeskWindows();
	void LoadHK();
	void SaveHK();

	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);

	

	virtual void OnSelectChanged(TNotifyUI &msg);




private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CTreeViewUI* m_pTree;
	CTabLayoutUI* m_pTab;
};

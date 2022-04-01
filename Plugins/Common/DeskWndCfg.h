#pragma once
//////////////////////////////////////////////////////////////////////////


class CDeskWnd;
class CDeskWndCfg : public WindowImplBase
{
	
public:
	CDeskWnd* m_pDeskWnd;

	virtual void SetDBSection(const TString& s)
	{
		m_sSection = s;
	}

	CDeskWndCfg();
	~CDeskWndCfg(void);

	virtual void OnOK();
	virtual void OnApply();

	virtual void SaveCfg();
	virtual void LoadCfg();
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
protected:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;

	CTabLayoutUI* m_pTab;
	COptionUI* m_pOpt1;
	COptionUI* m_pOpt2;
	COptionUI* m_pOpt3;

	bool m_bInited;
	TString m_sSection;
};

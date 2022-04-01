#pragma once
//////////////////////////////////////////////////////////////////////////

#include "AVCapWrapper.h"

class CCamRecPreviewWnd : public WindowImplBase
{
private:
	HWND m_hMainWnd;
	HWND m_hVideoCapPre = NULL;
	
	TVideoCapParam* m_pVideCapParam = NULL;
	

	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
public:
	CCamRecPreviewWnd(HWND hMainWnd);
	~CCamRecPreviewWnd(void);
	TVideoCapParam* GetVideCapParam() const { return m_pVideCapParam; }
	void SetVideCapParam(TVideoCapParam* val) { m_pVideCapParam = val; }
	
	void Restore();
	
	HWND GetVideoCapPre() const { return m_hVideoCapPre; }
	virtual void OnFinalMessage( HWND );
	virtual bool UseCustomRes();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );

	virtual void InitWindow();
	//virtual LRESULT OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged( TNotifyUI &msg );
	virtual void OnItemClick( TNotifyUI &msg );
	virtual void OnItemSelect(TNotifyUI &msg);
private:

};

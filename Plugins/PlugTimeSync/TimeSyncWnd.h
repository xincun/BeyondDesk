#pragma once
//////////////////////////////////////////////////////////////////////////

#include "TimeSyncIntf.h"
#include "../../[Common]/TimeSync/include/TimeSync.h"

class CTimeSyncWnd : public WindowImplBase, public ITimeGetCallback
{
	CTimeSync* m_pSyncer;
	HWND m_hMainWnd;
public:
	CTimeSyncWnd(HWND hMainWnd);
	~CTimeSyncWnd(void);

	virtual void OnFinalMessage( HWND );
	virtual bool UseCustomRes();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );

	virtual void InitWindow();
	//virtual LRESULT OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnTimeGot(SYSTEMTIME* pTime);
	virtual void OnTimeFail();
	virtual void OnTimeSynced(SYSTEMTIME* pTime);
	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemClick( TNotifyUI &msg );
	virtual void OnItemSelect(TNotifyUI &msg);
private:
	CComboUI* m_pCmbSrv;
	CButtonUI* m_pBtnGet;
	CButtonUI* m_pBtnSync;
};

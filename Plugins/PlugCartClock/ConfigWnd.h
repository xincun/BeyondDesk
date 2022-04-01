#pragma once
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Common/DeskWndCfg.h"

class CConfigWnd : public CDeskWndCfg
{
	
	HWND m_hWndPre;
public:
	virtual void SaveCfg();
    virtual void LoadCfg();

	void DrawPreview();
	virtual CDuiString GetSkinFile();

	virtual void Notify( TNotifyUI &msg );

	virtual void InitWindow();
	
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	virtual void OnClick(TNotifyUI& msg);
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
};

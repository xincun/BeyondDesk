#pragma once
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Common/DeskWndCfg.h"
#include "Img/GDIPHelper.h"

class CConfigWnd : public CDeskWndCfg
{
public:
	virtual void SaveCfg();
    virtual void LoadCfg();

	virtual CDuiString GetSkinFile();

	virtual void Notify( TNotifyUI &msg );

	void Repaint();

	virtual void InitWindow();
	
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	virtual void OnClick(TNotifyUI& msg);

	void OnTimer(void* pOwner, UINT uiID);
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
};

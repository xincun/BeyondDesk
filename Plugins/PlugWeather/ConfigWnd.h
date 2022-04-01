#pragma once
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Common/DeskWndCfg.h"
#include "Img/GDIPHelper.h"


class CMainUI;
class CConfigWnd : public CDeskWndCfg
{
	
	
public:
	

	CConfigWnd();

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

	CMainUI* m_pMainUI;

	void HideDeskAboutCtrls();
private:
	bool m_bInited = false;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;

	CComboUI* m_pProvinceCmb;
	CComboUI* m_pCityCmb;
	CComboUI* m_pCountyCmb;

	Json::Value m_oLocationJsonRoot;
	bool LoadLocationJson();
	void InitLocationCmb();
	void OnSelectProvince();
	void OnSelectCity();
	TString m_strProvince;
	TString m_strCity;
	TString m_strCounty;
	bool m_bInitLocationJson = false;
	CColorComboUI* m_pColorCombo;
	void TryGetControls();
};

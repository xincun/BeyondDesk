#pragma once
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Common/DeskWndCfg.h"
#include "Img/GDIPHelper.h"

class CConfigWnd : public CDeskWndCfg
{
	Gdiplus::Image *m_pImageClock;
	Gdiplus::Image *m_pImageHHour;
	Gdiplus::Image *m_pImageHMinu;
	Gdiplus::Image *m_pImageHSec;

	float m_BakWidth, m_BakHeight;
	float m_HourWidth, m_HourHeight;
	float m_MinuWidth, m_MinuHeight;
	float m_SecWidth, m_SecHeight;

	float m_fZoom;
	float m_nPtLen;
	float HOX, HOY, MOX, MOY, SOX, SOY;
	bool m_bShowSecPt;
	std::shared_ptr<TransWndHelper> m_pWndHelper;
	HWND m_hWndPre;
	Bitmap* m_pBmp;
public:
	virtual void SaveCfg();
    virtual void LoadCfg();

	void DrawPreview();
	void OnUpdate(Graphics* pGraphic);
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

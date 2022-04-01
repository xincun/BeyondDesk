#pragma once

#include "stdafx.h"
#include "Img/ContourBitmap.h"
#include "SimpleTimer.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"

class CCartWnd : public CDeskWnd
{
	SIZE sizBack;
	CContourBitmap m_bmpBack;
	CContourBitmap m_bmpNum;
	int m_xStart = 0, m_yStart = 0; //换图时，要重置
	
	virtual void OnConfig()
	{
		ShowConfigWnd<CConfigWnd>(L"卡通时钟设置");
	}

	void InitDraw(LPCTSTR pstrSection)
	{
		
		m_xStart = 0; m_yStart = 0; //重新初始化，应用配置时

		TString sPath = gPlugInfo.Path;
		sPath += _T("Res\\");
		sPath += Path::GetPathLastName(gUnify->DB()->PlugDB()->GetString(pstrSection, _T("Back"), _T("bobdog.bmp")));
		m_bmpBack.LoadBitmap(sPath.c_str());

		sPath = gPlugInfo.Path;
		sPath += _T("Res\\Digit\\");
		sPath += Path::GetPathLastName(gUnify->DB()->PlugDB()->GetString(pstrSection, _T("Num"), _T("blackgreen.bmp")));
		m_bmpNum.LoadBitmap(sPath.c_str());

		sizBack = m_bmpBack.GetSize();

		
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, sizBack.cx, sizBack.cy, SWP_SHOWWINDOW | SWP_NOMOVE);

		HRGN hrgnShape = m_bmpBack.CreateRegion(CLR_DEFAULT);
		SetWindowRgn(m_hWnd, hrgnShape, TRUE);
	}
public:
	virtual void ApplyCfg() {
		__super::ApplyCfg();

		TString sSection = GetDBSection();

		InitDraw(sSection.c_str());
		
		Draw();

		UINT nAlpha = 255 - gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("Alpha"), 0);
		m_pm.SetTransparent(nAlpha);
	};

	virtual LPCTSTR GetWindowClassName() const {
		return _T("CCartWnd");
	};

	void OnTimer(void* pOwner, UINT uiID)
	{
		if (uiID == 1)
		{
			RECT rc = { m_xStart, m_yStart, sizBack.cx, sizBack.cy };
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}

	virtual void LoadStat(LPCTSTR pstrSection) 
	{
		__super::LoadStat(pstrSection); ///会调用ApplyCfg

		gUnify->Core()->Timer()->SetTimer(this, 1, 1000, &CCartWnd::OnTimer);

		//Draw();

		/*m_CheckList[_T("mnuTop")] = true;*/
		//m_CheckList[_T("mnuTop")] = true;
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{	
		SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

		return __super::OnCreate(uMsg, wParam, lParam, bHandled);
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		gUnify->Core()->Timer()->StopTimer(this, 1);

		return __super::OnDestroy(uMsg, wParam, lParam, bHandled);
	}

	void Draw()
	{
		//LOGCON("draw");
		HDC hdc = ::GetDC(m_hWnd);

		DCMem memDC(hdc, sizBack.cx, sizBack.cy);

		m_bmpBack.PaintTo(memDC);
		//::SelectClipRgn(memDC, NULL);

		//找到第一个黑色区域，就是数字画的位置
		bool bFound = false;

		//检查连续区域都是黑色，防止有黑边判断失误
		auto InsureBlack = [&](int ix, int iy)->bool {
			for (int i = ix; i < ix + 10; ++i)
			{
				for (int j = iy; j < iy + 10; j++)
				{
					if (memDC.GetPixel(i, j) != 0)
					{
						return false;
					}
				}
			}

			return true;
		};

		if (m_xStart == 0 && m_yStart == 0)
		{
			//dc.BitBlt(0, 0, sizBack.cx, sizBack.cy, memDC, 0, 0, SRCCOPY);

			for (int ix = 5; ix < 30; ++ix)
			{
				for (int iy = 5; iy < sizBack.cy; ++iy)
				{
					/*int n = memDC.GetPixel(ix, iy);
					LOGCON(n);*/
					if (memDC.GetPixel(ix, iy) == 0)
					{
						//LOGCON("%d，%d", x, y);
						if (InsureBlack(ix, iy))

						{
							m_xStart = ix;
							m_yStart = iy;
							bFound = true;
							break;
						}
					}
				}

				if (bFound) break;
			}
		}

		if (m_xStart == 0 && m_yStart == 0) return;
		//LOGCON("%d,%d=%d,%d", sizBack.cx, sizBack.cy, m_xStart, m_yStart);

		CTime Now = CTime::GetCurrentTime();
		CString sNow = Now.Format(_T("%H:%M:%S"));

		int nDrawShift = m_xStart;
		for (int i = 0; i < sNow.GetLength(); ++i)
		{
			TCHAR c = sNow[i];
			int n = c- 48;

			if (n != 10)
			{
				
				m_bmpNum.PaintTo(memDC, nDrawShift, m_yStart, 15, 25, 15 * n, 0);
				nDrawShift += 15;
			}
			else
			{
				m_bmpNum.PaintTo(memDC, nDrawShift, m_yStart, 10, 25, 150, 0);
				nDrawShift += 10;
			}
		}

		BitBlt(hdc, 0, 0, sizBack.cx, sizBack.cy, memDC.GetDC(), 0, 0, SRCCOPY);
		::ReleaseDC(m_hWnd, hdc);
	}
};


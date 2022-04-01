#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"

using namespace DuiLib;

class CDigiClockWnd : public CDeskWnd
{
	HWND m_hMainWnd;

	std::shared_ptr<TransWndHelper> m_pWndHelper;

	Gdiplus::Image *m_pImageNum;
public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CDigiClockWnd");
	};

	virtual void OnConfig()
	{
		ShowConfigWnd<CConfigWnd>(L"数字时钟设置");
		
	}

	CDigiClockWnd() : 
		m_pImageNum(NULL),
		m_hMainWnd(NULL)
		{}

	~CDigiClockWnd()
	{
		delete m_pImageNum;

		gUnify->Core()->Timer()->StopTimer(this, 1);
	};

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_EXSTYLE);
		styleValue |= WS_EX_LAYERED;
		styleValue |= WS_EX_TOOLWINDOW;
		::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

		m_pImageNum = new Gdiplus::Image((gUnify->App()->Path()->ExePath + L"Plugins\\DigiClock\\back.png").c_str());

		m_pWndHelper = GDIPHelper::CreateTransWndHelper(m_hWnd, m_pImageNum->GetWidth() * 1.2, m_pImageNum->GetHeight() * 1.5);

		//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, m_BakWidth, m_BakHeight, SWP_NOSIZE | SWP_NOMOVE);

		gUnify->Core()->Timer()->SetTimer(this, 1, 1000, &CDigiClockWnd::OnTimer);
		
		return __super::OnCreate(uMsg, wParam, lParam, bHandled);
	}

	void OnTimer(void *pOwner, UINT nTimerID)
	{
		//LOGCON("on DigiClock timer...");

		if (nTimerID == 1 && pOwner == this) {
			Draw();
		}
	}

	void Draw()
	{
		int nTrans = 105 - m_nTransparent;
		nTrans = nTrans > 100 ? 100 : nTrans;
		nTrans = nTrans < 0 ? 0 : nTrans;

		m_pWndHelper->SetTransparent(int(nTrans*2.55));//1~255

		//onUpdate为画背景时回调
		float fZoom = float(m_nZoom) / 100;
		float nWidth = m_pWndHelper->GetBKSize().cx * fZoom;
		float nHeight = m_pWndHelper->GetBKSize().cy * fZoom;
		m_pWndHelper->SetDrawSize(nWidth, nHeight);
		m_pWndHelper->Update(this, &CDigiClockWnd::OnUpdate);
	}

	void OnUpdate(Graphics* pGraphic)
	{
		SYSTEMTIME SystemTime;   // address of system time structure
		GetLocalTime(&SystemTime);

		//float fBlend = (float)m_nTransparent / 100.0;

		ImageAttributes ImgAttr;
		GDIPHelper::AdjustRGB(ImgAttr, m_nR, m_nG, m_nB, m_nRS, m_nGS, m_nBS);

		int Out[8] = { SystemTime.wHour, -1, SystemTime.wMinute, -1, SystemTime.wSecond };

		float fZoom = float(m_nZoom) / 100;
		float fWidth = 14 * fZoom;
		float fHeight = 23 * fZoom;
		float nShift = 0;
		for (int i = 0; i < 5; ++i)
		{
			if (Out[i] == -1) //冒号
			{
				RectF rcDest(nShift, 0, fWidth, fHeight);
				nShift += fZoom * 15;
				pGraphic->DrawImage(m_pImageNum, rcDest, 140, 0, 14, 23, UnitPixel, &ImgAttr);
			}
			else
			{
				
				RectF rcDest(nShift, 0, fWidth, fHeight);
				nShift += fZoom * 20;
				pGraphic->DrawImage(m_pImageNum, rcDest, 14 * (Out[i] / 10), 0, 14, 23, UnitPixel, &ImgAttr);
				
				rcDest = { nShift, 0, fWidth, fHeight };
				nShift += fZoom * 15;
				pGraphic->DrawImage(m_pImageNum, rcDest, 14 * (Out[i] % 10), 0, 14, 23, UnitPixel, &ImgAttr);
			}
		}
	}

	/*virtual void OnBeforeContextMenu() {
		m_VisbileList[_T("mnuSet")] = false;
	};*/
};
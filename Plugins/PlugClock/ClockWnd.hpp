#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"
#include "IniFile.h"

using namespace DuiLib;

class CClockWnd : public CDeskWnd
{
	SIZE m_BgSize;  //背景图片大小
	HWND m_hMainWnd;

	std::shared_ptr<TransWndHelper> m_pWndHelper;

	Gdiplus::Image *m_pImageClock;
	Gdiplus::Image *m_pImageHHour;
	Gdiplus::Image *m_pImageHMinu;
	Gdiplus::Image *m_pImageHSec;

	float m_BakWidth, m_BakHeight;
	float m_HourWidth, m_HourHeight;
	float m_MinuWidth, m_MinuHeight;
	float m_SecWidth, m_SecHeight;

	float m_nPtLen;
	float HOX, HOY, MOX, MOY, SOX, SOY;
	bool m_bShowSecPt;

	void InitDraw(LPCTSTR pstrSection)
	{
		TString sPathBack = gPlugInfo.Path;
		sPathBack += _T("Res\\");
		sPathBack += Path::GetPathLastName(gUnify->DB()->PlugDB()->GetString(pstrSection, _T("Back"), _T("Default2.png")));

		TString sPathHand = gPlugInfo.Path;
		sPathHand += _T("Res\\Hand\\");
		sPathHand += Path::GetPathLastName(gUnify->DB()->PlugDB()->GetString(pstrSection, _T("Pointer"), _T("Dashboard.ini")));
		CIniFile ini;
		ini.SetFileName(sPathHand.c_str());

		m_pImageClock = new Gdiplus::Image(sPathBack.c_str());
		sPathHand = gPlugInfo.Path;
		sPathHand += _T("Res\\Hand\\");
		m_pImageHHour = new Gdiplus::Image((sPathHand + ini.GetString(_T("Hand"), _T("H"), _T("Dashboard Hour.png")).GetBuffer()).c_str());
		m_pImageHMinu = new Gdiplus::Image((sPathHand + ini.GetString(_T("Hand"), _T("M"), _T("Dashboard Mins.png")).GetBuffer()).c_str());
		m_pImageHSec = new Gdiplus::Image((sPathHand + ini.GetString(_T("Hand"), _T("S"), _T("Dashboard Secs.png")).GetBuffer()).c_str());

		HOX = ini.GetInt(_T("Hand"), _T("HOX"), 1);
		HOY = ini.GetInt(_T("Hand"), _T("HOY"), 8);
		MOX = ini.GetInt(_T("Hand"), _T("MOX"), 1);
		MOY = ini.GetInt(_T("Hand"), _T("MOY"), 8);
		SOX = ini.GetInt(_T("Hand"), _T("SOX"), 23);
		SOY = ini.GetInt(_T("Hand"), _T("SOY"), 6);

		m_nPtLen = gUnify->DB()->PlugDB()->GetInt(pstrSection, _T("PtLen"), 52);
		m_bShowSecPt = gUnify->DB()->PlugDB()->GetBool(pstrSection, _T("chkSec"), true);

		m_pWndHelper = GDIPHelper::CreateTransWndHelper(m_hWnd, m_pImageClock);

		m_BakWidth = m_pImageClock->GetWidth();
		m_BakHeight = m_pImageClock->GetHeight();
		m_HourWidth = m_pImageHHour->GetWidth();
		m_HourHeight = m_pImageHHour->GetHeight();
		m_MinuWidth = m_pImageHMinu->GetWidth();
		m_MinuHeight = m_pImageHMinu->GetHeight();
		m_SecWidth = m_pImageHSec->GetWidth();
		m_SecHeight = m_pImageHSec->GetHeight();
	}
public:
	

	virtual LPCTSTR GetWindowClassName() const {
		return _T("CClockWnd");
	};

	CClockWnd() : m_pImageClock(NULL), 
		m_pImageHHour(NULL),
		m_pImageHMinu(NULL),
		m_pImageHSec(NULL),
		m_hMainWnd(NULL),
		m_nPtLen(40),
		m_bShowSecPt(true)
	{}

	~CClockWnd()
	{
		delete m_pImageClock;
		delete m_pImageHHour;
		delete m_pImageHMinu;
		delete m_pImageHSec;

		gUnify->Core()->Timer()->StopTimer(this, 1);
	};

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_EXSTYLE);
		styleValue |= WS_EX_LAYERED;
		styleValue |= WS_EX_TOOLWINDOW;
		::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

		//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, m_BakWidth, m_BakHeight, SWP_NOSIZE | SWP_NOMOVE);

		gUnify->Core()->Timer()->SetTimer(this, 1, 1000, &CClockWnd::OnTimer);
		
		return __super::OnCreate(uMsg, wParam, lParam, bHandled);
	}

	virtual void OnConfig()
	{
		ShowConfigWnd<CConfigWnd>(L"桌面时钟设置");
	}

	virtual void ApplyCfg() {
		__super::ApplyCfg();

		TString sSection = GetDBSection();

		InitDraw(sSection.c_str());

		Draw();
	};

	void OnTimer(void *pOwner, UINT nTimerID)
	{
		//LOGCON("on Clock timer...");

		if (nTimerID==1 && pOwner == this)
		    Draw();
	}

	virtual void Draw()
	{
		if (!m_pWndHelper) return;
		//if (m_nTransparent < 0 || m_nTransparent>100)	m_nTransparent = 100;
		int nTrans = 105 - m_nTransparent;
		nTrans = nTrans > 100 ? 100 : nTrans;
		nTrans = nTrans < 0 ? 0 : nTrans;

		m_pWndHelper->SetTransparent(int(nTrans*2.55));//1~255

		//onUpdate为画背景时回调
		//m_pWndHelper->Update(this, &CClockWnd::OnUpdate);
		float fZoom = float(m_nZoom) / 100;
		float nWidth = m_pWndHelper->GetBKSize().cx * fZoom;
		float nHeight = m_pWndHelper->GetBKSize().cy * fZoom;
		m_pWndHelper->SetDrawSize(nWidth, nHeight);
		m_pWndHelper->Update(this, m_nR, m_nG, m_nB, m_nRS, m_nGS, m_nBS,
			&CClockWnd::OnUpdate);
	}

	void OnUpdate(Graphics* pGraphic)
	{
		Point points[] = { Point(0, 0),
			Point(m_pImageClock->GetWidth(), 0),
			Point(0, m_pImageClock->GetHeight())
		};

		float fZoom = float(m_nZoom) / 100;
		float nWidth = m_pWndHelper->GetBKSize().cx * fZoom;
		float nHeight = m_pWndHelper->GetBKSize().cy * fZoom;
	/*	int OxyX = nWidth / 2;
		int OxyY = nHeight / 2;*/
		SYSTEMTIME SystemTime;   // address of system time structure
		GetLocalTime(&SystemTime);

		float r, Xo, Yo, Xs, Ys, Xm, Ym, Xh, Yh, H, M, S, Rs, Rm, Rh, OShift, AnH, AnM, AnS, HandR;
		Graphics& g = *pGraphic;

		//中心
		Xo = nWidth / 2;
		Yo = nHeight / 2;

		if (Xo == 0) Xo = 1; 
		if (Yo == 0) Yo = 1;

		g.ResetTransform();

		H = SystemTime.wHour;
		M = SystemTime.wMinute;
		S = SystemTime.wSecond;

		AnS = S * 6 - 90; //秒针角度 ,因为是垂直开始算起，而图像是水平方向算起，所以减去90度
	    AnM= M * 6 - 90; //分针角度
	    AnH= H * 30 + ((M * 60 + S) / 3600 * 30) - 90; //时针角度

	    HandR=m_nPtLen * fZoom; //指针缩放半径

		g.ResetTransform();
		//指针变换
		g.TranslateTransform(Xo, Yo); // 坐标转换，到时钟中心，从中心开始旋转

		g.ScaleTransform(HandR / Xo, HandR / Yo); //缩放到合适大小
		g.RotateTransform(AnH); //旋转角度
		g.TranslateTransform(0 - HOX, 0 - HOY); //因为旋转是X，Y向后偏移，对齐指针开始点

		ImageAttributes imgAttr;
		GDIPHelper::AdjustRGB(imgAttr, m_nR, m_nG, m_nB, m_nRS, m_nGS, m_nBS);

		g.DrawImage(m_pImageHHour,
			RectF(0, 0, m_HourWidth, m_HourHeight),
			0, 0, m_HourWidth, m_HourHeight,
			UnitPixel, &imgAttr);

		g.ResetTransform();
		g.TranslateTransform(Xo, Yo); 
		g.ScaleTransform(HandR / Xo, HandR / Yo); 
		g.RotateTransform(AnM); 
		g.TranslateTransform(0 - MOX, 0 - MOY);

		g.DrawImage(m_pImageHMinu,
			RectF(0, 0, m_MinuWidth, m_MinuHeight),
			0, 0, m_MinuWidth, m_MinuHeight,
			UnitPixel, &imgAttr);

		if (m_bShowSecPt) 
		{
			g.ResetTransform();
			g.TranslateTransform(Xo, Yo); 
			g.ScaleTransform(HandR / Xo, HandR / Yo);
			g.RotateTransform(AnS);
			g.TranslateTransform(0 - SOX, 0 - SOY);

			g.DrawImage(m_pImageHSec,
				RectF(0, 0, m_SecWidth, m_SecHeight),
				0, 0, m_SecWidth, m_SecHeight,
				UnitPixel, &imgAttr);
		}
		
	}

	//LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	//{
	//	//LOGCON_WM(uMsg);
	//	LRESULT lRes = 0;
	//	BOOL bHandled = TRUE;
	//	switch (uMsg) {
	//		case WM_SIZE:{
	//			Draw();
	//			lRes = 0;
	//			break;
	//		}
	//	    //case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	//		/*case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;*/
	//	default:
	//		bHandled = FALSE;
	//	}

	//	if (bHandled) return lRes;

	//	return __super::HandleMessage(uMsg, wParam, lParam);
	//}
};

//end;
//end
//Matrix matrixH(1, 0, 0, 1, OxyX, OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
//matrixH.Rotate(SystemTime.wHour * 30 + SystemTime.wMinute / 2.0 - 180); // 时针旋转的角度度
//Point pointsH[] = { Point(0, 0), Point(m_HourWidth, 0), Point(0, m_HourHeight * fZoom) };
//matrixH.Translate(-m_HourWidth / 2, -m_HourHeight / 6);
//matrixH.TransformPoints(pointsH, 3); // 用该矩阵转换points
//pGraphic->DrawImage(m_pImageHHour, pointsH, 3);

//Matrix matrixM(1, 0, 0, 1, OxyX, OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
//matrixM.Rotate(SystemTime.wMinute * 6 - 180); // 分针旋转的角度度
//Point pointsM[] = { Point(0, 0), Point(m_MinuWidth, 0), Point(0, m_MinuHeight) };
//matrixM.Translate(-m_MinuWidth / 2, -m_MinuHeight / 6);
//matrixM.TransformPoints(pointsM, 3); // 用该矩阵转换pointsM
//pGraphic->DrawImage(m_pImageHMinu, pointsM, 3);

//Matrix matrix(1, 0, 0, 1, OxyX, OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
//matrix.Rotate(SystemTime.wSecond * 6 - 180); // 秒针旋转的角度度
//Point pointsS[] = { Point(0, 0), Point(m_SecWidth, 0), Point(0, m_SecHeight) };
//matrix.Translate(-m_SecWidth / 2, -m_SecHeight / 7);
//matrix.TransformPoints(pointsS, 3); // 用该矩阵转换pointsS
//pGraphic->DrawImage(m_pImageHSec, pointsS, 3);

//Matrix matrixH(1, 0, 0, 1, OxyX, OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
//matrixH.Rotate(SystemTime.wHour * 30 + SystemTime.wMinute / 2.0 - 90); // 时针旋转的角度度
//Point pointsH[] = { Point(0, 0), Point(m_HourWidth, 0), Point(0, m_HourHeight) };
//matrixH.Translate(-m_HourHeight / 2, -m_HourWidth / 6);
//matrixH.TransformPoints(pointsH, 3); // 用该矩阵转换points
//pGraphic->DrawImage(m_pImageHHour, pointsH, 3);

//Matrix matrixM(1, 0, 0, 1, OxyX, OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
//matrixM.Rotate(SystemTime.wMinute * 6 - 90); // 分针旋转的角度度
//Point pointsM[] = { Point(0, 0), Point(m_MinuWidth, 0), Point(0, m_MinuHeight) };
//matrixM.Translate(-m_MinuHeight / 2, -m_MinuWidth / 6);
//matrixM.TransformPoints(pointsM, 3); // 用该矩阵转换pointsM
//pGraphic->DrawImage(m_pImageHMinu, pointsM, 3);

//Matrix matrix(1, 0, 0, 1, OxyX, OxyY); // 定义一个单位矩阵，坐标原点在表盘中央
//matrix.Rotate(SystemTime.wSecond * 6 - 90); // 秒针旋转的角度度
//Point pointsS[] = { Point(0, 0), Point(m_SecWidth, 0), Point(0, m_SecHeight) };
//matrix.Translate(-m_SecHeight / 2, -m_SecWidth / 6);
//matrix.TransformPoints(pointsS, 3); // 用该矩阵转换pointsS
//pGraphic->DrawImage(m_pImageHSec, pointsS, 3);
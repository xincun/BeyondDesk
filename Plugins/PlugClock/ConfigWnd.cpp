#include "stdafx.h"
#include "resource.h"
#include "ConfigWnd.h"
#include "../../Unify/Unify.h"
#include "DeskWnd.h"
#include <future>

//////////////////////////////////////////////////////////////////////////
///

DuiLib::CDuiString CConfigWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("Res\\PlugCfgClk.xml")).c_str();
	//return _T("XML\\PlugCfgClk.xml");
}


void CConfigWnd::OnClick( TNotifyUI &msg )
{
	return __super::OnClick(msg);
}

void CConfigWnd::OnTimer(void* pOwner, UINT uiID)
{
	//DrawPreview();
}

void CConfigWnd::Notify(TNotifyUI &msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		if (m_bInited)
			DrawPreview();

		/*std::async(std::launch::async, [&](){
			Sleep(100);
			DrawPreview();
		});*/
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (m_bInited)
		{
			DrawPreview();
		}

	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		//if (msg.pSender->GetName() == _T("chkSec"))
		if (m_bInited)
		{
			

			
			DrawPreview();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
	{
		//if (msg.pSender->GetName() == _T("edtPtLen"))
		if (m_bInited)
		{
			DrawPreview();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		//if (msg.pSender->GetName() == _T("edtPtLen"))
		if (m_bInited)
		{
			DrawPreview();
		}
	}

	return __super::Notify(msg);
}

void CConfigWnd::Repaint()
{
	InvalidateRect(m_hWnd, NULL, true);
	::UpdateWindow(m_hWnd);
}

void CConfigWnd::InitWindow()
{
	TString dir = gPlugInfo.Path;
	dir += L"Res\\";

	vector<TString> vecFiles;
	File::RecruseDirFiles(vecFiles, dir.c_str(), false, L"*.png");

	CComboUI * pCombBack = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	auto it = vecFiles.begin();
	for (; it != vecFiles.end(); ++it)
	{
		CListLabelElementUI *pListE = new CListLabelElementUI;
		pListE->SetText(Path::GetFileNameWithoutExtension(it->c_str()).c_str());
		pListE->SetUserData(it->c_str());
		pCombBack->Add(pListE);
	}

	vecFiles.clear();
	dir += _T("Hand\\");
	File::RecruseDirFiles(vecFiles, dir.c_str(), false, L"*.ini");

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbPt")));

	it = vecFiles.begin();
	for (; it != vecFiles.end(); ++it)
	{
		CListLabelElementUI *pListE = new CListLabelElementUI;
		pListE->SetText(Path::GetFileNameWithoutExtension(it->c_str()).c_str());
		pListE->SetUserData(it->c_str());
		pComb->Add(pListE);
	}
	pComb->SelectItem(0);
	pCombBack->SelectItem(0);

	__super::InitWindow();

	m_bInited = true;


}

LRESULT CConfigWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//switch (uMsg)
	//{
	//case WM_PAINT:
	////case WM_NCPAINT:
	//{
	//	DrawPreview();
	//	break;
	//}
	//default:
	//	break;
	//}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

static LRESULT CALLBACK	 _WndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

CControlUI* CConfigWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	return NULL;
}

void CConfigWnd::SaveCfg()
{
	__super::SaveCfg();

	TString sSection = m_pDeskWnd->GetDBSection();

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Back"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());

	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbPt")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Pointer"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());
	
	CSpinEditUI* pEdt = static_cast<CSpinEditUI*>(m_PaintManager->FindControl(_T("edtPtLen")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("PtLen"), pEdt->GetText().GetData());

	CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(_T("chkSec")));
	gUnify->DB()->PlugDB()->SetBool(sSection.c_str(), _T("chkSec"), pChk->GetCheck());
}

void CConfigWnd::LoadCfg()
{
	__super::LoadCfg();

	TString sSection = m_pDeskWnd->GetDBSection();

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	for (int i = 0; i < pComb->GetCount(); ++i)
	{
		if (Path::GetPathLastName(pComb->GetItemAt(i)->GetUserData().GetData()) == Path::GetPathLastName(gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Back"), _T("Default2.png"))))
		{
			pComb->SelectItem(i);
		}
	}

	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbPt")));
	for (int i = 0; i < pComb->GetCount(); ++i)
	{
		if (Path::GetPathLastName(pComb->GetItemAt(i)->GetUserData().GetData()) == Path::GetPathLastName(gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Pointer"), _T("Dashboard.ini"))))
		{
			pComb->SelectItem(i);
		}
	}

	CSpinEditUI* pEdt = static_cast<CSpinEditUI*>(m_PaintManager->FindControl(_T("edtPtLen")));
	pEdt->SetText(gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("PtLen"), _T("52")).GetBuffer());

	CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(_T("chkSec")));
	pChk->SetCheck(gUnify->DB()->PlugDB()->GetBool(sSection.c_str(), _T("chkSec"), true));
}

void CConfigWnd::DrawPreview()
{
	if (m_pTab->GetCurSel() != 0) { 
		
		return; 
	}

	TString sPathBack = gPlugInfo.Path;
	sPathBack += _T("Res\\");
	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	sPathBack += Path::GetPathLastName(pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());

	TString sPathHand = gPlugInfo.Path;
	sPathHand += _T("Res\\Hand\\");
	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbPt")));
	sPathHand += Path::GetPathLastName(pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());
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

	CSpinEditUI* pEdt = static_cast<CSpinEditUI*>(m_PaintManager->FindControl(_T("edtPtLen")));
	m_nPtLen = String::StrToInt(pEdt->GetText().GetData());
	CCheckBoxUI* pChk = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(_T("chkSec")));
	m_bShowSecPt = pChk->GetCheck();

	m_pWndHelper.reset();
	m_pWndHelper = GDIPHelper::CreateTransWndHelper(m_hWnd, m_pImageClock);

	m_BakWidth = m_pImageClock->GetWidth();
	m_BakHeight = m_pImageClock->GetHeight();
	m_HourWidth = m_pImageHHour->GetWidth();
	m_HourHeight = m_pImageHHour->GetHeight();
	m_MinuWidth = m_pImageHMinu->GetWidth();
	m_MinuHeight = m_pImageHMinu->GetHeight();
	m_SecWidth = m_pImageHSec->GetWidth();
	m_SecHeight = m_pImageHSec->GetHeight();


	m_fZoom = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldZoom")))->GetValue() / 100.0;

	const RECT rcDest = FindControl<CControlUI>("imgPreview")->GetPos();
	//m_pWndHelper->Update(this, m_hWnd, rc, &CConfigWnd::OnUpdate);
	m_pBmp = new Bitmap(rcDest.right - rcDest.left, rcDest.bottom - rcDest.top, PixelFormat32bppARGB);
	Graphics g(m_pBmp);
	ImageAttributes ImgAttr;
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	RectF rcDraw(0, 0, rcDest.right - rcDest.left, rcDest.bottom - rcDest.top);
	g.DrawImage(m_pBmp, 0, 0);
	g.DrawImage(m_pImageClock, rcDraw, 0, 0, m_BakWidth, m_BakHeight, UnitPixel);
	OnUpdate(&g);
	//Util::Image::SaveBmpToPngFile(m_pBmp, L"D:\\1.png");
	m_PaintManager->RemoveImage(L"111");
	FindControl<CControlUI>("imgPreview")->SetBkImage(L"111", m_pBmp);
	
	delete m_pBmp;
	m_pBmp = NULL;
}

void CConfigWnd::OnUpdate(Graphics* pGraphic)
{
	Point points[] = { Point(0, 0),
		Point(m_pImageClock->GetWidth(), 0),
		Point(0, m_pImageClock->GetHeight())
	};

	const RECT rc = FindControl<CControlUI>("imgPreview")->GetPos();
	float nWidth = rc.right - rc.left;//m_BakWidth;
	float nHeight = rc.bottom - rc.top;//m_BakHeight;
	float OxyX = nWidth / 2;
	float OxyY = nHeight / 2;
	SYSTEMTIME SystemTime;   // address of system time structure
	GetLocalTime(&SystemTime);

	float r, Xo, Yo, Xs, Ys, Xm, Ym, Xh, Yh, H, M, S, Rs, Rm, Rh, OShift, AnH, AnM, AnS, HandR;
	Graphics& g = *pGraphic;

	//中心
	

	Xo =nWidth / 2;
	Yo = nHeight / 2;

	if (Xo == 0) Xo = 1;
	if (Yo == 0) Yo = 1;

	g.ResetTransform();

	H = 9;
	M = 17;
	S = 28;

	AnS = S * 6 - 90; //秒针角度 ,因为是垂直开始算起，而图像是水平方向算起，所以减去90度
	AnM = M * 6 - 90; //分针角度
	AnH = H * 30 + ((M * 60 + S) / 3600 * 30) - 90; //时针角度

	HandR = m_nPtLen * /*m_fZoom * */(nWidth / m_BakWidth) * (nHeight / m_BakHeight); //指针缩放半径, 要注意和当前画出的大小匹配
	//LOGCON(HandR);

	g.ResetTransform();
	//指针变换
	g.TranslateTransform(Xo, Yo); // 坐标转换，到时钟中心，从中心开始旋转

	g.ScaleTransform((HandR) / OxyX, (HandR) / OxyY); //缩放到合适大小
	g.RotateTransform(AnH); //旋转角度
	g.TranslateTransform(0 - HOX, 0 - HOY); //因为旋转是X，Y向后偏移，对齐指针开始点

	ImageAttributes imgAttr;
	
	g.DrawImage(m_pImageHHour,
		RectF(0, 0, m_HourWidth, m_HourHeight),
		0, 0, m_HourWidth, m_HourHeight,
		UnitPixel, &imgAttr);

	g.ResetTransform();
	g.TranslateTransform(Xo, Yo);
	g.ScaleTransform(HandR / OxyX, HandR / OxyY);
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
		g.ScaleTransform((HandR) / OxyX, (HandR) / OxyY);
		g.RotateTransform(AnS);
		g.TranslateTransform(0 - SOX, 0 - SOY);

		g.DrawImage(m_pImageHSec,
			RectF(0, 0, m_SecWidth, m_SecHeight),
			0, 0, m_SecWidth, m_SecHeight,
			UnitPixel, &imgAttr);
	}

}


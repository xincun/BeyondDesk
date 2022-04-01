#include "stdafx.h"
#include "resource.h"
#include "ConfigWnd.h"
#include "../../Unify/Unify.h"
#include "Img/ContourBitmap.h"
#include "DeskWnd.h"
//////////////////////////////////////////////////////////////////////////
///

DuiLib::CDuiString CConfigWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("Res\\PlugCfgCart.xml")).c_str();
	//return _T("XML\\PlugCfgCart.xml");
}


void CConfigWnd::OnClick( TNotifyUI &msg )
{
	return __super::OnClick(msg);
}

void CConfigWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		if (m_bInited)
			DrawPreview();
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (m_bInited)
		{
			DrawPreview();
		}
	}
	if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender->GetName() == _T("opt1"))
		{
			::ShowWindow(m_hWndPre, SW_SHOW);
			if (m_bInited)
				DrawPreview();
		}
		else if (msg.pSender->GetName() == _T("opt2"))
		{
			::ShowWindow(m_hWndPre, SW_HIDE);
		}
	}

	return __super::Notify(msg);
}

void CConfigWnd::InitWindow()
{
	TString dir = gPlugInfo.Path;
	dir += L"Res\\";

	vector<TString> vecFiles;
	File::RecruseDirFiles(vecFiles, dir.c_str(), false, L"*.bmp");

	CComboUI * pCombBack = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	auto it = vecFiles.begin();
	for (; it != vecFiles.end(); ++it)
	{
		CListLabelElementUI *pListE = new CListLabelElementUI;
		if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("bobdog"))
			pListE->SetText(_T("巴比豆"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("don"))
			pListE->SetText(_T("唐老鸭"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("fishbone"))
			pListE->SetText(_T("鱼骨头"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("mickey"))
			pListE->SetText(_T("米老鼠"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("no"))
			pListE->SetText(_T("简洁"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("snoopy"))
			pListE->SetText(_T("史努比"));
		else pListE->SetText(Path::GetFileNameWithoutExtension(it->c_str()).c_str());

		pListE->SetUserData(it->c_str());
		pCombBack->Add(pListE);
	}

	vecFiles.clear();
	dir += _T("Digit\\");
	File::RecruseDirFiles(vecFiles, dir.c_str(), false, L"*.bmp");

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbNum")));

	it = vecFiles.begin();
	for (; it != vecFiles.end(); ++it)
	{
		CListLabelElementUI *pListE = new CListLabelElementUI;
		if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("blackgreen"))
			pListE->SetText(_T("黑绿"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("bluegreen"))
			pListE->SetText(_T("蓝绿"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("greyyellow"))
			pListE->SetText(_T("灰黄"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("led"))
			pListE->SetText(_T("液晶"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("lightblue"))
			pListE->SetText(_T("浅蓝"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("orangeyellow"))
			pListE->SetText(_T("桔黄"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("pink"))
			pListE->SetText(_T("粉红"));
		else if (Path::GetFileNameWithoutExtension(it->c_str()) == _T("yellowwhite"))
			pListE->SetText(_T("黄白"));
		else pListE->SetText(Path::GetFileNameWithoutExtension(it->c_str()).c_str());

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

	return __super::HandleMessage(uMsg, wParam, lParam);
}

CControlUI* CConfigWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	if (_tcsicmp(pstrClass, _T("Wnd")) == 0)
	{
		//CWndHostUI  *pUI = new CWndHostUI;
		//CDateTimeCtrl *dtTime = new CDateTimeCtrl;

		//gCfgWnd = this;
		m_hWndPre = Window::CreateSimpleWndow(m_hWnd, _T("DigitClockPreview"), _T(""));

		//pUI->Attach(m_hWndPre);
		SetParent(m_hWndPre, m_hWnd);
		//CButton *pBtn = new CButton;    // 记得释放内存
		//pBtn->Create(_T("MFC"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, CRect(0, 0, 0, 0), CWnd::FromHandle(this->m_hWnd), 0);
		//pUI->Attach(pBtn->GetSafeHwnd());

		//return pUI;
	}

	return NULL;
}

void CConfigWnd::SaveCfg()
{
	__super::SaveCfg();

	TString sSection = m_pDeskWnd->GetDBSection();

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Back"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());

	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbNum")));
	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Num"), pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData());
}

void CConfigWnd::LoadCfg()
{
	__super::LoadCfg();

	TString sSection = m_pDeskWnd->GetDBSection();

	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	for (int i = 0; i < pComb->GetCount(); ++i)
	{
		if (Path::GetPathLastName(pComb->GetItemAt(i)->GetUserData().GetData()) == Path::GetPathLastName(gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Back"))))
		{
			pComb->SelectItem(i);
		}
	}

	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbNum")));
	for (int i = 0; i < pComb->GetCount(); ++i)
	{
		if (Path::GetPathLastName(pComb->GetItemAt(i)->GetUserData().GetData()) == Path::GetPathLastName(gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Num"))))
		{
			pComb->SelectItem(i);
		}
	}
}

void CConfigWnd::DrawPreview()
{
	
	TString sPath;
	CComboUI * pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbBack")));
	sPath = pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData();

	CContourBitmap m_bmpBack;
	m_bmpBack.LoadBitmap(sPath.c_str());

	sPath = _T("");
	pComb = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbNum")));
	sPath = pComb->GetItemAt(pComb->GetCurSel())->GetUserData().GetData();

	CContourBitmap m_bmpNum;
	m_bmpNum.LoadBitmap(sPath.c_str());

	SIZE sizBack = m_bmpBack.GetSize();
	CDuiRect rc;
	::GetWindowRect(m_hWndPre, &rc);

	CHorizontalLayoutUI* pHorz = FindControl<CHorizontalLayoutUI>("horzPreview");
	CDuiRect rcParent = pHorz->GetPos();

	CDuiRect rcWnd;
	::GetWindowRect(m_hWnd, &rcWnd);

	int x = (rcWnd.GetWidth() - sizBack.cx) / 2 + 10;
	::MoveWindow(m_hWndPre, x,
		150 + (rcParent.GetHeight() - sizBack.cy) / 2,
		sizBack.cx,
		sizBack.cy,
		true 
		);


	int m_xStart = 0; int m_yStart = 0;

	{
		//LOGCON("draw");
		//::SendMessage(m_hWndPre, WM_PAINT, 0, 0);
		::ShowWindow(m_hWndPre, SW_HIDE);
		::ShowWindow(m_hWndPre, SW_SHOW);

		HDC dc = GetDC(m_hWndPre);

		DCMem memDC(dc, sizBack.cx, sizBack.cy);

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
			;
		};

		if (m_xStart == 0 && m_yStart == 0)
		{
			BitBlt(dc, 0, 0, sizBack.cx, sizBack.cy, memDC, 0, 0, SRCCOPY);

			for (int ix = 5; ix < 30; ++ix)
			{
				for (int iy = 5; iy < sizBack.cy; ++iy)
				{
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

		CTime Now = CTime::GetCurrentTime();
		CString sNow = Now.Format(_T("%H:%M:%S"));

		int nDrawShift = m_xStart;
		for (int i = 0; i < sNow.GetLength(); ++i)
		{
			TCHAR c = sNow[i];
			int n = c - 48;

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

		
		BitBlt(dc, 0, 0, sizBack.cx, sizBack.cy, memDC, 0, 0, SRCCOPY);

	
	}
}


#include "stdafx.h"
#include "resource.h"
#include "DeskWnd.h"
#include <shellapi.h>
#include "SimpleTimer.h"


void CDeskWnd::SaveSize()
{
	TString sSection = GetDBSection();

	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("width"), rc.right - rc.left);
	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("height"), rc.bottom - rc.top);
}

void CDeskWnd::LoadSize(int defaultWidth, int defaultHeight)
{
	TString sSection = GetDBSection();

	int w = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("width"), defaultWidth);
	int h = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("height"), defaultHeight);
	SetWindowPos(m_hWnd, NULL, 0, 0, w, h, SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
}

void CDeskWnd::LoadBack()
{
	TString sSection = GetDBSection();

	int nTrans = 105 - m_nTransparent;
	nTrans = nTrans > 100 ? 100 : nTrans;
	nTrans = nTrans < 0 ? 0 : nTrans;

	m_pm.SetTransparent(int(nTrans*2.55));//1~255

	CVerticalLayoutUI* pBack = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("pnlBack")));
	if (pBack) {
		CString sBack = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Back"), _T("black"));
		if (sBack == _T("black"))
		{
			//pBack->SetBkImage(_T("Res/Black.png"));
			CDuiString s;
			s.Format(L"file='%s' corner='20,20,20,30'", _T("Res/Black.png"));
			pBack->SetAttribute(L"bkimage", s);
		}
		else if (sBack == _T("gray"))
		{
			//pBack->SetBkImage(_T("Res/Gray.png"));
			CDuiString s;
			s.Format(L"file='%s' corner='20,20,20,30'", _T("Res/Gray.png"));
			pBack->SetAttribute(L"bkimage", s);
		}
		else if (sBack == _T("blue"))
		{
			//pBack->SetBkImage(_T("Res/Blue.png"));
			CDuiString s;
			s.Format(L"file='%s' corner='20,20,20,30'", _T("Res/Blue.png"));
			pBack->SetAttribute(L"bkimage", s);
		}
		else if (sBack == _T("glass"))
		{
			//pBack->SetBkImage(_T("Res/Glass.png"));
			CDuiString s;
			s.Format(L"file='%s' corner='20,20,20,30'", _T("Res/Glass.png"));
			pBack->SetAttribute(L"bkimage", s);
		}
		else if (sBack == _T("none"))
		{
			pBack->SetBkImage(_T(""));

		}
	}
}

TString CDeskWnd::GetDBSection()
{
	TString sSection = GetWindowClassName();
	sSection += ID;
	return sSection;
}

void CDeskWnd::SaveStat()
{
	TString sSection = GetDBSection();

	gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("ID"), ID.c_str());

	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("Left"), rc.left);
	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("Top"), rc.top);

	gUnify->DB()->PlugDB()->SetBool(sSection.c_str(), _T("mnuTop"), m_CheckList[_T("mnuTop")]);
	gUnify->DB()->PlugDB()->SetBool(sSection.c_str(), _T("mnuNoMove"), m_CheckList[_T("mnuNoMove")]);
	gUnify->DB()->PlugDB()->SetBool(sSection.c_str(), _T("mnuTrans"), m_CheckList[_T("mnuTrans")]);
	gUnify->DB()->PlugDB()->SetBool(sSection.c_str(), _T("mnuHide"), m_CheckList[_T("mnuHide")]);

	gUnify->DB()->PlugDB()->SetBool(sSection.c_str(), _T("visible"), ::IsWindowVisible(m_hWnd));
}

void CDeskWnd::OnTimer(void* pOwner, UINT uiID)
{
	if (uiID == 9999)
	{
		//LOGCON("ontimer");
		//if (GetForegroundWindow() != m_hWnd && m_CheckList[_T("mnuTop")])
		//{

		//	LOGCON("no topmosed, change...");
		//	//SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE);
		//	SetForegroundWindow(m_hWnd);
		//}

		if (GetAsyncKeyState(VK_CONTROL) && 0x8000) //ctrl按下
		{
			m_bCtrlPressed = true;

			if (m_CheckList[_T("mnuTrans")]) //屏蔽鼠标穿透暂时
			{
				StopMouseTrans();

			}
			else if (m_CheckList[_T("mnuHide")])
			{
				ShowWindow(true);
			}
		}
		else
		{
			m_bCtrlPressed = false;

			if (m_CheckList[_T("mnuTrans")])
			{
				StartMouseTrans();
			}
			else if (m_CheckList[_T("mnuHide")])
			{
				ShowOrHide();
			}
		}
	}
}

void CDeskWnd::StartMouseTrans()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_EXSTYLE);
	styleValue |= WS_EX_TRANSPARENT;
	styleValue |= WS_EX_LAYERED;
	::SetWindowLong(*this, GWL_EXSTYLE, styleValue);
}

void CDeskWnd::StopMouseTrans()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_EXSTYLE);
	styleValue &= ~WS_EX_TRANSPARENT;
	::SetWindowLong(*this, GWL_EXSTYLE, styleValue);
}

void CDeskWnd::ShowOrHide()
{
	RECT rc;
	::GetWindowRect(m_hWnd, &rc);

	CDuiPoint pt;
	GetCursorPos(&pt);

	if (PtInRect(&rc, pt))
	{
		ShowWindow(false);
	}
	else
	{
		ShowWindow(true);
	}
}

void CDeskWnd::OnConfig()
{

}

void CDeskWnd::LoadStat(LPCTSTR pstrSection)
{
	ID = gUnify->DB()->PlugDB()->GetString(pstrSection, _T("ID"), String::newGUID().c_str());

	RECT rc = Window::GetWorkRect(gUnify->App()->m_MainWnd);

	int nLeft = gUnify->DB()->PlugDB()->GetInt(pstrSection, _T("Left"), (rc.right - rc.left) / 2);
	int nTop = gUnify->DB()->PlugDB()->GetInt(pstrSection, _T("Top"), (rc.right - rc.left) / 2);

	int w, h;
	Window::InsurePosition(m_hWnd, nLeft, nTop, w, h);

	::SetWindowPos(m_hWnd, HWND_TOP,
		nLeft,
		nTop,
		0, 0, SWP_NOSIZE | SWP_NOREDRAW);

	m_CheckList[_T("mnuTop")] = gUnify->DB()->PlugDB()->GetBool(pstrSection, _T("mnuTop"), false);
	if (m_CheckList[_T("mnuTop")])
		SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE);
	else
		SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE);

	m_CheckList[_T("mnuNoMove")] = gUnify->DB()->PlugDB()->GetBool(pstrSection, _T("mnuNoMove"), false);

	m_CheckList[_T("mnuTrans")] = gUnify->DB()->PlugDB()->GetBool(pstrSection, _T("mnuTrans"), false);
	if (m_CheckList[_T("mnuTrans")])
	{
		m_CheckList[_T("mnuHide")] = false;

		StartMouseTrans();
	}
	else
	{
		StopMouseTrans();
	}

	m_CheckList[_T("mnuHide")] = gUnify->DB()->PlugDB()->GetBool(pstrSection, _T("mnuHide"), false);
	if (m_CheckList[_T("mnuHide")])
		m_CheckList[_T("mnuTrans")] = false;

	bool bVisible = gUnify->DB()->PlugDB()->GetBool(pstrSection, _T("visible"), true);
	if (bVisible) {
		::ShowWindow(m_hWnd, SW_SHOW);
	}
	else
		::ShowWindow(m_hWnd, SW_HIDE);

	ApplyCfg();
}

void CDeskWnd::Init()
{
	ID = String::newGUID();
	TString sSection = GetDBSection();
	LoadStat(sSection.c_str());
}

CDeskWnd::CDeskWnd() :m_nTransparent(0),
m_nZoom(100),
 m_nR(100),
 m_nG(100),
 m_nB(100),
 m_nRS(0),
 m_nGS(0),
 m_nBS(0)
/*m_bCustomColor(false),
m_dwColor(0) *//* : gUnify(NULL)*/
{
	
}

void CDeskWnd::ApplyCfg()
{
	TString sSection = GetDBSection();

	m_nTransparent = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("Alpha"), 0);
	m_nTransparent = float(m_nTransparent) / 230 * 100;
	m_nZoom = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("Zoom"), 100);
	/*m_bCustomColor = gUnify->DB()->PlugDB()->GetBool(sSection.c_str(), _T("IsColor"), false);

	TString sClr = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Color"), L"4286244241");
	m_dwColor = String::StrToInt64(sClr.c_str());*/
	

	ApplyColorR(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("RValue"), 100));
	ApplyColorG(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("GValue"), 100));
	ApplyColorB(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("BValue"), 100));
	ApplyColorRS(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("RSValue"), 0));
	ApplyColorGS(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("GSValue"), 0));
	ApplyColorBS(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("BSValue"), 0));

	
}

void CDeskWnd::ApplyColorR(int r)
{
    m_nR = r;
}

void CDeskWnd::ApplyColorG(int g)
{
	m_nG = g;
}

void CDeskWnd::ApplyColorB(int b)
{
	m_nB = b;
}

void CDeskWnd::ApplyColorRS(int rs)
{
	m_nRS = rs;
}

void CDeskWnd::ApplyColorGS(int gs)
{
	m_nGS = gs;
}

void CDeskWnd::ApplyColorBS(int bs)
{
	m_nBS = bs;
}

void CDeskWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);

	auto it = gUnify->App()->Status()->DeskWindows.begin();
	for (; it != gUnify->App()->Status()->DeskWindows.end();)
	{
		if ((*it).WndObject == this)
		{
			it = gUnify->App()->Status()->DeskWindows.erase(it);
		}
		else
			it++;
	}

	gUnify->Core()->Event()->PostEvent(EVENT_DESK_WND_CLOSE);

	delete this;
}

void CDeskWnd::Draw()
{

}

LRESULT CDeskWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	gUnify->Core()->Timer()->SetTimer(this, 9999, 1000, &CDeskWnd::OnTimer);

	m_pm.Init(m_hWnd); //否则接收不到菜单消息

	return 0;
}

LRESULT CDeskWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Draw();
	bHandled = FALSE;

	return 0;
}

LRESULT CDeskWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_CheckList[_T("mnuHide")])
	{
		if (!m_bCtrlPressed)
		{
			ShowOrHide();
		}
		else
		{
			ShowWindow(true);
		}
	}

	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (!::IsZoomed(*this))
	{
		RECT rcSizeBox = m_pm.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) 
				return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) 
				return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) 
                return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) 
				return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}

	RECT rcCaption = m_pm.GetCaptionRect();
	if (rcCaption.left == 0 && rcCaption.right == 0 && rcCaption.top == 0 && rcCaption.bottom == 0) return HTCAPTION;
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		//_LOGCON(pControl->GetClass());
		if (pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0) /*&&
			_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("EditUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("CheckBoxUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("RoundControlUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("ComboUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("SliderUI")) != 0)*/
			//string sControl = CDialogBuilder::GetControlClassStr(pControl->GetClass());
			/*tstring sCmp = DUI_CTR_HORIZONTALLAYOUT;
			sCmp += _T("UI");
			if (_tcscmp(pControl->GetClass(), sCmp.c_str()) == 0)*/
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CDeskWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	gUnify->Core()->Timer()->StopTimer(this, 9999);

	bHandled = FALSE;
	return 0;
}

LRESULT CDeskWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDeskWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//这个$F012这个命令在定义里没有，其实它是SC_MOVE or 2的结果，微软的文档中提到WM_SYSCOMMAND命令中wParam的值的低4位值是保留的
	if (wParam == 0xF012 && m_CheckList[_T("mnuNoMove")])
	{
		bHandled = TRUE;
		return 0;
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CDeskWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDeskWnd::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDeskWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CDeskWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	{
		switch (uMsg) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
			//case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_PAINT:       lRes = OnPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_ERASEBKGND: lRes = TRUE;   break;
		case WM_SYSCOMMAND: lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:  lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYUP: lRes = OnKeyUp(uMsg, wParam, lParam, bHandled); break;
		//case WM_MOUSEHOVER: lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
		case WM_EXITSIZEMOVE:
		{
			SaveStat();
			break;
		}
		case WM_NCRBUTTONUP: {
			lRes = TRUE;

			OnContextMenu();

			break;
		}
		case WM_MENUCLICK:
		{
			CDuiString *strMenuName = (CDuiString*)wParam;
			if (strMenuName->IsEmpty()) break;

			bool bChecked = (bool)lParam;

			if (*strMenuName == _T("mnuTop"))
			{
				if (bChecked)
					SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE);
				else
					SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE);

				SaveStat();

				bHandled = TRUE;

				delete strMenuName;
			}
			else if (*strMenuName == _T("mnuTrans"))
			{
				if (bChecked)
				{
					m_CheckList[_T("mnuHide")] = false;

					StartMouseTrans();
				}
				else
				{
					StopMouseTrans();
				}

				SaveStat();

				delete strMenuName;

				bHandled = TRUE;
			}
			else if (*strMenuName == _T("mnuHide"))
			{
				if (bChecked)
					m_CheckList[_T("mnuTrans")] = false;

				SaveStat();

				delete strMenuName;

				bHandled = TRUE;
			}
			else if (*strMenuName == _T("mnuClose"))
			{
				if (MessageBox(this->m_hWnd, _T("您确定要关闭吗？"), _T(""), MB_OKCANCEL) == IDCANCEL)
				{
					break;
				}

				delete strMenuName;
				
				SelfClose();

				bHandled = FALSE;
			}
			else if (*strMenuName == _T("mnuSet"))
			{
				OnConfig();

				delete strMenuName;

				bHandled = TRUE;
			}

			//把剩下的留给其他处理
		    break;
		}
		/*case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;*/
		default:
			bHandled = FALSE;
		}
	}
	if (bHandled) return lRes;

	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CDeskWnd::OnContextMenu()
{
	CMenuWnd* pMenu = new CMenuWnd();
	DuiLib::CDuiPoint point;
	GetCursorPos(&point);

	OnBeforeContextMenu();
	pMenu->Init(NULL, _T("XML\\MenuDeskWnd.xml"), point, &m_pm, &m_CheckList, &m_VisbileList);
}

void CDeskWnd::SelfClose()
{
	TString sSection = GetDBSection();
	gUnify->DB()->PlugDB()->DeleteSection(sSection.c_str());

	Close();
}

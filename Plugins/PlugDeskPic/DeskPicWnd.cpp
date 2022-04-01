#include "stdafx.h"
#include "DeskPicWnd.h"
#include "EventDefine.h"


#define WM_RECRUSEFILES_DONE WM_USER + 1010

void CDeskPicWnd::Notify(TNotifyUI& msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		::GetWindowRect(*this, &m_OriginalRect);
	}
	else if (msg.sType == DUI_MSGTYPE_DBCLICK)
	{
		return;
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnPause"))
		{
			CButtonUI *pBtn = static_cast<CButtonUI*>(m_pm.FindControl(L"btnPlay"));
			pBtn->SetVisible(true);
			pBtn = static_cast<CButtonUI*>(m_pm.FindControl(L"btnPause"));
			pBtn->SetVisible(false);

			m_bPaused = true;
		}
		else if (msg.pSender->GetName() == _T("btnPlay"))
		{
			CButtonUI *pBtn = static_cast<CButtonUI*>(m_pm.FindControl(L"btnPlay"));
			pBtn->SetVisible(false);
			pBtn = static_cast<CButtonUI*>(m_pm.FindControl(L"btnPause"));
			pBtn->SetVisible(true);

			m_bPaused = false;

			
		}
		else if (msg.pSender->GetName() == _T("btnNext"))
		{
			m_bPaused = true;
			CButtonUI *pBtn = static_cast<CButtonUI*>(m_pm.FindControl(L"btnPlay"));
			pBtn->SetVisible(true);
			pBtn = static_cast<CButtonUI*>(m_pm.FindControl(L"btnPause"));
			pBtn->SetVisible(false);

			m_nPlayPosition++;
			if (m_nPlayPosition >= mPlaySequence.size()) {
				m_nPlayPosition = mPlaySequence.size() - 1;

				Update();
				return;
			}

			if (mPlaySequence.size() > 0)
				DrawPicByNumber(mPlaySequence[m_nPlayPosition]);
		}
		else if (msg.pSender->GetName() == _T("btnPrevious"))
		{
			m_bPaused = true;
			CButtonUI *pBtn = static_cast<CButtonUI*>(m_pm.FindControl(L"btnPlay"));
			pBtn->SetVisible(true);
			pBtn = static_cast<CButtonUI*>(m_pm.FindControl(L"btnPause"));
			pBtn->SetVisible(false);

			m_nPlayPosition--;
			if (m_nPlayPosition < 0)
				m_nPlayPosition = 0;

			if (mPlaySequence.size()>0)
			    DrawPicByNumber(mPlaySequence[m_nPlayPosition]);
		}
	}

	//return CNotifyPump::NotifyPump(msg);
}

CDeskPicWnd::~CDeskPicWnd()
{
	gUnify->Core()->Timer()->StopTimer(this, 1);
	gUnify->Core()->Timer()->StopTimer(this, 2);
}

LRESULT CDeskPicWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	vector<TString> files;
	File::RecruseDirFiles(files, gUnify->App()->Path()->TempPath.c_str(), false, L"*.tmp");
	for (auto& item : files) {
		::DeleteFile(item.c_str());
	}
	
	CDialogBuilder builder;

	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	//	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	styleValue |= WS_EX_TOOLWINDOW; //不显示在任务栏
	::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

	m_pm.Init(m_hWnd);
	m_pm.AddResourcePath(gPlugInfo.Path.c_str()); //设置备用路径，Render取图片用到，duilib修改
	CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("Res\\PlugWndDeskPic.xml")).c_str(), (UINT)0, this, &m_pm);
	//CControlUI *pRoot = builder.Create(_T("D:\\[Work]\\desktop\\BeyondDesk\\bin\\Debug\\Res\\XML\\DlgAbout.xml"), (UINT)0, this, &m_pm);
	
	ASSERT(pRoot && "Failed to parse XML");

	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

	gUnify->Core()->Timer()->SetTimer(this, 2, 400, &CDeskPicWnd::OnTimer);

	return lRet;
}

void CDeskPicWnd::OnEvent(CEvent* e)
{
	
}

void CDeskPicWnd::OnContextMenu()
{
	CMenuWnd* pMenu = new CMenuWnd();
	DuiLib::CDuiPoint point;
	GetCursorPos(&point);

	System::TOSVer os = System::GetOSVer();
	if (os >= System::osWin7) {
	    if (os >= System::osWin8)
		    pMenu->Init(NULL, (gPlugInfo.Path + _T("Res\\MenuDeskPic8.xml")).c_str(), point, &m_pm, &m_CheckList, &m_VisbileList);
		else
			pMenu->Init(NULL, (gPlugInfo.Path + _T("Res\\MenuDeskPic7.xml")).c_str(), point, &m_pm, &m_CheckList, &m_VisbileList);
	}
	else
		pMenu->Init(NULL, (gPlugInfo.Path + _T("Res\\MenuDeskPic.xml")).c_str(), point, &m_pm, &m_CheckList, &m_VisbileList);
}


void CDeskPicWnd::DrawPicByNumber(int number) {
	if (mFilesList.size() == 0) return;

	while (!PathFileExists(mFilesList[number].c_str())) {
		number++;
		if (number >= mFilesList.size())
			number = 0;
	}
	

	m_sFileName = mFilesList[number];

	DrawFile();
	//InvalidateRect(m_hWnd, NULL, FALSE);
	//UpdateWindow(m_hWnd);
}

void CDeskPicWnd::DrawFile()
{
	CControlUI* pImg = static_cast<CControlUI*>(m_pm.FindControl(L"pnlPic"));
	if (!pImg)
	return;

	RECT rc;
	::GetWindowRect(m_pm.GetPaintWindow(), &rc);
	if (rc.right <= 0 && rc.bottom <= 0)
		return; //刚启动时，获取不到的

	if (!PathFileExists(m_sFileName.c_str())) return;

	//::DeleteFile(m_sTmpFile.c_str());

	Gdiplus::Bitmap* srcBmp = Bitmap::FromFile(m_sFileName.c_str());
	Gdiplus::Bitmap* pBmp = Util::Image::ScaleBitmapRatio(srcBmp, rc.right - rc.left - 2, rc.bottom - rc.top - 2);

	/*static TString sPath;
	sPath = gUnify->App()->Path()->TempPath;*/
	//srand((int)time(0));
	//TCHAR  pszName[MAX_PATH];
	//::GetTempFileName(sPath.c_str(), L"kingtime-desk-pic-", 0/*random(100)*/, pszName);
	//Util::Image::SaveBmpToPngFile(pBmp, pszName);
	pImg->SetBkImage(m_sFileName.c_str(), pBmp);
	//pImg->SetBkImage(pszName); //名字必须不同。。。

	//if (PathFileExists(sPath.c_str())) ::DeleteFile(sPath.c_str());
	//::DeleteFile(pszName);
	//m_sTmpFile = pszName;

	delete srcBmp;
	delete pBmp;

	//UpdateWindow(m_hWnd);
}

void CDeskPicWnd::Update() {
	TString sSection = GetDBSection();
	
	int count = mFilesList.size();
	if (count == 0) return;

	static int number = -1;
	if (gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), L"chkRandom", true)) {
		srand((int)time(0));
		number = random(count);
	}
	else {
		number++;
	}
	if (number >= count)
		number = 0;

	DrawPicByNumber(number);

	mPlaySequence.push_back(number);
	if (mPlaySequence.size() >= mFilesList.size()) {
		mPlaySequence.erase(mPlaySequence.begin());
	}
	m_nPlayPosition = mPlaySequence.size() - 1;
}

void CDeskPicWnd::ApplyCfg()
{
	__super::ApplyCfg();

	TString sSection = GetDBSection();

	int nTrans = 105 - m_nTransparent;
	nTrans = nTrans > 100 ? 100 : nTrans;
	nTrans = nTrans < 0 ? 0 : nTrans;

	m_pm.SetTransparent(int(nTrans*2.55));//1~255

	if (!m_bFullScreen) {
		LoadSize(800, 600);
	}

	//gUnify->Core()->Timer()->StopTimer(this, 1);
	//gUnify->Core()->Timer()->SetTimer(this, 2, m_nUpdateTime *100, &CWeatherGetter::OnUpdateTimer);
	CString s = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), L"edtInterval", L"");
	if (!s.IsEmpty()) {
		int n = String::StrToInt(s.GetBuffer());

		gUnify->Core()->Timer()->StopTimer(this, 1);
		//gUnify->Core()->Timer()->SetTimer(this, 2, m_nUpdateTime *100, &CWeatherGetter::OnUpdateTimer);
		gUnify->Core()->Timer()->SetTimer(this, 1, n * 1000, &CDeskPicWnd::OnTimer);
	}

	/*mThdHelper.Clear();
	mThdHelper.BeginThread(this, &CDeskPicWnd::ThdRecruseDirFiles);*/
	ThdRecruseDirFiles();

	Update();
}

void CDeskPicWnd::ThdRecruseDirFiles()
{
	try {
		TString sSection = GetDBSection();
		CString s = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), L"edtDir", L"");

		vector<TString> types;
		types.push_back(L"*.png");
		types.push_back(L"*.jpg");
		types.push_back(L"*.jpeg");
		types.push_back(L"*.bmp");
		vector<TString>().swap(mFilesList);
		File::RecruseDirFiles(mFilesList, s.GetBuffer(), gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), L"chkSubDir", true), types);

		//::PostMessage(*this, WM_RECRUSEFILES_DONE, 0, 0);
	}
	catch (...) {

	}
}

void CDeskPicWnd::OnTimer(void *pOwner, UINT nTimerID)
{
	//if paused return

	if (pOwner == this && nTimerID == 1)
	{
		if (!m_bPaused)
		    Update();
	}
	if (pOwner == this && nTimerID == 2) {
		SetPlayBarPos();

		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		CHorizontalLayoutUI* pPlay = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"pnlPlay"));

		POINT pt;
		GetCursorPos(&pt);
		if (PtInRect(&rcWnd, pt)) {
			pPlay->SetVisible(true);
		}
		else
			pPlay->SetVisible(false);
	}
}

void CDeskPicWnd::SetPlayBarPos()
{
	CDuiRect rcWnd;
	::GetWindowRect(*this, &rcWnd);

	CHorizontalLayoutUI* pPlay = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"pnlPlay"));
	RECT rc;
	rc.left = 0;
	rc.top = rcWnd.GetHeight() - 48;
	rc.right = rcWnd.GetWidth();
	//LOGCON("%d", rc.right);
	rc.bottom = rcWnd.GetHeight();
	pPlay->SetPos(rc);
}

LRESULT CDeskPicWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);

		//CDuiRect rcWindow, rcClient;
		//HRGN rgnWindow, rgnMinus, rgnAdd;
		// 
		//::GetWindowRect(*this, &rcWindow);
		//::GetClientRect(*this, &rcClient);
		////ClientToScreen(*this, &rcClient);
		//MapWindowRect(*this, HWND_DESKTOP, &rcClient);

		//int nRoundSize = szRoundCorner.cx;
		////rcClient.OffsetRect(-rcWindow.TopLeft());
		//rcClient.Offset(-rcWindow.left,-rcWindow.top);
		////1
		//rgnWindow = CreateRectRgn(rcClient.left, rcClient.top + nRoundSize, rcClient.right, rcClient.bottom - nRoundSize + 1);
		////2
		//rgnAdd = CreateRectRgn(rcClient.left + nRoundSize, rcClient.top, rcClient.right - nRoundSize, rcClient.top + (nRoundSize-1));
		//CombineRgn(rgnWindow, rgnWindow, rgnAdd, RGN_OR);
		////3
		//OffsetRgn(rgnAdd, 0, rcClient.GetHeight() - (nRoundSize-1));

		//CombineRgn(rgnWindow, rgnWindow, rgnAdd, RGN_OR);
		////4
		//SetRectRgn(rgnAdd, rcClient.left + (nRoundSize - 1), rcClient.top + (nRoundSize - 1), rcClient.right - (nRoundSize - 1), rcClient.top + nRoundSize);
		//
		//CombineRgn(rgnWindow, rgnWindow, rgnAdd, RGN_OR);
		////5
		//OffsetRgn(rgnAdd, 0, rcClient.GetHeight() - (nRoundSize + 1));
		//CombineRgn(rgnWindow, rgnWindow, rgnAdd, RGN_OR);

		//SetWindowRgn(*this, rgnWindow, true);

		//CDuiRect rect;
		//::GetWindowRect(*this, &rect);
		//HRGN hRgn;
		//int nHeight, nWidth, height, width;
		//nHeight = rect.bottom - rect.top; // 计算高度
		//nWidth = rect.right - rect.left; // 计算宽度
		//width = 2;//设置小角的宽度
		//height = 2;//设置小角的高度
		//POINT point[8] = {
		//		{ 0, height }, // left-left-top
		//		{ width, 0 }, // left-top-left
		//		{ nWidth - width, 0 },
		//		{ nWidth, height }, // right-top
		//		{ nWidth, nHeight - height },// right-bottom-right
		//		{ nWidth - width, nHeight }, // right-bottom-bottom
		//		{ width, nHeight }, // left-bottom
		//		{ 0, nHeight - height }
		//};
		//hRgn = CreatePolygonRgn(point, 8, WINDING);
		//SetWindowRgn(*this, hRgn, TRUE);
	}
#endif
	bHandled = FALSE;
	return 0;
}

LRESULT CDeskPicWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE; //传到到上层

	{
		/*if (uMsg != 15) {
			LOGCON("%d", uMsg);
		}*/
		
		switch (uMsg) {
			
		case WM_NCLBUTTONDBLCLK: {
			//LockWindowUpdate(m_hWnd);

			if (!m_bFullScreen) {
				//m_bOriginalTop = Window::isWndTopMost(*this);

				::GetWindowRect(*this, &m_OriginalRect);

				const RECT& rect = Window::GetMonitorRect(*this, MONITOR_DEFAULTTONEAREST);

				SetWindowPos(*this, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);

				m_bFullScreen = true;
				m_CheckList[_T("mnuFull")] = true;
			}
			else {
				/*if (m_bOriginalTop)
				Window::setWndTopMost(*this);
				else
				Window::setWndNoTopMost(*this);*/

				SetWindowPos(*this, NULL, m_OriginalRect.left, m_OriginalRect.top, m_OriginalRect.GetWidth(), m_OriginalRect.GetHeight(), SWP_SHOWWINDOW);

				m_bFullScreen = false;
				m_CheckList[_T("mnuFull")] = false;
			}

			DrawFile();

			//LockWindowUpdate(0);

			break;
		}
		case WM_RECRUSEFILES_DONE: {
			Update();

			break;
		}
		case WM_EXITSIZEMOVE:
		{
			//Update();
			SetPlayBarPos();

			

			break;
		}
		case WM_SIZE: {
			
			lRes = OnSize(uMsg, wParam, lParam, bHandled);
			break;
		}
		//case WM_PAINT: {

		//	PAINTSTRUCT ps;
		//	BeginPaint(m_hWnd, &ps);
		//	DrawFile();
		//	EndPaint(m_hWnd, &ps);
		//	/*bHandled = TRUE;*/
		//	break;
		//}
		case WM_MENUCLICK:
		{
			CDuiString *strMenuName = (CDuiString*)wParam;
			if (strMenuName->IsEmpty()) break;

			bool bChecked = (bool)lParam;

			if (*strMenuName == _T("mnuFull"))
			{
				if (bChecked) {
					//m_bOriginalTop = Window::isWndTopMost(*this);

					::GetWindowRect(*this, &m_OriginalRect);

					const RECT& rect = Window::GetMonitorRect(*this, MONITOR_DEFAULTTONEAREST);
				
					SetWindowPos(*this, NULL, rect.left, rect.top, rect.right-rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);

					m_bFullScreen = true;
				}
				else {
					/*if (m_bOriginalTop)
						Window::setWndTopMost(*this);
					else
						Window::setWndNoTopMost(*this);*/

					SetWindowPos(*this, NULL, m_OriginalRect.left, m_OriginalRect.top, m_OriginalRect.GetWidth(), m_OriginalRect.GetHeight(), SWP_SHOWWINDOW);

					m_bFullScreen = false;
				}

				DrawFile();

				bHandled = TRUE;

				delete strMenuName;
			}
			else if (*strMenuName == _T("mnuLocate"))
			{
				TString sFile = mFilesList[mPlaySequence[m_nPlayPosition]];
				File::ShellLocateFile(sFile);

				bHandled = TRUE;

				delete strMenuName;
			}
			/*mnuCenter" te
				mnuTile" text
				mnuStretch" t
				mnuKeepAspect
				mnuCropToFit"
				mnuSpan" text*/
			else if (*strMenuName == _T("mnuCenter"))
			{
				TString sFile = mFilesList[mPlaySequence[m_nPlayPosition]];
				System::SetMyWallpaper(sFile.c_str(), 0);

				bHandled = TRUE;

				delete strMenuName;
			}
			else if (*strMenuName == _T("mnuTile"))
			{
				TString sFile = mFilesList[mPlaySequence[m_nPlayPosition]];
				System::SetMyWallpaper(sFile.c_str(),1);

				bHandled = TRUE;

				delete strMenuName;
			}
			else if (*strMenuName == _T("mnuStretch"))
			{
				TString sFile = mFilesList[mPlaySequence[m_nPlayPosition]];
				System::SetMyWallpaper(sFile.c_str(), 2);

				bHandled = TRUE;

				delete strMenuName;
			}
			else if (*strMenuName == _T("mnuKeepAspect"))
			{
				TString sFile = mFilesList[mPlaySequence[m_nPlayPosition]];
				System::SetMyWallpaper(sFile.c_str(), 3);

				bHandled = TRUE;

				delete strMenuName;
			}
			else if (*strMenuName == _T("mnuCropToFit"))
			{
				TString sFile = mFilesList[mPlaySequence[m_nPlayPosition]];
				System::SetMyWallpaper(sFile.c_str(), 4);

				bHandled = TRUE;

				delete strMenuName;
			}
			else if (*strMenuName == _T("mnuSpan"))
			{
				TString sFile = mFilesList[mPlaySequence[m_nPlayPosition]];
				System::SetMyWallpaper(sFile.c_str(), 5);

				bHandled = TRUE;

				delete strMenuName;
			}

			break;
		}
		default:
			bHandled = FALSE;
		}
	}
	if (bHandled) return lRes;

	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

	return CDeskWnd::HandleMessage(uMsg, wParam, lParam);
}

void CDeskPicWnd::SaveStat()
{
	__super::SaveStat();

	TString sSection = GetDBSection();

	if (!m_bFullScreen) {
		SaveSize(); //分开写，有的桌面窗口，比如时钟等，不需要保存
	}
}

void CDeskPicWnd::LoadStat(LPCTSTR pstrSection)
{
	__super::LoadStat(pstrSection); //super会调用applyacfg，不用重新写了
}

void CDeskPicWnd::OnConfig()
{
	ShowConfigWnd<CConfigWnd>(L"桌面相框设置");
}



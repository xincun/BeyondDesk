
#include "stdafx.h"
#include "UITrayProxy.h"
#include "../MainFrame.h"
#include "Control/UIMenu.h"
#include "Reg/Reg.h"

#ifdef ENABLE_TRAY

void UITrayProxy::MinToTray()
{
	::SendMessage(m_pPaintManager->GetPaintWindow(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
	//this->ShowWindow(FALSE);
	::ShowWindow(m_pPaintManager->GetPaintWindow(), SW_HIDE);
	SetProcessWorkingSetSize(::GetCurrentProcess(), -1, -1);

	//TString s = gMain->App()->AppPath()->UserPath + _T(".FirstRun");
	//if (!Util::File::FileExist(s.c_str()))
	//{
	//	Util::File::CreateFile(s.c_str(), "", TRUE);
	//	CDuiString sHint = MAIN_TITLE;
	//	//sHint += CMultiLang::GetLangStr("IDS_LANG_9").c_str();
	//	gMain->App()->Tray()->ShowBalloonHint(_T(""), sHint);
	//}
}

UITrayProxy::UITrayProxy()
{
	
}

void UITrayProxy::Notify(TNotifyUI& msg)
{

	UIProxy::Notify(msg); //not forget
}


CString UITrayProxy::GetCurrentDT()
{
	CString sWeek;
	CTime Now = CTime::GetCurrentTime();
	int nWeek = Now.GetDayOfWeek();
	switch (nWeek) {
	case 0: sWeek = _T("六"); break;
	case 1: sWeek = _T("天"); break;
	case 2: sWeek = _T("一"); break;
	case 3: sWeek = _T("二"); break;
	case 4: sWeek = _T("三"); break;
	case 5: sWeek = _T("四"); break;
	case 6: sWeek = _T("五"); break;
	case 7: sWeek = _T("六"); break;
	}

	CString sNewDT;
	sNewDT.Format(_T("%d-%d-%d 星期%s"), Now.GetYear(), Now.GetMonth(), Now.GetDay(), sWeek);

	return sNewDT;
}

LRESULT UITrayProxy::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bHandled = false;
	TString sTip;

	
	switch (uMsg)
	{
	case WM_TRAYCALLBACK:
	{
		switch (UINT(wParam))
		{
		case WM_RBUTTONDOWN:
		{
			OnContextMenu();

			
			bHandled = TRUE;
			break;
		}
		case WM_MOUSEMOVE: {
			 sTip = GetTip();
			gUnify->App()->Tray()->SetTooltipText(sTip.c_str());

			break;
		}
		case WM_MOUSEHOVER: {
			
		}
		case WM_MOUSELEAVE: {
			
		}
		}
		break;
	}
	case WM_USER + 103:
		//托盘时钟回调
		OnContextMenu(-150);
		break;
	case WM_USER + 104:
		//托盘时钟回调
		sTip= GetTip();
		//gUnify->App()->Tray()->ShowBalloonHint(L"", sTip.c_str());
		
		break;
	case WM_DESTROY:
	{
		//_Tray.SetIcon(::LoadIcon(m_PaintManager->GetInstance(), MAKEINTRESOURCE(IDR_MAINFRAME)));
		gUnify->App()->Tray()->RemoveIcon();
		bHandled = TRUE;
	}
	case WM_SYSCOMMAND:
	{
		switch (UINT(wParam))
		{
		case SC_MINIMIZE:
		{
			MinToTray();
			bHandled = TRUE;
			
			break;
		}
		
		}
	}
	}

	return -1;
}

void UITrayProxy::OnContextMenu(int x)
{
	//自己取位置，弹出菜单窗口
	CMenuWnd* pMenu = new CMenuWnd();
	CDuiPoint point;
	GetCursorPos(&point);
	if (x < 0)
		point.x += x;
	//m_TextList[_T("mnuMain")] = _T("东西");
	
	//此处mute动态，不能在main->ApplyCfg里设，因为hotkey会动态改变，不走ApplyCfg
	bool bMute = gUnify->DB()->SysConfig()->GetBool(L"Stat", L"MuteMode", false);
	(*m_CheckList)[_T("mnuMute")] = bMute;
	pMenu->Init(NULL, _T("XML\\MenuTray.xml"), point, m_pPaintManager, m_CheckList, m_VisibleList, m_TextList);
}

TString UITrayProxy::GetTip()
{
	DWORD dwTick = GetTickCount();
	CTime Now = CTime::GetCurrentTime();
	CTime BootTime = Now - CTimeSpan(0, 0, 0, dwTick / 1000);

	TString sTick = Time::ConvertTime(dwTick / 1000);
	CString sBoot;
	CString s = BootTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	sBoot.Format(_T("开机时间：%s (%s)"), sTick.c_str(), s);

	const COleDateTime& dt = COleDateTime::GetCurrentTime();
	CLunar lunar;
	lunar.Lunar(dt);

	CString sLunar = Now.Format(_T("%H:%M:%S   ")) + GetCurrentDT() + lunar.m_cWordMonth + lunar.m_cWordDay;
	CString sFeast;
	if (!lunar.m_solarFestival.IsEmpty())
	{
		sFeast.Format(_T("%s"), lunar.m_solarFestival);
	}

	CString sTip;
	if (!sFeast.IsEmpty())
	sTip.Format(_T("%s\n%s\n%s\n%s"), sLunar, sFeast, sBoot, gUnify->App()->Status()->TodayWeatherDesc.c_str());
	else
		sTip.Format(_T("%s\n%s\n%s"), sLunar, sBoot, gUnify->App()->Status()->TodayWeatherDesc.c_str());
	return sTip.GetBuffer();
	//gUnify->App()->Tray()->SetTooltipText(L"对方是否\n水电费是否的方式");
	//gUnify->App()->Tray()->ShowBalloonHint(_T("想"), L"上厕所是否士大夫");

	   //         if (m_bMouseTracking) break;
	//TRACKMOUSEEVENT tme;
	//tme.cbSize = sizeof(TRACKMOUSEEVENT); //监控鼠标离开   
	//tme.dwFlags = TME_LEAVE;
	//tme.hwndTrack = this->m_hWnd;
	//if (::TrackMouseEvent(&tme))
	//{
	//	m_bMouseTracking = TRUE; //其他鼠标进入时的操作      
	//}
}

#endif
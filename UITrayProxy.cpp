
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
	case 0: sWeek = _T("��"); break;
	case 1: sWeek = _T("��"); break;
	case 2: sWeek = _T("һ"); break;
	case 3: sWeek = _T("��"); break;
	case 4: sWeek = _T("��"); break;
	case 5: sWeek = _T("��"); break;
	case 6: sWeek = _T("��"); break;
	case 7: sWeek = _T("��"); break;
	}

	CString sNewDT;
	sNewDT.Format(_T("%d-%d-%d ����%s"), Now.GetYear(), Now.GetMonth(), Now.GetDay(), sWeek);

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
		//����ʱ�ӻص�
		OnContextMenu(-150);
		break;
	case WM_USER + 104:
		//����ʱ�ӻص�
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
	//�Լ�ȡλ�ã������˵�����
	CMenuWnd* pMenu = new CMenuWnd();
	CDuiPoint point;
	GetCursorPos(&point);
	if (x < 0)
		point.x += x;
	//m_TextList[_T("mnuMain")] = _T("����");
	
	//�˴�mute��̬��������main->ApplyCfg���裬��Ϊhotkey�ᶯ̬�ı䣬����ApplyCfg
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
	sBoot.Format(_T("����ʱ�䣺%s (%s)"), sTick.c_str(), s);

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
	//gUnify->App()->Tray()->SetTooltipText(L"�Է��Ƿ�\nˮ����Ƿ�ķ�ʽ");
	//gUnify->App()->Tray()->ShowBalloonHint(_T("��"), L"�ϲ����Ƿ�ʿ���");

	   //         if (m_bMouseTracking) break;
	//TRACKMOUSEEVENT tme;
	//tme.cbSize = sizeof(TRACKMOUSEEVENT); //�������뿪   
	//tme.dwFlags = TME_LEAVE;
	//tme.hwndTrack = this->m_hWnd;
	//if (::TrackMouseEvent(&tme))
	//{
	//	m_bMouseTracking = TRUE; //����������ʱ�Ĳ���      
	//}
}

#endif
#include "stdafx.h"
#include "UIToday.h"
#include "Utils/datehelper.h"
#include "EventDefine.h"

CTodayUI::CTodayUI() : m_pPaintManager(NULL), m_pCal(NULL), m_sOldDT(_T(""))
{
	//CTime Now = CTime::GetCurrentTime();
	//m_SelDate.SetDateTime(Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond());
	gUnify->Core()->Event()->AddEventListen(EVENT_MAIN_SHOW_DONE, this, &CTodayUI::OnEvent);
}

CTodayUI::~CTodayUI()
{
	gUnify->Core()->Event()->RemoveEventListen(EVENT_MAIN_SHOW_DONE, this, &CTodayUI::OnEvent);
}

void CTodayUI::OnEvent(CEvent* p)
{
	SetFeastListUI();
}

void CTodayUI::InitWindow()
{
	//SetTodayHisUI();

	SetDayCtrls();

	gUnify->Core()->Timer()->SetTimer(this, 1, 5000, &CTodayUI::OnTimerDay);
}

void CTodayUI::Create(CPaintManagerUI* pManager)
{
	m_hWnd = pManager->GetPaintWindow();
	m_pPaintManager = pManager;
	m_pPaintManager->AddNotifier(this);
	m_pPaintManager->AddMessageFilter(this);

	CDialogBuilder Builder;
	m_pPaintManager->AddResourcePath(gPlugInfo.Path.c_str());
	CContainerUI* pCtrl = static_cast<CContainerUI*>(Builder.Create((gPlugInfo.Path + _T("UIToday.xml")).c_str(), _T("xml"), this, m_pPaintManager));
	this->Add(pCtrl);
};

void CTodayUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString s = msg.pSender->GetName();

	}
}

CControlUI* CTodayUI::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{

	return NULL;
}

LRESULT CTodayUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	

	return -1; //不要自己截取消息
}

LRESULT CTodayUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	bHandled = (HandleMessage(uMsg, wParam, lParam) == 0);

	return bHandled;
}

CString CTodayUI::GetCurrentDT()
{
	CString sWeek;
	CTime Now = CTime::GetCurrentTime();
	int nWeek = Now.GetDayOfWeek();
	switch (nWeek){
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

void CTodayUI::SetDayCtrls()
{
	m_sOldDT = GetCurrentDT();
	CString s;
	const COleDateTime& dt = COleDateTime::GetCurrentTime();

	CLabelUI* pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblWeek")));
	pLbl->SetText(GetCurrentDT().GetBuffer());

	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblDay")));
	s.Format(_T("%d"), dt.GetDay());
	pLbl->SetText(s.GetBuffer());
	s.ReleaseBuffer();

	
	CLunar lunar;
	lunar.Lunar(dt);

	CString sLunar = lunar.m_cWordMonth + lunar.m_cWordDay;
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(L"lblNL"));
	pLbl->SetText(sLunar.GetBuffer());

	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblGZ")));
	s.Format(_T("%s年 %s月"), lunar.m_cYear, lunar.m_cMonth);
	pLbl->SetText(s.GetBuffer());
	s.ReleaseBuffer();

	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblSX")));
	s.Format(_T("生肖：%s"), lunar.m_cAnimal);
	pLbl->SetText(s.GetBuffer());
	s.ReleaseBuffer();

	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblXZ")));
	s.Format(_T("%s"), lunar.m_Constellation);
	pLbl->SetText(s.GetBuffer());
	s.ReleaseBuffer();

	//CString m_lunarFestival; //农历节日
	//CString m_solarFestival; //公历节日
	//CString m_solarTerms;    //节气
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblGLJR")));
	if (!lunar.m_solarFestival.IsEmpty())
	{
		s.Format(_T("%s"), lunar.m_solarFestival);
		pLbl->SetText(s.GetBuffer());
		pLbl->SetToolTip(s.GetBuffer());
		s.ReleaseBuffer();
	}
	else
	{
		pLbl->SetVisible(false);
	}

	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblNLJR")));
	if (!lunar.m_lunarFestival.IsEmpty())
	{

		if (!lunar.m_solarTerms.IsEmpty())
			s.Format(_T("%s %s"), lunar.m_lunarFestival, lunar.m_solarTerms);
		else
			s.Format(_T("%s"), lunar.m_lunarFestival);
		pLbl->SetText(s.GetBuffer());
		pLbl->SetToolTip(s.GetBuffer());
		s.ReleaseBuffer();
	}
	else
	{
		pLbl->SetVisible(false);
	}

	SetFeastListUI();
}

void CTodayUI::SetFeastListUI()
{
	CContainerUI* pPnl = FindControl<CContainerUI>("pnlFeast");
	if (pPnl)
	{
		CLunar lunar;
		pPnl->RemoveAll();
		int n = pPnl->GetHeight();
		n = n / 23;

		int nCount = 0;

		for (int i = 0; i <= 365; ++i)
		{	
			if (nCount == n) break;

			COleDateTime& dt = COleDateTime::GetCurrentTime();
			dt += COleDateTimeSpan(i, 0, 0, 0);

			lunar.Lunar(dt);
			
			//CString m_lunarFestival; //农历节日
			//CString m_solarFestival; //公历节日
			//CString m_solarTerms;    //节气
			CString sJQ = lunar.m_solarTerms;
			CString sNL = lunar.m_lunarFestival;
			CString sGL = lunar.m_solarFestival;
			CString s;
			s.Format(_T("%d-%d %s %s %s"), dt.GetMonth(), dt.GetDay(), lunar.m_solarFestival, lunar.m_lunarFestival, lunar.m_solarTerms);

			if (sJQ.GetLength() > 0 || sNL.GetLength() > 0 || sGL.GetLength() > 0)
			{
				CLabelUI* pLbl = new CLabelUI;
				pLbl->SetFixedHeight(23);
				RECT rc = { 20, 2, 0, 0 };
				pLbl->SetTextPadding(rc);
				pLbl->SetAttribute(_T("endellipsis"), _T("true"));
				pLbl->SetText(s.GetBuffer());
				pLbl->SetToolTip(s.GetBuffer());
				s.ReleaseBuffer();

				pPnl->Add(pLbl);

				++nCount;
			}
		}
	}

	pPnl->Invalidate();
	pPnl->NeedUpdate();
	UpdateWindow(m_pPaintManager->GetPaintWindow());
}

void CTodayUI::OnTimerDay(void* pOwner, UINT uiTimerID)
{
	if (m_sOldDT != GetCurrentDT())
	{
		SetDayCtrls();

		gUnify->Core()->Event()->PostEvent(EVENT_NEW_DAY);
	}
}
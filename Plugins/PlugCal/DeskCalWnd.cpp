#include "stdafx.h"
#include "DeskCalWnd.h"
#include "EventDefine.h"
#include "DeskConfigWnd.h"
#include "DBManager.h"
#include "SchEditWnd.h"

void CDeskCalWnd::FreshCalSchList()
{
	GetDB()->Open((gUnify->App()->Path()->UserPath + L"CalSch.dat").c_str());

	CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
	const COleDateTime& dt = pCal->GetSelDateTime();

	CppSQLite3Query q = GetDB()->QueryYear(dt.GetYear());

	vector <DuiLib::CSchItem>().swap(m_CalSchList);

	while (!q.eof()) {
		DuiLib::CSchItem item;

		CString sDate = q.fieldValue(L"date");
		sDate.Replace(L"-", L"");
		item.Date = sDate.GetBuffer();
		item.RID = q.fieldValue(L"rid");
		item.Sch = q.fieldValue(L"sch");
		item.SID = q.fieldValue(L"uid");

		m_CalSchList.push_back(item);

		q.nextRow();
	}

	pCal->SetSchList(&m_CalSchList);
}

void CDeskCalWnd::OnCalMonthChange()
{
	FreshCalSchList();

	CLabelUI *pLbl = FindControl<CLabelUI>("lblDate");

	CString s;
	CLunar lunar;

	CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
	const COleDateTime& dt = pCal->GetSelDateTime();
	lunar.Lunar(dt);
	s.Format(_T("%d年%d月%d日  %s"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), lunar.m_Constellation);

	pLbl->SetText(s.GetBuffer());

}

void CDeskCalWnd::Notify(TNotifyUI& msg)
{
	/*wprintf(msg.sType.GetData());
	wprintf(L"\n");*/
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_DBCLICK) {
		CDuiString s = msg.pSender->GetName();
		if (s.Find(_T("CalendGroupName_")) == 0) {
			OnAddSch();

		}
	}
	else if (msg.sType == DUI_MSGTYPE_MENU) {
		int n = 1;
	}
	else  if (msg.sType == DUI_MSGTYPE_RBUTTONUP)
	{
		CDuiString s = msg.pSender->GetName();
		if (s.Find(_T("CalendGroupName_")) == 0) {

			OnCalMonthChange();

			CMenuWnd* pMenu = new CMenuWnd();
			DuiLib::CDuiPoint point;
			GetCursorPos(&point);
			pMenu->Init(NULL, (gPlugInfo.Path + _T("MenuDay.xml")).c_str(), point, &m_pm, NULL);
			//gMenuWnd = pMenu;
		}
	}
	else
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnFresh"))
		{

		}
		else if (msg.pSender->GetName() == _T("btnMore"))
		{

		}
		else if (msg.pSender->GetName() == _T("btnToday"))
		{

			GotoToday();

		}
		else if (msg.pSender->GetName() == _T("btnLastMon"))
		{

			CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
			//LOGCON(pCal->GetCurSelDateTime().GetData());
			pCal->GotoMonth(pCal->GetCurrentMonth() - 1);

			/*CComboUI* pCombYear = FindControl<CComboUI>("cmbYear");
			CComboUI* pCombMon = FindControl<CComboUI>("cmbMonth");

			if (pCombMon->GetCurSel() == 0)
			{
				if (pCombYear->GetCurSel() > 0)
					pCombYear->SelectItem(pCombYear->GetCurSel() - 1);
				pCombMon->SelectItem(11);
			}
			else
			{
				pCombMon->SelectItem(pCombMon->GetCurSel() - 1);
			}*/

			OnCalMonthChange();
		}
		else if (msg.pSender->GetName() == _T("btnNextMon"))
		{

			CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
			//LOGCON(pCal->GetCurSelDateTime().GetData());
			pCal->GotoMonth(pCal->GetCurrentMonth() + 1);

			/*CComboUI* pCombYear = FindControl<CComboUI>("cmbYear");
			CComboUI* pCombMon = FindControl<CComboUI>("cmbMonth");
			if (pCombMon->GetCurSel() == 11)
			{
				if (pCombYear->GetCurSel() < pCombYear->GetCount() - 1)
					pCombYear->SelectItem(pCombYear->GetCurSel() + 1);
				pCombMon->SelectItem(0);
			}
			else
			{
				pCombMon->SelectItem(pCombMon->GetCurSel() + 1);
			}*/

			OnCalMonthChange();
		}
		else {
			FreshCalDisplay();
		}
	}

	//return CNotifyPump::NotifyPump(msg);
}

void CDeskCalWnd::GotoToday()
{
	CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
	//LOGCON(pCal->GetCurSelDateTime().GetData());

	/*CComboUI* pCombYear = FindControl<CComboUI>("cmbYear");
	CComboUI* pCombMon = FindControl<CComboUI>("cmbMonth");
	CTime Now = CTime::GetCurrentTime();
	pCombYear->SelectItem(Now.GetYear() - 1900);
	pCombMon->SelectItem(Now.GetMonth() - 1);*/

	//m_SelDate.SetDateTime(Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond());

	pCal->GotoToday();

	OnCalMonthChange();
}

CDeskCalWnd::~CDeskCalWnd()
{
	
	gUnify->Core()->Event()->RemoveEventListen(EVENT_HOLIDAY_GET_DONE, this, &CDeskCalWnd::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_SCH_ADD, this, &CDeskCalWnd::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_SCH_UPDATE, this, &CDeskCalWnd::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_SCH_DELETE, this, &CDeskCalWnd::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_NEW_DAY, this, &CDeskCalWnd::OnEvent);
}

LRESULT CDeskCalWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDialogBuilder builder;

	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	//	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	styleValue |= WS_EX_TOOLWINDOW; //不显示在任务栏
	::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

	m_pm.Init(m_hWnd);
	m_pm.AddResourcePath(gPlugInfo.Path.c_str()); //设置备用路径，Render取图片用到，duilib修改
	CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("WndDeskCal.xml")).c_str(), (UINT)0, this, &m_pm);
	//CControlUI *pRoot = builder.Create(_T("D:\\[Work]\\desktop\\BeyondDesk\\bin\\Debug\\Res\\XML\\DlgAbout.xml"), (UINT)0, this, &m_pm);

	ASSERT(pRoot && "Failed to parse XML");

	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);



	LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

	m_pCal = static_cast<CCalendarUI*>(m_pm.FindControl(_T("Cal")));

	gUnify->Core()->Event()->AddEventListen(EVENT_HOLIDAY_GET_DONE, this, &CDeskCalWnd::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_SCH_ADD, this, &CDeskCalWnd::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_SCH_UPDATE, this, &CDeskCalWnd::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_SCH_DELETE, this, &CDeskCalWnd::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_NEW_DAY, this, &CDeskCalWnd::OnEvent);

	OnCalMonthChange();

	return lRet;
}


int CDeskCalWnd::GetTodaySchCount() {
	int nCount = 0;

	if (m_CalSchList.size() > 0) {
		int i = 0;
		CTime Now = CTime::GetCurrentTime();
		CString sDate;
		sDate.Format(_T("%.04d%.02d%.02d"), Now.GetYear(), Now.GetMonth(), Now.GetDay());
		for (auto iter = m_CalSchList.begin(); iter != m_CalSchList.end(); ++iter) {
			const CSchItem& item = *iter;
			//if (m_pSchList && std::find(m_pSchList->begin(), m_pSchList->end(), sDate.GetData()) != m_pSchList->end())
			if (_tcsicmp(item.Date.c_str(), sDate.GetBuffer()) == 0)
			{
				nCount++;
			}
		}
	}

	return nCount;
}

void CDeskCalWnd::UpdateTodaySchCount() {
	int nTotal = GetTodaySchCount();
	TString sCount = String::IntToStr(nTotal);
	
	/*CButtonUI *pBtn = static_cast<CButtonUI*>(gUnify-> FindControl(_T("btnTodaySch")));
	if (sCount == _T("0"))
	{
		CDuiString s = _T("无日程");
		pBtn->SetText(s);
	}
	else
	{
		CDuiString s;
		s.Format(_T("%s 个日程"), sCount.c_str());
		pBtn->SetText(s);
	}*/

	//vector<TString> list;
	//if (m_CalSchList.size() > 0) {
	//	int i = 0;
	//	CTime Now = CTime::GetCurrentTime();
	//	CString sDate;
	//	sDate.Format(_T("%.04d%.02d%.02d"), Now.GetYear(), Now.GetMonth(), Now.GetDay());
	//	for (auto iter = m_CalSchList.begin(); iter != m_CalSchList.end(); ++iter) {
	//		const CSchItem& item = *iter;
	//		//if (m_pSchList && std::find(m_pSchList->begin(), m_pSchList->end(), sDate.GetData()) != m_pSchList->end())
	//		if (_tcsicmp(item.Date.c_str(), sDate.GetBuffer()) == 0)
	//		{
	//			CString s;
	//			s.Format(L"%s", item.Sch.c_str());
	//			list.push_back(s.GetBuffer());
	//		}
	//	}
	//}
	//gUnify->Core()->Event()->SendEvent(EVENT_DESK_SCH_SCH_UPDATE, &list);
}

void CDeskCalWnd::OnEvent(CEvent* param)
{
	if (param->EventID == EVENT_NEW_DAY) {
		UpdateTodaySchCount();
		GotoToday();
	}
	else if (param->EventID == EVENT_HOLIDAY_GET_DONE)
	{
		while (!m_pCal) {
			App::IdleWait(1000);
		}
		m_pCal->SetHolidayList(&gHolidayGetter.m_HolidayList);
		m_pCal->SetWorkdayList(&gHolidayGetter.m_WorkdayList);
		FreshCalDisplay();


		UpdateTodaySchCount();
	}
	else if (param->EventID == EVENT_SCH_ADD)
	{
		/*TSchItem* pItem = (TSchItem*)param->pSender;

		GetDB()->InsertSch(pItem->SID.c_str(), pItem->RID.c_str(), pItem->Sch.c_str(), pItem->DateTimeStr.c_str());*/

		//get current month schs
		FreshCalSchList();

		const COleDateTime& dt = m_pCal->GetCurDateTime();
		m_pCal->GotoMonth(dt.GetMonth());

		UpdateTodaySchCount();
	}
	else if (param->EventID == EVENT_SCH_UPDATE)
	{
		/*TSchItem* pItem = (TSchItem*)param->pSender;

		GetDB()->UpdateSch(pItem->SID.c_str(), pItem->RID.c_str(), pItem->Sch.c_str(), pItem->DateTimeStr.c_str());*/

		//get current month schs
		FreshCalSchList();

		const COleDateTime& dt = m_pCal->GetCurDateTime();
		m_pCal->GotoMonth(dt.GetMonth());

		UpdateTodaySchCount();
	}
	else if (param->EventID == EVENT_SCH_DELETE)
	{
		/*CDuiString sid, rid;
		USES_CONVERSION;
		sid = A2T(param->wParam.c_str());
		rid = A2T(param->lParam.c_str());

		GetDB()->DeleteSch(sid.GetData());
		TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));

		pPlugInfo->GetPlugIntf()->OnInvoke(this, 2, rid.GetData());*/

		FreshCalSchList();

		const COleDateTime& dt = m_pCal->GetCurDateTime();
		m_pCal->GotoMonth(dt.GetMonth());

		UpdateTodaySchCount();
	}
}

void CDeskCalWnd::FreshCalDisplay()
{
	//m_pCal->GotoToday(); //强制刷新
	const COleDateTime& dt = m_pCal->GetCurDateTime();
	m_pCal->GotoMonth(dt.GetMonth());
}

//主界面也有一个，保证两边同步更新日程


CString CDeskCalWnd::GetCurrentDT()
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
	sNewDT.Format(_T("%d年%d月%d日 星期%s"), Now.GetYear(), Now.GetMonth(), Now.GetDay(), sWeek);

	return sNewDT;
}

void CDeskCalWnd::Update() {
	
}

void CDeskCalWnd::ApplyCfg()
{
	__super::ApplyCfg();

	LoadBack();

	UpdateWindow(m_hWnd);
}

void CDeskCalWnd::OnAddSch()
{
	TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));

	CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
	const COleDateTime& dt = pCal->GetSelDateTime();
	CString sDate;
	sDate.Format(_T("%.04d-%.02d-%.02d"), dt.GetYear(), dt.GetMonth(), dt.GetDay());

	pPlugInfo->GetPlugIntf()->OnInvoke(this, 1, sDate.GetBuffer()); //新增sid为空
}

LRESULT CDeskCalWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE; //传到到上层

	{
		switch (uMsg) {
		case WM_EXITSIZEMOVE:
		{
			Update();
			break;
		}
		case WM_RBUTTONUP: {
			LOGCON("rbuuton up.......................");
			break;
		}
		case WM_MENUCLICK:
		{
			CDuiString* sName = (CDuiString*)(wParam);

			if (_tcsicmp(sName->GetData(), _T("mnuAddSch")) == 0) {
				
				OnAddSch();
			}
			else
			if (_tcsicmp(sName->GetData(), _T("mnuEditSch")) == 0) {
				

				CSchEditWnd* pEditWnd = new CSchEditWnd(m_hWnd);
				if (pEditWnd == NULL) return -1;

				pEditWnd->SetSchList(&m_CalSchList);
				const COleDateTime& dt = m_pCal->GetSelDateTime();
				CString sDate;
				sDate.Format(_T("%.04d-%.02d-%.02d"), dt.GetYear(), dt.GetMonth(), dt.GetDay());
				pEditWnd->SetDate(sDate.GetBuffer());
				pEditWnd->Create(m_hWnd, _T("编辑日程"), UI_WNDSTYLE_DIALOG, NULL, 0, 0, 0, 0);
				//skin_changed_observer_.AddReceiver(pChatDialog);
				pEditWnd->CenterWindow();
				//pTaskEditWnd->InitControl();

				pEditWnd->ShowModal();
			}

			break;
		}
		
		default:
			bHandled = FALSE;
		}
	}
	if (bHandled) return lRes;

	//if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes; //否则会调用两次

	return CDeskWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDeskCalWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	bHandled = (HandleMessage(uMsg, wParam, lParam) == 0);

	return bHandled;
}

void CDeskCalWnd::SaveStat()
{
	__super::SaveStat();

	SaveSize(); //分开写，有的桌面窗口，比如时钟等，不需要保存大小
}

void CDeskCalWnd::LoadStat(LPCTSTR pstrSection)
{
	__super::LoadStat(pstrSection); //super会调用applyacfg，不用重新写了

	LoadSize(400, 360);
}

void CDeskCalWnd::OnConfig()
{
	ShowConfigWnd<CDeskConfigWnd>(L"桌面日历设置");
}

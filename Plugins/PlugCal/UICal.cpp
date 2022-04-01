#include "stdafx.h"
#include "UICal.h"
#include "Utils/datehelper.h"
#include "DBManager.h"
#include "SchEditWnd.h"


//DUI_BEGIN_MESSAGE_MAP(CCalUI, UIProxy)
//DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
//DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
//DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)
//DUI_ON_MSGTYPE(DUI_MSGTYPE_HEADERCLICK, OnHeaderClick)
//DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE, OnItemActivate)
//DUI_END_MESSAGE_MAP()

CCalUI::CCalUI() : m_pPaintManager(NULL), m_pCal(NULL)
{
	//CTime Now = CTime::GetCurrentTime();
	//m_SelDate.SetDateTime(Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond());
	gUnify->Core()->Event()->AddEventListen(EVENT_HOLIDAY_GET_DONE, this, &CCalUI::OnEvent); 
	gUnify->Core()->Event()->AddEventListen(EVENT_SCH_ADD, this, &CCalUI::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_SCH_UPDATE, this, &CCalUI::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_SCH_DELETE, this, &CCalUI::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_NEW_DAY, this, &CCalUI::OnEvent);

	GetDB()->Open((gUnify->App()->Path()->UserPath + L"CalSch.dat").c_str());
	GetDB()->TryCreate();
}

CCalUI::~CCalUI()
{
	gUnify->Core()->Event()->RemoveEventListen(EVENT_HOLIDAY_GET_DONE, this, &CCalUI::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_SCH_ADD, this, &CCalUI::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_SCH_UPDATE, this, &CCalUI::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_SCH_DELETE, this, &CCalUI::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_NEW_DAY, this, &CCalUI::OnEvent);

	GetDB()->Close();
}

void CCalUI::InitWindow()
{
	

	CPaintManagerUI*p = m_pPaintManager;

	m_pCal = static_cast<CCalendarUI*>(m_pPaintManager->FindControl(_T("Cal")));

	CTime Now = CTime::GetCurrentTime();

	CComboUI* pComb = FindControl<CComboUI>("cmbYear");
	CString sYear;
	for (int i = 1900; i < 2051; ++i)
	{
		sYear.Format(_T("%d年"), i);
		CListLabelElementUI* pItem = new CListLabelElementUI;
		pItem->SetText(sYear);
		pItem->SetTag(i);
		pItem->SetFixedHeight(23);
		pComb->Add(pItem);

		if (i == Now.GetYear())
		{
			pComb->SelectItem(i);
		}
	}

	pComb = FindControl<CComboUI>("cmbMonth");
	pComb->SelectItem(Now.GetMonth() - 1);

	OnCalMonthChange();
}

void CCalUI::Create(CPaintManagerUI* pManager)
{
	m_hWnd = pManager->GetPaintWindow();
	m_pPaintManager = pManager;
	m_pPaintManager->AddNotifier(this);
	m_pPaintManager->AddMessageFilter(this);

	CDialogBuilder Builder;
	m_pPaintManager->AddResourcePath(gPlugInfo.Path.c_str());
	CContainerUI* pCtrl = static_cast<CContainerUI*>(Builder.Create((gPlugInfo.Path + _T("UICalendar.xml")).c_str(), _T("xml"), this, m_pPaintManager));
	this->Add(pCtrl);

	//CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
	//pCal->SetOnEvent(std::bind(&CCalUI::OnCalEvent, this, std::placeholders::_1, std::placeholders::_2));
};

void CCalUI::OnCalEvent(LPCTSTR event, LPCTSTR param) {
	/*CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
	COleDateTime dt = pCal->GetSelDateTime();

	if (_tcsicmp(event, DUI_MSGTYPE_DBCLICK)==0) {
		TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
		pPlugInfo->GetPlugIntf()->OnInvoke(this, 1);
	}
	else if (_tcsicmp(event, DUI_MSGTYPE_RBUTTONDOWN) == 0) {
		LOGCON("rbuttondown");
		CMenuWnd* pMenu = new CMenuWnd();
		DuiLib::CDuiPoint point;
		GetCursorPos(&point);
		pMenu->Init(NULL, _T("XML\\MenuMainDrop.xml"), point, m_pPaintManager, NULL);
	}*/
}

//CMenuWnd* gMenuWnd = nullptr;

void CCalUI::Notify(TNotifyUI& msg)
{
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
		if (s.Find(_T("CalendGroupName_")) == 0)  {

			OnCalMonthChange();

			CMenuWnd* pMenu = new CMenuWnd();
			DuiLib::CDuiPoint point;
			GetCursorPos(&point);
			pMenu->Init(NULL, (gPlugInfo.Path + _T("MenuDay.xml")).c_str(), point, m_pPaintManager, NULL);
			//gMenuWnd = pMenu;
		}
	}
	else  if (msg.sType == DUI_MSGTYPE_MOUSEHOVER)
	{
		/*CDuiString s = msg.pSender->GetName();
		if (s.Find(_T("CalendGroupName_")) == 0) {
			TCalendarInfo* pTCalendarInfo = (TCalendarInfo*)msg.pSender->GetParent()->GetTag();*/
			
			//

		//}
	}
	else if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		/*CWaterEffectUI* pWater = FindControl<CWaterEffectUI>("water");
		RECT rc = pWater->GetPos();
		pWater->Init(IDB_LOGOFLIPPED);*/
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.pSender->GetName() == _T("cmbYear"))
		{
			CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
			CComboUI* pCombYear = FindControl<CComboUI>("cmbYear");
			pCal->GotoYear(pCombYear->GetItemAt(pCombYear->GetCurSel())->GetTag());
		}
		else if (msg.pSender->GetName() == _T("cmbMonth"))
		{
			CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
			CComboUI* pCombMon = FindControl<CComboUI>("cmbMonth");
			pCal->GotoMonth(pCombMon->GetItemAt(pCombMon->GetCurSel())->GetTag());
		}

		OnCalMonthChange();
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString s = msg.pSender->GetName();
		if (s.Find(_T("CalendGroupName_")) == 0) //项目选中
		{
			//CString sDate = msg.pSender->GetUserData();
			//COleDateTime dt = m_pCal->GetCurSelDateTime();
			//CLunar lunar;

			///*
			//COleDateTime dt;
			//dt.ParseDateTime(sDate + _T(" 00:00:00"));*/
			////m_SelDate = dt;
			//lunar.Lunar(dt);
			//CString s;
			//s.Format(_T("%d年%d月%d日 %s年 %s月 生肖：%s 星座：%s"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), lunar.m_cYear, lunar.m_cMonth, lunar.m_cAnimal, lunar.m_Constellation);

			//CLabelUI *pLbl = FindControl<CLabelUI>("lblCalDesc");
			//pLbl->SetText(s.GetBuffer());
			OnCalMonthChange();
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

			CComboUI* pCombYear = FindControl<CComboUI>("cmbYear");
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
			}

			OnCalMonthChange();
		}
		else if (msg.pSender->GetName() == _T("btnNextMon"))
		{

			CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
			//LOGCON(pCal->GetCurSelDateTime().GetData());
			pCal->GotoMonth(pCal->GetCurrentMonth() + 1);

			CComboUI* pCombYear = FindControl<CComboUI>("cmbYear");
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
			}

			OnCalMonthChange();
		}
		

	}
}

void CCalUI::GotoToday()
{
	CCalendarUI *pCal = FindControl<CCalendarUI>("Cal");
	//LOGCON(pCal->GetCurSelDateTime().GetData());

	CComboUI* pCombYear = FindControl<CComboUI>("cmbYear");
	CComboUI* pCombMon = FindControl<CComboUI>("cmbMonth");
	CTime Now = CTime::GetCurrentTime();
	pCombYear->SelectItem(Now.GetYear() - 1900);
	pCombMon->SelectItem(Now.GetMonth() - 1);

	//m_SelDate.SetDateTime(Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond());

	pCal->GotoToday();

	OnCalMonthChange();
}

CControlUI* CCalUI::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{

	return NULL;
}

LRESULT CCalUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_RBUTTONDOWN) {
		int n = 1;
	}
	if (uMsg == WM_MENUCLICK)
	{
		CDuiString* sName = (CDuiString*)(wParam);

		if (_tcsicmp(sName->GetData(), _T("mnuAddSch")) == 0) {
			OnAddSch();
		}
		if (_tcsicmp(sName->GetData(), _T("mnuEditSch")) == 0) {
			

			CSchEditWnd* pEditWnd = new CSchEditWnd(gUnify->App()->m_MainWnd);
			if (pEditWnd == NULL) return -1;

			pEditWnd->SetSchList(&m_CalSchList);
			const COleDateTime& dt = m_pCal->GetSelDateTime();
			CString sDate;
			sDate.Format(_T("%.04d-%.02d-%.02d"), dt.GetYear(), dt.GetMonth(), dt.GetDay());
			pEditWnd->SetDate(sDate.GetBuffer());
			pEditWnd->Create(gUnify->App()->m_MainWnd, _T("编辑日程"), UI_WNDSTYLE_DIALOG, NULL, 0, 0, 0, 0);
			//skin_changed_observer_.AddReceiver(pChatDialog);
			pEditWnd->CenterWindow();
			//pTaskEditWnd->InitControl();

			pEditWnd->ShowModal();
		}

		
	}
	
	return -1; //不要自己截取消息
}

LRESULT CCalUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	bHandled = (HandleMessage(uMsg, wParam, lParam) == 0);

	return bHandled;
}

LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	

	return 0;
}

void CCalUI::OnCalMonthChange()
{
	FreshCalSchList();

	CLabelUI *pLbl = FindControl<CLabelUI>("lblCalDesc");
	
	CString s;
	CLunar lunar;
	
	const COleDateTime& dt = m_pCal->GetSelDateTime();
	lunar.Lunar(dt);
	s.Format(_T("%d年%d月%d日 %s年 %s月 生肖：%s 星座：%s"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), lunar.m_cYear, lunar.m_cMonth, lunar.m_cAnimal, lunar.m_Constellation);

	pLbl->SetText(s.GetBuffer());
}

void CCalUI::OnItemActivate(TNotifyUI& msg)
{
	
}

void CCalUI::OnClick(TNotifyUI& msg)
{
	
}

void CCalUI::OnSelectChanged(TNotifyUI &msg)
{
	
}

void CCalUI::OnItemClick(TNotifyUI &msg)
{
	
}

void CCalUI::FreshCalSchList() {
	const COleDateTime& dt = m_pCal->GetSelDateTime();
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

	m_pCal->SetSchList(&m_CalSchList);


}

int CCalUI::GetTodaySchCount() {
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

void CCalUI::UpdateTodaySchCount() {
	int nTotal = GetTodaySchCount();
	TString sCount = String::IntToStr(nTotal);
	CButtonUI *pBtn = static_cast<CButtonUI*>(m_pPaintManager->FindControl(_T("btnTodaySch")));
	if (pBtn == nullptr) return;

	bool enabled = gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncCal"), false);

	if(!enabled)
	{
		pBtn->SetText(_T(""));
	}
	else {
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
		}
	}

	vector<TString> list;
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
				CString s;
				s.Format(L"%s", item.Sch.c_str());
				list.push_back(s.GetBuffer());
			}
		}
	}
	gUnify->Core()->Event()->SendEvent(EVENT_DESK_SCH_SCH_UPDATE, &list);
}

void CCalUI::SetEnabled(bool val)
{
	if (!val) {

		gUnify->Core()->Event()->PostEvent(EVENT_DESK_SCH_SCH_CLEAR);
	}
	else {

		UpdateTodaySchCount();
	}
}

void CCalUI::OnEvent(CEvent* param)
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
		//m_pCal->GotoToday(); //强制刷新
		const COleDateTime& dt = m_pCal->GetCurDateTime();
		m_pCal->GotoMonth(dt.GetMonth());

		UpdateTodaySchCount();
	}
	else if (param->EventID == EVENT_SCH_ADD)
	{
		TSchItem* pItem = (TSchItem*)param->pSender;

		GetDB()->InsertSch(pItem->SID.c_str(), pItem->RID.c_str(), pItem->Sch.c_str(), pItem->DateTimeStr.c_str());
		
		//get current month schs
		FreshCalSchList();

		const COleDateTime& dt = m_pCal->GetCurDateTime();
		m_pCal->GotoMonth(dt.GetMonth());

		UpdateTodaySchCount();
	}
	else if (param->EventID == EVENT_SCH_UPDATE)
	{
		TSchItem* pItem = (TSchItem*)param->pSender;

		GetDB()->UpdateSch(pItem->SID.c_str(), pItem->RID.c_str(), pItem->Sch.c_str(), pItem->DateTimeStr.c_str());

		//get current month schs
		FreshCalSchList();

		const COleDateTime& dt = m_pCal->GetCurDateTime();
		m_pCal->GotoMonth(dt.GetMonth());

		UpdateTodaySchCount();
	}
	else if (param->EventID == EVENT_SCH_DELETE)
	{
		CDuiString sid, rid;
		USES_CONVERSION;
		sid = A2T(param->wParam.c_str());
		rid = A2T(param->lParam.c_str());

		GetDB()->DeleteSch(sid.GetData());
		TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));

		pPlugInfo->GetPlugIntf()->OnInvoke(this, 2, rid.GetData());

		FreshCalSchList();

		const COleDateTime& dt = m_pCal->GetCurDateTime();
		m_pCal->GotoMonth(dt.GetMonth());

		UpdateTodaySchCount();
	}
}

void CCalUI::OnAddSch()
{
	TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));

	const COleDateTime& dt = m_pCal->GetSelDateTime();
	CString sDate;
	sDate.Format(_T("%.04d-%.02d-%.02d"), dt.GetYear(), dt.GetMonth(), dt.GetDay());

	pPlugInfo->GetPlugIntf()->OnInvoke(this, 1, sDate.GetBuffer()); //新增sid为空


}

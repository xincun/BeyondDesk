#include "stdafx.h"
#include "UIHis.h"
#include "Utils/datehelper.h"
#include "EventDefine.h"

CHisUI::CHisUI() : m_pPaintManager(NULL), m_pCal(NULL)
{
	//CTime Now = CTime::GetCurrentTime();
	//m_SelDate.SetDateTime(Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond());
	gUnify->Core()->Event()->AddEventListen(EVENT_MAIN_SHOW_DONE, this, &CHisUI::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_NEW_DAY, this, &CHisUI::OnEvent);
}

CHisUI::~CHisUI()
{
	gUnify->Core()->Event()->RemoveEventListen(EVENT_MAIN_SHOW_DONE, this, &CHisUI::OnEvent);
	gUnify->Core()->Event()->RemoveEventListen(EVENT_NEW_DAY, this, &CHisUI::OnEvent);
}

//在插件启用的情况下获取到今日历史数据
void CHisUI::SetTodayHisUI()
{
	TPlugInfo* pPlugInfo = &gPlugInfo;
	if (!pPlugInfo) return;

	IPlugBase* pHisPlug = pPlugInfo->GetPlugIntf();
	if (pHisPlug)
	{
		CTime Now = CTime::GetCurrentTime();
		CString sDate;
		sDate.Format(_T("%02d%02d"), Now.GetMonth(), Now.GetDay());

		TString sHis = pHisPlug->OnGet(NULL, 0, sDate.GetBuffer());

		CContainerUI* pPnlHis = FindControl<CContainerUI>("pnlTodayHis");
		if (pPnlHis)
		{
			//for (int i = 0; i < pPnlHis->GetCount(); ++i)
			{
				pPnlHis->RemoveAll();
			}

			vector<TString> vec;
			String::SplitString(sHis, _T("\r\n"), vec);

			int n = pPnlHis->GetHeight();
			n = n / 23;

			int i = 0;
			for (auto& item : vec)
			{
				i++;
				if (i > n) break;

				CLabelUI* pLbl = new CLabelUI;

				pLbl->SetFixedHeight(23);
				RECT rc = { 10, 2, 0, 0 };
				pLbl->SetTextPadding(rc);
				pLbl->SetText(item.c_str());

				pPnlHis->Add(pLbl);
			}
		}

		pPnlHis->Invalidate();
		pPnlHis->NeedUpdate();
		UpdateWindow(m_pPaintManager->GetPaintWindow());
	}
}

void CHisUI::OnEvent(CEvent* p)
{
	if (p->EventID == EVENT_NEW_DAY)
	{
//		LOGCON("新的天：历史插件");
	}
	SetTodayHisUI();
}

void CHisUI::InitWindow()
{
	//SetTodayHisUI();
}

void CHisUI::Create(CPaintManagerUI* pManager)
{
	m_hWnd = pManager->GetPaintWindow();
	m_pPaintManager = pManager;
	m_pPaintManager->AddNotifier(this);
	m_pPaintManager->AddMessageFilter(this);

	CDialogBuilder Builder;
	m_pPaintManager->AddResourcePath(gPlugInfo.Path.c_str());
	CContainerUI* pCtrl = static_cast<CContainerUI*>(Builder.Create((gPlugInfo.Path + _T("UIHistory.xml")).c_str(), _T("xml"), this, m_pPaintManager));
	this->Add(pCtrl);
};

void CHisUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		/*CWaterEffectUI* pWater = FindControl<CWaterEffectUI>("water");
		RECT rc = pWater->GetPos();
		pWater->Init(IDB_LOGOFLIPPED);*/
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString s = msg.pSender->GetName();
		
	 if (msg.pSender->GetName() == _T("btnMoreHis"))
	{
		IPlugBase* pHisPlug = gUnify->PlugManager()->GetPlugInfoByName(_T("History"))->GetPlugIntf(0);
		if (pHisPlug)
		{
			((IToolPlug*)pHisPlug)->OnClick(msg.pSender);
		}
	}

	}
}

CControlUI* CHisUI::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{

	return NULL;
}

LRESULT CHisUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	

	return -1; //不要自己截取消息
}

LRESULT CHisUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	bHandled = (HandleMessage(uMsg, wParam, lParam) == 0);

	return bHandled;
}


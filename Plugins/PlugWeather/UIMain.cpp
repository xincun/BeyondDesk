#include "stdafx.h"
#include "UIMain.h"
#include "Utils/datehelper.h"
#include "EventDefine.h"
#include "Utils/DateHelper.h"
#include "ConfigWnd.h"

CMainUI::CMainUI() : m_pPaintManager(NULL)
{
	//CTime Now = CTime::GetCurrentTime();
	//m_SelDate.SetDateTime(Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond());
	gUnify->Core()->Event()->AddEventListen(EVENT_CFG_FRESH, this, &CMainUI::OnEvent);
}

CMainUI::~CMainUI()
{
	gUnify->Core()->Event()->RemoveEventListen(EVENT_CFG_FRESH, this, &CMainUI::OnEvent);
}

void CMainUI::OnEvent(CEvent* p)
{
	if (p->EventID == EVENT_CFG_FRESH)
	{
		//this->SetVisible(gUnify->DB()->PlugDB()->GetBool(_T("UIMainWeather"), _T("Enabled"), true));
		this->SetVisible(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTodayWeather"), true));

		if (this->IsVisible())
		{
			WeaGetter.SetEnabled(false);
			WeaGetter.DoGetNow();
		}
	}
}

void CMainUI::InitWindow()
{
	ApplyCfg();
}

void CMainUI::Create(CPaintManagerUI* pManager)
{
	m_hWnd = pManager->GetPaintWindow();
	m_pPaintManager = pManager;
	m_pPaintManager->AddNotifier(this);
	m_pPaintManager->AddMessageFilter(this);

	CDialogBuilder Builder;
	m_pPaintManager->AddResourcePath(gPlugInfo.Path.c_str());
	CContainerUI* pCtrl = static_cast<CContainerUI*>(Builder.Create((gPlugInfo.Path + _T("Res\\UIMain.xml")).c_str(), _T("xml"), this, m_pPaintManager));
	this->Add(pCtrl);

	bool bEnabled = gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTodayWeather"), true);
	this->SetVisible(bEnabled);
	WeaGetter.SetEnabled(bEnabled);
};

void CMainUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnFresh"))
		{
			WeaGetter.DoGetNow(); //
		}
		else if (msg.pSender->GetName() == _T("btnMore"))
		{
			if (WeaGetter.m_bGettting) return;

			CWeaDetailWnd* pWnd = CWeaDetailWnd::GetWnd(m_hWnd);
			pWnd->SetWeatherGetter(&WeaGetter);
			pWnd->SetWeatherParser(&WeaParser);
			pWnd->SetDetailsShow();
			pWnd->CenterWindow();
			pWnd->ShowWindow();
		}
		else if (msg.pSender->GetName() == _T("btnSet"))
		{
			OnConfig();
			return;
		}
		else if (msg.pSender->GetName() == _T("btnClose"))
		{
			if (MessageBox(m_pPaintManager->GetPaintWindow(), _T("确定要关闭这里的天气显示吗？"), _T("提示"), MB_OKCANCEL) == IDOK)
			{
				WeaGetter.SetEnabled(false);
				this->SetVisible(false);

				gUnify->DB()->PlugDB()->SetBool(m_sSection.c_str(), _T("Enabled"),  false);
				gUnify->DB()->SysConfig()->SetBool(_T("Func"), _T("optFuncTodayWeather"), false);
			}
		}
	}


}

void CMainUI::OnConfig()
{
	CConfigWnd* pWnd = new CConfigWnd();

	pWnd->m_pMainUI = this;
	pWnd->SetDBSection(m_sSection); //为了其它的用，分离开设置
	pWnd->Create(m_hWnd, L"天气设置", UI_WNDSTYLE_DIALOG | WS_POPUP, WS_EX_APPWINDOW, 0, 0, 0);
	if (pWnd == NULL) return;

	pWnd->HideDeskAboutCtrls();
	pWnd->CenterWindow();
	pWnd->ShowModal();
}

CControlUI* CMainUI::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{

	return NULL;
}

LRESULT CMainUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	

	return -1; //不要自己截取消息
}

LRESULT CMainUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	bHandled = (HandleMessage(uMsg, wParam, lParam) == 0);

	return bHandled;
}

void CMainUI::ApplyCfg() {

	this->SetVisible(gUnify->DB()->SysConfig()->GetBool(_T("Func"), _T("optFuncTodayWeather"), true));

	TString sSection = m_sSection;

	//DWORD dwColor = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("FontColor"), RGB(0, 0, 0));
	DWORD dwColor =RGB(0, 0, 0);
	int R = (dwColor & 0xFF0000) >> 16;    //先取出表示颜色分量R，移位得到真实的颜色分量值, 跟Alpha通道有关
	int G = (dwColor & 0x00FF00) >> 8;
	int B = dwColor & 0x0000FF;
	dwColor = RGB(R, G, B);
	
	CLabelUI* pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblCity")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblCityWea")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl1")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl1Wea")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl1Temp")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl2")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl2Wea")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl2Temp")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl3")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl3Wea")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl3Temp")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl4")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl4Wea")));
	pLbl->SetTextColor(dwColor);
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lbl4Temp")));
	pLbl->SetTextColor(dwColor);

	WeaGetter.m_sProvience = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Prov"), _T("北京"));
	WeaGetter.m_sCity = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("City"), _T("北京"));
	WeaGetter.m_sRegion = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Region"), _T("朝阳区"));
	WeaGetter.SetUpdateTime(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("Fresh"), 30));
	WeaGetter.SetOnGetWeather(this, &CMainUI::OnGetWeather);
	WeaGetter.DoGet();

	UpdateWindow(m_hWnd);
};

bool CMainUI::OnGetWeather(void* pOwner, const CString& sWeaData)
{
	if (!WeaParser.DoParse(sWeaData)) return false; //解析失败

	CString s;
	CControlUI* pImg = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("imgWea")));
	s.Format(_T("Res\\%s"), WeaParser.Now[_T("Img")]);
	pImg->SetBkImage(s.GetBuffer());
	gUnify->App()->Status()->WeatherImage = gPlugInfo.Path + s.GetBuffer();
	gUnify->App()->Status()->WeatherTemperature = WeaParser.Now[_T("Temp")];
	gUnify->Core()->Event()->PostEvent(EVENT_WEATHER_UPDATE, this);
	/*pImg = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("imgTodayWea")));
	pImg->SetBkImage(s.GetBuffer());*/

	CLabelUI* pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblCity")));
	s.Format(_T("%s %s  (%s-%s)"), WeaParser.m_sUIRegion, WeaParser.Now[_T("Temp")],
		WeaParser.Now[_T("LowTemp")], WeaParser.Now[_T("HighTemp")]);
	pLbl->SetText(s.GetBuffer());
	gUnify->App()->Status()->TodayWeatherDesc = s.GetBuffer();
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblCityWea")));
	s.Format(_T("%s    %s"), WeaParser.Now[_T("Wea")], WeaParser.Now[_T("Wind")]);
	pLbl->SetText(s.GetBuffer());
	gUnify->App()->Status()->TodayWeatherDesc += L" " + s;

	/*pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblTodayWea")));
	s = WeaParser.Now[_T("Wea")];
	pLbl->SetText(s.GetBuffer());
	pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("lblTodayTemp")));
	s.Format(_T("%s~%s"), WeaParser.Now[_T("LowTemp")], WeaParser.Now[_T("HighTemp")]);
	pLbl->SetText(s.GetBuffer());*/

	static auto SetWeaCtrl = [&](int n){
		if (n <= 0) return;

		CDuiString sName;

		sName.Format(_T("lbl%d"), n);
		CLabelUI* pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(sName));
		pLbl->SetText(WeaParser.Future[n - 1][_T("Week")].GetBuffer());

		sName.Format(_T("lbl%dWea"), n);
		pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(sName));
		CString s = WeaParser.Future[n - 1][_T("Wea")];
		pLbl->SetText(s.GetBuffer());

		sName.Format(_T("lbl%dTemp"), n);
		pLbl = static_cast<CLabelUI*>(m_pPaintManager->FindControl(sName));
		s.Format(_T("%s~%s"), WeaParser.Future[n - 1][_T("LowTemp")], WeaParser.Future[n - 1][_T("HighTemp")]);
		pLbl->SetText(s.GetBuffer());

		sName.Format(_T("img%d"), n);
		CControlUI* pImg = static_cast<CLabelUI*>(m_pPaintManager->FindControl(sName));
		s.Format(_T("Res/%s"), WeaParser.Future[n - 1][_T("Img")]);
		pImg->SetBkImage(s.GetBuffer());
	};

	SetWeaCtrl(1);
	SetWeaCtrl(2);
	SetWeaCtrl(3);
	SetWeaCtrl(4);

	return true;
}
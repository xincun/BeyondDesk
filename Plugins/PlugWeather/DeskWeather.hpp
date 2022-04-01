#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"
#include "IniFile.h"
#include "WeatherGetter.h"
#include "WeatherParser.h"
#include "WeaDetail.h"

using namespace DuiLib;

class CDeskWeatherWnd : public CDeskWnd, public IDialogBuilderCallback, public INotifyUI/*, public CNotifyPump*/
{
	CWeatherGetter WeaGetter;
	CWeatherParser WeaParser;
public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CDeskWeatherWnd");
	};

	virtual UINT GetClassStyle() const
	{
		return CS_DBLCLKS;
	}

	virtual void  Notify(TNotifyUI& msg)
	{
		//LOGCON(msg.sType.GetData());
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (msg.pSender->GetName() == _T("btnFresh"))
			{
				WeaGetter.DoGetNow(); //
			}
			else if (msg.pSender->GetName() == _T("btnMore"))
			{
				CWeaDetailWnd* pWnd= CWeaDetailWnd::GetWnd(m_hWnd);
				pWnd->SetWeatherGetter(&WeaGetter);
				pWnd->SetWeatherParser(&WeaParser);
				pWnd->SetDetailsShow();
				pWnd->CenterWindow();
				pWnd->ShowWindow();
			}
		}

		//return CNotifyPump::NotifyPump(msg);
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = FALSE;

		{
			switch (uMsg) {
			case WM_CLOSE:
			{
				break;
			}
			case WM_NCLBUTTONDBLCLK: {
			

				break;
			}
			case WM_MENUCLICK:
			{
				CDuiString *strMenuName = (CDuiString*)wParam;
				bool bChecked = (bool)lParam;

				if (*strMenuName == _T("mnuFresh"))
				{
					WeaGetter.DoGetNow(); //

					bHandled = TRUE;

					delete strMenuName; //放到这里，因为父窗口不处理了
				}

				break;
			}

			default:
				bHandled = FALSE;
			}
		}
		if (bHandled) return lRes;

		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

		return __super::HandleMessage(uMsg, wParam, lParam);
	}

	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
	{
		return NULL;
	}

	CDeskWeatherWnd() {}

	~CDeskWeatherWnd()
	{
		

		
	};

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CDialogBuilder builder;

		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		//	styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		styleValue |= WS_EX_TOOLWINDOW; //不显示在任务栏
		::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

		m_pm.Init(m_hWnd);
		m_pm.AddResourcePath(gPlugInfo.Path.c_str()); //设置备用路径，Render取图片用到，duilib修改
		CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("Res\\PlugWndDeskWeather.xml")).c_str(), (UINT)0, this, &m_pm);
		//CControlUI *pRoot = builder.Create(_T("D:\\[Work]\\desktop\\KingTime\\bin\\Debug\\Res\\XML\\DlgAbout.xml"), (UINT)0, this, &m_pm);
		
		ASSERT(pRoot && "Failed to parse XML");

		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		
		LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

		return lRet;
	}

	virtual void ApplyCfg() {
		__super::ApplyCfg();

		TString sSection = GetDBSection();

		LoadBack();

		DWORD dwColor = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("FontColor"), RGB(255, 255, 255));
		int R = (dwColor & 0xFF0000) >> 16;    //先取出表示颜色分量R，移位得到真实的颜色分量值, 跟Alpha通道有关
		int G = (dwColor & 0x00FF00) >> 8;
		int B = dwColor & 0x0000FF;
		dwColor = RGB(R, G, B);

		CLabelUI* pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblCity")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblCityWea")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl1")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl1Wea")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl1Temp")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl2")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl2Wea")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl2Temp")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl3")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl3Wea")));
		pLbl->SetTextColor(dwColor);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbl3Temp")));
		pLbl->SetTextColor(dwColor);

		WeaGetter.m_sProvience = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Prov"), _T("北京"));
		WeaGetter.m_sCity = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("City"), _T("北京"));
		WeaGetter.m_sRegion = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Region"), _T("朝阳区"));
		WeaGetter.SetUpdateTime(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("Fresh"), 30));
		WeaGetter.SetOnGetWeather(this, &CDeskWeatherWnd::OnGetWeather);
		WeaGetter.DoGet();

		UpdateWindow(m_hWnd);
	};

	bool OnGetWeather(void* pOwner, const CString& sWeaData)
	{
		if (!WeaParser.DoParse(sWeaData)) return false; //解析失败

		CString s;
		CControlUI* pImg = static_cast<CControlUI*>(m_pm.FindControl(_T("imgWea")));
		s.Format(_T("Res/%s"), WeaParser.Now[_T("Img")]);
		pImg->SetBkImage(s.GetBuffer());
		/*pImg = static_cast<CControlUI*>(m_pm.FindControl(_T("imgTodayWea")));
		pImg->SetBkImage(s.GetBuffer());*/

		CLabelUI* pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblCity")));
		s.Format(_T("%s %s  (%s-%s)"), WeaParser.m_sUIRegion, WeaParser.Now[_T("Temp")], 
			WeaParser.Now[_T("LowTemp")], WeaParser.Now[_T("HighTemp")]);
		pLbl->SetText(s.GetBuffer());
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblCityWea")));
		s.Format(_T("%s    %s"), WeaParser.Now[_T("Wea")], WeaParser.Now[_T("Wind")]);
		pLbl->SetText(s.GetBuffer());
		
		/*pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblTodayWea")));
		s = WeaParser.Now[_T("Wea")];
		pLbl->SetText(s.GetBuffer());
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblTodayTemp")));
		s.Format(_T("%s~%s"), WeaParser.Now[_T("LowTemp")], WeaParser.Now[_T("HighTemp")]);
		pLbl->SetText(s.GetBuffer());*/

		 auto SetWeaCtrl = [&](int n){
			if (n <= 0) return;

			CDuiString sName;

			sName.Format(_T("lbl%d"), n);
			CLabelUI* pLbl = static_cast<CLabelUI*>(m_pm.FindControl(sName));
			pLbl->SetText(WeaParser.Future[n - 1][_T("Week")].GetBuffer());

			sName.Format(_T("lbl%dWea"), n);
			pLbl = static_cast<CLabelUI*>(m_pm.FindControl(sName));
			CString s = WeaParser.Future[n - 1][_T("Wea")];
			pLbl->SetText(s.GetBuffer());

			sName.Format(_T("lbl%dTemp"), n);
			pLbl = static_cast<CLabelUI*>(m_pm.FindControl(sName));
			s.Format(_T("%s~%s"), WeaParser.Future[n - 1][_T("LowTemp")], WeaParser.Future[n - 1][_T("HighTemp")]);
			pLbl->SetText(s.GetBuffer());

			sName.Format(_T("img%d"), n);
			CControlUI* pImg = static_cast<CLabelUI*>(m_pm.FindControl(sName));
			s.Format(_T("Res/%s"), WeaParser.Future[n - 1][_T("Img")]);
			pImg->SetBkImage(s.GetBuffer());
		};

		SetWeaCtrl(1); 
		SetWeaCtrl(2);
		SetWeaCtrl(3);

		return true;
	}

	virtual void OnConfig()
	{
		ShowConfigWnd<CConfigWnd>(L"桌面天气设置");
		
	}


	virtual void OnContextMenu()
	{
		CMenuWnd* pMenu = new CMenuWnd();
		DuiLib::CDuiPoint point;
		GetCursorPos(&point);

		pMenu->Init(NULL, (gPlugInfo.Path + _T("Res\\MenuDeskWea.xml")).c_str(), point, &m_pm, &m_CheckList, &m_VisbileList);
	}
	
};
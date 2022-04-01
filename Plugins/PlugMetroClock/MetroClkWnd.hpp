#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"
#include "IniFile.h"
#include "EventDefine.h"
#include "Event.h"

using namespace DuiLib;

class CMetroClkWnd : public CDeskWnd, public IDialogBuilderCallback/*, public INotifyUI*/
{
public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CMetroClkWnd");
	};

	/*virtual void    Notify(TNotifyUI& msg)
	{

	}*/

	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
	{
		return NULL;
	}

	CMetroClkWnd() {}

	~CMetroClkWnd()
	{
		

		gUnify->Core()->Timer()->StopTimer(this, 1);
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
		CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("Res\\PlugWndMetroClk.xml")).c_str(), (UINT)0, this, &m_pm);
		
		ASSERT(pRoot && "Failed to parse XML");

		m_pm.AttachDialog(pRoot);
		//m_pm.AddNotifier(this);
		
		gUnify->Core()->Timer()->SetTimer(this, 1, 1000, &CMetroClkWnd::OnTimer);
		
		LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

		SetTime();

		return lRet;
	}

	virtual void OnConfig()
	{
		ShowConfigWnd<CConfigWnd>(L"Metro时钟设置");
		
	}



	virtual void ApplyCfg() {
		__super::ApplyCfg();

		int nTrans = 105 - m_nTransparent;
		nTrans = nTrans > 100 ? 100 : nTrans;
		nTrans = nTrans < 0 ? 0 : nTrans;

		m_pm.SetTransparent(int(nTrans*2.55));//1~255
	};

	void OnTimer(void *pOwner, UINT nTimerID)
	{
		//LOGCON("on MetroClk timer...");

		if (nTimerID==1 && pOwner==this)
		SetTime();

	}

	void SetTime()
	{
		CControlUI* pHour = m_pm.FindControl(_T("ctrlHour"));
		CControlUI* pHour2 = m_pm.FindControl(_T("ctrlHour2"));
		CControlUI* pMin = m_pm.FindControl(_T("ctrlMin"));
		CControlUI* pMin2 = m_pm.FindControl(_T("ctrlMin2"));
		CDuiString s;
		CTime Now = CTime::GetCurrentTime();

		int nHour = Now.GetHour();
		int nHour1, nHour2;
		if (nHour > 9)
		{
			nHour1 = nHour / 10;
			nHour2 = nHour % 10;
		}
		else
		{
			nHour1 = 0;
			nHour2 = nHour;
		}
		s.Format(_T("Res/%dB.png"), nHour1);
		pHour->SetBkImage(s);
		s.Format(_T("Res/%dB.png"), nHour2);
		pHour2->SetBkImage(s);

		int nMin = Now.GetMinute();
		int nMin1, nMin2;
		if (nMin > 9)
		{
			nMin1 = nMin / 10;
			nMin2 = nMin % 10;
		}
		else
		{
			nMin1 = 0;
			nMin2 = nMin;
		}
		s.Format(_T("Res/%ds.png"), nMin1);
		pMin->SetBkImage(s);
		s.Format(_T("Res/%ds.png"), nMin2);
		pMin2->SetBkImage(s);

		CLabelUI* pDate = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblDate")));

		TCHAR* WeekList[7] = { _T("日"), _T("一"), _T("二"), _T("三"), _T("四"), _T("五"), _T("六")};
		
		s.Format(_T("%d年%d月%d日 星期%s"), Now.GetYear(), Now.GetMonth(), Now.GetDay(), WeekList[Now.GetDayOfWeek()-1]);
		pDate->SetText(s);
	}


	/*virtual void OnBeforeContextMenu() {
		m_VisbileList[_T("mnuSet")] = false;
	};*/
	
};
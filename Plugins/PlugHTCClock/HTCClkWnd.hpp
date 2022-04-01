#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"
#include "IniFile.h"

using namespace DuiLib;

class CHTCClkWnd : public CDeskWnd, public IDialogBuilderCallback/*, public INotifyUI*/
{
	int m_nHour = -1, m_nMin =-1;
public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CHTCClkWnd");
	};

	/*virtual void    Notify(TNotifyUI& msg)
	{

	}*/

	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
	{
		return NULL;
	}

	~CHTCClkWnd()
	{
		gUnify->Core()->Timer()->StopTimer(this, 1);
	}

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
		CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("Res\\PlugWndHTCClk.xml")).c_str(), (UINT)0, this, &m_pm);
		
		ASSERT(pRoot && "Failed to parse XML");

		m_pm.AttachDialog(pRoot);
		//m_pm.AddNotifier(this);

		gUnify->Core()->Timer()->SetTimer(this, 1, 1000, &CHTCClkWnd::OnTimer);
		
		LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

		SetTime();

		return lRet;
	}

	virtual void ApplyCfg() {
		__super::ApplyCfg();

		//TString sSection = GetDBSection();

		int nTrans = 105 - m_nTransparent;
		nTrans = nTrans > 100 ? 100 : nTrans;
		nTrans = nTrans < 0 ? 0 : nTrans;

		m_pm.SetTransparent(int(nTrans*2.55));//1~255
	};

	void OnTimer(void *pOwner, UINT nTimerID)
	{
		//LOGCON("on HTCClk timer...");

		if (pOwner == this && nTimerID == 1) {

			SetTime();
		}
	}

	virtual void OnConfig()
	{
		ShowConfigWnd<CConfigWnd>(L"HTC时钟设置");
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

		if (m_nHour != nHour) {
			m_nHour = nHour;

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
			s.Format(_T("Res/%d.png"), nHour1);
			pHour->SetBkImage(s);
			s.Format(_T("Res/%d.png"), nHour2);
			pHour2->SetBkImage(s);
		}

		int nMin = Now.GetMinute();

		if (m_nMin != nMin) {
			m_nMin = nMin;

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
			s.Format(_T("Res/%d.png"), nMin1);
			pMin->SetBkImage(s);
			s.Format(_T("Res/%d.png"), nMin2);
			pMin2->SetBkImage(s);
		}
	}


	/*virtual void OnBeforeContextMenu() {
		m_VisbileList[_T("mnuSet")] = false;
	};*/
	
};
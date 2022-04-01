#include "stdafx.h"
#include "DeskCountTimeWnd.h"
#include "EventDefine.h"


void CDeskCountTimeWnd::Notify(TNotifyUI& msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnFresh"))
		{

		}
		else if (msg.pSender->GetName() == _T("btnMore"))
		{

		}
	}

	//return CNotifyPump::NotifyPump(msg);
}

CDeskCountTimeWnd::~CDeskCountTimeWnd()
{
	
	gUnify->Core()->Timer()->StopTimer(this, 1);
}

LRESULT CDeskCountTimeWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDialogBuilder builder;

	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	//	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	styleValue |= WS_EX_TOOLWINDOW; //不显示在任务栏
	::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

	m_pm.Init(m_hWnd);
	m_pm.AddResourcePath(gPlugInfo.Path.c_str()); //设置备用路径，Render取图片用到，duilib修改
	CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("Res\\PlugWndDeskCountTime.xml")).c_str(), (UINT)0, this, &m_pm);
	//CControlUI *pRoot = builder.Create(_T("D:\\[Work]\\desktop\\BeyondDesk\\bin\\Debug\\Res\\XML\\DlgAbout.xml"), (UINT)0, this, &m_pm);

	ASSERT(pRoot && "Failed to parse XML");

	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

	gUnify->Core()->Timer()->SetTimer(this, 1, 1000, &CDeskCountTimeWnd::OnTimer);

	return lRet;
}

void CDeskCountTimeWnd::OnTimer(void *pOwner, UINT nTimerID)
{
	if (pOwner == this && nTimerID == 1)
	{
			Update();
	}
}

void CDeskCountTimeWnd::OnEvent(CEvent* e)
{
	
}


void CDeskCountTimeWnd::Update() {
	TString sSection = GetDBSection();

	CTime Now = CTime::GetCurrentTime();
	CString sDate = Now.Format(L"%Y-%m-%d %H:%M:%S");
	CTime EndDateTime = Util::Time::StrToTime(m_sDateEnd.GetBuffer());
	if (EndDateTime <= Now) {
		CLabelUI* pLbl = static_cast<CLabelUI*>(m_pm.FindControl(L"lblCount"));
		pLbl->SetText(L"已过时");
		return;
	}

	CTimeSpan  Span = EndDateTime - Now;
	UINT n = Span.GetTotalSeconds();
	TString s = Time::ConvertTime2(Span.GetTotalSeconds());
	CLabelUI* pLbl = static_cast<CLabelUI*>(m_pm.FindControl(L"lblCount"));
	pLbl->SetText(s.c_str());
}

void CDeskCountTimeWnd::ApplyCfg()
{
	__super::ApplyCfg();

	TString sSection = GetDBSection();

	LoadBack();

	auto GetRGBColor = [&](LPCTSTR szCtrlName)->DWORD {
		DWORD dwColor = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), szCtrlName, RGB(255, 255, 255));
		int R = (dwColor & 0xFF0000) >> 16;    //先取出表示颜色分量R，移位得到真实的颜色分量值, 跟Alpha通道有关
		int G = (dwColor & 0x00FF00) >> 8;
		int B = dwColor & 0x0000FF;
		dwColor = RGB(R, G, B);

		return dwColor;
	};

	DWORD dwColorTitle = GetRGBColor(_T("FontColorTitle"));
	DWORD dwColorCount = GetRGBColor(_T("m_pColorComboCount"));

	CLabelUI* pLbl;
	CHorizontalLayoutUI* pnl;
	pLbl = static_cast<CLabelUI*>(m_pm.FindControl(L"lblTitle"));
	CString sFormat = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), L"edtTitle", L"距离{t}剩余");

	CTime Now = CTime::GetCurrentTime();
	CString sDate = Now.Format(L"%Y-%m-%d %H:%M:%S");
	CTime EndDateTime = Util::Time::StrToTime(gUnify->DB()->PlugDB()->GetString(sSection.c_str(), L"DateEnd", sDate));
	sFormat.Replace(L"{t}", EndDateTime.Format(L"%Y-%m-%d %H:%M:%S"));
	pLbl->SetText(sFormat.GetBuffer());

	pnl = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"pnlTitle"));
	pnl->SetVisible(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), L"chkTitle", true));
	pLbl->SetTextColor(dwColorTitle);

	pLbl = static_cast<CLabelUI*>(m_pm.FindControl(L"lblCount"));
	pLbl->SetText(gUnify->DB()->PlugDB()->GetString(sSection.c_str(), L"lblCount", L"99").GetBuffer());
	pLbl->SetTextColor(dwColorCount);
	
	LoadSize(320, 160);

	m_sDateEnd = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), L"DateEnd", sDate);

	Update();

	UpdateWindow(m_hWnd);
}

LRESULT CDeskCountTimeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE; //传到到上层

	{
		switch (uMsg) {
		case WM_EXITSIZEMOVE:
		{
			//Update();

			break;
		}
		
		default:
			bHandled = FALSE;
		}
	}
	if (bHandled) return lRes;

	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

	return CDeskWnd::HandleMessage(uMsg, wParam, lParam);
}

void CDeskCountTimeWnd::SaveStat()
{
	__super::SaveStat();

	SaveSize(); //分开写，有的桌面窗口，比如时钟等，不需要保存
}

void CDeskCountTimeWnd::LoadStat(LPCTSTR pstrSection)
{
	__super::LoadStat(pstrSection); //super会调用applyacfg，不用重新写了


}



void CDeskCountTimeWnd::OnConfig()
{
	ShowConfigWnd<CConfigWnd>(L"桌面时间倒计时设置");
	
}

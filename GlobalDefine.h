#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__

#pragma once

using namespace std;

#define MAIN_TITLE  (_T("BeyondDesk")) //界面显示用，标题
#define MAIN_CLASS _T("UIMainFrame")
#define CORP_NAME _T("www.etctime.com") //公司名
#define APP_NAME  (_T("BeyondDesk")) //产品名，一般内部路径等用
#define APP_VER _T("1.1.7")

#define URL_FAQ  _T("http://wwww.etctime.com/faq.html")
#define HOLIDAY_URL_BASE  _T("http://etctime.com:8080/holiday/%d.dat")
#define WORKDAY_URL_BASE  _T("http://etctime.com:8080/holiday/%d_w.dat")
#define URL_HOME  _T("http://www.etctime.com")
#define URL_BUY  _T("http://www.etctime.com/buykingtime")
#define UP_INFO_URL_BASE  _T("http://etctime.com:8080/up/verbd.ini")

//////////////////////程序内嵌网页地址///////////////////
////////////////////////////////////////////////////////////自定义消息///////////////////////////

#define WM_MENU_TRAY_CLICK  WM_USER + 1026

template <typename T>
T * GetControl(CPaintManagerUI&m_PaintManager, LPCTSTR pszCtrlName)
{
	CControlUI* pUI = m_PaintManager->FindControl(pszCtrlName);
	if (pUI)
		return static_cast<T*>(pUI);
	return NULL;
};

template <typename T>
T * GetControl(CPaintManagerUI&m_PaintManager, LPCSTR pszCtrlName)
{
	USES_CONVERSION;

	CControlUI* pUI = m_PaintManager->FindControl(A2T(pszCtrlName));
	if (pUI)
		return static_cast<T*>(pUI);
	return NULL;
};

#endif // 


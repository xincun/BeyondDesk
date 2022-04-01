#include "stdafx.h"
#include "resource.h"
#include "WorldTimeWnd.h"
#include <shellapi.h>
#include <string>

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CWorldTimeWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CWorldTimeWnd::CWorldTimeWnd(HWND hMainWnd) : m_pHisObj(NULL)
{
	m_hMainWnd = hMainWnd;
}

CWorldTimeWnd::~CWorldTimeWnd(void)
{
	
}

void CWorldTimeWnd::OnFinalMessage( HWND hWnd)
{
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CWorldTimeWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CWorldTimeWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CWorldTimeWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("PlugWndWorldTime.xml")).c_str();
	//return _T("XML\\PlugWndWorldTime.xml");
}

UILIB_RESOURCETYPE CWorldTimeWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CWorldTimeWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CWorldTimeWnd::GetWindowClassName( void ) const
{
	return _T("UIWorldTime");
}

void CWorldTimeWnd::OnClick( TNotifyUI &msg )
{
	__super::OnClick(msg);
	
	 if (msg.pSender->GetName() == _T("btnQuery"))
	{
		CDuiString s = FindControl<CComboUI>("cmbZone")->GetItemAt(FindControl<CComboUI>("cmbZone")->GetCurSel())->GetText();

		float xShift = 0;
		int n1 = s.Find(_T("(GMT"));
		int n2 = s.Find(')', n1);
		if (n1 >= 0 && n2 > n1)
		{
			CDuiString sZone = s.Mid(n1 + 4, n2 - n1 - 4); // 结果格式：-12:00
			
			//可以用直接替换：00成空，：30成 .5 的算法代替，不再次写了，麻烦
			if (!sZone.IsEmpty())
			{
				int x1 = sZone.Find(':');
				if (x1 >= 0)
				{
					CDuiString s1 = sZone.Mid(0, x1);
					CDuiString s2 = sZone.Mid(x1 + 1, sZone.GetLength() - x1 - 1);
					
					if (s1.GetAt(1) == '0')
					{
						s1.Replace(_T("0"), _T(""));
					}

					int y1 = s1.ToInt();
					float y2 = s2.ToInt() / 60.0;

					if (sZone[0] == '-')
					xShift = y1 - y2;
					else
						xShift = y1 + y2;
				}
			}
		}

		MessageBox(NULL, GetTimeForZoneShift(xShift).GetBuffer(), _T("查询时间"), MB_OK);
	}
	 else if (msg.pSender->GetName() == _T("btnFresh"))
	 {
		 FillList();
	 }
}

CString CWorldTimeWnd::GetTimeForZoneShift(float xShift)
{
	SYSTEMTIME stUTC;
	::GetSystemTime(&stUTC);

	CTime tUTC(stUTC.wYear, stUTC.wMonth, stUTC.wDay, stUTC.wHour, stUTC.wMinute, stUTC.wSecond);

	int nHour = xShift / 1;
	int nMin = (xShift - nHour) * 60;
	CTime t = tUTC + CTimeSpan(0, nHour, nMin, 0);

	CString sTime;

	static CString WeekList[] = { _T("星期天"), _T("星期一"), _T("星期二"), _T("星期三"), _T("星期四"), _T("星期五"), _T("星期六") };

	sTime.Format(_T("%02d年%2d月%02d日 %s %02d:%02d"), 
		t.GetYear(), t.GetMonth(), t.GetDay(), 
		WeekList[t.GetDayOfWeek() - 1].GetBuffer(), 
		t.GetHour(), t.GetMinute());

	return sTime;
}

void CWorldTimeWnd::FillList()
{
	CListUI* pList = FindControl<CListUI>("lstZone");
	for (int i = 0; i < pList->GetCount(); i++)
	{
		CListContainerElementUI* pItem = static_cast<CListContainerElementUI*>(pList->GetItemAt(i));
	
		CDuiString s = pItem->GetUserData();
		float x = _ttof(s.GetData());

		pItem->GetItemAt(1)->SetText(GetTimeForZoneShift(x).GetBuffer());
	}
}

void CWorldTimeWnd::OnSelectChanged( TNotifyUI &msg )
{

}

void CWorldTimeWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CWorldTimeWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

void CWorldTimeWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);
	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE);

	FindControl<CComboUI>("cmbZone")->SelectItem(0);

	FillList();
}

LRESULT CWorldTimeWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	/*演示键盘消息的处理*/
	/*TCHAR press_char = (TCHAR)wParam;
	if(press_char == VK_BACK)
	{
	MessageBox(NULL, _T("按下了回退键"), _T("DUILIB DEMO"), MB_OK);
	}
	else
	{
	bHandled = FALSE;
	}*/
	return 0;
}

LRESULT CWorldTimeWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	//{
	//	return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	//}

	 return __super::HandleMessage(uMsg, wParam, lParam);;
}

void CWorldTimeWnd::OnItemSelect(TNotifyUI &msg)
{
	
}

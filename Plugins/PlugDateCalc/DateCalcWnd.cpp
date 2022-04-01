#include "stdafx.h"
#include "resource.h"
#include "DateCalcWnd.h"
#include <shellapi.h>
#include "Utils/DateHelper.h"
//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CDateCalcWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CDateCalcWnd::CDateCalcWnd(HWND hMainWnd) : m_pHisObj(NULL)
{
	m_hMainWnd = hMainWnd;
}

CDateCalcWnd::~CDateCalcWnd(void)
{
	
}

void CDateCalcWnd::OnFinalMessage( HWND hWnd)
{
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CDateCalcWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CDateCalcWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CDateCalcWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("PlugWndDateCalc.xml")).c_str();
	//return _T("XML\\PlugWndDateCalc.xml");
}

UILIB_RESOURCETYPE CDateCalcWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CDateCalcWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CDateCalcWnd::GetWindowClassName( void ) const
{
	return _T("UIDateCalcWnd");
}

void CDateCalcWnd::OnClick( TNotifyUI &msg )
{
	__super::OnClick(msg);
}

void CDateCalcWnd::CalcDate()
{
	if (FindControl<COptionUI>("optDays")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();
		CTime t2 = FindControl<CDateTimeUI>("dtDays")->GetTime();

		CTimeSpan t = t2 - t1;

		CDuiString s;
		s.Format(_T("%d年%d月%d日 和 %d年%d月%d日 之间相隔 %d 天"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			t2.GetYear(), t2.GetMonth(), t2.GetDay(),
			t.GetDays()
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
	else if (FindControl<COptionUI>("OptWork")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();
		CTime t2 = FindControl<CDateTimeUI>("dtWork")->GetTime();

		CDuiString s;
		s.Format(_T("%d年%d月%d日 和 %d年%d月%d日 之间有工作日(包含节假日) %d 天"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			t2.GetYear(), t2.GetMonth(), t2.GetDay(),
			CDateHelper::GetWorkDays(t1, t2)
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
	else if (FindControl<COptionUI>("optBack")->IsSelected())
	{
		
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();

		CDuiString sDay = FindControl<CSpinEditUI>("edtBack")->GetText();
		CTime t2 = t1 - CTimeSpan(sDay.ToInt(), 0, 0, 0);

		CDuiString s;
		s.Format(_T("%d年%d月%d日 向后跳转 %d 天 是：%d年%d月%d日"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			sDay.ToInt(),
			t2.GetYear(), t2.GetMonth(), t2.GetDay()
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
	else if (FindControl<COptionUI>("optForward")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();

		CDuiString sDay = FindControl<CSpinEditUI>("edtForward")->GetText();
		CTime t2 = t1 + CTimeSpan(sDay.ToInt(), 0, 0, 0);

		CDuiString s;
		s.Format(_T("%d年%d月%d日 向前跳转 %d 天 是：%d年%d月%d日"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			sDay.ToInt(),
			t2.GetYear(), t2.GetMonth(), t2.GetDay()
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
	else if (FindControl<COptionUI>("optWeekNum")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();

		CString sWeek =t1.Format(_T("%U"));

		CDuiString s;
		s.Format(_T("%d年%d月%d日 是该年的第 %s 周"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			sWeek.GetBuffer()
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
	else if (FindControl<COptionUI>("optDayNum")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();

		CString sWeek = t1.Format(_T("%j"));

		CDuiString s;
		s.Format(_T("%d年%d月%d日 是该年的第 %s 天"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			sWeek.GetBuffer()
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
	else if (FindControl<COptionUI>("optAnimal")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();
		CLunar l;
		COleDateTime t;
		t.SetDate(t1.GetYear(), t1.GetMonth(), t1.GetDay());
		l.Lunar(t);

		CDuiString s;
		s.Format(_T("%d年%d月%d日 的生肖是 %s"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			l.m_cAnimal.GetBuffer()
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	   
	}
	else if (FindControl<COptionUI>("optConste")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();

		CDuiString s;
		s.Format(_T("%d年%d月%d日 的星座是： %s"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			CDateHelper::GetConste(t1).GetBuffer()
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
	else if (FindControl<COptionUI>("optToLunar")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();

		CLunar l;
		COleDateTime t;
		t.SetDate(t1.GetYear(), t1.GetMonth(), t1.GetDay());
		l.Lunar(t);

		CDuiString s;
		s.Format(_T("%d年%d月%d日 转成农历是： %d年%d月%d日"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			l.m_lYear, l.m_lMonth, l.m_lDay
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
	else if (FindControl<COptionUI>("optToSolar")->IsSelected())
	{
		CTime t1 = FindControl<CDateTimeUI>("dtStart")->GetTime();

		CTime t2 = CDateHelper::GetSolar(t1.GetYear(), t1.GetMonth(), t1.GetDay());

		CDuiString s;
		s.Format(_T("%d年%d月%d日 转成公历是： %d年%d月%d日"), t1.GetYear(), t1.GetMonth(), t1.GetDay(),
			t2.GetYear(), t2.GetMonth(), t2.GetDay()
			);

		FindControl<CLabelUI>("lblStat")->SetText(s);
	}
}

void CDateCalcWnd::OnSelectChanged( TNotifyUI &msg )
{
	CalcDate();
}

void CDateCalcWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CDateCalcWnd::Notify(TNotifyUI &msg)
{
	//LOGCON(msg.sType.GetData());
	//LOGCON(msg.pSender->GetName().GetData());
	if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
	{
		
		if (msg.pSender->GetName() == _T("dtStart") ||
			msg.pSender->GetName() == _T("dtDays") ||
			msg.pSender->GetName() == _T("dtWork") ||
			msg.pSender->GetName() == _T("edtBack") ||
			msg.pSender->GetName() == _T("edtForward")
			)
		{
			CalcDate();
		}
	}

	return WindowImplBase::Notify(msg);
}

void CDateCalcWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);
	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE,FALSE);

	CalcDate();
}


LRESULT CDateCalcWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CDateCalcWnd::OnItemSelect( TNotifyUI &msg )
{

}

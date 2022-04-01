#pragma once

#include "stdafx.h"
#include <atlstr.h>
#include "AppPath.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"
#include "IniFile.h"
#include "EventDefine.h"
#include "ThreadHelper.h"

class CHolidayGetter
{
	ThreadHelper<CHolidayGetter> mThdHelper;
	bool m_bDone = false;
	
	void AddToHolidayList(int iYear, LPCTSTR pszFilePath);

	void AddToWorkdayList(int iYear, LPCTSTR pszFilePath);

	void DoGet();
public:
	vector<TString> m_HolidayList;
	vector<TString> m_WorkdayList; //特殊工作日

	//void OnEvent(CEvent* param)
	//{
	//	//CTime Now = CTime::GetCurrentTime();
	//	if (param->EventID == EVENT_HOLIDAY_GET_DONE)
	//	{
	//		gUnify->Core()->Event()->RemoveEventListen(EVENT_HOLIDAY_GET_DONE, this, &CHolidayGetter::OnEvent);

	//		m_bDone = true;
	//	}
	//}

	void GetHoliday();

	CHolidayGetter()
	{
		
	}

	~CHolidayGetter()
	{
		
	}
};

extern CHolidayGetter gHolidayGetter;

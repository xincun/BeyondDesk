#include "stdafx.h"
#include "ITasker.h"

CTime GetMatchWeekDateTime(const vector<TString> &pList, const ITasker* p, CTime &ADateStart, BOOL ANextMatch = FALSE) //取任务的周几列表中当前周几中下一个周几
{
	CTime rt(ADateStart.GetYear(), ADateStart.GetMonth(), ADateStart.GetDay(), 0, 0, 0);
	CTime StartTime = rt;
	int idx = ADateStart.GetDayOfWeek(); //天在周中索引，从周日开始计为1,到周六为7
	if (idx == 1)  idx = 7; else idx = idx - 1; //变为从周一开始计数

	/*int StartIndex = 0;
	if (ANextMatch)
	StartIndex = idx + 1;
	else
	StartIndex = idx;
	if (StartIndex >7) StartIndex = StartIndex - 7;*/

	for (int j = idx; j <= 7; j++)
	{
		if (find(pList.begin(), pList.end(), String::IntToStr(j).c_str()) != pList.end())  //找到下一个日期了
		{
			rt = rt + CTimeSpan(j - idx, 0, 0, 0);
			rt = rt + CTimeSpan(0, p->StartDateTime.GetHour(), p->StartDateTime.GetMinute(), 0);

			if (rt > CTime::GetCurrentTime())
				return rt;
			else
				rt = StartTime;
			//StrToDateTimeFmt(FormatDateTime('yyyy-mm-dd ', ADateStart + ) +
			//FormatDateTime('hh:nn', AItem.StartDateTime));
			//Result := j;
		}
	}

	for (int j = 1; j <= 7; j++)
	{
		if (find(pList.begin(), pList.end(), String::IntToStr(j).c_str()) != pList.end())//找到下一个日期了
		{
			rt = rt + CTimeSpan(7 - idx + j, 0, 0, 0);
			rt = rt + CTimeSpan(0, p->StartDateTime.GetHour(), p->StartDateTime.GetMinute(), 0);
			return rt;
			/*return StrToDateTimeFmt(FormatDateTime('yyyy-mm-dd ', ADateStart + 7 - idx + j) +
			FormatDateTime('hh:nn', AItem.StartDateTime));*/
		}
	}

	return NULL;
}

void ITasker::SetNextRunTime()
{
	CTime DTStart;
	CTime Now = CTime::GetCurrentTime();
	//if (StartDateTime < Now) StartDateTime = Now;

	switch (SchType)
	{
	case stEveryMin: //
	{
		if (StartDateTime > Now)
			DTStart = StartDateTime;
		else
		{	//DTStart = Now + CTimeSpan(0, 0, GetParamInt(_T("EveryMin")), 0);
			int n = GetParamInt(_T("EveryMin"));
			int i = 1;
			while (DTStart <= Now) //如果只判断<，可能会运行两次任务
			{
				DTStart = CTime(StartDateTime.GetYear(), StartDateTime.GetMonth(), StartDateTime.GetDay(), StartDateTime.GetHour(),
					StartDateTime.GetMinute(), 0) + CTimeSpan(0, 0, i * n, 0);
				i++;
			}
		}

		break;
	}

	case stEveryHour:
	{
		if (StartDateTime > Now)
			DTStart = StartDateTime;
		else
		{	//DTStart = Now + CTimeSpan(0, GetParamInt(_T("EveryHour")), 0, 0);
			int n = GetParamInt(_T("EveryHour"));
			int i = 1;
			while (DTStart <= Now)
			{
				DTStart = CTime(StartDateTime.GetYear(), StartDateTime.GetMonth(), StartDateTime.GetDay(), StartDateTime.GetHour(),
					StartDateTime.GetMinute(), 0) + CTimeSpan(0, i * n, 0, 0);
				i++;
			}
		}

		break;
	}
	case stEveryDay:
	{
		if (StartDateTime > Now)
			DTStart = StartDateTime;
		else
		{
			//CString s1, s2;
			//s1 = StartDateTime.Format("%H:%M");
			//s2 = Now.Format("%H:%M");
			//if (s1.Compare(s2) > 0) //开始时分>当前
			//	DTStart = Now + CTimeSpan(0, StartDateTime.GetHour(), StartDateTime.GetMinute(), 0);
			//else
			{
				int n = GetParamInt(_T("EveryDay"));
				int i = 1;
				while (DTStart <= Now)
				{
					DTStart = CTime(StartDateTime.GetYear(), StartDateTime.GetMonth(), StartDateTime.GetDay(), StartDateTime.GetHour(),
						StartDateTime.GetMinute(), 0) + CTimeSpan(i * n, 0, 0, 0);
					i++;
				}
			}
			//		if FormatDateTime('hh:nn', StartDateTime) > FormatDateTime('hh:nn', Now)  //开始时分>当前
			//DTStart = StrToDateTimeFmt(FormatDateTime('yyyy-mm-dd ', Now) + FormatDateTime('hh:nn', StartDateTime))
			//		else
			//DTStart = StrToDateTimeFmt(FormatDateTime('yyyy-mm-dd ', Now +  GetParamInt(_T("EveryDay"))) + FormatDateTime('hh:nn', StartDateTime));
		}

		break;
	}
	case stEveryWeek: //每周
	{
		BOOL bNextMatch = FALSE;

		if (StartDateTime > Now)
			DTStart = StartDateTime;
		else
		{ //设为今天+设定的开始时间
			//DecodeTime(StartDateTime, StartHour, StartMin, StartSec, StartMSec);
			//DecodeDate(Now, StartYear, StartMonth, StartDay);

			DTStart = CTime(Now.GetYear(), Now.GetMonth(), Now.GetDay(),
				StartDateTime.GetHour(), StartDateTime.GetMinute(), 0);
			//EncodeDate(StartYear, StartMonth, StartDay) + EncodeTime(StartHour, StartMin, StartSec, StartMSec);

			//如果时间《=当前。。。匹配下一个
			//if FormatDateTime('hh:nn', Now) >= FormatDateTime('hh:nn', StartDateTime) 
			CString s1, s2;
			s1 = Now.Format(L"%H:%M");
			s2 = StartDateTime.Format(L"%H:%M");
			if (s1.Compare(s2) >= 0)
				bNextMatch = TRUE;
		};

		//DTStart = CTime(Now.GetYear(), Now.GetMonth(), Now.GetDay(),
		//StartDateTime.GetHour(), StartDateTime.GetMinute(), 0);
		////StrToDateTimeFmt(FormatDateTime('yyyy-mm-dd ', Now) + FormatDateTime('hh:nn', StartDateTime))

		vector<TString> WeekList;
		String::SplitString(GetParam(_T("EveryWeek")).c_str(), ',', WeekList);

		//int idx = DTStart.GetDayOfWeek(); //天在周中索引，从周日开始计为1,到周六为7
		//if (idx == 1)  idx = 7; else idx = idx - 1; //变为从周一开始计数

		DTStart = GetMatchWeekDateTime(WeekList, this, DTStart, bNextMatch);

		break;
	}; //2
	case stEveryMonth: //每月
	{
		//////DecodeTime(StartDateTime, StartHour, StartMin, StartSec, StartMSec);
		if (GetParamInt(L"chkMonthLunar"))
		{
			CLunar l;
			COleDateTime t = COleDateTime::GetCurrentTime(); 
			l.Lunar(t);//当前农历

			int setDay = GetParamInt(_T("EveryMonthDay"));

			int startDay = setDay;
			int startMonth = l.m_lMonth;;
			int startYear = l.m_lYear;

			if (l.m_lDay > setDay) //当前农历日超过设置的农历日，延到下一月
			{
				startMonth += 1;
				if (startMonth > 12)
				{
					startMonth = 1;
					startYear += 1;
				}
			}

		GetMonthTypeLunar:
			CTime solar = CDateHelper::GetSolar(startYear, startMonth, startDay);

			int StartHour = StartDateTime.GetHour();
			int StartMin = StartDateTime.GetMinute();
			int StartSec = StartDateTime.GetSecond();
			int StartYear = solar.GetYear();
			int StartMonth = solar.GetMonth();
			int StartDay = solar.GetDay();

			DTStart = CTime(StartYear, StartMonth, StartDay, StartHour, StartMin, 0);

			while (DTStart < Now)
			{
				startMonth += 1;
				goto GetMonthTypeLunar;
			};
		}
		else 
		{
			int StartHour = StartDateTime.GetHour();
			int StartMin = StartDateTime.GetMinute();
			int StartSec = StartDateTime.GetSecond();
			//计算的时候，开始年，肯定是今年，
			int  StartYear = StartDateTime.GetYear();
			if (StartYear < Now.GetYear())
				StartYear = Now.GetYear();

			int  StartMonth = StartDateTime.GetMonth();
			int  StartDay = StartDateTime.GetDay();

			if (StartDay > GetParamInt(_T("EveryMonthDay")))
			{
				StartDay = GetParamInt(_T("EveryMonthDay"));
				StartMonth = StartMonth + 1;

				if (StartMonth > 12)
				{
					StartMonth = 1;
					StartYear += 1;
				}
			}
			else if (StartDay == GetParamInt(_T("EveryMonthDay")))
			{

			}
			else // <
			{
				StartDay = GetParamInt(_T("EveryMonthDay"));
			};

			//while EncodeDate(StartYear, StartMonth, StartDay) + EncodeTime(StartHour, StartMin, StartSec, StartMSec)< Now do
			while (CTime(StartYear, StartMonth, StartDay, StartHour, StartMin, StartSec) < Now)
			{
				StartMonth = StartMonth + 1;
				if (StartMonth > 12)
				{
					StartMonth = 1;
					StartYear += 1;
				}
			};

			DTStart = CTime(StartYear, StartMonth, StartDay, StartHour, StartMin, StartSec);
		}

		break;
		//DTStart = EncodeDate(StartYear, StartMonth, StartDay) + EncodeTime(StartHour, StartMin, StartSec, StartMSec);
	};
	case stEveryYear: //每年
	{
		//DecodeTime(StartDateTime, StartHour, StartMin, StartSec, StartMSec);
		if (GetParamInt(L"chkYearLunar"))
		{
			CLunar l;
			COleDateTime t = COleDateTime::GetCurrentTime();
			l.Lunar(t);//当前农历


			int setMonth = GetParamInt(_T("EveryYearMonth"));
			int setDay = GetParamInt(_T("EveryYearDay"));

			int startDay = setDay;
			int startMonth = setMonth;
			int startYear = l.m_lYear;

			if (l.m_lMonth > setMonth) //当前农历月超过设置的农历月，则到下一年去了
			{
				startYear = startYear + 1;
			}
			else if (l.m_lMonth == setMonth) //同一个农历月，则判断日期
			{
				if (l.m_lDay > setDay) //当前农历日超过设置的农历日，延到下一年
				{
					startYear = startYear + 1;
				}
			}

GetYearTypeLunar:
			CTime solar = CDateHelper::GetSolar(startYear, startMonth, startDay);

			int StartHour = StartDateTime.GetHour();
			int StartMin = StartDateTime.GetMinute();
			int StartSec = StartDateTime.GetSecond();
			int StartYear = solar.GetYear();
			int StartMonth = solar.GetMonth();
			int StartDay = solar.GetDay();

			DTStart = CTime(StartYear, StartMonth, StartDay, StartHour, StartMin, 0);

			while (DTStart < Now)
			{
				startYear += 1;
				goto GetYearTypeLunar;
			};
		}
		else 
		{
			int StartHour = StartDateTime.GetHour();
			int StartMin = StartDateTime.GetMinute();
			int StartSec = StartDateTime.GetSecond();
			//计算的时候，开始年，肯定是今年，
			int  StartYear = StartDateTime.GetYear();
			if (StartYear < Now.GetYear())
				StartYear = Now.GetYear();

			int  StartMonth = StartDateTime.GetMonth();
			int  StartDay = StartDateTime.GetDay();

			if (CTime(Now.GetYear(), StartMonth, StartDay, 0, 0, 0) >
				CTime(Now.GetYear(), GetParamInt(_T("EveryYearMonth")), GetParamInt(_T("EveryYearDay")), 0, 0, 0))  //比较月日
			{
				StartMonth = GetParamInt(_T("EveryYearMonth"));
				StartDay = GetParamInt(_T("EveryYearDay"));

				StartYear = StartYear + 1;
			}
			else
			{
				StartMonth = GetParamInt(_T("EveryYearMonth"));
				StartDay = GetParamInt(_T("EveryYearDay"));
			};

			//if EncodeDate(StartYear, StartMonth, StartDay) + EncodeTime(StartHour, StartMin, StartSec, StartMSec)
			if (CTime(StartYear, StartMonth, StartDay, StartHour, StartMin, StartSec) < Now)
			{
				StartYear = StartYear + 1;
			};

			DTStart = CTime(StartYear, StartMonth, StartDay, StartHour, StartMin, 0);
		}

		break;
		//DTStart = EncodeDate(StartYear, StartMonth, StartDay) + EncodeTime(StartHour, StartMin, StartSec, StartMSec);
	};
	case stOnce:
	{
		/*农历日期
		if StrToBool(item.SubItems[41])
		NextRunTime = DateToStr(gDateMgr.cnDateToDate(StrToDateFmt(item.SubItems[7]))) + ' ' + item.SubItems[8]
		else*/
		DTStart = StartDateTime;
		if (DTStart < Now)  //启动加载任务后，如果过期，一次性失效
			Stopped = TRUE;
		break;
	};
	case stCountDown: //对于倒计时和开机延时任务，没有时间段范围
	{
		if (StartDateTime > Now)
			DTStart = StartDateTime;
		else
			DTStart = Now;

		
		UINT sec = Now.GetSecond() + GetParamInt(_T("CountDownHour")) * 3600 + GetParamInt(_T("CountDownMin")) * 60 +
			GetParamInt(_T("CountDownSec"));

		//DTStart = StartDateTime + CTimeSpan(0, 0, 0, sec);

		DTStart = CTime(DTStart.GetYear(), DTStart.GetMonth(), DTStart.GetDay(), DTStart.GetHour(),
			DTStart.GetMinute(), 0) + CTimeSpan(0, 0, 0, sec);

		break;;
	};
	case stZero:
	{
		UINT nDay = Now.GetDay();
		UINT nHour = Now.GetHour();
		UINT nMin = Now.GetMinute();

		if (GetParamInt(_T("chkZeroHalf")) == 1 && GetParamInt(_T("chkZeroFull")) == 1)
		{
			if (nMin >= 0 && nMin < 30)
			{
				DTStart = Now + CTimeSpan(0, 0, 30 - nMin, 0);
			}
			else if (nMin >= 30 && nMin <= 59)
			{
				DTStart = Now + CTimeSpan(0, 0, 60 - nMin, 0);
			}
		}
		else if (GetParamInt(_T("chkZeroHalf")) == 1)
		{
			if (nMin >= 0 && nMin < 30)
			{
				DTStart = Now + CTimeSpan(0, 0, 30 - nMin, 0);
			}
			else if (nMin >= 30 && nMin <= 59)
			{
				DTStart = Now + CTimeSpan(0, 0, 90 - nMin, 0);
			}
		}
		else if (GetParamInt(_T("chkZeroFull")) == 1)
		{
			DTStart = Now + CTimeSpan(0, 0, 60 - nMin, 0);
		}

		break;
	}
	case stIdle:
	{
		break;
	}
	
	};

	switch (SchType)
	{
	case stCountDown: //倒计时需要秒
	{
		NextRunDateTime = DTStart;
		NextRunTime = NextRunDateTime.Format(L"%Y-%m-%d %H:%M:%S");//FormatDateTime('yyyy-mm-dd hh:nn:ss', NextRunDateTime);

		break;
	}
	case stIdle:
	{
		NextRunDateTime = Now + CTimeSpan(3650, 0, 0, 0);//反正比现在大
		NextRunTime = _T("下次空闲");

		break;
	}
	case stCpu:
	{
		NextRunDateTime = Now + CTimeSpan(3650, 0, 0, 0);//反正比现在大
		NextRunTime = _T("");

		break;
	}
	case stNetSpd:
	{
		NextRunDateTime = Now + CTimeSpan(3650, 0, 0, 0);//反正比现在大
		NextRunTime = _T("");

		break;
	}
	case stMem:
	{
		NextRunDateTime = Now + CTimeSpan(3650, 0, 0, 0);//反正比现在大
		NextRunTime = _T("");

		break;
	}
	case stBoot:
	{
		UINT sec = GetParamInt(_T("CountDownHour")) * 3600 + GetParamInt(_T("CountDownMin")) * 60 +
			GetParamInt(_T("CountDownSec"));

		UINT nBootSec = GetTickCount() / 1000;
		LOGCON("Sec=%d,BootSec=%d", sec, nBootSec);
		if (nBootSec < sec)
		{
			DTStart = Now + CTimeSpan(0, 0, 0, sec - nBootSec);

			NextRunDateTime = DTStart;
			NextRunTime = NextRunDateTime.Format(L"%Y-%m-%d %H:%M:%S");
			LOGCONW(L"NextRunTime=%s", NextRunTime.GetBuffer());
		}
		else
		{
			NextRunDateTime = Now + CTimeSpan(3650,0,0,0);//反正比现在大

			NextRunTime = _T("下次启动后");
			LOGCONW(L"NextRunTime=%s", NextRunTime);
		}

		break;
	}
	case stRestore:
	{
		UINT sec = GetParamInt(_T("CountDownHour")) * 3600 + GetParamInt(_T("CountDownMin")) * 60 +
			GetParamInt(_T("CountDownSec"));

		INT64 nBootSec = 9999999999999999;

		if (gUnify->App()->Status()->ResumeFromHibernateTime != 9999999999999999)
			nBootSec = gUnify->App()->Status()->ResumeFromHibernateTime / 1000;
		else if (gUnify->App()->Status()->ResumeFromHibernateTime != 9999999999999999)
			nBootSec = gUnify->App()->Status()->ResumeFromStandbyTime / 1000;

		if (nBootSec < sec)
		{
			DTStart = Now + CTimeSpan(0, 0, 0, sec - nBootSec);

			NextRunDateTime = DTStart;
			NextRunTime = NextRunDateTime.Format(L"%Y-%m-%d %H:%M:%S");
		}
		else
		{
			NextRunDateTime = Now + CTimeSpan(3650, 0, 0, 0);//反正比现在大

			NextRunTime = _T("下次恢复后");
		}

		break;
	}
	default:
	{
		DTStart = Time::ZeroTimeSecond(DTStart);
		NextRunDateTime = DTStart;//DateTimeFromZeroSecond(DTStart);
		NextRunTime = NextRunDateTime.Format(L"%Y-%m-%d %H:%M:%S");//FormatDateTime('yyyy-mm-dd hh:nn', NextRunDateTime);
	}
	}

	///MessageBox(0, p->NextRunTime, "", MB_OK);
}


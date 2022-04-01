#pragma once


//��������С��صģ��ṩ������

enum CScheduleType {
	stUnknown = -1,
	stEveryMin = 0, stEveryHour, stEveryDay, stEveryWeek, stEveryMonth,
	stEveryYear, stOnce, stCountDown, stZero, stIdle, stBoot, stRestore, stCpu, stNetSpd, stMem
};

struct ITasker
{
	virtual ~ITasker() {};

	string TaskType;
	CScheduleType SchType = stUnknown;

	CString ID = _T(""); //���棬���أ����У����ģ�������У�Ψһ��ʶ��
	CString LastRunTime = _T(""), NextRunTime = _T("");
	BOOL HasEndDateTime = FALSE;
	CTime NextRunDateTime, StartDateTime, EndDateTime;
	
	BOOL Stopped = FALSE, Executing = FALSE;

	map<TString, TString> ParamList; //name = value
									 //map<CString, CString> SchParamList; //�ƻ������б�name = value

	virtual void Run() = 0;  //command pattern

	virtual TString GetParam(LPCTSTR szName)
	{
		return ParamList[szName];
	}

	virtual int GetParamInt(LPCTSTR szName, int nDefault = 1)
	{
		TString s = ParamList[szName];

		if (!s.empty())
			return _ttoi(s.c_str());
		else
			return nDefault;
	}

	virtual float GetParamFloat(LPCTSTR szName, float nDefault = 1.0)
	{
		TString s = ParamList[szName];

		if (!s.empty())
			return _ttof(s.c_str());
		else
			return nDefault;
	}

	virtual void SetParam(LPCTSTR szName, LPCTSTR szValue)
	{
		ParamList[szName] = szValue;
	}
	virtual void SetParam(LPCTSTR szName, int nValue)
	{
		TCHAR szValue[10];
		_itot(nValue, szValue, 10);
		ParamList[szName] = szValue;
	}

	ITasker()
	{
		ID = Util::String::newGUID().c_str();
		TaskType = "ttUnknown";
	}

	void Start() {
		Stopped = FALSE;
		if (NextRunDateTime <= CTime::GetCurrentTime())
			SetNextRunTime();
	}

	void Stop()
	{
		Stopped = TRUE;
	}

	void SetNextRunTime();

	bool operator==(const ITasker& rhs)
	{
		return rhs.ID == this->ID;
	}
};


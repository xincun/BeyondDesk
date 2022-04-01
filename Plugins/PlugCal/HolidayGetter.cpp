#include "stdafx.h"
#include "HolidayGetter.h"
#include "GlobalDefine.h"

CHolidayGetter gHolidayGetter;

void CHolidayGetter::AddToHolidayList(int iYear, LPCTSTR pszFilePath)
{
	wifstream in(pszFilePath, ios::in);
	//in.imbue(std::locale("chs"));
	if (!in) return;

	TCHAR buf[102400];
	try
	{
		while (in.getline(buf, sizeof(buf)))
		{
			if (_tcsicmp(buf, _T("")) > 0)
			{
				TString sFile = String::IntToStr(iYear);
				sFile += buf;

				if (std::find(m_HolidayList.begin(), m_HolidayList.end(), sFile) == m_HolidayList.end())
					m_HolidayList.push_back(sFile);
			}
		}
	}
	catch (...)
	{
		
	}
}

void CHolidayGetter::AddToWorkdayList(int iYear, LPCTSTR pszFilePath)
{
	if (!File::FileExist(pszFilePath)) return;
	if (File::GetFileSize(pszFilePath) > 1024) return;

	wifstream in(pszFilePath, ios::in);
	//in.imbue(std::locale("chs"));
	if (!in) return;

	TCHAR buf[1024];
	while (in.getline(buf, sizeof(buf)))
	{
		if (_tcsicmp(buf, _T("")) > 0)
		{
			TString sFile = String::IntToStr(iYear);
			sFile += buf;
			if (std::find(m_WorkdayList.begin(), m_WorkdayList.end(), sFile) == m_WorkdayList.end())
				m_WorkdayList.push_back(sFile);
		}
	}
}

void CHolidayGetter::DoGet()
{
	CHttp http;

	USES_CONVERSION;

	THttpProxy proxy;

	proxy.enabled = gUnify->DB()->SysConfig()->GetBool(_T("Net"), _T("chkProxy"), false);
	proxy.needAuth = gUnify->DB()->SysConfig()->GetBool(_T("Net"), _T("chkProxyAuth"), false);

	proxy.address = T2A(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyAddr")));
	CString s = gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyPort"));
	int nPort = String::StrToInt(s.GetBuffer());
	proxy.port = nPort;

	proxy.user = T2A(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyUser")));
	proxy.password = T2A(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyPass")));

	s = gUnify->DB()->SysConfig()->GetString(_T("Base"), L"cmbProxyType", L"1");
	int nType = String::StrToInt(s.GetBuffer());
	proxy.nType = nType;

	http.SetProxy(proxy);
	//http.AddHeader(_T("apikey"), _T("8e24de50a9fc879ef85d9e3cf61f081c"));

	CTime Now = CTime::GetCurrentTime();
	CString sPath;
	CString sURL;
	sPath.Format(_T("%s%s"), gUnify->App()->Path()->UserPath.c_str(), _T("Holiday"));
	File::CreateFolder(sPath.GetBuffer());

	for (int i = 2009; i <= Now.GetYear()+1; ++i)
	{
		CString sFilePath;
		sFilePath.Format(_T("%s\\%d.dat"), sPath.GetBuffer(), i);
		if (!File::FileExist(sFilePath.GetBuffer()) ||
			File::GetFileSize(sFilePath.GetBuffer()) >= 2048) 
		{
			sURL.Format(HOLIDAY_URL_BASE, i);
			
			http.DownloadFile(T2A(sURL.GetBuffer()), T2A(sFilePath.GetBuffer()));
		}

		AddToHolidayList(i, sFilePath.GetBuffer());

		sFilePath.Format(_T("%s\\%d_w.dat"), sPath.GetBuffer(), i);
		if (!File::FileExist(sFilePath.GetBuffer()) ||
			File::GetFileSize(sFilePath.GetBuffer()) >= 2048)
		{
			sURL.Format(WORKDAY_URL_BASE, i);
			USES_CONVERSION;
			http.DownloadFile(T2A(sURL), T2A(sFilePath));
		}

		AddToWorkdayList(i, sFilePath.GetBuffer());
	}

	//先加入当前旧的
	CString sFilePath;
	sFilePath.Format(_T("%s\\%d.dat"), sPath.GetBuffer(), Now.GetYear());
	//if (PathFileExists(sFilePath))
	if (File::GetFileSize(sFilePath.GetBuffer()) <= 1024)  //错误下载的情形，文件大于1K
		AddToHolidayList(Now.GetYear(), sFilePath.GetBuffer());
	sFilePath.Format(_T("%s\\%d_w.dat"), sPath.GetBuffer(), Now.GetYear());
	if (File::GetFileSize(sFilePath.GetBuffer()) <= 1024) {
		AddToWorkdayList(Now.GetYear(), sFilePath.GetBuffer());
	}

	//先设置一下，防止连不上网取不到情况
	//gUnify->Core()->Event()->SendEvent(EVENT_HOLIDAY_GET_DONE);

	//今年每次启动时都下载，防止有更新
	try {
		sFilePath.Format(_T("%s\\%d.dat"), sPath.GetBuffer(), Now.GetYear());
		sURL.Format(HOLIDAY_URL_BASE, Now.GetYear());
		//DeleteFile(sFilePath.GetBuffer());
		USES_CONVERSION;
		http.DownloadFile(T2A(sURL), T2A(sFilePath));
		AddToHolidayList(Now.GetYear(), sFilePath.GetBuffer());

		sFilePath.Format(_T("%s\\%d_w.dat"), sPath.GetBuffer(), Now.GetYear());
		sURL.Format(WORKDAY_URL_BASE, Now.GetYear());
		//DeleteFile(sFilePath.GetBuffer());

		http.DownloadFile(T2A(sURL), T2A(sFilePath));
		AddToWorkdayList(Now.GetYear(), sFilePath.GetBuffer());
	}
	catch (...) {

	}

	gUnify->Core()->Event()->SendEvent(EVENT_HOLIDAY_GET_DONE); //阻止释放
}

void CHolidayGetter::GetHoliday()
{
	//return;
	/*gUnify->Core()->Event()->RemoveEventListen(EVENT_HOLIDAY_GET_DONE, this, &CHolidayGetter::OnEvent);
	gUnify->Core()->Event()->AddEventListen(EVENT_HOLIDAY_GET_DONE, this, &CHolidayGetter::OnEvent);*/

	//GetHoliday();

	/*std::thread([&]() {
		DoGet();
	}).detach();*/
	mThdHelper.BeginThread(this, &CHolidayGetter::DoGet);
}

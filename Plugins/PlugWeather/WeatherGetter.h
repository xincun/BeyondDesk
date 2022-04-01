#pragma once;

#include "stdafx.h"
#include "AppPath.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"
#include "IniFile.h"
#include <thread>
#include "ThreadHelper.h"

class CWeatherGetter
{
	ThreadHelper<CWeatherGetter> mThdHelper;

	string GetCityID(const char* city, const char* region) {
		vector<string> m_CityList;
		wstring sPath = gUnify->App()->Path()->ExePath + _T("Data\\city.dat");
		USES_CONVERSION;
		string saPath = W2A(sPath.c_str());
		ifstream in(saPath, ios::in);
		//in.imbue(std::locale("chs"));
		if (!in) return "";

		char buf[1024];
		while (in.getline(buf, sizeof(buf)))
		{
			if (strcmp(buf, ("")) > 0)
			{
				m_CityList.push_back(buf);
			}
		}

		int nIndexCity = 0;
		auto iter = m_CityList.begin();
		for (int i = 0; i < m_CityList.size(); ++i) {
			CStringA s = m_CityList.at(i).c_str();
			int n = s.Find('=');
			if (s.Left(n) == city) {
				nIndexCity = i;

				break;
			}
		}

		int nFind = -1;
		//���ҵ���City��ʼ��
		for (int i = nIndexCity; i < nIndexCity + 101 ; ++i) {
			if (i >= m_CityList.size()) break;

			CStringA s = m_CityList.at(i).c_str();
			int n = s.Find('=');
			CStringA s1 = s.Left(n);
			
			if (s1 == region) {
				nFind = i;
				return s.Mid(n + 1, s.GetLength()).GetBuffer();
			}
		}

		if (nFind == -1) {
			for (int i = nIndexCity; i < nIndexCity + 50; ++i) {
				if (i >= m_CityList.size()) break;

				CStringA s = m_CityList.at(i).c_str();
				int n = s.Find('=');
				CStringA s1 = s.Left(n) + "��";
				
				if (s1 == region) {
					return s.Mid(n + 1, s.GetLength()).GetBuffer();
				}
			}
		}

		return "";
	}


	void ThdGetWeather()
	{
		
		if (m_bGettting)
		{
			LOGCON("�ϴ����ڻ�ȡ���˳����λ�ȡ...");
			return;
		}

		DWORD dwTick = GetTickCount();
		while  (dwTick <= 60 * 1000 && !gUnify->Net()->IsNetAlive()) { //�ٶ����ڿ���, �޷�����
			LOGCON("���ڿ������޷�����...");
			Sleep(1000);
		}

		if (!gUnify->Net()->IsNetAlive()) { //�޷�����
			return;
		}

		gUnify->App()->Status()->WeatherImage =L"";
		gUnify->App()->Status()->WeatherTemperature = L"";
		m_bGettting = true;
		
		//std::thread([=](){
		try
		{
			USES_CONVERSION;
			//string saRegion = T2A(m_sRegion.GetBuffer());
			string sCityID;// = strCityID;

			int nRetryCount = 0;

			CHttp http;
			//http.AddHeader(_T("apikey"), _T("8e24de50a9fc879ef85d9e3cf61f081c"));

			//if (sCityID.IsEmpty())
			//GetID:
			//TString sURL = _T("http://apis.baidu.com/apistore/weatherservice/citylist?cityname=");
			string city = T2A(m_sCity);
			string region = T2A(m_sRegion);
			sCityID = GetCityID(city.c_str(), region.c_str());
			LOGCON("cityid = %s", sCityID.c_str());

			string sURL = ("http://wthrcdn.etouch.cn/weather_mini?citykey=") + sCityID;
			//sURL += String::AnsiToUnicode(String::URLEncode(saRegion.c_str()).c_str());
			//sURL += String::AnsiToUnicode(saRegion.c_str()).c_str();
			//sURL += String::URLEncode(saRegion.c_str()).c_str();

			string sRet;

			/*FindControl<CCheckBoxUI>("chkProxy")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Net"), _T("chkProxy"), false));
			FindControl<CCheckBoxUI>("chkProxyAuth")->SetCheck(gUnify->DB()->SysConfig()->GetBool(_T("Net"), _T("chkProxyAuth"), false));
			SetCmoboCtrlByParam(L"cmbProxyType", L"cmbProxyType", true);
			FindControl<CEditUI>("edtProxyAddr")->SetText(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyAddr")));
			FindControl<CEditUI>("edtProxyPort")->SetText(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyPort")));
			FindControl<CEditUI>("edtProxyUser")->SetText(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyUser")));
			FindControl<CEditUI>("edtProxyPass")->SetText(gUnify->DB()->SysConfig()->GetString(_T("Net"), _T("edtProxyPass")));*/

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

			http.Get(sURL.c_str(), sRet);

			//LOGCON(sRet.c_str());
			sRet = String::UTF8ToAnsi(sRet);
			LOGCON(sRet.c_str());

			wstring sWea = String::AnsiToUnicode(sRet.c_str());
			if (sWea.length() > 256)
			{
				wofstream ofs(m_sDataFileName.GetBuffer());
				ofs.imbue(std::locale("chs"));
				ofs << sWea << endl;
				ofs.close();

				m_LastUpdateTime = CTime::GetCurrentTime();
				m_Ini.SetDateTime(_T("Weather"), _T("LastUpdate"), m_LastUpdateTime);

				if (m_OnGetFunc)
				{
					if (!m_OnGetFunc(m_pOwner, sWea.c_str())) //����ʧ�ܣ���ȡ����
					{

					}
				}

				m_bFirstGot = true;
			}
		}
		catch (...)
		{
			LOGCON("������ȡ����...");
		}
		
		m_bGettting = false;
			//	USES_CONVERSION;
			//	string sJson = W2A(sID.c_str());

			//	Json::Reader* reader = new Json::Reader;
			//	Json::Value *root = new Json::Value;
			//	if (reader->parse(sJson, *root))
			//	{
			//		int err = (*root)["errNum"].asInt();
			//		if (err != 0) //���Դ��ϼ���ȡ
			//		{
			//			nRetryCount++;
			//			if (nRetryCount == 1)
			//			{
			//				CString sNewRegion = m_sRegion.Left(m_sRegion.GetLength() - 1);
			//				saRegion = T2A(sNewRegion); //�ض����һ������������֧�ּӡ�����һ���
			//				Sleep(1000);
			//				goto GetID;
			//			}
			//			else if (nRetryCount == 2)
			//			{
			//				saRegion = T2A(m_sCity);
			//				Sleep(1000);
			//				goto GetID;
			//			}
			//			else
			//			{
			//				m_bGettting = false;
			//				return;
			//			}
			//		}

			//		Json::Value Data = (*root)["retData"];

			//		auto it = Data.begin();
			//		int i = 0;
			//		for (; it != Data.end(); ++it, ++i)
			//		{
			//			string sCity = Data[i]["district_cn"].asString();
			//			string sProv = Data[i]["province_cn"].asString(); 
			//			if (sCity.compare(T2A(m_sCity.GetBuffer())) == 0 && sProv.compare(T2A(m_sProvience.GetBuffer())) == 0)
			//			{
			//				string saCityID = Data[i]["area_id"].asString();
			//				LOGCON("cityid:%s", saCityID.c_str());

			//				sCityID = A2T(saCityID.c_str());

			//				break;
			//			}
			//		}
			//	}

			//	//���̣�����ȡ��, �Է���һ��ÿ�ζ�ȡ��
			//	//gUnify->DB()->SysConfig()->SetString(_T("Weather"), _T("CityID"), sCityID);

			//	delete root;
			//	delete reader;
			//}

			//wstring sWea;
			//if (!sCityID.IsEmpty())
			//{
			//	CString strURL;
			//	strURL.Format(_T("http://apis.baidu.com/apistore/weatherservice/recentweathers?cityname=%s&cityid=%s"),
			//		A2T(String::URLEncode(saRegion.c_str()).c_str()),
			//		sCityID.GetBuffer()
			//		);

			//	//wchar_t* szWea = new wchar_t[2048];
			//	try
			//	{
			//		sWea = http.GetResponse(strURL.GetBuffer());
			//	}
			//	catch (...)
			//	{
			//		LOGCON("ȡ�����������");

			//		goto GetID;
			//	}


			//	//LOGCON(s.c_str());
			//	CLog::InsureConsole();
			//	setlocale(LC_ALL, "chs");
			//	wprintf(L"%s", sWea.c_str());

			

		
			//	}
			//	else
			//	{
			//		goto GetID;
			//	}
			//}

			////m_sUIRegion = A2T(saRegion.c_str()); //�м���ܸ�����Ӧ���������ڵ����������䵱������ʾ��
			//m_bGettting = false;
			//::PostMessage(pobjDlg->GetSafeHwnd(), WM_THREADFINISHED, 0, 0);

		//}).detach();

	}
	
	void OnTimer(void* pOwner, UINT uiTimerID)
	{
		if (!m_bEnabled) return;
	
		if (pOwner == this && uiTimerID == 1)
		{
			//m_nCount++;
			if (m_bFirstGot)
			{
				LOGCON("�����Ѿ���ȡ����ֹͣTimer");
				gUnify->Core()->Timer()->StopTimer(this, 1);
			}
		}
	}

	void OnUpdateTimer(void* pOwner, UINT uiTimerID)
	{
		if (!m_bEnabled) return;

		if (pOwner == this && uiTimerID == 2)
		{
			if (m_sProvience.IsEmpty() || m_sCity.IsEmpty() || m_sRegion.IsEmpty()) return;

			LOGCON("��ʱˢ��ʱ�䵽������ˢ��...");

			//ThdGetWeather();
			mThdHelper.BeginThread(this, &CWeatherGetter::ThdGetWeather);
		}
	}

	/*
	sCity��ĳ����
	sRegion: ĳ��
	��sRegion��ѯ����ʱ�������ϼ�sCity
	nMin: ����ʱ�䣬��λ���ӣ�Ĭ��30����
	*/
	bool GetExpiredWeather()
	{
		CString sFile = gUnify->App()->Path()->UserPath.c_str();
		sFile += _T("Weather\\") + m_sProvience + _T("\\") + m_sCity + _T("\\") + m_sRegion + _T("\\");
		m_sDataFileName = sFile + L"Weather.dat";

		File::CreateFolder(sFile.GetBuffer());
		m_Ini.SetFileName(sFile + _T("Update.dat"));

		CTimeSpan span = CTime::GetCurrentTime() - m_Ini.GetDateTime(_T("Weather"), _T("LastUpdate"), 0);
		//CString sCityID = gUnify->DB()->SysConfig()->GetString(_T("Weather"), _T("CityID"), _T(""));

		int n1 = span.GetDays();
		int n2 = span.GetMinutes();
		if (span.GetDays() > 0 || span.GetHours() > 0 || span.GetMinutes() >= m_nUpdateTime)
		{
			LOGCON("�����ѹ��ڣ����»�ȡ");
			mThdHelper.BeginThread(this, &CWeatherGetter::ThdGetWeather);
			return false;
		}
		else
		{
			LOGCON("����δ���ڣ�ֱ�Ӳ��ô�������");

			m_bGettting = true;

			wstring sWea;
			
			wifstream ofs(m_sDataFileName.GetBuffer());
			ofs.imbue(std::locale("chs"));
			ofs >> sWea;
			ofs.close();

			if (sWea.length() < 256)
			{
				LOGCON("������Ϣ�𻵣����»�ȡ");
				m_bGettting = false;
				mThdHelper.BeginThread(this, &CWeatherGetter::ThdGetWeather);

				return false;
			}

			if (m_OnGetFunc)
				m_OnGetFunc(m_pOwner, sWea.c_str());

			m_bGettting = false;

			return true;
		}
	}

	CIniFile m_Ini;
	CString m_sDataFileName;

	typedef function<bool(void* pOwner, const CString& sWeaData)> OnGetFunc;
	OnGetFunc m_OnGetFunc;
	void* m_pOwner = NULL;
	

	CTime m_LastTimeNow; //����ģʽ��������ʱ��
	UINT m_nUpdateTime = 30; //ˢ��ʱ�䣬����
	
	bool m_bFirstGot = false; //��һ���Ѿ�ȡ��
	bool m_bEnabled = true;
public:
	bool m_bGettting = false;
	CString m_sProvience;
	CString m_sCity;
	CString m_sRegion;
	
	//CString m_sUIRegion;

	CTime m_LastUpdateTime;

	void SetParam(const CString& sProvience, const CString& sCity, const CString& sRegion, UINT nUpdateTime)
	{
		m_sProvience = sProvience;
		m_sCity = sCity;
		m_sRegion = sRegion;
		m_nUpdateTime = nUpdateTime;

		m_LastUpdateTime = CTime::GetCurrentTime();
		m_LastTimeNow = CTime::GetCurrentTime() - CTimeSpan(0,0,30, 0);
	}

	void SetUpdateTime(UINT val) { 
		m_nUpdateTime = val; 

		gUnify->Core()->Timer()->StopTimer(this, 2);
		//gUnify->Core()->Timer()->SetTimer(this, 2, m_nUpdateTime *100, &CWeatherGetter::OnUpdateTimer);
		gUnify->Core()->Timer()->SetTimer(this, 2, m_nUpdateTime * 60 * 1000, &CWeatherGetter::OnUpdateTimer);
	}

	CWeatherGetter()
	{

	}

	CWeatherGetter(const CString& sProvience, const CString& sCity, const CString& sRegion, UINT nUpdateTime)
	{
		SetParam(sProvience, sCity, sRegion, nUpdateTime);

		gUnify->Core()->Timer()->SetTimer(this, 2, m_nUpdateTime * 60 * 1000, &CWeatherGetter::OnUpdateTimer);
		
	}

	~CWeatherGetter()
	{
		gUnify->Core()->Timer()->StopTimer(this, 1);
		gUnify->Core()->Timer()->StopTimer(this, 2);
	}

	void DoGet()
	{
		if (!m_bEnabled) return;
		if (m_sProvience.IsEmpty() || m_sCity.IsEmpty() || m_sRegion.IsEmpty()) return;

		/*if (m_sRegion.Right(1) == _T("��") || m_sRegion.Right(1) == _T("��"))
			m_sUIRegion = m_sRegion.Left(m_sRegion.GetLength() - 1);*/

		if (!GetExpiredWeather())
			gUnify->Core()->Timer()->SetTimer(this, 1, 3000, &CWeatherGetter::OnTimer); //��ֹû�л�ȡ������
	}

	void DoGetNow()
	{
		if (!m_bEnabled) return;
		if (m_sProvience.IsEmpty() || m_sCity.IsEmpty() || m_sRegion.IsEmpty()) return;
		
		//������೤ʱ���ڲ��ø���
		CTime Now = CTime::GetCurrentTime();
		CTimeSpan t = Now - m_LastTimeNow;

		int n = t.GetTotalSeconds();
		if (t.GetTotalSeconds() < 30)
		{
			LOGCON("30���ڣ���ˢ��");
			return;
		}

		m_LastTimeNow = CTime::GetCurrentTime();

		ThdGetWeather();
	}

	template<class T>
	void SetOnGetWeather(T *pOwner, bool (T::* pOnGet)(void* pOwner, const CString& sWeaData))
	{
		m_pOwner = pOwner;
		m_OnGetFunc = std::bind(pOnGet, pOwner, std::placeholders::_1, std::placeholders::_2);
	}

	void SetEnabled(bool b)
	{
		m_bEnabled = b;
	}
};
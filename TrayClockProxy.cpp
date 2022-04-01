#include "stdafx.h"
#include "TrayClockProxy.h"

TrayClockProxy* TrayClockProxy::_instance = NULL;

TrayClockProxy::TrayClockProxy()
{
	gUnify->Core()->Event()->AddEventListen(EVENT_WEATHER_UPDATE, this, &TrayClockProxy::OnEvent);
	gUnify->Core()->Timer()->SetTimer(this, 1, 1000, &TrayClockProxy::OnTimer); 
}

void TrayClockProxy::OnTimer(void* pOwner, UINT uiTimerID)
{
	if (pOwner == this && uiTimerID == 1) {
		HWND TrayClockMsgWnd = FindWindow(L"TRAYCLOCKPROXY", L"TrayClockProxy");
		SendMessage(TrayClockMsgWnd, WM_USER + 1024, 0, 0);
	}
}

TrayClockProxy::~TrayClockProxy()
{
	gUnify->Core()->Event()->RemoveEventListen(EVENT_WEATHER_UPDATE, this, &TrayClockProxy::OnEvent);
	gUnify->Core()->Timer()->StopTimer(this, 1);
}

void TrayClockProxy::OnEvent(CEvent* e)
{
	if (e->EventID == EVENT_WEATHER_UPDATE)
	{
		//LOGCON("刷新托盘时钟天气...........");
		Registry::CRegistry reg;
		reg.SetRootKey(HKEY_CURRENT_USER);
		reg.SetKey(L"Software\\BeyondDesk\\TrayClock\\Format", TRUE);

		if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockWea"), false)) {

			reg.WriteString(L"Weather", gUnify->App()->Status()->WeatherImage.c_str());

			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockTmp"), false))
				reg.WriteString(L"Temperature", gUnify->App()->Status()->WeatherTemperature.c_str());
			else
				reg.WriteString(L"Temperature", L"");
		}
		else {
			reg.WriteString(L"Temperature", L"");
			reg.WriteString(L"Temperature", L"");
		}
		
		

		HWND TrayClockMsgWnd = FindWindow(L"TRAYCLOCKPROXY", L"TrayClockProxy");
		SendMessage(TrayClockMsgWnd, WM_USER + 1024, 0, 0);
	}
}


void TrayClockProxy::Load(HWND mainWnd)
{
	TString s;
	if (System::IsWin64()) {
		Process::KillProcessByName(L"TrayClockProxy64.exe");
		s = gUnify->App()->Path()->ExePath + L"TrayClockProxy64.exe";
	}
	else {
		Process::KillProcessByName(L"TrayClockProxy.exe");
		s = gUnify->App()->Path()->ExePath + L"TrayClockProxy.exe";
	}

	//s += L" " + String::IntToStr(int(mainWnd));
	//Process::ExecProcessWait(s.c_str(), false);
    ShellExecute(mainWnd, L"open", s.c_str(), String::IntToStr(int(mainWnd)).c_str(), gUnify->App()->Path()->ExePath.c_str(), SW_HIDE);

	HWND TrayClockMsgWnd = FindWindow(L"TRAYCLOCKPROXY", L"TrayClockProxy");
	int n = GetTickCount();
	while (!TrayClockMsgWnd)
	{
		TrayClockMsgWnd = FindWindow(L"TRAYCLOCKPROXY", L"TrayClockProxy");
		if ((GetTickCount() - n) > 20000) { //超
			return;
		}

		App::AppProcessMessage();
		//Sleep(1);
	}
}

void TrayClockProxy::Unload()
{
	HWND TrayClockMsgWnd = FindWindow(L"TRAYCLOCKPROXY", L"TrayClockProxy");
	if (!TrayClockMsgWnd) return;

	PostMessage(TrayClockMsgWnd, WM_CLOSE, 0, 0);
	PostMessage(TrayClockMsgWnd, WM_DESTROY, 0, 0);
	
	if (System::IsWin64()) {
		Process::KillProcessByName(L"TrayClockProxy64.exe");
	}
	else {
		Process::KillProcessByName(L"TrayClockProxy.exe");
	}
}

void TrayClockProxy::SetInt(LPCTSTR szSection, LPCTSTR szName, int nValue) {
	////等待时钟exe加载完毕，发过来其窗口句柄
	//while (!TrayClockMsgWnd) {
	//	Sleep(1);
	//}
	//Wait
	HWND TrayClockMsgWnd = FindWindow(L"TRAYCLOCKPROXY", L"TrayClockProxy");
	
	CString s;
	s.Format(L"%s|%s|%d", szSection, szName, nValue);
	wchar_t buf[256] = { 0 };
	COPYDATASTRUCT copyData = { 0 };
	lstrcpy(buf, s.GetBuffer());
	copyData.lpData = buf;
	copyData.cbData = 256;
	::SendMessage(TrayClockMsgWnd, WM_COPYDATA, 0, LPARAM(&copyData));
}

void TrayClockProxy::SetStr(LPCTSTR szSection, LPCTSTR szName, LPCTSTR szValue) {
	////等待时钟exe加载完毕，发过来其窗口句柄
	//while (!TrayClockMsgWnd) {
	//	Sleep(1);
	//}
	
	HWND TrayClockMsgWnd = FindWindow(L"TRAYCLOCKPROXY", L"TrayClockProxy");
	
	CString s;
	s.Format(L"%s|%s|%s", szSection, szName, szValue);
	wchar_t buf[256] = {0};
	COPYDATASTRUCT copyData = { 0 };
	lstrcpy(buf, s.GetBuffer());
	copyData.lpData = buf;
	copyData.cbData = 256;
	::SendMessage(TrayClockMsgWnd, WM_COPYDATA,1, LPARAM(&copyData));
}


void TrayClockProxy::RefreshConfig()
{
	if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClock"), false)) {
		Load(gUnify->App()->m_MainWnd);

		//放到此处，否则如果没有加载托盘时钟，while (!TrayClockMsgWnd) 一直在等待
		//THREAD_BEGIN		
		SetStr(L"Preview", L"Font", gUnify->DB()->SysConfig()->GetString(_T("Base"), L"cmbTrayClockFont", L"Tahoma"));
		SetInt(L"Preview", L"FontSize", _ttoi(gUnify->DB()->SysConfig()->GetString(_T("TrayClock"), _T("edtTrayClockFontSize"), _T("10")).GetBuffer()));
		SetInt(L"Preview", L"LineHeight", _ttoi(gUnify->DB()->SysConfig()->GetString(_T("TrayClock"), _T("edtTrayClockLineSpace"), _T("4")).GetBuffer()));
		SetInt(L"Preview", L"ForeColor", gUnify->DB()->SysConfig()->GetInt(_T("TrayClock"), _T("cmbTrayClockFontColor"), RGB(0, 0, 0)));
		SetInt(L"Preview", L"BackColor", gUnify->DB()->SysConfig()->GetInt(_T("TrayClock"), _T("cmbTrayClockBackColor"), 4210884338));
		SetInt(L"Preview", L"Bold", gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockFontBold"), false));
		SetInt(L"Preview", L"Italic", gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockFontItalic"), false));

		//FindControl<COptionUI>("optTrayClockFmt1")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("optTrayClockFmt1"), true));
		//FindControl<COptionUI>("optTrayClockFmt2")->Selected(gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("optTrayClockFmt2"), false));
		if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("optTrayClockFmt2"), false)) {
			SetStr(L"Preview", L"Format", gUnify->DB()->SysConfig()->GetString(_T("TrayClock"), _T("edtTrayClockFmt2"), _T("H:nn:ss\\nmm/dd ddd")).GetBuffer());
			SetStr(L"Format", L"Format", gUnify->DB()->SysConfig()->GetString(_T("TrayClock"), _T("edtTrayClockFmt2"), _T("H:nn:ss\\nmm/dd ddd")).GetBuffer());
		}
		else 
		{
			CString s;
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockAP"), false)) s += L"tt ";
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockHour"), true)) {
				if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClock24"), true))
					s += L"H";
				else
					s += L"h";
			}
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockMin"), false)) s += L"nn";
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockSec"), false)) s += L"ss";
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockNewLine"), true)) s += L"\\n"; else s += L" ";
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockYear4"), false)) s += L"yyyy";
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockYear"), false)) s += L"yy";
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockMon"), true)) s += L"m";
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockDay"), true)) s += L"d ";
			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockWeek"), true)) s += L"ddd";
			int n = -1;
			n= s.Find(L"Hnn");
			if (n >= 0) {
				s.Insert(n + 1, L":");
			}
			n = s.Find(L"hnn");
			if (n >= 0) {
				s.Insert(n + 1, L":");
			}
			n = s.Find(L"nnss");
			if (n >= 0) {
				s.Insert(n+2, L":");
			}
			/*n = s.Find(L"ss");
			if (n >= 0) {
				s.Insert(n, L":");
			}*/
			n = s.Find(L"yyyym");
			if (n >= 0) {
				s.Insert(n + 4, L"/");
			}
			else {
				n = s.Find(L"yym");
				if (n >= 0) {
					s.Insert(n + 2, L"/");
				}
			}
			n = s.Find(L"md");
			if (n >= 0) {
				s.Insert(n+1, L"/");
			}
			/*n = s.Find(L"d");
			if (n >= 0) {
				s.Insert(n, L"/");
			}*/
			SetStr(L"Preview", L"Format", s.GetBuffer());
			SetStr(L"Format", L"Format", s.GetBuffer());

			
			/*if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("optTrayClockFmt1"), true)) {
				SetInt(L"Format", L"Custom", 0);
				SetStr(L"Format", L"CustomFormat", L"");
			}
			else
			{
				SetInt(L"Format", L"Custom", 1);
				SetStr(L"Format", L"CustomFormat", gUnify->DB()->SysConfig()->GetString(_T("TrayClock"), _T("edtTrayClockFmt2"), L"H:nn:ss\\nmm/dd ddd"));
			}*/
		}
	
		if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockWea"), false)) {

			SetStr(L"Format", L"Weather", gUnify->App()->Status()->WeatherImage.c_str());

			if (gUnify->DB()->SysConfig()->GetBool(_T("TrayClock"), _T("chkTrayClockTmp"), false))
			    SetStr(L"Format", L"Temperature", gUnify->App()->Status()->WeatherTemperature.c_str());
			else
				SetStr(L"Format", L"Temperature", L"");
		}
		else {
			SetStr(L"Format", L"Weather", L"");
			SetStr(L"Format", L"Temperature", L"");
		}

		HWND TrayClockMsgWnd = FindWindow(L"TRAYCLOCKPROXY", L"TrayClockProxy");
		SendMessage(TrayClockMsgWnd, WM_USER + 1024, 0, 0);
		//THREAD_END
	}
	else {
		Unload();
		//TrayClockMsgWnd = NULL;
	}

}

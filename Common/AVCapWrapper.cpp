#include "stdafx.h"
#include "AVCapWrapper.h"
#include <ShellAPI.h>
#include "CamRecPreviewWnd.h"
#include "../Plugins/Common/ITasker.h"
#include "GlobalDefine.h"

HINSTANCE g_hAVCap = NULL;
bool g_bAVCaping = false;
//HANDLE g_hAVCapEvent = NULL;

#define  AVCAP_DLL_NAME  _T("AVCap.dll")

void CAVCapDLL::Init()
{
	if (g_hAVCap) return;

	//if (!g_hAVCap) 
	{
		g_hAVCap = LoadLibrary(AVCAP_DLL_NAME);
		if (!g_hAVCap) return;

		//g_hAVCapEvent = CreateEvent(NULL, FALSE, FALSE, L"8A1CB0CB - 8CE0 - 4FD9 - 80E8 - E79562FFF724 - AVCapEvent");
	}

	//THREAD_Start
	ProcInit p = (ProcInit)::GetProcAddress(g_hAVCap, "Init"); //
	p("XX");

	//WaitForSingleObject(g_hAVCapEvent, INFINITE);
	//int n = 1;
	//THREAD_Stop
}

void CAVCapDLL::Clear()
{
	if (!g_hAVCap) return;

	//	SetEvent(g_hAVCapEvent);
	//THREAD_Start
	ProcClear p = (ProcClear)::GetProcAddress(g_hAVCap, "Clear"); //
	p();

	//OutputDebugStringA("free lib");
	//FreeLibrary(g_hAVCap);
	//OutputDebugStringA("free lib done");
	g_hAVCap = NULL;
	
	//	THREAD_Stop

}

void CAVCapDLL::GetProp(LPCSTR pszCat, LPWSTR pszRet)
{
	if (!g_hAVCap) return;

	ProcGet p = (ProcGet)::GetProcAddress(g_hAVCap, "GetProperties"); //
	p(pszCat, pszRet);
}

void CAVCapDLL::SetProp(LPCSTR pszCat, LPCSTR pszText)
{
	if (!g_hAVCap) return;

	ProcSet p = (ProcSet)::GetProcAddress(g_hAVCap, "SetProperties"); //
	p(pszCat, pszText);
}

void CAVCapDLL::ShowSetup(LPCSTR pszCat)
{
	if (!g_hAVCap) return;
	
	ProcShowSetup p = (ProcShowSetup)::GetProcAddress(g_hAVCap, "ShowSetup"); //
	p(pszCat);
}

void CAVCapDLL::StartPreview(HWND hShow, int nWidth, int nHeight)
{
	if (!g_hAVCap) return;

	ProcStartPreview p = (ProcStartPreview)::GetProcAddress(g_hAVCap, "StartPreview"); //
	p(hShow, nWidth, nHeight);
}

void CAVCapDLL::StopPreview()
{
	if (!g_hAVCap) return;

	ProcStopPreview p = (ProcStopPreview)::GetProcAddress(g_hAVCap, "StopPreview"); //
	p();
}


void CAVCapDLL::StartVideoCap(LPCSTR pszFileName, bool bAudio)
{
	if (!g_hAVCap) return;
	/*if (g_bAVCaping) {
	LOGCON("已经有采集任务，退出先");
	return; }*/

	g_bAVCaping = true;
	ProcStartVideoCap p = (ProcStartVideoCap)::GetProcAddress(g_hAVCap, "StartVideoCap"); //
	p(pszFileName, bAudio);
}

void CAVCapDLL::StopVideoCap()
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcStopVideoCap p = (ProcStopVideoCap)::GetProcAddress(g_hAVCap, "StopVideoCap"); //
	p();
}

void CAVCapDLL::SetVideoSource(int nSource)
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcSetVideoSource p = (ProcSetVideoSource)::GetProcAddress(g_hAVCap, "SetVideoSource"); //
	p(nSource);
}

void CAVCapDLL::SetScreenRecMon(int nSource)
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcSetScrRecMon p = (ProcSetScrRecMon)::GetProcAddress(g_hAVCap, "SetScreenRecordMonitor"); //
	p(nSource);
}

void CAVCapDLL::SetFrameRate(int n)
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcSetFrameRate p = (ProcSetFrameRate)::GetProcAddress(g_hAVCap, "SetFrameRate"); //
	p(n);
}

void CAVCapDLL::SetWmvVer(int n)
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcSetWmvVer p = (ProcSetWmvVer)::GetProcAddress(g_hAVCap, "SetWMVProfileVer"); //
	p(n);
}

void CAVCapDLL::CapturePicture(LPCSTR pszFileName)
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcCapturePicture p = (ProcCapturePicture)::GetProcAddress(g_hAVCap, "CapturePicture"); //
	p(pszFileName);
}

void CAVCapDLL::SetAutoRefreshPreview(bool b)
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcSetAutoRefreshPreview p = (ProcSetAutoRefreshPreview)::GetProcAddress(g_hAVCap, "SetAutoRefreshPreview"); //
	p(b);
}

void CAVCapDLL::SetVideoWindowFloat(bool b)
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcSetVideoWindowFloat p = (ProcSetVideoWindowFloat)::GetProcAddress(g_hAVCap, "SetVideoWindowFloat"); //
	p(b);
}

void CAVCapDLL::ShowMicSet()
{
	ShellExecuteA(0, "open", "rundll32.exe", "shell32.dll,Control_RunDLL mmsys.cpl,,1", "", SW_SHOWNORMAL);
}

void CAVCapDLL::StartScreenRecord(LPCSTR pszFileName, bool bAudio)
{
	if (!g_hAVCap) return;

	g_bAVCaping = true;
	ProcStartScreenRec p = (ProcStartScreenRec)::GetProcAddress(g_hAVCap, "StartScreenRecord"); //
	p(pszFileName, bAudio);
}

void CAVCapDLL::StopScreenRecord()
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcStopScreenRec p = (ProcStopScreenRec)::GetProcAddress(g_hAVCap, "StopScreenRecord"); //
	p();
}

void CAVCapDLL::StartAudioCap(LPCSTR pszFileName)
{
	if (!g_hAVCap) return;

	g_bAVCaping = true;
	ProcStartAudioCap p = (ProcStartAudioCap)::GetProcAddress(g_hAVCap, "StartAudioCap"); //
	p(pszFileName);
}

void CAVCapDLL::StopAudioCap()
{
	if (!g_hAVCap) return;

	g_bAVCaping = false;
	ProcStopAudioCap p = (ProcStopAudioCap)::GetProcAddress(g_hAVCap, "StopAudioCap"); //
	p();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////VideoCapExec/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CVideoCapExec::CVideoCapExec() : m_hEvent(NULL), m_hMapFile(NULL), m_pShareMem(NULL)
{
	m_hMapFile = ::CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0,
		sizeof(TVideoCapParam), VIDEO_CAP_FILE_MAP);

	m_pShareMem = (TVideoCapParam*)MapViewOfFile(m_hMapFile, FILE_MAP_WRITE, 0, 0, 0);

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, VIDEO_CAP_EVENT); //存在则返回存在的句柄
}

CVideoCapExec::~CVideoCapExec()
{
	UnmapViewOfFile(m_pShareMem);
	CloseHandle(m_hMapFile);

	CloseHandle(m_hEvent);
}

void CVideoCapExec::Exec(ITasker* p)
{
	TVideoCapParam param;
	TString s = p->GetParam(_T("cmbVideoCam"));
	strcpy(param.VideoDevice, String::UnicodeToAnsi(s.c_str()).c_str());

	CVideoCapProc::Stop(s.c_str()); //先停止上一个

	s = p->GetParam(_T("cmbVideoComp"));
	strcpy(param.VideoCompressor, String::UnicodeToAnsi(s.c_str()).c_str());

	s = p->GetParam(_T("cmbVideoMic"));
	strcpy(param.AudioDevice, String::UnicodeToAnsi(s.c_str()).c_str());

	s = p->GetParam(_T("cmbAudioComp"));
	strcpy(param.AudioCompressor, String::UnicodeToAnsi(s.c_str()).c_str());

	s = p->GetParam(_T("cmbVideoReso"));
	strcpy(param.VideoResolution, String::UnicodeToAnsi(s.c_str()).c_str());

	string sFileName = String::UnicodeToAnsi(p->GetParam(_T("edtCamDir")).c_str());
	if (sFileName.length() == 0)
		sFileName = String::UnicodeToAnsi(gUnify->App()->Path()->ExePath.c_str());

	if (sFileName.rfind('\\') > 0) sFileName += "\\";

	//SYSTEMTIME t;
	//GetSystemTime(&t);
	CTime Now = CTime::GetCurrentTime();
	CString sNow = Now.Format(_T(" %Y-%m-%d %H时%M分%S秒.avi"));//%03d毫秒
	//sNow.Format(_T("%02d-%02d-%02d %02d时%02d分%02d秒.avi"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	sFileName += param.VideoDevice;
	sFileName += String::UnicodeToAnsi(sNow.GetBuffer());

	strcpy(param.FileName, sFileName.c_str());

	param.FrameRate = p->GetParamInt(_T("edtVideoCapFrameRate"), 30);

	UINT nSec = p->GetParamInt(_T("edtVideoTime"), 10) * 60;
	param.CapTime = nSec;
	param.IsAudio = p->GetParamInt(_T("chkVideoAudio"), 0);
	param.IsPreview = p->GetParamInt(_T("chkVideoShowPre"), 0);

	memcpy(m_pShareMem, &param, sizeof(TVideoCapParam));

	//启动进程，等待进程处理完成
	// s = gUnify->App()->Path()->ExeFullPath;
	THREAD_BEGIN
	ShellExecute(NULL, _T("open"), gUnify->App()->Path()->ExeFullPath.c_str(), _T("videocap"), gUnify->App()->Path()->ExePath.c_str(), SW_HIDE);
	THREAD_END

	WaitForSingleObject(m_hEvent, 10000);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////VideoCapPROC/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool bStopped = false;
TVideoCapParam* g_pVideoParam = NULL;

static LRESULT CALLBACK	 _VideoCapWndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	if (wMsg == WM_VIDEO_CAP_EXIT)
	{
		OutputDebugStringA("!!收到退出消息!!");
		if (!bStopped)
		{
			OutputDebugStringA("【强制】停止摄像头");
			CAVCapDLL::StopVideoCap();

			OutputDebugString(L"摄像头采集已【强制】停止");
			CAVCapDLL::Clear();
		}

		PostQuitMessage(0);
	}
	else if (wMsg == WM_VIDEO_CAP_CAPTURE_PICTURE && g_pVideoParam)
	{
		string sFileName = Path::GetParentDirA(g_pVideoParam->FileName);
		if (sFileName.length() == 0)
			sFileName = String::UnicodeToAnsi(gUnify->App()->Path()->ExePath.c_str());

		if (sFileName.rfind('\\') > 0) sFileName += "\\";

		//SYSTEMTIME t;
		//GetSystemTime(&t);
		CTime Now = CTime::GetCurrentTime();
		CString sNow = Now.Format(_T(" %Y-%m-%d %H时%M分%S秒.jpg"));//%03d毫秒
		//sNow.Format(_T("%02d-%02d-%02d %02d时%02d分%02d秒.avi"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		sFileName += g_pVideoParam->VideoDevice;
		sFileName += String::UnicodeToAnsi(sNow.GetBuffer());

		OutputDebugStringA("拍照：");
		OutputDebugStringA(sFileName.c_str());
		CAVCapDLL::CapturePicture(sFileName.c_str());
	}

	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	TCHAR lpWinTitle[MAX_PATH];
	::GetWindowText(hwnd, lpWinTitle, MAX_PATH - 1);
	TString strTitle = lpWinTitle;
	//LOGCON(strTitle.c_str());

	if (strTitle == LPCTSTR(lparam))
	{
		//LOGCON(L"窗口已经找到");
		
		::SendMessage(hwnd, WM_VIDEO_CAP_EXIT, 0, 0);

		return FALSE;
	}
	return TRUE;
}

void CVideoCapProc::Stop(LPCTSTR pszDevName)
{
	EnumWindows(EnumWindowsProc, LPARAM(pszDevName));
}

struct TStopParam
{
	UINT nSec = 0;
	HWND hMsgWnd = NULL;
};


DWORD WINAPI _StopVideoCapProc(LPVOID lpParam) {
	TStopParam* p = static_cast<TStopParam*>(lpParam);

	UINT nSec = p->nSec;
	HWND hMsgWnd = p->hMsgWnd;

	delete p;

	::Sleep((nSec + 1) * 1000);

	// OutputDebugString(L"线程休眠完毕");

	try {
		CAVCapDLL::StopVideoCap();

		OutputDebugString(L"摄像头采集已停止===");
		CAVCapDLL::Clear();

	}
	catch (...)
	{

	}

	OutputDebugString(L"摄像头采集已清理||||||||||");

	::PostMessage(hMsgWnd, WM_QUIT, 0, 0);
	::PostMessage(hMsgWnd, WM_VIDEO_CAP_EXIT, 0, 0);
	//::PostMessage(hMsgWnd, WM_VIDEO_CAP_EXIT, 0, 0);

	bStopped = true;

	return 0;
}

int CVideoCapProc::Proc()
{
	UINT nSec = 0;
	HWND hMsgWnd = NULL;

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, VIDEO_CAP_EVENT);

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, VIDEO_CAP_FILE_MAP);
	if (hMapFile)
	{
		//OutputDebugStringA("open file map succed");
		TVideoCapParam* pSrc = (TVideoCapParam*)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
		TVideoCapParam* p = new TVideoCapParam;
		memcpy(p, pSrc, sizeof(TVideoCapParam));
		//MessageBoxA(NULL, pAVCapParam->AudioDevice, "", MB_OK);
		//OutputDebugStringA(pAVCapParam->AudioDevice);

		if (p)
		{
			g_pVideoParam = p;
			
			TString sTitle = String::AnsiToUnicode(p->VideoDevice); //针对摄像头设备名称做唯一标识

			hMsgWnd = Window::CreateSimpleWndow(NULL, VIDEO_CAP_WND_CLASS, sTitle.c_str(), WS_POPUPWINDOW, _VideoCapWndProc);

			OutputDebugString(L"<<<<摄像头开始采集");
			CAVCapDLL::Init();

			OutputDebugStringA(p->VideoDevice);
			CAVCapDLL::SetProp("VideoDevice", p->VideoDevice);
			OutputDebugStringA(p->VideoCompressor);
			CAVCapDLL::SetProp("VideoCompressor", p->VideoCompressor);
			OutputDebugStringA(p->AudioDevice);
			CAVCapDLL::SetProp("AudioDevice", p->AudioDevice);
			OutputDebugStringA(p->AudioCompressor);
			CAVCapDLL::SetProp("AudioCompressor", p->AudioCompressor);
			OutputDebugStringA(p->VideoResolution);
			CAVCapDLL::SetProp("VideoResolution", p->VideoResolution);

			OutputDebugStringA(String::IntToStrA(p->FrameRate).c_str());
			CAVCapDLL::SetFrameRate(p->FrameRate);

			OutputDebugStringA(p->FileName);
			CAVCapDLL::StartVideoCap(p->FileName, p->IsAudio);

			if (p->IsPreview)
			{
				CCamRecPreviewWnd* m_pWnd = new CCamRecPreviewWnd(hMsgWnd);
				if (m_pWnd != NULL)
				{
					//OutputDebugStringA("create succed");
					//gUnify->GetPaintManager()->GetPaintWindow()
					//m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
					m_pWnd->SetVideCapParam(p);
					CDuiString sCaption = MAIN_TITLE;
					sCaption += _T("正在录像");
					m_pWnd->Create(NULL, sCaption.GetData(), UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标
				
					

					/*string sRatio = p->VideoResolution;
					if (sRatio == "") sRatio = "640x480";
					string sWdith = String::GetSplitStringA(sRatio, 'x', 0);
					string sHeight = String::GetSplitStringA(sRatio, 'x', 1);
					int nDestBmpWidth = String::AStrToInt(sWdith);
					int nDestBmpHeight = String::AStrToInt(sHeight);*/
					CAVCapDLL::StartPreview(m_pWnd->GetVideoCapPre(), 640, 480);
					
					m_pWnd->Restore();
					
					m_pWnd->ShowWindow();
				}
			}

			nSec = p->CapTime;
		}
	}

	SetEvent(hEvent); //让调用者返回往下走

	OutputDebugString(L"摄像头采集等待......");

	/*CStringA s;
	s.Format("%d", nSec);
	OutputDebugStringA(s.GetBuffer());*/

	bStopped = false;

	//std::thread([&](){
		//OutputDebugString(L"进入线程");
		
	//}).detach();
	TStopParam* p = new TStopParam;
	p->nSec = nSec;
	p->hMsgWnd = hMsgWnd;
	HANDLE m_hThread = CreateThread(NULL, 0, &_StopVideoCapProc, p, 0, NULL);//创建新线程  
	CloseHandle(m_hThread);

	MSG msg = { 0 };
	while (::GetMessage(&msg, NULL, 0, 0)) {
		if (bStopped) {
			return 0;
		}

		::TranslateMessage(&msg);
		try{
			/*if (msg.message == WM_VIDEO_CAP_EXIT)
			{
			OutputDebugStringA("!!收到退出消息2!!");
			if (!bStopped)
			{
			OutputDebugStringA("【强制】停止摄像头");
			CAVCapDLL::StopVideoCap();

			OutputDebugString(L"摄像头采集已【强制】停止");
			CAVCapDLL::Clear();
			}

			break;
			}*/

			::DispatchMessage(&msg);
		}
		catch (...) {
		}

		Sleep(0);
	}

	OutputDebugString(L"摄像头采集进程已退出>>>>");

	return 0;
}

CScrRecExec::CScrRecExec()
{
	m_hMapFile = ::CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0,
		sizeof(TScrRecParam), SCR_CAP_FILE_MAP);

	m_pShareMem = (TScrRecParam*)MapViewOfFile(m_hMapFile, FILE_MAP_WRITE, 0, 0, 0);

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, SCR_CAP_EVENT); //存在则返回存在的句柄
}

CScrRecExec::~CScrRecExec()
{
	UnmapViewOfFile(m_pShareMem);
	CloseHandle(m_hMapFile);

	CloseHandle(m_hEvent);
}

void CScrRecExec::Exec(ITasker* p)
{
	TScrRecParam param;
	param.Monitor = p->GetParamInt(_T("cmbScrRecMon"), 0);

	//CScrRecProc::Stop(); //先停止上一个

	TString s;
	s = p->GetParam(_T("cmbScrRecVideoComp"));
	strcpy(param.VideoCompressor, String::UnicodeToAnsi(s.c_str()).c_str());

	s = p->GetParam(_T("cmbScrRecMic"));
	strcpy(param.AudioDevice, String::UnicodeToAnsi(s.c_str()).c_str());

	s = p->GetParam(_T("cmbScrRecAudioComp"));
	strcpy(param.AudioCompressor, String::UnicodeToAnsi(s.c_str()).c_str());

	string sFileName = String::UnicodeToAnsi(p->GetParam(_T("edtScrRecDir")).c_str());
	if (sFileName.length() == 0)
		sFileName = String::UnicodeToAnsi(gUnify->App()->Path()->ExePath.c_str());

	if (sFileName.rfind('\\') > 0) sFileName += "\\";

	//SYSTEMTIME t;
	//GetSystemTime(&t);
	CTime Now = CTime::GetCurrentTime();
	CString sNow = Now.Format(_T("%Y-%m-%d %H时%M分%S秒.asf"));//%03d毫秒
	//sNow.Format(_T("%02d-%02d-%02d %02d时%02d分%02d秒.avi"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	sFileName += String::UnicodeToAnsi(sNow.GetBuffer());

	strcpy(param.FileName, sFileName.c_str());

	param.FrameRate = p->GetParamInt(_T("edtScrRecFrameRate"), 30);
	param.WmvVer = p->GetParamInt(_T("WmvVer"), 0);

	UINT nSec = p->GetParamInt(_T("edtScrRecTime"), 10) * 60;
	param.CapTime = nSec;
	param.IsAudio = p->GetParamInt(_T("chkScrRecAudio"), 0);

	memcpy(m_pShareMem, &param, sizeof(TScrRecParam));

	//启动进程，等待进程处理完成
	// s = gUnify->App()->Path()->ExeFullPath;
	THREAD_BEGIN
		ShellExecute(NULL, _T("open"), gUnify->App()->Path()->ExeFullPath.c_str(), _T("scrrec"), gUnify->App()->Path()->ExePath.c_str(), SW_HIDE);
	THREAD_END

		WaitForSingleObject(m_hEvent, 10000);
}

BOOL CALLBACK EnumWindowsProcScrRec(HWND hwnd, LPARAM lparam)
{
	TCHAR lpWinTitle[MAX_PATH];
	::GetWindowText(hwnd, lpWinTitle, MAX_PATH - 1);
	TString strTitle = lpWinTitle;
	//LOGCON(strTitle.c_str());

	if (strTitle == LPCTSTR(lparam))
	{
//		LOGCON(L"窗口已经找到");

		::SendMessage(hwnd, WM_SCR_CAP_EXIT, 0, 0);

		return FALSE;
	}
	return TRUE;
}

void CScrRecProc::Stop()
{
	EnumWindows(EnumWindowsProcScrRec, LPARAM(_T("ScreenRecord")));
}

static LRESULT CALLBACK	 _ScrRecWndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	if (wMsg == WM_SCR_CAP_EXIT)
	{
		OutputDebugStringA("!!屏幕录像收到退出消息!!");
		if (!bStopped)
		{
			OutputDebugStringA("【强制】停止屏幕录像");
			CAVCapDLL::StopScreenRecord();

			OutputDebugString(L"屏幕录像已【强制】停止");
			CAVCapDLL::Clear();
		}

		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

DWORD WINAPI _StopScrRecProc(LPVOID lpParam) {
	TStopParam* p = static_cast<TStopParam*>(lpParam);

	UINT nSec = p->nSec;
	HWND hMsgWnd = p->hMsgWnd;

	delete p;

	::Sleep((nSec + 1) * 1000);

	// OutputDebugString(L"线程休眠完毕");

	try {
		CAVCapDLL::StopScreenRecord();

		OutputDebugString(L"屏幕录像已停止===");
		CAVCapDLL::Clear();

	}
	catch (...)
	{

	}

	OutputDebugString(L"屏幕录像已清理||||||||||");

	::PostMessage(hMsgWnd, WM_QUIT, 0, 0);
	::PostMessage(hMsgWnd, WM_SCR_CAP_EXIT, 0, 0);

	bStopped = true;

	return 0;
}

int CScrRecProc::Proc()
{
	UINT nSec = 0;
	HWND hMsgWnd = NULL;

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, SCR_CAP_EVENT);

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, SCR_CAP_FILE_MAP);
	if (hMapFile)
	{
		//OutputDebugStringA("open map  succ");
		//OutputDebugStringA("open file map succed");
		TScrRecParam* p = (TScrRecParam*)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
		//MessageBoxA(NULL, pAVCapParam->AudioDevice, "", MB_OK);
		//OutputDebugStringA(pAVCapParam->AudioDevice);

		if (p)
		{
			TString sTitle = _T("ScreenRecord");

			hMsgWnd = Window::CreateSimpleWndow(NULL, SCR_CAP_WND_CLASS, sTitle.c_str(), WS_POPUPWINDOW, _ScrRecWndProc);
			//if (hMsgWnd > 0) OutputDebugStringA("msgwnd create succ");

			OutputDebugString(L"<<<<开始屏幕录像");
			CAVCapDLL::Init();

			int n = p->Monitor;
			if (n < 0) n = 0;
			CAVCapDLL::SetScreenRecMon(n);

			/*OutputDebugStringA(p->VideoCompressor);
			CAVCapDLL::SetProp("VideoCompressor", p->VideoCompressor);*/
			if (p->IsAudio)
			{
				OutputDebugStringA(p->AudioDevice);
				CAVCapDLL::SetProp("AudioDevice", p->AudioDevice);
				OutputDebugStringA(p->AudioCompressor);
				CAVCapDLL::SetProp("AudioCompressor", p->AudioCompressor);
			}

			CAVCapDLL::SetFrameRate(p->FrameRate);
			CAVCapDLL::SetWmvVer(p->WmvVer);
			CAVCapDLL::StartScreenRecord(p->FileName, p->IsAudio);

			nSec = p->CapTime;
		}
	}

	SetEvent(hEvent); //让调用者返回往下走

	OutputDebugString(L"屏幕录像等待......");


	bStopped = false;

	//std::thread([&](){
		//OutputDebugString(L"进入线程");
		
	//}).detach();
	TStopParam* p = new TStopParam;
	p->nSec = nSec;
	p->hMsgWnd = hMsgWnd;
	HANDLE m_hThread = CreateThread(NULL, 0, &_StopScrRecProc, p, 0, NULL);//创建新线程  
	CloseHandle(m_hThread);

	MSG msg = { 0 };
	while (::GetMessage(&msg, NULL, 0, 0)) {
		if (bStopped) {
			return 0;
		}

		::TranslateMessage(&msg);
		try{
			::DispatchMessage(&msg);
		}
		catch (...) {
		}

		Sleep(0);
	}

	OutputDebugString(L"屏幕录像进程已退出>>>>");

	return 0;
}

CAudioCapExec::CAudioCapExec()
{
	m_hMapFile = ::CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0,
		sizeof(TAudioCapParam), AUDIO_CAP_FILE_MAP);

	m_pShareMem = (TAudioCapParam*)MapViewOfFile(m_hMapFile, FILE_MAP_WRITE, 0, 0, 0);

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, AUDIO_CAP_EVENT); //存在则返回存在的句柄
}

CAudioCapExec::~CAudioCapExec()
{
	UnmapViewOfFile(m_pShareMem);
	CloseHandle(m_hMapFile);

	CloseHandle(m_hEvent);
}

void CAudioCapExec::Exec(ITasker* p)
{
	TAudioCapParam param;
	TString s = p->GetParam(_T("cmbAudioCapMic"));
	strcpy(param.AudioDevice, String::UnicodeToAnsi(s.c_str()).c_str());

	CAudioCapProc::Stop(s.c_str()); //先停止上一个

	s = p->GetParam(_T("cmbAudioCapComp"));
	strcpy(param.AudioCompressor, String::UnicodeToAnsi(s.c_str()).c_str());

	string sFileName = String::UnicodeToAnsi(p->GetParam(_T("edtAudioCapDir")).c_str());
	if (sFileName.length() == 0)
		sFileName = String::UnicodeToAnsi(gUnify->App()->Path()->ExePath.c_str());

	if (sFileName.rfind('\\') > 0) sFileName += "\\";

	//SYSTEMTIME t;
	//GetSystemTime(&t);
	CTime Now = CTime::GetCurrentTime();
	CString sNow = Now.Format(_T(" %Y-%m-%d %H时%M分%S秒.mp3"));//%03d毫秒
	//sNow.Format(_T("%02d-%02d-%02d %02d时%02d分%02d秒.avi"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	sFileName += param.AudioDevice;
	sFileName += String::UnicodeToAnsi(sNow.GetBuffer());

	strcpy(param.FileName, sFileName.c_str());

	UINT nSec = p->GetParamInt(_T("edtAudioCapTime"), 10) * 60;
	param.CapTime = nSec;

	memcpy(m_pShareMem, &param, sizeof(TAudioCapParam));

	//启动进程，等待进程处理完成
	// s = gUnify->App()->Path()->ExeFullPath;
	THREAD_BEGIN
		ShellExecute(NULL, _T("open"), gUnify->App()->Path()->ExeFullPath.c_str(), _T("audiocap"), gUnify->App()->Path()->ExePath.c_str(), SW_HIDE);
	THREAD_END

		WaitForSingleObject(m_hEvent, 10000);
}

static LRESULT CALLBACK	 _AudioCapWndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	if (wMsg == WM_AUDIO_CAP_EXIT)
	{
		OutputDebugStringA("!!收到退出消息!!");
		if (!bStopped)
		{
			OutputDebugStringA("【强制】停止麦克风采集");
			CAVCapDLL::StopAudioCap();

			OutputDebugString(L"麦克风采集已【强制】停止");
			CAVCapDLL::Clear();
		}

		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProcAudioCap(HWND hwnd, LPARAM lparam)
{
	TCHAR lpWinTitle[MAX_PATH];
	::GetWindowText(hwnd, lpWinTitle, MAX_PATH - 1);
	TString strTitle = lpWinTitle;
	//LOGCON(strTitle.c_str());

	if (strTitle == LPCTSTR(lparam))
	{
//		LOGCON(L"窗口已经找到");

		::SendMessage(hwnd, WM_AUDIO_CAP_EXIT, 0, 0);

		return FALSE;
	}
	return TRUE;
}

void CAudioCapProc::Stop(LPCTSTR pszDevName)
{
	EnumWindows(EnumWindowsProcAudioCap, LPARAM(pszDevName));
}

DWORD WINAPI _StopAudioCapProc(LPVOID lpParam) {
	TStopParam* p = static_cast<TStopParam*>(lpParam);

	UINT nSec = p->nSec;
	HWND hMsgWnd = p->hMsgWnd;

	delete p;

	::Sleep((nSec + 1) * 1000);

	// OutputDebugString(L"线程休眠完毕");

	try {
		CAVCapDLL::StopAudioCap();

		OutputDebugString(L"麦克风采集已停止===");
		CAVCapDLL::Clear();

	}
	catch (...)
	{

	}

	OutputDebugString(L"麦克风采集已清理||||||||||");

	::PostMessage(hMsgWnd, WM_QUIT, 0, 0);
	::PostMessage(hMsgWnd, WM_AUDIO_CAP_EXIT, 0, 0);

	bStopped = true;

	return 0;
}

int CAudioCapProc::Proc()
{
	UINT nSec = 0;
	HWND hMsgWnd = NULL;

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, AUDIO_CAP_EVENT);

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, AUDIO_CAP_FILE_MAP);
	if (hMapFile)
	{
		//OutputDebugStringA("open file map succed");
		TAudioCapParam* p = (TAudioCapParam*)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
		//MessageBoxA(NULL, pAVCapParam->AudioDevice, "", MB_OK);
		//OutputDebugStringA(pAVCapParam->AudioDevice);

		if (p)
		{
			TString sTitle = String::AnsiToUnicode(p->AudioDevice); //针对设备名称做唯一标识

			hMsgWnd = Window::CreateSimpleWndow(NULL, AUDIO_CAP_WND_CLASS, sTitle.c_str(), WS_POPUPWINDOW, _AudioCapWndProc);

			OutputDebugString(L"<<<<麦克风开始采集");
			CAVCapDLL::Init();

			OutputDebugStringA(p->AudioDevice);
			CAVCapDLL::SetProp("AudioDevice", p->AudioDevice);
			OutputDebugStringA(p->AudioCompressor);
			CAVCapDLL::SetProp("AudioCompressor", p->AudioCompressor);

			CAVCapDLL::StartAudioCap(p->FileName);

			nSec = p->CapTime;
		}
	}

	SetEvent(hEvent); //让调用者返回往下走

	OutputDebugString(L"麦克风采集等待......");

	/*CStringA s;
	s.Format("%d", nSec);
	OutputDebugStringA(s.GetBuffer());*/

	bStopped = false;

	
	//std::thread([&](){
		//OutputDebugString(L"进入线程");
		
//	}).detach();
	TStopParam* p = new TStopParam;
	p->nSec = nSec;
	p->hMsgWnd = hMsgWnd;
	HANDLE m_hThread = CreateThread(NULL, 0, &_StopAudioCapProc, p, 0, NULL);//创建新线程  
	CloseHandle(m_hThread);

	MSG msg = { 0 };
	while (::GetMessage(&msg, NULL, 0, 0)) {
		if (bStopped) {
			return 0;
		}

		::TranslateMessage(&msg);
		try{
			::DispatchMessage(&msg);
		}
		catch (...) {
		}

		Sleep(0);
	}

	OutputDebugString(L"麦克风采集进程已退出>>>>");

	return 0;
}

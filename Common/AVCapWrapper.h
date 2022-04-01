#pragma once


const TCHAR* const VIDEO_CAP_FILE_MAP = _T("5E5A2515-8B28-4283-9F22-33D64D33F217-AVCapVideoFileMap");
const TCHAR* const VIDEO_CAP_EVENT = _T("C0911C5C-64B0-4FEC-ABFC-1C57DD500137-AVCapVideoEvent");
const TCHAR* const VIDEO_CAP_WND_CLASS = _T("E8E54F32-1F61-491F-A07E-DB971A89DA87-AVCapVideoWndProcClass");

const TCHAR* const AUDIO_CAP_FILE_MAP = _T("5E5A2515-8B28-4283-9F22-33D64D33F217-AVCapAudioFileMap");
const TCHAR* const AUDIO_CAP_EVENT = _T("C0911C5C-64B0-4FEC-ABFC-1C57DD500137-AVCapAudioEvent");
const TCHAR* const AUDIO_CAP_WND_CLASS = _T("E8E54F32-1F61-491F-A07E-DB971A89DA87-AVCapAudioWndProcClass");

const TCHAR* const SCR_CAP_FILE_MAP = _T("1E5A2515-8B28-4283-9F22-33D64D33F217-AVCapScrRecFileMap");
const TCHAR* const SCR_CAP_EVENT = _T("5F34A769 - 206C - 4053 - A069 - B6E35CDA353E-AVCapScrRecEvent");
const TCHAR* const SCR_CAP_WND_CLASS = _T("18E54F32-1F61-491F-A07E-DB971A89DA87-AVCapScrRecWndProcClass");

const int WM_VIDEO_CAP_EXIT = WM_USER + 917;
const int WM_SCR_CAP_EXIT = WM_USER + 918;
const int WM_AUDIO_CAP_EXIT = WM_USER + 919;
const int WM_VIDEO_CAP_CAPTURE_PICTURE = WM_USER + 920;

typedef struct  _AVCapVideoParam
{
	char VideoDevice[MAX_PATH];
	char VideoCompressor[MAX_PATH];
	char AudioDevice[MAX_PATH];
	char AudioCompressor[MAX_PATH];
	char VideoResolution[MAX_PATH];
	char FileName[MAX_PATH];
	UINT FrameRate;
	UINT CapTime;
	bool IsAudio;
	bool IsPreview;
} TVideoCapParam;

typedef struct  _AVCapAudioParam
{
	char AudioDevice[MAX_PATH];
	char AudioCompressor[MAX_PATH];
	char FileName[MAX_PATH];
	UINT CapTime;
} TAudioCapParam;

typedef struct  _AVCapScrRecParam
{
	char VideoCompressor[MAX_PATH];
	char AudioDevice[MAX_PATH];
	char AudioCompressor[MAX_PATH];
	char FileName[MAX_PATH];
	int Monitor;
	UINT WmvVer;
	UINT FrameRate;
	UINT CapTime;
	bool IsAudio;
} TScrRecParam;

class ITasker;
class CScrRecExec
{
	HANDLE m_hEvent;
	HANDLE m_hMapFile;
	TScrRecParam* m_pShareMem;
public:
	CScrRecExec();
	~CScrRecExec();

	void Exec(ITasker* p);
};

class CScrRecProc
{
public:
	static void Stop();
	static int Proc();
};

class CAudioCapExec
{
	HANDLE m_hEvent;
	HANDLE m_hMapFile;
	TAudioCapParam* m_pShareMem;
public:
	CAudioCapExec();
	~CAudioCapExec();

	void Exec(ITasker* p);
};

class CAudioCapProc
{
public:
	static void Stop(LPCTSTR pszDevName);
	static int Proc();
};

class CVideoCapExec
{
	HANDLE m_hEvent;
	HANDLE m_hMapFile;
	TVideoCapParam* m_pShareMem;
public:
	CVideoCapExec();
	~CVideoCapExec();

	void Exec(ITasker* p);
};

class CVideoCapProc
{
public:
	static void Stop(LPCTSTR pszDevName);
	static int Proc();
};

class CAVCapDLL
{
	typedef void (FAR __stdcall *ProcInit)(LPCSTR ID);
	typedef void (FAR __stdcall *ProcClear)();
	typedef void (FAR __stdcall *ProcGet)(LPCSTR pszCat, LPWSTR pszRet);
	typedef void (FAR __stdcall *ProcSet)(LPCSTR pszCat, LPCSTR pszText);
	typedef void (FAR __stdcall *ProcShowSetup)(LPCSTR pszCat);
	typedef void (FAR __stdcall *ProcStartPreview)(HWND hShow, int nWidth, int nHeight);
	typedef void (FAR __stdcall *ProcStopPreview)();
	typedef void (FAR __stdcall *ProcStartVideoCap)(LPCSTR pszFileName, bool bAudio);
	typedef void (FAR __stdcall *ProcStopVideoCap)();
	typedef void (FAR __stdcall *ProcSetAutoRefreshPreview)(bool b);
	typedef void (FAR __stdcall *ProcSetVideoWindowFloat)(bool b);
	typedef void (FAR __stdcall *ProcStartScreenRec)(LPCSTR pszFileName, bool bAudio);
	typedef void (FAR __stdcall *ProcStopScreenRec)();
	typedef void (FAR __stdcall *ProcStartAudioCap)(LPCSTR pszFileName);
	typedef void (FAR __stdcall *ProcStopAudioCap)();
	typedef void (FAR __stdcall *ProcSetVideoSource)(int nSource);
	typedef void (FAR __stdcall *ProcSetScrRecMon)(int nSource);
	typedef void (FAR __stdcall *ProcSetFrameRate)(int n);
	typedef void (FAR __stdcall *ProcSetWmvVer)(int n);
	typedef void (FAR __stdcall *ProcCapturePicture)(LPCSTR pszFileName);
public:
	static void Init();
	static void Clear();
	static void GetProp(LPCSTR pszCat, LPWSTR pszRet);
	static void SetProp(LPCSTR pszCat, LPCSTR pszText);
	static void ShowSetup(LPCSTR pszCat);
	static void StartPreview(HWND hShow, int nWidth, int nHeight);
	static void StopPreview();
	static void StartVideoCap(LPCSTR pszFileName, bool bAudio);
	static void StopVideoCap();
	static void SetAutoRefreshPreview(bool b);
	static void SetVideoWindowFloat(bool b);
	static void ShowMicSet();
	static void StartScreenRecord(LPCSTR pszFileName, bool bAudio);
	static void StopScreenRecord();
	static void StartAudioCap(LPCSTR pszFileName);
	static void StopAudioCap();
	static void SetVideoSource(int nSource);
	static void SetScreenRecMon(int nSource);
	static void SetFrameRate(int n);
	static void SetWmvVer(int n);
	static void CapturePicture(LPCSTR pszFileName);
};


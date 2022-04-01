//************************************
//
//Facade pattern
//Can used in plugins based program
//支持配置功能，参见ENABLE宏
//Author: LX.g
//************************************

#pragma once;



const TCHAR * const DB_TEST_NAME = _T("test.db");
const TCHAR * const DB_SYS_NAME = _T("Config.dat");
const TCHAR * const DB_PLUG_NAME = _T("Plugs.dat");


#define ENABLE_SINGLETON_APP
#define ENABLE_TRAY
#define  ENABLE_PLUG
#define  ENABLE_TIMER
#define ENABLE_EVENT
#define ENABLE_INI_CFG 
//#define ENABLE_SQLITE 
//#define ENABLE_XML_CFG
//#define ENABLE_THREAD_POOL
//#define ENABLE_MFC
//#define ENABLE_WTL
#define ENABLE_GDIP


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef ENABLE_PLUG
#include "PlugDefine.h"
#endif

//前置声明去耦合
class CAppPath;
class CCmdLine;
class CStatus;
#ifdef ENABLE_TRAY
class CTrayIcon;
#endif
class IApp
{
public:
	HWND m_MainWnd;
	virtual~IApp() {};
	virtual CAppPath* Path() = 0;
	virtual CCmdLine* CmdLine() = 0;
	virtual CStatus* Status() = 0;
#ifdef ENABLE_TRAY
	virtual CTrayIcon* Tray() = 0;
#endif
};

#ifdef ENABLE_TIMER
class CSimpleTimer;
#endif
#ifdef ENABLE_EVENT
class CEventManager;
#endif

class IMedia {
public:
	virtual void ShowVolumeWindow() = 0;
};

class CHotKey;
class ICore
{
public:
	virtual~ICore() {};
#ifdef ENABLE_TIMER
	virtual CSimpleTimer* Timer() = 0;
#endif
#ifdef ENABLE_EVENT
virtual CEventManager* Event() = 0;
#endif
	
virtual IMedia* Media() = 0;

#ifdef ENABLE_HOTKEY
    virtual CHotKey* HotKey() = 0;
#endif 

};

#ifdef ENABLE_INI_CFG
class CIniFile;
#endif
#ifdef ENABLE_SQLITE
class CTestDB;
#endif
class IDB
{
public:
	virtual~IDB() {};
#ifdef ENABLE_INI_CFG
	virtual CIniFile* SysConfig() = 0;
#ifdef ENABLE_PLUG
	virtual CIniFile* PlugDB() = 0;
#endif
#endif
	//virtual CIniFile* UserConfig() = 0;
#ifdef ENABLE_SQLITE
	virtual CTestDB *TestDB() = 0;
#endif

};

enum CSkinChangeType {sctBack, sctAlpha, sctColor};
#ifdef ENABLE_PLUG
static UINT NEAR WM_PLUG_WND_CLOSE = RegisterWindowMessage(_T("WM_PLUG_WND_CLOSE"));
#endif

class IUI
{
public:
	IUI() {};
	virtual ~IUI() {};

	virtual UINT ShowMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT utype) = 0;
	virtual void AddSkinChangeObserver(/*ISkinChangeReceiver *p, */CPaintManagerUI* pm) = 0;
	virtual void RemoveSkinChangeObserver(CPaintManagerUI* pm) = 0;
	virtual void BroadcastSkinChange(CSkinChangeType sct, LPCTSTR szParam) = 0;
	virtual TString GetBKImage() = 0;
};

class INetManager {
public:
	virtual bool IsNetAlive() = 0;
};

class IUnify
{
public:
	virtual~IUnify() {};
	virtual IApp* App() = 0;
	virtual ICore* Core() = 0;
	virtual IDB* DB() = 0;
	virtual IUI* UI() = 0;
	virtual INetManager* Net() = 0;
#ifdef ENABLE_PLUG
	virtual IPlugManager* PlugManager() = 0;
#endif
};
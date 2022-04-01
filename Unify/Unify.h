//************************************
//
//MainIntf实现
//
//Author: LX.g
//***

#pragma once

#include "StdAfx.h"
#include "AppPath.h"
#include "Status.h"
#include "UnifyIntf.h"
#include "NetManager.h"
#include "LogHelper.h"

#ifdef ENABLE_EVENT
#include "Event.h"
#endif

#ifdef ENABLE_TIMER
#include "SimpleTimer.h"
#endif

#include "CmdLine.h"

#ifdef ENABLE_INI_CFG
#include "IniFile.h"
#endif

#ifdef ENABLE_TRAY
#include "TrayIcon.h"
#endif

#ifdef ENABLE_PLUG
#include "PlugManager.h"
#endif

#ifdef ENABLE_SQLITE
#include "../[Common]/CppSQLite/CppSQLite3.h"
#pragma comment(lib, "../[Common]/CppSQLite/sqlite3.lib")
#endif

#ifdef ENABLE_HOTKEY
#include "../[Common]/HotKey/include/HotKeyExport.h"
#include "HotkeyManager.h"
#endif

class CApp :public IApp
{
	LAZY_CREATE_IF_NULL(CCmdLine, CCmdLine, CmdLine)
#ifdef ENABLE_TRAY
	LAZY_CREATE_IF_NULL(CTrayIcon, CTrayIcon, Tray)
#endif
public:
	CApp() : 
#ifdef ENABLE_TRAY
		m_pTray(NULL),
#endif
	    m_pCmdLine(NULL)
	{};

	virtual~CApp() { 
		delete GlobalPath;
		delete m_pCmdLine;
		delete GlobalStatus;

#ifdef ENABLE_TRAY
		delete m_pTray;
#endif
	};

	virtual CAppPath* Path()
	{
		return GlobalPath;
	}

	virtual CStatus* Status()
	{
		return GlobalStatus;
	}
};




extern IUnify* gUnify;

class CCore : public ICore
{
#ifdef ENABLE_TIMER
	CSimpleTimer* m_pTimer;
#endif
#ifdef ENABLE_EVENT
	CEventManager* m_pEvent;
#endif
#ifdef ENABLE_HOTKEY
	CHotKey* m_pHotKey;
#endif 

	IMedia* m_pMidea;
public:
	CCore();;
	virtual ~CCore() {
#ifdef ENABLE_HOTKEY
		delete m_pHotKey;
#endif
#ifdef ENABLE_TIMER
delete m_pTimer; 
#endif

#ifdef ENABLE_EVENT
delete m_pEvent;
#endif
		
delete m_pMidea;
	}

#ifdef ENABLE_TIMER
	virtual CSimpleTimer* Timer() {
		if (!m_pTimer)
			//m_pTimer = CSimpleTimer::GetInstance(gUnify->App()->m_MainWnd);
		   m_pTimer = new CSimpleTimer(gUnify->App()->m_MainWnd);
		
		return m_pTimer;
		/*if (!m_pTimer)
		{
		m_pTimer = new CSimpleTimer;
		}

		return m_pTimer;*/
	};
#endif
#ifdef ENABLE_EVENT
	virtual CEventManager* Event() {
		if (!m_pEvent)
			m_pEvent = new CEventManager;;
		return m_pEvent;
		//if (!m_pEvent)
		//{
		//	m_pEvent = //new CEventManager;
		//}

		//return m_pEvent;
	};
#endif

#ifdef ENABLE_HOTKEY
	virtual CHotKey* HotKey() {
		if (!m_pHotKey)
			m_pHotKey = new CHotKey;
		return m_pHotKey;
	};
#endif

	
	virtual IMedia* Media();

};

#ifdef ENABLE_SQLITE
//adapter
class CTestDB : public CppSQLite3DB
{
public:
	void Open() {
		USES_CONVERSION;

		this->open(T2A((GlobalPath->UserPath + DB_TEST_NAME).c_str()));
	}

	void Close()
	{
		this->close();
	}
};
#endif

class CDB : public IDB
{
	CDB* _P_Reserve_;
#ifdef ENABLE_INI_CFG
	CIniFile* m_pSysIni;
#ifdef ENABLE_PLUG
	CIniFile* m_pPlugDB;
#endif
#endif

#ifdef ENABLE_SQLITE
	LAZY_CREATE_IF_NULL(CTestDB, CTestDB, TestDB)
#endif
	
public:
	CDB() : _P_Reserve_(NULL)
#ifdef ENABLE_INI_CFG
		, m_pSysIni(NULL)
#ifdef ENABLE_PLUG
		, m_pPlugDB(NULL)
#endif
#endif
#ifdef ENABLE_SQLITE
		, m_pTestDB(NULL)
#endif
	{
		LOGCON("create db");
	};

	virtual~CDB() {
#ifdef ENABLE_INI_CFG
		delete m_pSysIni;

#ifdef ENABLE_PLUG
		delete m_pPlugDB;
#endif
#endif
		
#ifdef ENABLE_SQLITE
		m_pTestDB->Close();
		delete m_pTestDB;
#endif
	};

#ifdef ENABLE_INI_CFG
	virtual CIniFile* SysConfig() {
		if (!m_pSysIni)
		{
			LOGCON("sysini init");
			CAppPath path;
			
			m_pSysIni = new CIniFile((path.UserPath + DB_SYS_NAME).c_str());
		}
		return m_pSysIni;
	}

#ifdef ENABLE_PLUG
	virtual CIniFile* PlugDB() {
		if (!m_pPlugDB)
		{
			CAppPath path;
			m_pPlugDB = new CIniFile((path.UserPath + DB_PLUG_NAME).c_str());
		}

		return m_pPlugDB;
	}
#endif
#endif
	
};



class CUI : public IUI
{
	virtual TString GetBKImage()
	{
		return gUnify->DB()->SysConfig()->GetString(_T("Skin"), _T("BkImage"), _T("BKImage/0.png"));
	}

	void ApplySkinFromCfg(CPaintManagerUI* pm)
	{
		assert(pm);
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(pm->FindControl(_T("vertBack")));
		assert(pVert);

		UINT nAlpha = 255 - gUnify->DB()->SysConfig()->GetInt(_T("Skin"), _T("Alpha"), 0);
		pm->SetTransparent(nAlpha);

		if (gUnify->DB()->SysConfig()->GetInt(_T("Skin"), _T("Color"), -1) == -1) //木有设置颜色
		{
			pVert->SetBkImage(gUnify->DB()->SysConfig()->GetString(_T("Skin"), _T("BkImage"), _T("BKImage/0.png")));
		}
		else
		{
			CString sClr = gUnify->DB()->SysConfig()->GetString(_T("Skin"), _T("Color"), L"0xFF0080C0");
			DWORD dwClr = Util::String::StrToInt64(sClr.GetBuffer());
			pVert->SetBkImage(L"");
			pVert->SetBkColor(dwClr);
		}
	}

	//map<ISkinChangeReceiver*, CPaintManagerUI*> SkinChangeReceivers;
	vector<CPaintManagerUI*> SkinChangeReceivers;
public:
	UINT ShowMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT utype)
	{
		return 0;// CMsgBoxWnd::CreateInstance(hWnd, lpText, lpCaption, utype);
	}

	virtual void AddSkinChangeObserver(/*ISkinChangeReceiver *p,*/ CPaintManagerUI* pm)
	{
		//mapSkinChangeReceivers[p] = pm;
		SkinChangeReceivers.push_back(pm);

		ApplySkinFromCfg(pm);
	}

	virtual void RemoveSkinChangeObserver(CPaintManagerUI* pm)
	{
		SkinChangeReceivers.erase(std::remove(SkinChangeReceivers.begin(), SkinChangeReceivers.end(), pm), SkinChangeReceivers.end());
	}

	virtual void BroadcastSkinChange(CSkinChangeType sct, LPCTSTR szParam)
	{
		for (auto& item : SkinChangeReceivers)
		{
			//ISkinChangeReceiver* p = item.first;
			CPaintManagerUI* pm = item;
			
			switch (sct)
			{
			case sctBack:
				static_cast<CVerticalLayoutUI*>(pm->FindControl(L"vertBack"))->SetBkImage(szParam);
				break;
			case sctAlpha:
				pm->SetTransparent(255 - Util::String::StrToInt(szParam));
				break;
			case sctColor:
				static_cast<CVerticalLayoutUI*>(pm->FindControl(L"vertBack"))->SetBkImage(_T(""));
				static_cast<CVerticalLayoutUI*>(pm->FindControl(L"vertBack"))->SetBkColor(Util::String::StrToInt64(szParam));
				break;
			default:
				break;
			}
		}
	}
};


class CUnify : public IUnify
{
	LAZY_CREATE_IF_NULL(INetManager, CNetManager, Net)
	LAZY_CREATE_IF_NULL(IApp, CApp, App)
	LAZY_CREATE_IF_NULL(ICore, CCore, Core)
	LAZY_CREATE_IF_NULL(IDB, CDB, DB)
//private:
//	CDB* m_pDB;
//public:
//	CDB* DB() {
//		LOGCON("get db");
//		if (!m_pDB)
//			m_pDB = new CDB;
//		return m_pDB;
//	}
	LAZY_CREATE_IF_NULL(IUI, CUI, UI)
#ifdef ENABLE_PLUG
	LAZY_CREATE_IF_NULL(IPlugManager, CPlugManager, PlugManager)
#endif
public:
	CUnify() : m_pApp(NULL), m_pCore(NULL), m_pDB(NULL), m_pUI(NULL), m_pNet(NULL)/*, m_pFrame(NULL)*/
#ifdef ENABLE_PLUG
		, m_pPlugManager(NULL)
#endif
	{
		LOGCON("create unify");
	};

	virtual ~CUnify() { 
		delete m_pApp; 
		delete m_pCore;
		delete m_pDB;
		delete m_pUI;
		delete m_pNet;
#ifdef ENABLE_PLUG
		delete m_pPlugManager;
#endif
	};
};

void CreateUnify();

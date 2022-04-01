
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once

//#ifdef __GNUC__
// 怎么都没找到min，max的头文件-_-
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
//#endif

//#define WIN32_LEAN_AND_MEAN	
//#define _CRT_SECURE_NO_DEPRECATE
//#include <afxwin.h>
//#include <afxdtctl.h>
#include <atlstr.h>
#include <atltime.h>
#include <windows.h>
//#include <objbase.h>
#include "../[Common]/DuiLib/UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
#       pragma comment(lib, "../../../[Common]/lib/DuiLib_d.lib")
#       pragma comment(lib, "HotKey.lib")
#else
#       pragma comment(lib, "../../../[Common]/lib/DuiLib.lib")
#       pragma comment(lib, "HotKey.lib")
#endif

#include "GlobalDefine.h"
#include <memory>
#include "Base.h"
#include "Unify/Unify.h"
#include "DuiHelper.h"
#include "EventDefine.h"


//namespace FileUtil = Util::File;
//namespace AppUtil = Util::App;
//namespace EncodeUtil = Util::Encode;
//namespace NetUtil = Util::Net;
//namespace PathUtil = Util::Path;
//namespace RegUtil = Util::Registry;
//namespace ProcUtil = Util::Process;
//namespace SysUtil = Util::System;
//namespace TimeUtil = Util::Time;
//namespace WndUtil = Util::Window;

using namespace Util;
using namespace std;
using namespace DuiHelper;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)

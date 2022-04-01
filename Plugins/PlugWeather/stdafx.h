////// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

#include "../../../[Common]/DuiLib/UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
#       pragma comment(lib, "DuiLib_d.lib")
#       pragma comment(lib, "Http.lib")
//#       pragma comment(lib, "../../../[Common]/FrameBase/jsoncpp/lib/Debug/lib_json.lib")
#else
#       pragma comment(lib, "DuiLib.lib")
#       pragma comment(lib, "Http.lib")
#endif

#       pragma comment(lib, "luadyn.lib")

#include "../../Unify/UnifyIntf.h"
#include "WeatherDefine.h"
#include "../../Unify/PlugDefine.h"
#include "Utility.h"
#include "../../../[Common]/DuiLib/DuiHelper.h"
#include "../../../[Common]/Http/include/Http.h"
#include "../../../[Common]/jsoncpp/include/json/json.h"
#include "../../../[Common]/jsoncpp/include/json/reader.h"
#include "../../../[Common]/jsoncpp/include/json/value.h"



using namespace Util;

extern IUnify *gUnify;
extern TPlugInfo gPlugInfo;
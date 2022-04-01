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
#else
#       pragma comment(lib, "DuiLib.lib")
#       pragma comment(lib, "Http.lib")
#endif

#include "Utility.h"
#include "../../../[Common]/Http/include/Http.h"
#include "Event.h"
#include "../../Unify/UnifyIntf.h"
#include "BootDefine.h"
#include "../../Unify/PlugDefine.h"


using namespace Util;

extern IUnify *gUnify;
extern TPlugInfo gPlugInfo;
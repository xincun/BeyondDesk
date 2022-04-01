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
#else
#       pragma comment(lib, "DuiLib.lib")
#endif

#include "../../Unify/UnifyIntf.h"
#include "NoteDefine.h"
#include "../../Unify/PlugDefine.h"
#include "Utility.h"
#include "AppPath.h"
#include "Base.h"
#include "../../../[Common]/DuiLib/DuiHelper.h"

using namespace Util;

extern TPlugInfo gPlugInfo;
extern IUnify *gUnify;

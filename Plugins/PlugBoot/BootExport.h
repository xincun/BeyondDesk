#ifndef BootExport_h
#define BootExport_h

#include "stdafx.h"

//�����ӿ�
extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p);
extern "C" PLUG_API IPlugBase*  GetPlugIntf();
extern "C" PLUG_API void  FreePlugIntf();

#endif
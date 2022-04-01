#pragma once

#include "StdAfx.h"
#include "Unify.h"

#include "../Common/VolWnd.h"

IUnify* gUnify = NULL;

void CreateUnify()
{
	gUnify = new CUnify;
}

//CTaskerFactory* CFrame::TaskerFactory()
//{
//	return &gTaskerFactory;
//}

//对外隐藏实现，插件里不用引用cpp文件
class CMedia : public IMedia {
public:
	void ShowVolumeWindow();
};

void CMedia::ShowVolumeWindow()
{
	CVolWnd::Show();
}

CCore::CCore() :
#ifdef ENABLE_EVENT
	m_pEvent(NULL)
#endif
#ifdef ENABLE_TIMER
	, m_pTimer(NULL)
#endif
	, m_pMidea(NULL)
{
	CVolWnd::Init(); //必须init，否则在插件里不显示
}

IMedia* CCore::Media()
{
	if (!m_pMidea) {
		m_pMidea = new CMedia;
	}
	return m_pMidea;
}

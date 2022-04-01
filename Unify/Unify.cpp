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

//��������ʵ�֣�����ﲻ������cpp�ļ�
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
	CVolWnd::Init(); //����init�������ڲ���ﲻ��ʾ
}

IMedia* CCore::Media()
{
	if (!m_pMidea) {
		m_pMidea = new CMedia;
	}
	return m_pMidea;
}

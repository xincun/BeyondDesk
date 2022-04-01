//************************************
//���������ڲ�������Ϣ��Ӧ�߼���ÿһ���߼����Դ�UIProxy������Ӧ��������
//Ŀ�꣺֧�ַ��봰���߼���Ϊ����ԭ����Ŀ��ֲ��֧��ԭ���߼���֧��DUI_DECLARE_MESSAGE_MAP
//��һ��Proxy����һ���߼�����Proxy�߼�������XML�������൱��һ���³���
//Author: LX.g
//************************************

#pragma once

#include "stdafx.h"
#include "Core/UIProxy.h"
#include "GlobalDefine.h"

class UIPlugProxy : public UIProxy, public IDialogBuilderCallback
{
	
	bool m_bInited = false;
	bool m_bShowed = false;

	void OnClick(TNotifyUI& msg);
	virtual void OnInit();
	void OnEvent(CEvent* e);
	void OnItemActivate(TNotifyUI& msg);
	void OnHeaderClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI &msg);
	void OnItemClick(TNotifyUI &msg);
	DUI_DECLARE_MESSAGE_MAP()
public:
	

	void TryGetTodayHis();


	UIPlugProxy();
	~UIPlugProxy();

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
};

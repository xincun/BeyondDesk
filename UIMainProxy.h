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
#include "AboutWnd.h"

class CUIPlugHelper
{
	CPaintManagerUI* m_pPaintManager = NULL;
	vector<IUIPlug*> m_PlugList;
public:
	void SetManager(CPaintManagerUI* val) { m_pPaintManager = val; }

	CControlUI* CreateUI(LPCTSTR szPlugName, UINT nIntfIndex = 0)
	{
		TString sPlugName = szPlugName;
		TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(sPlugName.c_str());
		if (pPlugInfo)
		{
			IPlugBase* pPlug = pPlugInfo->GetPlugIntf(nIntfIndex); //��Լ��0��UI plug

			CControlUI* pCtrl = static_cast<IUIPlug*>(pPlug)->GetUI();
			m_PlugList.push_back(static_cast<IUIPlug*>(pPlug));

			return pCtrl;
		}

		return NULL;
	}

	void InitPlugs()
	{
		int i = 0;
		for (auto& item : m_PlugList)
		{
			
			IUIPlug* p = item;
			TString s = p->GetPlugInfo()->FileName;

			LOGCONW(L"load ui plug: %s\n", s.c_str());
			item->Init(m_pPaintManager);
		}
	}
};

class UIMainProxy : public UIProxy, public IDialogBuilderCallback/*, public ISkinChangeReceiver*/
{
	CUIPlugHelper m_TabPlugHelper;
	CAnimationTabLayoutUI* m_pTabMain;
	bool m_bShowed = false;
	CAboutWnd* pAboutWnd;
	BOOL m_TrayFlag;
	//HICON m_hIcon;
	CTrayIcon m_Tray;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	
	void CheckUpdate(bool rightNow = false);
	void ExitApp();

	void SaveStat();
	void LoadStat();
	void ApplyCfg();

	static int CALLBACK ListSortItems(UINT_PTR data1, UINT_PTR data2, UINT_PTR userDef);
	
	void OnClick(TNotifyUI& msg);

	void ToTray();
	void TryShowFirstHideToTrayHint();

	void ShowAboutWnd();

	
	virtual void OnInit();

	void OnEvent(CEvent* e);
	void OnItemActivate(TNotifyUI& msg);
	void OnHeaderClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI &msg);
	void OnItemClick(TNotifyUI &msg);

	DUI_DECLARE_MESSAGE_MAP()
public:
	TMenuCheckList m_CheckList;
	TMenuTextList m_TextList;
	TMenuVisibleList m_VisibleList;

	UIMainProxy();
	~UIMainProxy();
	void OnUISet(unsigned int index = 0);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);
	//virtual void OnSkinChange(CSkinChangeType sct, LPCTSTR szParam);
	
};

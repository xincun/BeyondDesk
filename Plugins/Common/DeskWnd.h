#pragma once
//////////////////////////////////////////////////////////////////////////
#include "../../Unify/UnifyIntf.h"
#include "IniFile.h"
#include "../../Unify/Status.h"
#include "../../Unify/Unify.h"
#include "EventDefine.h"
#include "Event.h"

class CDeskWnd;
static vector<CDeskWnd*> gDeskWndList;

class CDeskWnd : public CWindowWnd/*, public INotifyUI*/
{
	bool m_bApplyed = false;
	bool m_bCtrlPressed = false;
	TString ID;

	void OnTimer(void* pOwner, UINT uiID);

	void StartMouseTrans();
	void StopMouseTrans();

	void ShowOrHide();

	virtual void OnConfig();
protected:
	TMenuCheckList m_CheckList;
	TMenuVisibleList m_VisbileList;

	int m_nTransparent;
	int m_nZoom;
	//bool m_bCustomColor;
	int m_nR;
	int m_nG;
	int m_nB;
	int m_nRS;
	int m_nGS;
	int m_nBS;

	virtual void SaveSize();

	virtual void LoadSize(int defaultWidth, int defaultHeight);

	template <class T>
	void ShowConfigWnd(LPCTSTR szTitle) {
		T* pWnd = new T();

		pWnd->m_pDeskWnd = this;
		pWnd->SetDBSection(GetDBSection()); //为了其它的用，分离开设置
		pWnd->Create(m_hWnd, szTitle, UI_WNDSTYLE_DIALOG | WS_POPUP, WS_EX_APPWINDOW, 0, 0, 0);
		if (pWnd == NULL) return;
		pWnd->CenterWindow();
		//SetWindowPos(pWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE);
		pWnd->ShowModal();

		if (m_CheckList[_T("mnuTop")])
			SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE);
		else
			SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE);
	}
public:
	virtual void LoadBack();

	virtual TString GetDBSection();

	virtual void OnBeforeContextMenu() {}; //初始化菜单状态，弹出前
	virtual void SaveStat(); //保存窗口状态，设置
	virtual void LoadStat(LPCTSTR pstrSection); //加载
	virtual void Init();

	//IUnify *m_pUnify;

	static void Clear(LPCTSTR pstrClassName)
	{
		for (auto& item : gDeskWndList)
		{
			if (IsWindow(item->GetHWND()) && _tcsicmp(item->GetWindowClassName(), pstrClassName) == 0)
			{
				::SendMessage(item->m_hWnd, WM_CLOSE, 0, 0);
			}
		}
	}

	template <class T>
	static T* CreateNew(IToolPlug* parent)
	{
		T *pWnd = new T();
		if (pWnd == NULL) return NULL;

		//pWnd->m_pUnify = p;
		pWnd->Create(NULL, _T(""), WS_VISIBLE | WS_POPUP, NULL, 0, 0, 0, 0);
		pWnd->Init();

		pWnd->CenterWindow();
		pWnd->ShowWindow();
		::InvalidateRect(pWnd->GetHWND(), NULL, TRUE);
		::UpdateWindow(pWnd->GetHWND());
		pWnd->SaveStat();

		gDeskWndList.push_back(pWnd);

		TDeskWindow deskWnd;
		deskWnd.PlugObject = parent;
		deskWnd.WndObject = pWnd;
		gUnify->App()->Status()->DeskWindows.push_back(deskWnd);

		return pWnd;
	}

	template <class T>
	static void RestoreFromDB(LPCTSTR sTag, IToolPlug* parent)
	{
		vector<CString> Sections;
		Sections = gUnify->DB()->PlugDB()->GetSections();

		for (auto& item : Sections)
		{
			if (item.Left(_tcslen(sTag)) == sTag)
			{
				T *pWnd = new T();
				if (pWnd == NULL) continue;;

				//pWnd->m_pUnify = p;
				pWnd->Create(NULL, _T(""), WS_VISIBLE | WS_POPUP, NULL, 0, 0, 0, 0);
				pWnd->LoadStat(item.GetBuffer()); //会调用Init()
				//pWnd->CenterWindow();
				//pWnd->ShowWindow();

				gDeskWndList.push_back(pWnd); //管理，退出发送close消息

				TDeskWindow deskWnd;
				deskWnd.PlugObject = parent;
				deskWnd.WndObject = pWnd;
				gUnify->App()->Status()->DeskWindows.push_back(deskWnd);
			}
		}
	}

	CPaintManagerUI m_pm;

	CDeskWnd();

	virtual void ApplyCfg();
	void ApplyColorR(int r);
	void ApplyColorG(int g);
	void ApplyColorB(int b);
	void ApplyColorRS(int rs);
	void ApplyColorGS(int gs);
	void ApplyColorBS(int bs);

	virtual LPCTSTR GetWindowClassName() const = 0; //串行化用， GetDBSection

	virtual void OnFinalMessage(HWND hWnd);

	virtual void Draw() ;

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);;
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/*virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	Draw();
	return 0;
	}*/
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual  LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	/*LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	return 0;
	}*/
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnContextMenu();
	virtual void SelfClose();
};

//vector<CDeskWnd*> CDeskWnd::gDeskWndList;
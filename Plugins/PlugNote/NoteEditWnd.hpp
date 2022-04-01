#include "stdafx.h"

using namespace DuiLib;

template <class T>
class CNoteEditWnd : public CWindowWnd, public IDialogBuilderCallback, public INotifyUI
{
	CPaintManagerUI m_pm;
	T* m_pOwner = NULL;
public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CNoteEditWnd");
	};

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;

		{
			switch (uMsg) {
			case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
			default:
				bHandled = FALSE;
			}
		}
		if (bHandled) return lRes;

		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

		return __super::HandleMessage(uMsg, wParam, lParam);
	}

	void SetText(LPCTSTR szText)
	{
		CRichEditUI *pLbl = static_cast<CRichEditUI*>(m_pm.FindControl(_T("lblNote")));
		pLbl->SetText(szText);
	}

	void SetOwner(T* p)
	{
		m_pOwner = p;
	}

	virtual void    Notify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (msg.pSender->GetName() == _T("btnCancel"))
			{
				Close();
			}
			if (msg.pSender->GetName() == _T("btnOK"))
			{
				//
				CRichEditUI *pLbl = static_cast<CRichEditUI*>(m_pm.FindControl(_T("lblNote")));
				if (m_pOwner)
					m_pOwner->SetText(pLbl->GetText());

				Close();
			}
		}
	}

	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
	{
		return NULL;
	}

	CNoteEditWnd() {}

	~CNoteEditWnd()
	{
		delete this;

		//gUnify->Core()->Timer()->StopTimer(this, 1);
	};

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CDialogBuilder builder;

		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		//	styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		styleValue |= WS_EX_TOOLWINDOW; //不显示在任务栏
		::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

		m_pm.Init(m_hWnd);
		m_pm.AddResourcePath(gPlugInfo.Path.c_str()); //设置备用路径，Render取图片用到，duilib修改
		CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("Res\\PlugWndNoteEdit.xml")).c_str(), (UINT)0, this, &m_pm);
		
		ASSERT(pRoot && "Failed to parse XML");

		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

	
		
		//LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);
		//m_pm.Init(m_hWnd); //否则接收不到菜单消息


		return 0;
	}
	
};
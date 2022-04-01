#include "stdafx.h"
#include "Img/GDIPHelper.h"
#include "../Common/DeskWnd.h"
#include "ConfigWnd.h"
#include "IniFile.h"
#include "NoteEditWnd.hpp"
#include "base64.hpp"

using namespace DuiLib;

class CNoteWnd : public CDeskWnd, public IDialogBuilderCallback, public INotifyUI
{
public:
	virtual LPCTSTR GetWindowClassName() const {
		return _T("CNoteWnd");
	};

	virtual void LoadStat(LPCTSTR pstrSection) //继承
	{
		__super::LoadStat(pstrSection);

		CLabelUI *pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblNote")));
		
		//load text
		CString s = gUnify->DB()->PlugDB()->GetString(pstrSection, _T("Note"), _T(""));
		if (!s.IsEmpty())
		{
			TString st = Base64::base64_decode(s.GetBuffer());
			pLbl->SetText(st.c_str());
		}
	}

	void SetText(LPCTSTR szText)
	{
		CLabelUI *pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblNote")));
		pLbl->SetText(szText);

		TString sSection = GetDBSection();
		
		TString st = Base64::base64_encode(szText);
		gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Note"), st.c_str());
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = FALSE;

		{
			switch (uMsg) {
			case WM_CLOSE:
			{
				break;
			}
			case WM_NCLBUTTONDBLCLK: {
				ShowEditWnd();

				break;
			}
			case WM_MENUCLICK:
			{
				CDuiString *strMenuName = (CDuiString*)wParam;
				bool bChecked = (bool)lParam;

				if (*strMenuName == _T("mnuEdit"))
				{
					ShowEditWnd();
					bHandled = TRUE;

					delete strMenuName; //放到这里，因为父窗口不处理了
				}
				
				break;
			}
		
			default:
				bHandled = FALSE;
			}
		}
		if (bHandled) return lRes;

		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

		return __super::HandleMessage(uMsg, wParam, lParam);
	}

	void ShowEditWnd()
	{
		CNoteEditWnd<CNoteWnd> *pWnd = new CNoteEditWnd<CNoteWnd>();

		//pWnd->m_pUnify = p;
		pWnd->Create(m_hWnd, _T(""), WS_VISIBLE | WS_POPUP, NULL, 0, 0, 0, 0);
		//pWnd->CenterWindow();
		//::SetParent(*pWnd, m_hWnd);

		CLabelUI *pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblNote")));
		CDuiString sTxt = pLbl->GetText();
		pWnd->SetText(sTxt);
		pWnd->SetOwner(this);

		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		::SetWindowPos(*pWnd, m_hWnd, rc.left , rc.top + 20, 0, 0, SWP_NOSIZE);
		pWnd->ShowWindow();
	}

	virtual void    Notify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			
		}
		else if (msg.sType == DUI_MSGTYPE_DBCLICK)
		{
			
		}
	}

	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
	{
		return NULL;
	}

	CNoteWnd() {}

	~CNoteWnd()
	{
		
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
		CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("Res\\PlugWndNote.xml")).c_str(), (UINT)0, this, &m_pm);
		
		ASSERT(pRoot && "Failed to parse XML");

		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		
		LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

		CLabelUI *pLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblNote")));
		if (pLbl != NULL)
		{
			UINT uStyle = pLbl->GetTextStyle();
			uStyle &= (~DT_SINGLELINE);
			uStyle |= (DT_WORDBREAK);

			uStyle &= (~DT_VCENTER);
			

			/*if (pLbl->GetText().GetLength() > 25)
			{
				uStyle &= (~DT_CENTER);
			}
			else
			{
				uStyle |= (DT_CENTER);
			}*/

			pLbl->SetTextStyle(uStyle);
		}

		return lRet;
	}

	virtual void ApplyCfg() {
		__super::ApplyCfg();

		//TString sSection = GetDBSection();

		int nTrans = 105 - m_nTransparent;
		nTrans = nTrans > 100 ? 100 : nTrans;
		nTrans = nTrans < 0 ? 0 : nTrans;

		m_pm.SetTransparent(int(nTrans*2.55));//1~255
	};

	void OnTimer(void *pOwner, UINT nTimerID)
	{
		//LOGCON("on Note timer...");

		//SetTime();
	}

	virtual void OnConfig()
	{
		ShowConfigWnd<CConfigWnd>(L"便签设置");
	}

	virtual void OnContextMenu()
	{
		CMenuWnd* pMenu = new CMenuWnd();
		DuiLib::CDuiPoint point;
		GetCursorPos(&point);

		pMenu->Init(NULL, (gPlugInfo.Path + _T("Res\\MenuDeskWnd.xml")).c_str(), point, &m_pm, &m_CheckList, &m_VisbileList);
	}

	/*virtual void OnBeforeContextMenu() {
		m_VisbileList[_T("mnuSet")] = false;
	};*/
	
};
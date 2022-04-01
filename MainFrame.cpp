#include "stdafx.h"
#include "MainFrame.h"
#include "Event.h"
#include "LogHelper.h"
#include "resource.h"
		
CMainFrameWnd::CMainFrameWnd()
{
	//SetEscCloseSelf(false);
	//m_MainProxy.SetPaintManager(m_PaintManager);
}

CMainFrameWnd::~CMainFrameWnd()
{
	PostQuitMessage(0);
}

LRESULT CMainFrameWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
	return __super::ResponseDefaultKeyEvent(wParam);
}

void CMainFrameWnd::Notify(TNotifyUI& msg)
{
	return __super::Notify(msg); //notice
}

void CMainFrameWnd::OnClick(TNotifyUI& msg)
{
	//保留，否则最小化按钮处理冲突
}

LRESULT CMainFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
    //if( wParam == SC_CLOSE ) {
    //    //Exit();
    //    PostQuitMessage(0); //防止有模态窗口时，从任务栏关闭会报错
    //    bHandled = TRUE;
    //    return 0;
    //}

    BOOL bZoomed = ::IsZoomed(*this);
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);

    if( ::IsZoomed(*this) != bZoomed ) {
        if( !bZoomed ) {
            CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager->FindControl(_T("maxbtn")));
            if( pControl ) pControl->SetVisible(false);
            pControl = static_cast<CControlUI*>(m_PaintManager->FindControl(_T("restorebtn")));
            if( pControl ) pControl->SetVisible(true);
        }
        else {
            CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager->FindControl(_T("maxbtn")));
            if( pControl ) pControl->SetVisible(true);
            pControl = static_cast<CControlUI*>(m_PaintManager->FindControl(_T("restorebtn")));
            if( pControl ) pControl->SetVisible(false);
        }
    }
    return lRes;
}

UINT CMainFrameWnd::GetClassStyle() const
{
    return CS_DBLCLKS;
}

void CMainFrameWnd::OnFinalMessage( HWND hWnd)
{
    __super::OnFinalMessage(hWnd);

    delete this;
}

CControlUI* CMainFrameWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	m_MainProxy.SetPaintManager(m_PaintManager);
	return m_MainProxy.CreateControl(pstrClass, pstrName);

	return NULL;
}

//class b :public atest{
//public:
//	b() { m_bTest = false; };
//	/*void dosth2()
//	{
//		dosth();
//	}*/
//
//	virtual CDuiString GetSkinFolder()
//	{
//		return L"";
//	}
//	virtual CDuiString GetSkinFile()
//	{
//		return L"";
//	}
//	virtual LPCTSTR GetWindowClassName(void) const
//	{
//		{
//			return L"";
//		}
//	}
//};

void CMainFrameWnd::InitWindow()
{
	LOGCON("CMainFrameWnd InitWindow");
	gUnify->App()->m_MainWnd =m_hWnd;
	m_bEscCloseSelf = false;
	LOGCON("m_PlugProxy init");
	m_PlugProxy.Init(this);

	// 注意时机，等主窗口创建完成，不要放在构造函数里，因为Proxy可能要取hwnd
#ifdef ENABLE_TRAY
	m_TrayProxy.Init(this);
#endif
	m_MainProxy.Init(this);

	m_TrayProxy.SetCheckList(&m_MainProxy.m_CheckList);
	m_TrayProxy.SetTextList(&m_MainProxy.m_TextList);
	m_TrayProxy.SetVisibleList(&m_MainProxy.m_VisibleList);

	//解决Comb下拉显示字体为默认宋体的问题
    m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE, FALSE);
}

DuiLib::CDuiString CMainFrameWnd::GetSkinFolder()
{
#ifdef _DEBUG
    return _T("");
	//return _T("");
#else
    return _T("");
#endif

}

DuiLib::CDuiString CMainFrameWnd::GetSkinFile()
{
    return _T("XML\\WndMainFrame.xml");
}

UILIB_RESOURCETYPE CMainFrameWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;// UILIB_FILE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CMainFrameWnd::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

LRESULT CMainFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

//加密方式必须
DuiLib::CDuiString CMainFrameWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CMainFrameWnd::GetWindowClassName( void ) const
{
	return MAIN_CLASS;
}

LRESULT CMainFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;

    return 0;
}

LRESULT CMainFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

//void CMainFrameWnd::BeforeNavigate2(IDispatch *pDisp, VARIANT *&url, VARIANT *&Flags, VARIANT *&TargetFrameName, VARIANT *&PostData, VARIANT *&Headers, VARIANT_BOOL *&Cancel)
//{
//    
//}
//
//void CMainFrameWnd::NewWindow3(IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
//{
//    
//}

//HRESULT STDMETHODCALLTYPE CMainFrameWnd::GetHostInfo(/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
//{
//    //if (pInfo != NULL)
//    {
//        pInfo->cbSize = sizeof(DOCHOSTUIINFO);
//        pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
//        pInfo->pchHostCss = 0;
//        pInfo->pchHostNS = 0;
//        pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO | DOCHOSTUIFLAG_NO3DBORDER;
//    }
//    
//    return S_OK;
//}

//bool CMainFrameWnd::OnWebkitNavigate(const wstring &sURL)
//{
//	MessageBox(0, sURL.c_str(), NULL, MB_OK);
//
//	return false;
//}

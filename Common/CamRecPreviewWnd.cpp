#include "stdafx.h"
#include "../resource.h"
#include "CamRecPreviewWnd.h"
#include <shellapi.h>
#include "AVCapWrapper.h"
//#include "base64.hpp"
#include "IniFile.h"
#include "DlgTipExitCamRec.h"
#include "DuiHelper.h"

CControlUI* CCamRecPreviewWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	if (_tcsicmp(pstrClass, _T("VideoCapPreview")) == 0)
	{
		CWndHostUI  *pUI = new CWndHostUI;
		//CDateTimeCtrl *dtTime = new CDateTimeCtrl;

		m_hVideoCapPre = Window::CreateSimpleWndow(m_hWnd, _T("VideoCapPreview"), _T(""), WS_VISIBLE | WS_CHILD);
		pUI->Attach(m_hVideoCapPre);

		return pUI;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CCamRecPreviewWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CCamRecPreviewWnd::CCamRecPreviewWnd(HWND hMainWnd)
{
	m_hMainWnd = hMainWnd;
}

CCamRecPreviewWnd::~CCamRecPreviewWnd(void)
{
	
}

void CCamRecPreviewWnd::Restore()
{
	if (m_pVideCapParam)
	{
		//string s = Base64::base64_encode(m_pVideCapParam->VideoDevice);
		TString sSection = String::AnsiToUnicode(m_pVideCapParam->VideoDevice);
		OutputDebugString(sSection.c_str());
		CIniFile ini;
		CString sFileName;
		sFileName.Format(_T("%sUser\\Config.dat"), Path::GetAppDir().c_str());
		ini.SetFileName(sFileName);
		int x = ini.GetInt(sSection.c_str(), _T("Left"), 99999);
		int y = ini.GetInt(sSection.c_str(), _T("Top"), 99999);
		int w = 0; int h = 0;
		if (x < 99999 && y < 99999)
		{
			OutputDebugStringA("XX");
			Window::InsurePosition(m_hWnd, x, y, w, h);

			::SetWindowPos(m_hWnd, HWND_TOP, x, y, w, h, SWP_NOSIZE);
		}
		else //没有保存过
			this->CenterWindow();
	}
}

void CCamRecPreviewWnd::OnFinalMessage(HWND hWnd)
{
	OutputDebugStringA("cam rec preview wnd final msg");

	if (IsWindow(m_hVideoCapPre)) CloseWindow(m_hVideoCapPre);

	//gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CCamRecPreviewWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CCamRecPreviewWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("");
#else
	return _T("");
#endif
	
}

DuiLib::CDuiString CCamRecPreviewWnd::GetSkinFile()
{
	//return (gUnify->App()->Path()->ResPath + _T("XML\\WndCamRecMonitor.xml")).c_str();
	return _T("XML\\HkWndCamRecMonitor.xml");
}

UILIB_RESOURCETYPE CCamRecPreviewWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;// UILIB_ZIP;
#endif
}

LPCTSTR CCamRecPreviewWnd::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CCamRecPreviewWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CCamRecPreviewWnd::GetWindowClassName( void ) const
{
	return _T("UICamRec");
}

void CCamRecPreviewWnd::OnClick( TNotifyUI &msg )
{
	if (msg.pSender->GetName() == _T("btnStop") || msg.pSender->GetName() == _T("closebtn"))
	{
		//if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("TipExitCamRec"), true))
		{
			//弹窗对话框，modal，保存选项
			CCamRecTipExitWnd *pWnd = DuiHelper::NewPopWindow<CCamRecTipExitWnd>(L"", m_hWnd);
			pWnd->CenterWindow();
			int nRet = pWnd->ShowModal();
			if (nRet == IDCANCEL)
				return;
			else if (nRet == IDYES)
			{
				Close();
			}
			else if (nRet == IDNO)
			{
				CAVCapDLL::StopPreview();
				CAVCapDLL::StopVideoCap();

				CAVCapDLL::Clear();
				::PostMessage(m_hMainWnd, WM_QUIT, 0, 0);
				::PostMessage(m_hMainWnd, WM_VIDEO_CAP_EXIT, 0, 0);
			}
		}
	}
	else
		if (msg.pSender->GetName() == _T("btnShot"))
		{
			::PostMessage(m_hMainWnd, WM_VIDEO_CAP_CAPTURE_PICTURE, 0, 0);
		}

	__super::OnClick(msg);
}

void CCamRecPreviewWnd::OnSelectChanged( TNotifyUI &msg )
{

}

void CCamRecPreviewWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CCamRecPreviewWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

void CCamRecPreviewWnd::InitWindow()
{
	//gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE, FALSE);
}

LRESULT CCamRecPreviewWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_EXITSIZEMOVE:
	{
		if (m_pVideCapParam)
		{

			RECT rc;
			::GetWindowRect(m_hWnd, &rc);

			TString sSection = String::AnsiToUnicode(m_pVideCapParam->VideoDevice);
			OutputDebugString(sSection.c_str());
			CIniFile ini;
			CString sFileName;
			sFileName.Format(_T("%sUser\\Config.dat"), Path::GetAppDir().c_str());
			ini.SetFileName(sFileName);

			ini.SetInt(sSection.c_str(), _T("Left"), rc.left);
			ini.SetInt(sSection.c_str(), _T("Top"), rc.top);
		}
	}
	default:
		break;
	}
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CCamRecPreviewWnd::OnItemSelect(TNotifyUI &msg)
{

}



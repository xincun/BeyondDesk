#include "stdafx.h"
#include "resource.h"
#include "CamRecWnd.h"
#include <shellapi.h>
#include "IniFile.h"
#include "AVCapWrapper.h"
#include "../PlugTask/Tasker/Taskers.h"

CControlUI* CCamRecWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	if (_tcsicmp(pstrClass, _T("VideoCapPreview")) == 0)
	{
		return m_CamRecProxy.CreateWnd(m_hWnd);
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CCamRecWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CCamRecWnd::CCamRecWnd(HWND hMainWnd) : m_pHisObj(NULL)
{
	m_hMainWnd = hMainWnd;
}

CCamRecWnd::~CCamRecWnd(void)
{
	
}

void CCamRecWnd::OnFinalMessage( HWND hWnd)
{
	//CAVCapDLL::StopPreview();
	CAVCapDLL::Clear();

	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CCamRecWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CCamRecWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CCamRecWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("PlugWndCamRec.xml")).c_str();
	//return _T("XML\\PlugWndCamRec.xml");
}

UILIB_RESOURCETYPE CCamRecWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CCamRecWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CCamRecWnd::GetWindowClassName( void ) const
{
	return _T("UICamRec");
}

void CCamRecWnd::OnClick( TNotifyUI &msg )
{
	__super::OnClick(msg);

	if (msg.pSender->GetName() == _T("btnStart"))
	{
		if (!m_CamRecProxy.DoCheckInput()) return;

		CEditUI *pEdit = FindControl<CEditUI>("edtCamDir");
		if (pEdit)
			gUnify->DB()->SysConfig()->SetString(_T("Stat"), _T("LastCamRecDir"), pEdit->GetText().GetData());
		//start
		CAVCapDLL::Clear();

		THREAD_BEGIN
		CVideoCapTasker* pTasker = new CVideoCapTasker;
		m_CamRecProxy.SaveParam(pTasker);
		pTasker->Run();
		delete pTasker;
		THREAD_END

		Close();
	}
}

void CCamRecWnd::OnSelectChanged( TNotifyUI &msg )
{

}

void CCamRecWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CCamRecWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

void CCamRecWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE, FALSE);

	m_CamRecProxy.Init(this);

	CEditUI *pEdit = FindControl<CEditUI>("edtCamDir");
	if (pEdit)
	{
		pEdit->SetText(gUnify->DB()->SysConfig()->GetString(_T("Stat"), _T("LastCamRecDir"), _T("")).GetBuffer());
	}
}

LRESULT CCamRecWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CCamRecWnd::OnItemSelect(TNotifyUI &msg)
{

}



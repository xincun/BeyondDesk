#include "stdafx.h"
#include "resource.h"
#include "ScrRecWnd.h"
#include <shellapi.h>
#include "IniFile.h"
#include "AVCapWrapper.h"
#include "../PlugTask/Tasker/Taskers.h"

CControlUI* CScrRecWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	if (_tcsicmp(pstrClass, _T("VideoScrRecPreview")) == 0)
	{
		return m_ScrRecProxy.CreateWnd(m_hWnd);
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CScrRecWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CScrRecWnd::CScrRecWnd(HWND hMainWnd) : m_pHisObj(NULL)
{
	m_hMainWnd = hMainWnd;
}

CScrRecWnd::~CScrRecWnd(void)
{
	
}

void CScrRecWnd::OnFinalMessage( HWND hWnd)
{
	//CAVCapDLL::StopPreview();
	CAVCapDLL::Clear();

	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CScrRecWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CScrRecWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CScrRecWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("PlugWndScrRec.xml")).c_str();
	//return _T("XML\\PlugWndScrRec.xml");
}

UILIB_RESOURCETYPE CScrRecWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CScrRecWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CScrRecWnd::GetWindowClassName( void ) const
{
	return _T("UIScrRec");
}

void CScrRecWnd::OnClick( TNotifyUI &msg )
{
	__super::OnClick(msg);

	if (msg.pSender->GetName() == _T("btnStart"))
	{
		if (!m_ScrRecProxy.DoCheckInput()) return;

		CEditUI *pEdit = FindControl<CEditUI>("edtScrRecDir");
		if (pEdit)
			gUnify->DB()->SysConfig()->SetString(_T("Stat"), _T("LastScrRecDir"), pEdit->GetText().GetData());
		//start
		CAVCapDLL::Clear();

		THREAD_BEGIN
			CScreenRecTasker* pTasker = new CScreenRecTasker;
		m_ScrRecProxy.SaveParam(pTasker);
		pTasker->Run();
		delete pTasker;
		THREAD_END

		Close();
	}
}

void CScrRecWnd::OnSelectChanged( TNotifyUI &msg )
{

}

void CScrRecWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CScrRecWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

void CScrRecWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE);

	m_ScrRecProxy.Init(this);

	CEditUI *pEdit = FindControl<CEditUI>("edtScrRecDir");
	if (pEdit)
	{
		pEdit->SetText(gUnify->DB()->SysConfig()->GetString(_T("Stat"), _T("LastScrRecDir"), _T("")).GetBuffer());
	}
}

LRESULT CScrRecWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CScrRecWnd::OnItemSelect(TNotifyUI &msg)
{

}



#include "stdafx.h"
#include "resource.h"
#include "MicRecWnd.h"
#include <shellapi.h>
#include "IniFile.h"
#include "AVCapWrapper.h"
#include "../PlugTask/Tasker/Taskers.h"

CControlUI* CMicRecWnd::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{


	return NULL;
}

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CMicRecWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CMicRecWnd::CMicRecWnd(HWND hMainWnd) : m_pHisObj(NULL)
{
	m_hMainWnd = hMainWnd;
}

CMicRecWnd::~CMicRecWnd(void)
{
	
}

void CMicRecWnd::OnFinalMessage( HWND hWnd)
{
	//CAVCapDLL::StopPreview();
	CAVCapDLL::Clear();

	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CMicRecWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CMicRecWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CMicRecWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("PlugWndMicRec.xml")).c_str();
	//return _T("XML\\PlugWndMicRec.xml");
}

UILIB_RESOURCETYPE CMicRecWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CMicRecWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CMicRecWnd::GetWindowClassName( void ) const
{
	return _T("UIMicRec");
}

void CMicRecWnd::OnClick( TNotifyUI &msg )
{
	__super::OnClick(msg);

	if (msg.pSender->GetName() == _T("btnStart"))
	{
		if (!m_MicRecProxy.DoCheckInput()) return;

		CEditUI *pEdit = FindControl<CEditUI>("edtAudioCapDir");
		if (pEdit)
			gUnify->DB()->SysConfig()->SetString(_T("Stat"), _T("LastMicRecDir"), pEdit->GetText().GetData());
		//start
		CAVCapDLL::Clear();

		THREAD_BEGIN
		CAudioCapTasker* pTasker = new CAudioCapTasker;
		m_MicRecProxy.SaveParam(pTasker);
		pTasker->Run();
		delete pTasker;
		THREAD_END

		Close();
	}
}

void CMicRecWnd::OnSelectChanged( TNotifyUI &msg )
{

}

void CMicRecWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CMicRecWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

void CMicRecWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE);

	m_MicRecProxy.Init(this);

	CEditUI *pEdit = FindControl<CEditUI>("edtAudioCapDir");
	if (pEdit)
	{
		pEdit->SetText(gUnify->DB()->SysConfig()->GetString(_T("Stat"), _T("LastMicRecDir"), _T("")).GetBuffer());
	}
}

LRESULT CMicRecWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CMicRecWnd::OnItemSelect(TNotifyUI &msg)
{

}



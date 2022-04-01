#include "stdafx.h"
#include "resource.h"
#include "TimeSyncWnd.h"
#include <shellapi.h>
#include "IniFile.h"
#include <thread>

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CTimeSyncWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CTimeSyncWnd::CTimeSyncWnd(HWND hMainWnd) : m_pSyncer(NULL)
{
	m_hMainWnd = hMainWnd;
}

CTimeSyncWnd::~CTimeSyncWnd(void)
{
	delete m_pSyncer;
}

void CTimeSyncWnd::OnFinalMessage( HWND hWnd)
{
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CTimeSyncWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CTimeSyncWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CTimeSyncWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("PlugWndTimeSync.xml")).c_str();
	//return _T("XML\\PlugWndTimeSync.xml");
}

UILIB_RESOURCETYPE CTimeSyncWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CTimeSyncWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CTimeSyncWnd::GetWindowClassName( void ) const
{
	return _T("UITimeSyncWnd");
}

void CTimeSyncWnd::OnClick( TNotifyUI &msg )
{
	__super::OnClick(msg);

	if (msg.pSender->GetName() == _T("btnGet"))
	{
		m_pBtnGet->SetEnabled(false);
		m_pBtnSync->SetEnabled(false);
		FindControl<CLabelUI>("lblStat")->SetText(_T(""));
		if (!m_pSyncer)
			m_pSyncer = new CTimeSync;
		CDuiString s = m_pCmbSrv->GetItemAt(m_pCmbSrv->GetCurSel())->GetText();
		m_pSyncer->GetTime(String::UnicodeToAnsi(s.GetData()).c_str(), this);
	}
	else if (msg.pSender->GetName() == _T("btnSync"))
	{
		FindControl<CLabelUI>("lblStat")->SetText(_T(""));
		if (!m_pSyncer)
			m_pSyncer = new CTimeSync;
		CDuiString s = m_pCmbSrv->GetItemAt(m_pCmbSrv->GetCurSel())->GetText();
		m_pSyncer->UpdateSysTime(String::UnicodeToAnsi(s.GetData()).c_str(), this);
	}
}

void CTimeSyncWnd::OnTimeGot(SYSTEMTIME* pTime)
{
	CString sTime;

	CTime Now = CTime::GetCurrentTime();
	sTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"),
		Now.GetYear(),
		Now.GetMonth(),
		Now.GetDay(),
		Now.GetHour(),
		Now.GetMinute(),
		Now.GetSecond());
	FindControl<CEditUI>("edtSys")->SetText(sTime.GetBuffer());

	
	sTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"),
		pTime->wYear,
		pTime->wMonth,
		pTime->wDay,
		pTime->wHour,
		pTime->wMinute,
		pTime->wSecond);
	FindControl<CEditUI>("edtSrv")->SetText(sTime.GetBuffer());

	FindControl<CLabelUI>("lblStat")->SetText(_T("��ȡʱ��ɹ���"));

	m_pBtnGet->SetEnabled(true);
	if (System::GetOSVer() > System::osWinXp && !System::IsRunasAdmin())
	{
		m_pBtnSync->SetEnabled(false);
	}
	else
		m_pBtnSync->SetEnabled(true);
	//Window::ShowMessage(sTime.GetBuffer());
}

void CTimeSyncWnd::OnTimeFail()
{
	FindControl<CLabelUI>("lblStat")->SetText(_T("��ȡʱ��ʧ�ܣ������޷����ӷ�����"));

	m_pBtnGet->SetEnabled(true);

	if (System::GetOSVer() > System::osWinXp && !System::IsRunasAdmin())
	{
		m_pBtnSync->SetEnabled(false);
	}
	else
	    m_pBtnSync->SetEnabled(true);

	/*THREAD_BEGIN
		Sleep(10000);
	FindControl<CLabelUI>("lblStat")->SetText(_T("����Ƶ��ͬ�����ᱻ��������ֹһ��ʱ��"));
	THREAD_END*/
}

void CTimeSyncWnd::OnTimeSynced(SYSTEMTIME* pTime)
{
	FindControl<CLabelUI>("lblStat")->SetText(_T("ͬ��ʱ��ɹ���"));

	CString sTime;

	CTime Now = CTime::GetCurrentTime();
	sTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"),
		Now.GetYear(),
		Now.GetMonth(),
		Now.GetDay(),
		Now.GetHour(),
		Now.GetMinute(),
		Now.GetSecond());
	FindControl<CEditUI>("edtSys")->SetText(sTime.GetBuffer());
	FindControl<CEditUI>("edtSrv")->SetText(sTime.GetBuffer());

	m_pBtnGet->SetEnabled(true);
	if (System::GetOSVer() > System::osWinXp && !System::IsRunasAdmin())
	{
		m_pBtnSync->SetEnabled(false);
	}
	else
		m_pBtnSync->SetEnabled(true);

	/*THREAD_BEGIN
		Sleep(10000);
	FindControl<CLabelUI>("lblStat")->SetText(_T("����Ƶ��ͬ�����ᱻ��������ֹһ��ʱ��"));
	THREAD_END*/
}

void CTimeSyncWnd::OnSelectChanged(TNotifyUI &msg)
{

}

void CTimeSyncWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CTimeSyncWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

void CTimeSyncWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	//���Comb������ʾ����ΪĬ�����������
	m_PaintManager->SetDefaultFont(_T("΢���ź�"), 12, FALSE, FALSE, FALSE);

	m_pBtnGet = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnGet")));
	m_pBtnSync = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnSync")));

	if (System::GetOSVer() > System::osWinXp && !System::IsRunasAdmin())
	{
		m_pBtnSync->SetEnabled(false);
		m_pBtnSync->SetToolTip(_T("��ǰ���ǹ���Ա�����������������Ի�ȡʱ�䣬���޷�ͬ��ʱ�䣡"));
		//Window::ShowMessage(_T("��ǰ���ǹ���Ա�����������������Ի�ȡʱ�䣬�����޷�ͬ��ʱ�䣡"));
	}

	m_pCmbSrv = static_cast<CComboUI*>(m_PaintManager->FindControl(_T("cmbServer")));

	TString s = gUnify->DB()->PlugDB()->GetString(_T("TimeSync"), _T("Servers"), _T("time.nist.gov;time.windows.com"));

	vector<TString> StrList;
	String::SplitString(s, ';', StrList);

	for (auto& item : StrList)
	{
		CListLabelElementUI *pListE = new CListLabelElementUI;
		pListE->SetText(item.c_str());
		m_pCmbSrv->Add(pListE);
	}

	m_pCmbSrv->SelectItem(0);
}

LRESULT CTimeSyncWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void CTimeSyncWnd::OnItemSelect( TNotifyUI &msg )
{

}

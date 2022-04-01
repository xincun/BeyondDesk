#include "stdafx.h"
#include "CalIntf.h"
#include "DeskCalWnd.h"


CDeskCal::CDeskCal(LPCTSTR pstrCaption, LPCTSTR pstrHint, LPCTSTR pstrIconFile) : m_pWnd(NULL)
{
	m_sCaption = pstrCaption;//;
	m_sHint = pstrHint;// ;
	m_sIconFile = pstrIconFile;// ;

	m_bCanNew = true;
}

CDeskCal::~CDeskCal()
{

}

void CDeskCal::OnCreate()
{
	//Window::ShowMessage(_T("Cal Plugin Created..."));
	int n = 1;
}

void CDeskCal::OnDestroy()
{
	
}

void CDeskCal::OnUnload(void *pSender)
{
	//Window::ShowMessage(L"Cal Plugin Unloaded...");
	//gUnify->Core()->Timer()->StopTimer(this, 1);
}

void CDeskCal::OnLoad(void *pSender)
{
	CDeskWnd::RestoreFromDB<CDeskCalWnd>(_T("CDeskCalWnd"), this);
}

void CDeskCal::OnNew(void* pSender ) {
	/*vector<CString> secList = gUnify->DB()->PlugDB()->GetSections();
	for (CString item : secList) {
		if (item.Find(L"CDeskCalWnd") == 0)
		{
			MessageBox(NULL, _T("只能有一个桌面日历!"), _T("提示"), MB_OK);
			return;
		}
	}*/

	 CDeskWnd::CreateNew<CDeskCalWnd>(this);

	//获取状态
	/*TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));
	if (pPlugInfo)
		pPlugInfo->GetPlugIntf()->OnInvoke(this, 3);
	pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Todo"));
	if (pPlugInfo)
		pPlugInfo->GetPlugIntf()->OnInvoke(this, 3);
	pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Cal"));
	if (pPlugInfo)
		pPlugInfo->GetPlugIntf()->OnInvoke(this, 3);*/
};

void CDeskCal::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CDeskCal::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	return L"";
}

void CDeskCal::OnClick(void *pSender /*= NULL*/)
{
	//Window::ShowMessage(L"clicked");

	//if (!m_pWnd || !IsWindow(*m_pWnd))
	//{
	//	m_pWnd = new CDeskCalWnd();
	//	if (m_pWnd == NULL) return;

	//	//gUnify->GetPaintManager()->GetPaintWindow()
	//	//m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
	//	m_pWnd->Create(0, m_sCaption.c_str(), UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标
	//	m_pWnd->ShowWindow(false);

	//		m_pWnd->CenterWindow();
	//		m_pWnd->ShowWindow();

	//}
	//else
	//{
	//	if (IsWindow(*m_pWnd))
	//	{
	//		
	//			SetForegroundWindow(*m_pWnd);
	//		
	//	}
	//}
}


#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "resource.h"
#include "SchEditWnd.h"
#include "GlobalDefine.h"
#include "DBManager.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CSchEditWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CSchEditWnd::CSchEditWnd(HWND hMainWnd)
{
	m_hMainWnd = hMainWnd;

	gUnify->Core()->Event()->AddEventListen(EVENT_SCH_UPDATE, this, &CSchEditWnd::OnEvent);
}

CSchEditWnd::~CSchEditWnd(void)
{
	gUnify->Core()->Event()->RemoveEventListen(EVENT_SCH_UPDATE, this, &CSchEditWnd::OnEvent);
}

void CSchEditWnd::OnFinalMessage( HWND hWnd)
{
	//gUnify->UI()->RemoveSkinChangeObserver(&m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

DuiLib::CDuiString CSchEditWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\"); //用公用目录底下资源
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CSchEditWnd::GetSkinFile()
{
	return (gPlugInfo.Path + _T("DlgSchEdit.xml")).c_str();
	//return _T("WndRemind.xml");
}

UILIB_RESOURCETYPE CSchEditWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_FILE;
#endif
}

DuiLib::CDuiString CSchEditWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CSchEditWnd::GetWindowClassName( void ) const
{
	return _T("UIExecTipWnd");
}

void CSchEditWnd::OnClick( TNotifyUI &msg )
{
	if( msg.pSender == m_pCloseBtn ) 
	{ 
		Close();
		return; 
	}else if( msg.pSender == m_pMinBtn ) 
	{ 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		return; 
	}else if( msg.pSender == m_pMaxBtn ) 
	{ 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
		return; 
	}else if( msg.pSender == m_pRestoreBtn ) 
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		return;
	}
	else if( msg.pSender->GetName() == _T("btnEdit") ) 
	{
		OnItemEdit();

		
	}
	else if (msg.pSender->GetName() == _T("btnDel"))
	{
		if (MessageBox(this->m_hWnd, _T("您确定要删除吗？"), _T(""), MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}

		CListUI* pList = static_cast<CListUI*>(m_PaintManager->FindControl(_T("lstSch")));
		int n = pList->GetCurSel();
	    
		CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(n));

		CDuiString sid = pItem->GetUserData();
		CDuiString rid = pItem->GetUserData2();

		USES_CONVERSION;
		string ssid = T2A(sid.GetData());
		string rrid = T2A(rid.GetData());
		gUnify->Core()->Event()->SendEvent(EVENT_SCH_DELETE, pItem, ssid, rrid);

		pList->RemoveAt(n);
	}
	else if( msg.pSender->GetName() == _T("btnClose") ) 
	{
        Close();
	}
	else if (msg.pSender->GetName() == _T("btnCancel"))
	{
		Close(IDCANCEL);
	}
}

void CSchEditWnd::OnItemEdit()
{
	CListUI* pList = static_cast<CListUI*>(m_PaintManager->FindControl(_T("lstSch")));
	int n = pList->GetCurSel();
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(n));

	for (auto iter = m_pSchList->begin(); iter != m_pSchList->end(); ++iter) {
		CSchItem& item = *iter;

		item.OwnerWnd = m_hWnd;
		//if (m_pSchList && std::find(m_pSchList->begin(), m_pSchList->end(), sDate.GetData()) != m_pSchList->end())
		/*CDuiString sDate = m_sDate.c_str();
		sDate.Replace(L"-", L"");
		if (_tcsicmp(item.Date.c_str(), sDate.GetData()) == 0)*/

		if (_tcsicmp(item.SID.c_str(), pItem->GetUserData().GetData()) == 0) {
			TPlugInfo* pPlugInfo = gUnify->PlugManager()->GetPlugInfoByName(_T("Task"));

			pPlugInfo->GetPlugIntf()->OnInvoke(this, (int)&item, m_sDate.c_str());

			break;
		}
	}
}

void CSchEditWnd::OnSelectChanged( TNotifyUI &msg )
{
	
}

void CSchEditWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CSchEditWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_ITEMACTIVATE)
	{
		CDuiString sName = msg.pSender->GetName();
		if (sName != _T("schitem")) return;

		/*CListContainerElementUI *pListC = static_cast<CListContainerElementUI*>(msg.pSender);
		int n = pListC->GetIndex();*/

		OnItemEdit();

		return;
	}

	return WindowImplBase::Notify(msg);
}

LRESULT CSchEditWnd::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	
	bHandled = FALSE;
	return 0;
}

LRESULT CSchEditWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		Close();
		bHandled = TRUE;
		return 0;
	}
	//BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	/*if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}*/
	return lRes;
}

void CSchEditWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));

	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE);

	InitControl();
}

LRESULT CSchEditWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	TCHAR press_char = (TCHAR)wParam;

	if (press_char == VK_RETURN || press_char == VK_SPACE) //空格或者回车关闭
	{
		Close(IDOK);
	}
	else
	{
		bHandled = FALSE;
	}
	return 0;
}

LRESULT CSchEditWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}

void CSchEditWnd::OnItemSelect( TNotifyUI &msg )
{
	
}

void CSchEditWnd::OnEvent(CEvent* param)
{
	if (param->EventID == EVENT_SCH_UPDATE)
	{
		TSchItem* pItem = (TSchItem*)param->pSender;

		CListUI* pList = static_cast<CListUI*>(m_PaintManager->FindControl(_T("lstSch")));
		for (int i = 0; i < pList->GetCount(); ++i) {
			CListTextElementUI* pListElement =static_cast<CListTextElementUI*>(pList->GetItemAt(i));

			CDuiString sID = pListElement->GetUserData();
			if (sID == pItem->SID.c_str()) {
				CDuiString s = L"  ";
				s += pItem->Sch.c_str();
				pListElement->SetText(0, s);

				if (pItem->RID.empty())
					pListElement->SetText(1, _T("    否"));
				else
					pListElement->SetText(1, _T("    是"));
			}
		}
	}
}

void CSchEditWnd::OnTimer(void* pOwner, UINT uiTimerId)
{
	CButtonUI *pClose= static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnClose")));
	CDuiString s;
	s.Format(L"(%d) 关闭", m_nCountSec);
	pClose->SetText(s);

	if (m_nCountSec <= 0) {
		Close();
	}

	m_nCountSec--;
}

void CSchEditWnd::InitControl()
{
	CListUI* pList = static_cast<CListUI*>(m_PaintManager->FindControl(_T("lstSch")));

	for (auto iter = m_pSchList->begin(); iter != m_pSchList->end(); ++iter) {
		const CSchItem& item = *iter;
		//if (m_pSchList && std::find(m_pSchList->begin(), m_pSchList->end(), sDate.GetData()) != m_pSchList->end())
		CDuiString sDate= m_sDate.c_str();
		sDate.Replace(L"-", L"");
		if (_tcsicmp(item.Date.c_str(), sDate.GetData()) == 0)
		{
			CListTextElementUI* pListElement = new CListTextElementUI;
			pList->Add(pListElement);
			pListElement->SetName(L"schitem");
			pListElement->SetUserData(item.SID.c_str()); //唯一ID
			pListElement->SetUserData2(item.RID.c_str()); //提醒任务ID，唤起编辑提醒窗口

			CDuiString s = L"  ";
			s += item.Sch.c_str();
			pListElement->SetText(0, s);
			
			if (item.RID.empty())
				pListElement->SetText(1, _T("    否"));
			else
				pListElement->SetText(1, _T("    是"));

			
		}
	}

	//

	//// 添加List列表内容，必须先Add(pListElement)，再SetText  
	//for (int i = 0; i < 100; i++)
	//{
	//	//CListContainerElementUI* parent = new CListContainerElementUI;
	//	
	//	//parent->Add(pListElement);
	//	//parent->SetText(L"Xxx");

	//	pList->Add(pListElement);
	//	pListElement->SetTag(i);
	//	//pListElement->SetAttribute(L"selected", L"true");

	//	CDuiString str;
	//	str.Format(_T("  %d"), i);
	//	


	//}


	
}



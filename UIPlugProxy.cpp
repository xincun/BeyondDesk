
#include "stdafx.h"
#include "UIPlugProxy.h"
#include "UIToolboxItem.h"

DUI_BEGIN_MESSAGE_MAP(UIPlugProxy, UIProxy)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_HEADERCLICK, OnHeaderClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE, OnItemActivate)
DUI_END_MESSAGE_MAP()

void UIPlugProxy::Notify(TNotifyUI& msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		if (!m_bInited)
		{
			OnInit();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_HEADWIDTHCHANGED)
	{
	}

	UIProxy::Notify(msg); //not forget
}

UIPlugProxy::UIPlugProxy()
{

}

UIPlugProxy::~UIPlugProxy()
{
	TPlugInfoList vecTools;
	gUnify->PlugManager()->GetPlugsByType(vecTools, PLUG_TYPE_TIMETOOL);

	for (auto& item : vecTools)
	{
		for (int i = 0; i < item->PlugIntfCount; i++)
		{
			IToolPlug* pPlug = (IToolPlug*)(item->GetPlugIntf(i));
			pPlug->OnUnload(NULL);
		}
	}

	vecTools.clear();
	gUnify->PlugManager()->GetPlugsByType(vecTools, PLUG_TYPE_MEDIATOOL);

	for (auto& item : vecTools)
	{
		for (int i = 0; i < item->PlugIntfCount; i++)
		{
			IToolPlug* pPlug = (IToolPlug*)(item->GetPlugIntf(i));
			pPlug->OnUnload(NULL);
		}
	}

	vecTools.clear();
	gUnify->PlugManager()->GetPlugsByType(vecTools, PLUG_TYPE_DESKWIDGET);

	for (auto& item : vecTools)
	{
		for (int i = 0; i < item->PlugIntfCount; i++)
		{
			IToolPlug* pPlug = (IToolPlug*)(item->GetPlugIntf(i));
			pPlug->OnUnload(NULL);
		}
	}

	vecTools.clear();
	gUnify->PlugManager()->GetPlugsByType(vecTools, PLUG_TYPE_NORMALTOOL);

	for (auto& item : vecTools)
	{
		for (int i = 0; i < item->PlugIntfCount; i++)
		{
			IToolPlug* pPlug = (IToolPlug*)(item->GetPlugIntf(i));
			pPlug->OnUnload(NULL);
		}
	}
}

void UIPlugProxy::OnInit()
{
	if (m_bInited) return;
	m_bInited = true;
	TPlugInfoList vecTools;

	static auto AddPlugToPanel = [&](UINT plgType, LPCTSTR pstrPanelName){
		vecTools.clear();
		CTileLayoutUI* m_pPnlTools = static_cast<CTileLayoutUI*>(m_pPaintManager->FindControl(pstrPanelName));
		gUnify->PlugManager()->GetPlugsByType(vecTools, plgType);

		for (auto& item : vecTools)
		{
			for (int i = 0; i < item->PlugIntfCount; i++)
			{
				/*string n = typeid(IToolPlug).name();
				string n2 = typeid((item->GetPlugIntf(i))).name();
				if (typeid(*(item->GetPlugIntf(i))) != typeid(IToolPlug)) continue;;*/

				IToolPlug* pPlug = dynamic_cast<IToolPlug*>(item->GetPlugIntf(i));
				if (!pPlug) continue;
				
				CDialogBuilder builder;
				CToolboxItemUI *pItem = static_cast<CToolboxItemUI*>(builder.Create(_T("XML\\UIToolBoxItem.xml"), 0, this, m_pPaintManager, NULL));
				if (!pItem) return;

				TString sPath = item->Path;
				sPath = Path::GetParetnDir(sPath.c_str());
				sPath += pPlug->GetIconFile();
				if (!PathFileExists(sPath.c_str()))
				{
					sPath = gUnify->App()->Path()->PlugPath;
					sPath += _T("Common\\");
					sPath += pPlug->GetIconFile();
				}

				CString sAttr;
				sAttr.Format(_T("file='%s' dest='16, 0, 64, 48' "), sPath.c_str());
				pItem->SetItemAttribute(_T("normalimage"), sAttr.GetBuffer());
				sAttr.Format(_T("file='%s' dest='17, 1, 65, 49' "), sPath.c_str());
				pItem->SetItemAttribute(_T("pushedimage"), sAttr.GetBuffer());

				pItem->SetItemAttribute(_T("textalign"), _T("center"));
				//pItem->SetItemAttribute(_T("textpadding"), _T("0, 68, 0, 2"));
				pItem->SetItemCaption(pPlug->GetCaption().c_str());
				pItem->SetItemTooltip(pPlug->GetHint().c_str());
				pItem->SetItemTag(int(pPlug)); //²å¼þÖ¸Õë

				pItem->SetItemNewBtnVisible(pPlug->GetCanNew());
				pItem->SetItemNewBtnTag(int(pPlug));
				pItem->SetItemEnabled(!pPlug->GetCanNew());

				m_pPnlTools->Add(pItem);
				
				pPlug->OnLoad(pItem);
			}
		}
	};

	AddPlugToPanel(PLUG_TYPE_DESKWIDGET, _T("pnlDesk"));
	AddPlugToPanel(PLUG_TYPE_TIMETOOL, _T("pnlTimeTools"));
	AddPlugToPanel(PLUG_TYPE_MEDIATOOL, _T("pnlMediaTools"));
	//AddPlugToPanel(PLUG_TYPE_NORMALTOOL, _T("pnlNormalTools"));
}

LRESULT UIPlugProxy::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SHOWWINDOW:
	{
		if (!m_bShowed)
		{
			m_bShowed = true;

			gUnify->Core()->Event()->PostEvent(EVENT_MAIN_SHOW_DONE);
		}

		return 0;
		break;
	}

	default:
	{

	}
	}

	return -1;	  
	//return __super::HandleMessage(uMsg, wParam, lParam);
}

CControlUI* UIPlugProxy::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	if (_tcsicmp(pstrClass, _T("ToolBoxItemUI")) == 0)
	{
		CToolboxItemUI *p = new  CToolboxItemUI;
		return p;
	}

	return NULL;
}

void UIPlugProxy::OnEvent(CEvent* e)
{
	/*if (e->EventID == EVENT_SKIN_CHANGE)
	{
		if (e->wParam == "alpha")
		{
			m_pPaintManager->SetTransparent(String::AStrToInt(e->lParam));
		}
		else if (e->wParam == "color")
		{
			FindControl<CVerticalLayoutUI>("vertBack")->SetBkImage(_T(""));
			FindControl<CVerticalLayoutUI>("vertBack")->SetBkColor(String::AStrToInt64(e->lParam));
		}
		else if (e->wParam == "bkimage")
		{
			FindControl<CVerticalLayoutUI>("vertBack")->SetBkImage(String::AnsiToUnicode(e->lParam.c_str()).c_str());
		}
	}*/
}

void UIPlugProxy::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("btnTool"))
	{
		IToolPlug* pPlug = (IToolPlug*)(msg.pSender->GetTag());

		pPlug->OnClick(msg.pSender);
	}
	else if (msg.pSender->GetName() == _T("btnNew"))
	{
		CButtonUI* pBtn = static_cast<CButtonUI*>(msg.pSender);
		TPlugInfoList vecTools;
		gUnify->PlugManager()->GetPlugsByType(vecTools, PLUG_TYPE_DESKWIDGET);

		for (auto& item : vecTools)
		{
	
			for (int i = 0; i < item->PlugIntfCount; i++)
			{
				IToolPlug* pPlug = (IToolPlug*)pBtn->GetTag();
				if (pPlug == (IToolPlug*)(item->GetPlugIntf(i)))
				//Window::ShowMessage(pPlug->GetPlugInfo().PlgName.c_str());
					pPlug->OnNew();
			}
		}
	}
}

void UIPlugProxy::OnSelectChanged(TNotifyUI &msg)
{

}

void UIPlugProxy::OnItemClick(TNotifyUI &msg)
{

}

void UIPlugProxy::OnItemActivate(TNotifyUI& msg)
{

}

void UIPlugProxy::OnHeaderClick(TNotifyUI& msg)
{

}
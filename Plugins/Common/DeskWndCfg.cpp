#include "stdafx.h"
#include "resource.h"
#include "DeskWndCfg.h"
#include "../../Unify/Unify.h"
#include "DeskWnd.h"
#include "../../Skin/PalletDialog.h"
#include "../../../[Common]/DuiLib/DuiHelper.h"
//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CDeskWndCfg, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()

CDeskWndCfg::CDeskWndCfg() : m_pDeskWnd(NULL), m_bInited(false)
{

}

CDeskWndCfg::~CDeskWndCfg(void)
{
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);
}

void CDeskWndCfg::OnOK()
{
	if (FindControl<CButtonUI>("btnApply")->IsEnabled()) //否则已经应用了
	{
		SaveCfg();

		if (m_pDeskWnd)
			m_pDeskWnd->ApplyCfg();
	}

	Close();
}

void CDeskWndCfg::OnApply()
{
	SaveCfg();

	if (m_pDeskWnd)
		m_pDeskWnd->ApplyCfg();

	FindControl<CButtonUI>("btnApply")->SetEnabled(false);
}

void CDeskWndCfg::SaveCfg()
{
	if (!m_pDeskWnd) return;

	TString sSection = m_pDeskWnd->GetDBSection();

	int nAlpha = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldAlpha")))->GetValue();
	gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("Alpha"), nAlpha);

	CSliderUI* pZoom = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldZoom")));
	if (pZoom)
	{
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("Zoom"), pZoom->GetValue());
	}

	CSliderUI* pSlider = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldR")));
	if (pSlider)
	{
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("RValue"), pSlider->GetValue());
	}
	pSlider = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldG")));
	if (pSlider)
	{
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("GValue"), pSlider->GetValue());
	}
	pSlider = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldB")));
	if (pSlider)
	{
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("BValue"), pSlider->GetValue());
	}

	pSlider = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldRS")));
	if (pSlider)
	{
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("RSValue"), pSlider->GetValue());
	}
	pSlider = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldGS")));
	if (pSlider)
	{
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("GSValue"), pSlider->GetValue());
	}
	pSlider = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldBS")));
	if (pSlider)
	{
		gUnify->DB()->PlugDB()->SetInt(sSection.c_str(), _T("BSValue"), pSlider->GetValue());
	}

	/*CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(_T("chkColor")));
	if (pCheck)
	{
		gUnify->DB()->PlugDB()->SetBool(sSection.c_str(), _T("IsColor"), pCheck->GetCheck());
	}

	CButtonUI* pBtnClr = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnColor")));
	if (pBtnClr)
	{
		TString sClr = String::Int64ToStr(pBtnClr->GetBkColor());
		gUnify->DB()->PlugDB()->SetString(sSection.c_str(), _T("Color"), sClr.c_str());
	}*/
}

void CDeskWndCfg::LoadCfg()
{
	if (!m_pDeskWnd) return;
	TString sSection = m_pDeskWnd->GetDBSection();

	int nValue = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("Alpha"), 0);
	DuiHelper::SetSliderAndAssociateLabelAlpha(m_PaintManager, _T("sldAlpha"), _T("lblAlphaValue"), nValue, 230);
	

	int x = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("Zoom"), 100);
	DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldZoom"), _T("lblZoom"), x);

	x = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("RValue"), 100);
	DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldR"), _T("lblR"), x);
	x = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("GValue"), 100);
	DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldG"), _T("lblG"), x);
	x = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("BValue"), 100);
	DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldB"), _T("lblB"), x);

	x = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("RSValue"), 0);
	DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldRS"), _T("lblRS"), x);
	x = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("GSValue"), 0);
	DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldGS"), _T("lblGS"), x);
	x = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("BSValue"), 0);
	DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldBS"), _T("lblBS"), x);

	/*CCheckBoxUI* pCheck = static_cast<CCheckBoxUI*>(m_PaintManager->FindControl(_T("chkColor")));
	if (pCheck)
	{
	pCheck->SetCheck(gUnify->DB()->PlugDB()->GetBool(sSection.c_str(), _T("IsColor"), false));
	}

	CButtonUI* pBtnClr = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("btnColor")));
	if (pBtnClr)
	{
	TString sClr = gUnify->DB()->PlugDB()->GetString(sSection.c_str(), _T("Color"), L"4286244241");
	DWORD dwColor = String::StrToInt64(sClr.c_str());
	pBtnClr->SetBkColor(dwColor);
	}*/

}

void CDeskWndCfg::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	
	delete this;
}

DuiLib::CDuiString CDeskWndCfg::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("\\Res");
#else
	return _T("\\Res");
#endif
	
}

DuiLib::CDuiString CDeskWndCfg::GetSkinFile()
{
	return _T("");
}

UILIB_RESOURCETYPE CDeskWndCfg::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_FILE;
#endif
}

DuiLib::CDuiString CDeskWndCfg::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CDeskWndCfg::GetWindowClassName( void ) const
{
	return _T("UIDeskWndCfg");
}

void CDeskWndCfg::OnClick( TNotifyUI &msg )
{
    if( msg.pSender == m_pMinBtn ) 
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
	else if (msg.pSender == m_pCloseBtn)
	{
		Close();
		return;
	}
	else if (msg.pSender->GetName() == _T("btnApply"))
	{
		OnApply();
	}
	else if (msg.pSender->GetName() == _T("btnOK"))
	{
		OnOK();
	}
	else if (msg.pSender->GetName() == _T("btnCancel"))
	{
		//取消改变的颜色，如果没有应用的话
		if (m_pDeskWnd)
		{
			TString sSection = m_pDeskWnd->GetDBSection();
			m_pDeskWnd->ApplyColorR(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("RValue"), 100));
			m_pDeskWnd->ApplyColorG(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("GValue"), 100));
			m_pDeskWnd->ApplyColorB(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("BValue"), 100));
			m_pDeskWnd->ApplyColorRS(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("RSValue"), 0));
			m_pDeskWnd->ApplyColorGS(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("GSValue"), 0));
			m_pDeskWnd->ApplyColorBS(gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("BSValue"), 0));
		}

		Close();
	}
	/*else if (msg.pSender->GetName() == _T("btnColor"))
	{
		FindControl<CButtonUI>("btnApply")->SetEnabled(true);

		CPalletDialog *pPalletDlg = new CPalletDialog(_T("XML\\DlgPallet.xml"), msg.pSender->GetBkColor(), gUnify->UI()->GetBKImage().c_str());
		pPalletDlg->Create(m_hWnd, _T("选择颜色"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
		pPalletDlg->CenterWindow();
		if (pPalletDlg->ShowModal() == IDOK)
		{
			DWORD dwColor = pPalletDlg->GetSelectColor();

			msg.pSender->SetBkColor(dwColor);
		}
		delete	pPalletDlg;
	}*/
}

void CDeskWndCfg::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender == m_pOpt1 || msg.pSender == m_pOpt2 || msg.pSender == m_pOpt3)
		{
			if (m_pOpt1->IsSelected())
			{
				m_pTab->SelectItem(0);
			}
			else if (m_pOpt2->IsSelected())
			{
				m_pTab->SelectItem(1);
			}
			else if (m_pOpt3->IsSelected())
			{
				m_pTab->SelectItem(2);
			}
		}

		if (m_bInited)
		{
			FindControl<CButtonUI>("btnApply")->SetEnabled(true);
		}
		/*else if (msg.pSender->GetName() == _T("chkColor")) {
			FindControl<CButtonUI>("btnApply")->SetEnabled(true);
		}
		else if (msg.pSender->GetName() == _T("btnColor")) {
			FindControl<CButtonUI>("btnApply")->SetEnabled(true);
		}*/
	}
	else if (msg.sType == DUI_MSGTYPE_VALUECHANGED)
	{
		//LOGCON("valuechanged..");
		if (msg.pSender->GetName() == _T("sldAlpha")) {
			int nAlpha = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldAlpha")))->GetValue();

			DuiHelper::SetSliderAndAssociateLabelAlpha(m_PaintManager, _T("sldAlpha"), _T("lblAlphaValue"), nAlpha, 230);

			FindControl<CButtonUI>("btnApply")->SetEnabled(true);
		}
		else if (msg.pSender->GetName() == _T("sldZoom")) {
			int n= static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldZoom")))->GetValue();
			DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldZoom"), _T("lblZoom"), n);

			FindControl<CButtonUI>("btnApply")->SetEnabled(true);
		}
		else if (msg.pSender->GetName() == _T("sldR")) {
			int n = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldR")))->GetValue();
			DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldR"), _T("lblR"), n);

			FindControl<CButtonUI>("btnApply")->SetEnabled(true);

			//gUnify->DB()->PlugDB()->SetInt(m_pDeskWnd->GetDBSection().c_str(), _T("RValue"), static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldR")))->GetValue());
			if (m_pDeskWnd)
			m_pDeskWnd->ApplyColorR(static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldR")))->GetValue());
		}
		else if (msg.pSender->GetName() == _T("sldG")) {
			int n = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldG")))->GetValue();
			DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldG"), _T("lblG"), n);

			FindControl<CButtonUI>("btnApply")->SetEnabled(true);

			//gUnify->DB()->PlugDB()->SetInt(m_pDeskWnd->GetDBSection().c_str(), _T("GValue"), static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldG")))->GetValue());
			if (m_pDeskWnd)
			m_pDeskWnd->ApplyColorG(static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldG")))->GetValue());
		}
		else if (msg.pSender->GetName() == _T("sldB")) {
			int n = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldB")))->GetValue();
			DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldB"), _T("lblB"), n);

			FindControl<CButtonUI>("btnApply")->SetEnabled(true);

			//gUnify->DB()->PlugDB()->SetInt(m_pDeskWnd->GetDBSection().c_str(), _T("BValue"), static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldB")))->GetValue());
			//m_pDeskWnd->ApplyCfg();
			if (m_pDeskWnd)
			m_pDeskWnd->ApplyColorB(static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldB")))->GetValue());
		}
		else if (msg.pSender->GetName() == _T("sldRS")) {
			int n = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldRS")))->GetValue();
			DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldRS"), _T("lblRS"), n);

			FindControl<CButtonUI>("btnApply")->SetEnabled(true);

			//gUnify->DB()->PlugDB()->SetInt(m_pDeskWnd->GetDBSection().c_str(), _T("RSValue"), static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldRS")))->GetValue());
			//m_pDeskWnd->ApplyCfg();
			if (m_pDeskWnd)
			m_pDeskWnd->ApplyColorRS(static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldRS")))->GetValue());
		}
		else if (msg.pSender->GetName() == _T("sldGS")) {
			int n = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldGS")))->GetValue();
			DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldGS"), _T("lblGS"), n);

			FindControl<CButtonUI>("btnApply")->SetEnabled(true);

			//gUnify->DB()->PlugDB()->SetInt(m_pDeskWnd->GetDBSection().c_str(), _T("GSValue"), static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldGS")))->GetValue());
			//m_pDeskWnd->ApplyCfg();
			if (m_pDeskWnd)
			m_pDeskWnd->ApplyColorGS(static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldGS")))->GetValue());
		}
		else if (msg.pSender->GetName() == _T("sldBS")) {
			int n = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldBS")))->GetValue();
			DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, _T("sldBS"), _T("lblBS"), n);

			FindControl<CButtonUI>("btnApply")->SetEnabled(true);

			//gUnify->DB()->PlugDB()->SetInt(m_pDeskWnd->GetDBSection().c_str(), _T("BSValue"), static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldBS")))->GetValue());
			//m_pDeskWnd->ApplyCfg();
			if (m_pDeskWnd)
			m_pDeskWnd->ApplyColorBS(static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldBS")))->GetValue());
		}

		FindControl<CButtonUI>("btnApply")->SetEnabled(true);
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnApply") && m_pDeskWnd == NULL) //另一种进入设置，不是桌面窗口进入的
		{
			OnApply();

			return; //返回不处理
		}
		else if (msg.pSender->GetName() == _T("btnOK") && m_pDeskWnd == NULL)
		{
			OnOK();

			return;
		}
		
	}
	else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
	{
		if (m_bInited)
		{
			FindControl<CButtonUI>("btnApply")->SetEnabled(true);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (m_bInited)
		{
			FindControl<CButtonUI>("btnApply")->SetEnabled(true);
		}

		return;
	}

	return WindowImplBase::Notify(msg);
}

LRESULT CDeskWndCfg::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
	}*/
	bHandled = false;
	return lRes;
}

void CDeskWndCfg::InitWindow()
{
	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE, FALSE);

	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);

	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager->FindControl(_T("minbtn")));

	m_pTab = static_cast<CTabLayoutUI*>(m_PaintManager->FindControl(_T("tabMain")));
	m_pOpt1 = static_cast<COptionUI*>(m_PaintManager->FindControl(_T("opt1")));
	m_pOpt2 = static_cast<COptionUI*>(m_PaintManager->FindControl(_T("opt2")));
	m_pOpt3 = static_cast<COptionUI*>(m_PaintManager->FindControl(_T("opt3")));
	if (m_pOpt1)
	    m_pOpt1->Selected(true);
	else if (m_pOpt2)
		m_pOpt2->Selected(true);
	else if (m_pOpt3)
		m_pOpt3->Selected(true);

	LoadCfg();

	FindControl<CButtonUI>("btnApply")->SetEnabled(false);

	m_bInited = true;
}

LRESULT CDeskWndCfg::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	{
		return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	}

	return 0;	  
}


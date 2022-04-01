#include "stdafx.h"
#include "Utils/PopupWidget.h"
#include "MainFrame.h"
#include "SkinPickerDialog.h"
#include "PalletDialog.h"
#include "SkinPickerPictureItem.h"
#include "../resource.h"

CSkinPickerDialog::CSkinPickerDialog() :
	m_pSkinTab(NULL),
	m_pBtnRestore(NULL),
	m_pBtnListTrans(NULL),
	m_pOptSkinTrans(NULL),
	m_pBtnSkinTrans(NULL),
	m_pBtnPattet(NULL),
	m_pBtnCustom(NULL)
{
}

CSkinPickerDialog::~CSkinPickerDialog()
{

}

LPCTSTR CSkinPickerDialog::GetWindowClassName() const 
{ 
	return _T("UISkinPickerDialog");
}

UINT CSkinPickerDialog::GetClassStyle() const 
{ 
	return CS_DBLCLKS;
}

DuiLib::CDuiString CSkinPickerDialog::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("");
#else
	return _T("");
#endif

}

DuiLib::CDuiString CSkinPickerDialog::GetSkinFile()
{
	return _T("XML\\DlgSkinPicker.xml");
}

UILIB_RESOURCETYPE CSkinPickerDialog::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_ZIPRESOURCE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CSkinPickerDialog::GetResourceID() const
{
	return  MAKEINTRESOURCE(IDR_ZIPRES1);
}

DuiLib::CDuiString CSkinPickerDialog::GetZIPFileName() const
{
	return _T("Res.dll");
}

void CSkinPickerDialog::OnFinalMessage(HWND /*hWnd*/) 
{ 
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);
	return;
}

void CSkinPickerDialog::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);
	int nValue = gUnify->DB()->SysConfig()->GetInt(_T("Skin"), _T("Alpha"), 0);
	static_cast<CSliderUI*>(m_PaintManager->FindControl(kAlphaSlider))->SetValue(nValue);

	CLabelUI *pAlpha = static_cast<CLabelUI*>(m_PaintManager->FindControl(_T("lblAlpha")));
	assert(pAlpha);

	CString sAlpha;
	int x = float(nValue) / 220 * 100;
	sAlpha.Format(L"透明度(%d)", x);
	pAlpha->SetText(sAlpha.GetBuffer());

	vector<TString> files;
	File::RecruseDirFiles(files, gUnify->App()->Path()->CustomSkinFilesPath.c_str(), false);

	for (auto& item : files)
	{
		AddCustomBKImage(item.c_str());
	}

	files.clear();
	File::RecruseDirFiles(files, (gUnify->App()->Path()->ExePath  + _T("Data\\BKImage")).c_str(), false);

	int i = 0;
	CTileLayoutUI* pMySKin = static_cast<CTileLayoutUI*>(m_PaintManager->FindControl(_T("pnlSkin")));
	for (auto& item : files)
	{
		for (int i = 0; i < pMySKin->GetCount(); i++)
		{
			CSkinPikerPictureItemUI * pItem = static_cast<CSkinPikerPictureItemUI*>(pMySKin->GetItemAt(i));
			/*CDuiString s1 = pItem->GetUserData();
			TString s2 = PathFindFileName(item.c_str());*/
			if (pItem->GetUserData() == PathFindFileName(item.c_str()))
				pItem->SetBkImage(item.c_str());
		}
	}

	m_pSkinTab = static_cast<CAnimationTabLayoutUI*>(m_PaintManager->FindControl(kSkinTab));
	m_pSkinTab->SetAnimationEnabled(gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkTabAnimation"), false));
	m_pBtnRestore = static_cast<CButtonUI*>(m_PaintManager->FindControl(kBtnRestore));
	/*m_pBtnListTrans = static_cast<CButtonUI*>(m_PaintManager->FindControl(kBtnListTrans));
	m_pOptSkinTrans = static_cast<COptionUI*>(m_PaintManager->FindControl(kOptSkinTrans));
	m_pBtnSkinTrans = static_cast<CButtonUI*>(m_PaintManager->FindControl(kBtnSkinTrans));*/
	m_pBtnPattet = static_cast<CButtonUI*>(m_PaintManager->FindControl(kBtnPattet));
	m_pBtnCustom = static_cast<CButtonUI*>(m_PaintManager->FindControl(kBtnCustom));

	//*******SetBkColor(m_pMainWnd->GetBkColor());
	//*******SetBkImage(m_pMainWnd->GetBkImage());
	//CDuiString szValue;
	//*******	szValue.SmallFormat(_T("%d%%"),m_pMainWnd->GetListTrans()* 100 / 255 );
	//m_pBtnListTrans->SetText(szValue);
	//*******	m_pBtnListTrans->SetTag(m_pMainWnd->GetListTrans());

	//*******	szValue.SmallFormat(_T("%d%%"),m_pMainWnd->GetSkinTrans() * 100 / 150);
	//m_pBtnSkinTrans->SetText(szValue);
	//*******	m_pBtnSkinTrans->SetTag(m_pMainWnd->GetSkinTrans());

	//*******m_pOptSkinTrans->Selected(m_pMainWnd->GetIsSkinTrans());

	/*CWndShadow::Initialize(m_PaintManager->GetInstance());
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(1, 1);*/

}

CControlUI* CSkinPickerDialog::CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName)
{
	if (_tcsicmp(pstrClass, kSkinPickerPictureItemInterface) == 0)
		return	new CSkinPikerPictureItemUI();

	return NULL;
}

DWORD CSkinPickerDialog::GetBkColor() const
{
	CControlUI* background = m_PaintManager->FindControl(L"vertBack");
	if (background != NULL)
		return background->GetBkColor();
	return 0;
}

void CSkinPickerDialog::SetBkColor(DWORD dwBackColor)
{
	CControlUI* background = m_PaintManager->FindControl(L"vertBack");
	if (background != NULL)
	{
		background->SetBkImage(_T(""));
		background->SetBkColor(dwBackColor);
		background->NeedUpdate();
	}

}

void CSkinPickerDialog::SetBkImage(CDuiString strBkImage)
{
	CControlUI* background = m_PaintManager->FindControl(L"vertBack");
	if (background != NULL)
	{
		TCHAR szBuf[MAX_PATH] = {0};
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,1000,30'"), strBkImage.GetData());
		background->SetBkImage(szBuf);
	}
}

LPCTSTR CSkinPickerDialog::GetBkImage()
{
	CControlUI* background = m_PaintManager->FindControl(L"vertBack");
	if (background != NULL)
		return background->GetBkImage();
	return _T("");
}

void CSkinPickerDialog::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == _T("closebtn")) {
			Close(IDOK);
			return; 
		}
		else if (msg.pSender->GetName() == kBtnListTrans)
		{
			/*CPopupWidget* pWidget = new CPopupWidget;
			CPoint point;
			RECT rc	= m_pBtnListTrans->GetPos();
			point.x = rc.left;
			point.y = rc.top;			
			pWidget->Init(_T("xmls\\Skin\\ListTrans.xml"), m_hWnd, m_PaintManagerainWnd->m_PaintManager, point);*/
			 
			return;
		}
		else if (msg.pSender->GetName() == kBtnSkinTrans)
		{
			/*CPopupWidget* pWidget = new CPopupWidget;
			CPoint point;
			RECT rc	= m_pBtnSkinTrans->GetPos();
			point.x = rc.left;
			point.y = rc.top;			
			pWidget->Init(_T("xmls\\Skin\\ListTrans.xml"), m_hWnd, m_PaintManagerainWnd->m_PaintManager, point);*/

			return;
		}
		else if (msg.pSender->GetName() == kBtnCustom)
		{
			//vector<CDuiString> names;
			//if (CFileUtil::OpenFile(L"JEPG Files(*.jpg)\0*.jpg\0BMP Files\0*.bmp\0PNG Files\0*.png\0\0",GetHWND(),names, false))
			//{

			//}	

			TCHAR fileName[MAX_PATH] = {};
			File::ShellBrowseOpenFile(fileName, _T("所有图片文件\0*.jpg;*.png;*.bmp\0*.jpg\0*.jpg\0*.png\0*.png\0*.bmp\0*.bmp\0"));

			if (_tcsicmp(fileName, L"") == 0) return;

			//LOGCON(fileName);

			TString sFile = gUnify->App()->Path()->CustomSkinFilesPath + Path::GetPathLastName(fileName);
			CopyFile(fileName, sFile.c_str(), FALSE);

			AddCustomBKImage(sFile.c_str());

			gUnify->DB()->SysConfig()->SetInt(_T("Skin"), _T("Color"), -1);
			gUnify->DB()->SysConfig()->SetString(_T("Skin"), _T("BkImage"), sFile.c_str());

			gUnify->UI()->BroadcastSkinChange(sctBack, sFile.c_str());
		}
		else if (msg.pSender->GetName() == kSkinPickerPicutreItem)
		{
			CSkinPikerPictureItemUI * pItem = static_cast<CSkinPikerPictureItemUI*>(msg.pSender);
			CDuiString strBkImage = pItem->GetBkImage();
			strBkImage.Replace(_T("small"), _T(""));
			
			gUnify->DB()->SysConfig()->SetInt(_T("Skin"), _T("Color"), -1);
			gUnify->DB()->SysConfig()->SetString(_T("Skin"), _T("BkImage"), strBkImage.GetData());

			gUnify->UI()->BroadcastSkinChange(sctBack, strBkImage.GetData());
		}
		else if (msg.pSender->GetName() == kBtnRestore)
		{
			//*******			m_pMainWnd->LoadDefaultState();
			//*******SetBkColor(m_pMainWnd->GetBkColor());
			//*******SetBkImage(m_pMainWnd->GetBkImage());

			/*m_pBtnListTrans->SetTag(kMainWndDefaultListTrans);
			m_pBtnListTrans->SetText(_T("30%"));*/

			//m_pBtnSkinTrans->SetEnabled(false);
			////*******			m_pMainWnd->SetIsSkinTrans(false);
			//m_pBtnSkinTrans->SetText(_T("0%"));
			//m_pBtnSkinTrans->SetTag(0);
			//CenterWindow();
		}
		else if (msg.pSender->GetName() == kBtnPattet)
		{
			CPalletDialog *pPalletDlg = new CPalletDialog(this->GetBkColor(), this->GetBkImage());
			pPalletDlg->Create(m_hWnd,_T("调色板"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
			pPalletDlg->CenterWindow();
			if(pPalletDlg->ShowModal() == IDOK)
			{
				DWORD dwColor = pPalletDlg->GetSelectColor();

				TString sClr = String::Int64ToStr(dwColor);
				gUnify->DB()->SysConfig()->SetString(_T("Skin"), _T("Color"), sClr.c_str());

				gUnify->UI()->BroadcastSkinChange(sctColor, sClr.c_str());
			}			
			delete	pPalletDlg;
		}
		
		//Save color
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender->GetName() == kOfficialSkin) {
		    m_pSkinTab->SelectItem(0);
		}
		else if (msg.pSender->GetName() == kMySkin) {
			m_pSkinTab->SelectItem(1);
		}
		//else if (msg.pSender->GetName() == ski) {
		//	if (static_cast<COptionUI*>(msg.pSender)->isselected())
		//	{
		//		//m_pbtnskintrans->setenabled(true);
		//		//*******				m_pmainwnd->setisskintrans(true);
		//	}
		//	else
		//	{
		//		//m_pbtnskintrans->setenabled(false);
		//		////*******				m_pmainwnd->setisskintrans(false);
		//		//m_pbtnskintrans->settext(_t("0%"));
		//		//m_pbtnskintrans->settag(0);
		//	}
		//}
     }
	else if (msg.sType == DUI_MSGTYPE_VALUECHANGED)
	{
		if (msg.pSender->GetName() == _T("sldAlpha")) {
			int nAlpha = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("sldAlpha")))->GetValue();

			CLabelUI *pAlpha = static_cast<CLabelUI*>(m_PaintManager->FindControl(_T("lblAlpha")));
			assert(pAlpha);

			CString sAlpha;
			int x = float(nAlpha) / 220 * 100;
			sAlpha.Format(L"透明度(%d)", x);
			pAlpha->SetText(sAlpha.GetBuffer());
		
			gUnify->DB()->SysConfig()->SetInt(_T("Skin"), _T("Alpha"), nAlpha);

			gUnify->UI()->BroadcastSkinChange(sctAlpha, String::IntToStr(nAlpha).c_str());
		}
	}

}

void CSkinPickerDialog::AddBKImage(int index, const TCHAR * fileName)
{
	CTileLayoutUI* pMySKin = static_cast<CTileLayoutUI*>(m_PaintManager->FindControl(_T("pnlSkin")));

	/*if (pMySKin->GetCount() >= 20) {
		CSkinPikerPictureItemUI * pItem = static_cast<CSkinPikerPictureItemUI*>(pMySKin->GetItemAt(19));
		DeleteFile(pItem->GetBkImage());
		pMySKin->Remove(pItem);
	}*/

	//for (int i = 0; i < pMySKin->GetCount(); i++) 
	{
		CSkinPikerPictureItemUI * pItem = static_cast<CSkinPikerPictureItemUI*>(pMySKin->GetItemAt(index));
		pItem->SetBkImage(fileName);
	}
	//pMySKin->Add(pItem);
}

void CSkinPickerDialog::AddCustomBKImage(const TCHAR * fileName)
{
	CTileLayoutUI* pMySKin = static_cast<CTileLayoutUI*>(m_PaintManager->FindControl(_T("pnlMySkin")));

	if (pMySKin->GetCount() >= 20) { 
		CSkinPikerPictureItemUI * pItem = static_cast<CSkinPikerPictureItemUI*>(pMySKin->GetItemAt(19));
		DeleteFile(pItem->GetBkImage());
		pMySKin->Remove(pItem);	
	}

	CSkinPikerPictureItemUI* pItem = new CSkinPikerPictureItemUI;
	pItem->SetName(kSkinPickerPicutreItem);
	pItem->SetFixedWidth(118);
	pItem->SetFixedHeight(70);
	pItem->SetBkImage(fileName);

	pMySKin->Add(pItem);
}

LRESULT CSkinPickerDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
	}
	return lRes;
}

LRESULT CSkinPickerDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}


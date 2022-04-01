#pragma once

class CToolboxItemUI :
	public CContainerUI
{
public:
	CToolboxItemUI() {};
	~CToolboxItemUI() {};

	/*void SetNormalImage(LPCTSTR szImage)
	{
	for (int i = 0; i < this->GetCount(); ++i)
	{
	if (this->GetItemAt(i)->GetName() == _T("btnTool"))
	{
	CButtonUI* pBtn = static_cast<CButtonUI*>(this->GetItemAt(i));
	pBtn->SetNormalImage(szImage);
	}
	}
	}
	void SetPushedImage(LPCTSTR szImage)
	{
	for (int i = 0; i < this->GetCount(); ++i)
	{
	if (this->GetItemAt(i)->GetName() == _T("btnTool"))
	{
	CButtonUI* pBtn = static_cast<CButtonUI*>(this->GetItemAt(i));
	pBtn->SetPushedImage(szImage);
	}
	}
	}*/

	void SetItemNewBtnTag(int nTag)
	{
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(this->GetItemAt(0));
		CHorizontalLayoutUI* pHorz = static_cast<CHorizontalLayoutUI*>(pVert->GetItemAt(1));

		CButtonUI* pBtn = static_cast<CButtonUI*>(pHorz->GetItemAt(1));
		pBtn->SetTag(nTag);
	}

	void SetItemTooltip(LPCTSTR pstrTooltip)
	{
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(this->GetItemAt(0));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetToolTip(pstrTooltip);
	}

	void SetItemTag(int nTag)
	{
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(this->GetItemAt(0));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetTag(nTag);
	}

	void SetItemEnabled(bool bEnabled = true)
	{
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(this->GetItemAt(0));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetEnabled(bEnabled);
	}

	void SetItemAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(this->GetItemAt(0));
		CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
		pBtn->SetAttribute(pstrName, pstrValue);
	}

	void SetItemCaption(LPCTSTR szCaption)
	{
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(this->GetItemAt(0));
		CHorizontalLayoutUI* pHorz = static_cast<CHorizontalLayoutUI*>(pVert->GetItemAt(1));
		
		CLabelUI* pLbl = static_cast<CLabelUI*>(pHorz->GetItemAt(0));
		pLbl->SetText(szCaption);
	}

	void SetItemNewBtnVisible(bool bVisible)
	{
		CVerticalLayoutUI* pVert = static_cast<CVerticalLayoutUI*>(this->GetItemAt(0));
		CHorizontalLayoutUI* pHorz = static_cast<CHorizontalLayoutUI*>(pVert->GetItemAt(1));

		CButtonUI* pBtnNew = static_cast<CButtonUI*>(pHorz->GetItemAt(1));
		pBtnNew->SetVisible(bVisible);

		if (!bVisible)
		{
			/*CButtonUI* pBtn = static_cast<CButtonUI*>(pVert->GetItemAt(0));
			pBtn->SetAttribute(_T("textalign"), _T("center"));*/

			CLabelUI* pLbl = static_cast<CLabelUI*>(pHorz->GetItemAt(0));
			int n = this->GetFixedWidth();
			pLbl->SetFixedWidth(n);
			pLbl->SetAttribute(_T("textalign"), _T("center"));
		}
	}
};


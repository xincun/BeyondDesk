#ifndef PALLET_DIALOG_H
#define PALLET_DIALOG_H
#include "SkinPickerDialog.h"

const TCHAR kPalletPallet[] = _T("Pallet");
const TCHAR kPalletOk[] =		_T("Pallet_BTN_OK");
const TCHAR kPalletCancel[] =	_T("Pallet_BTN_CANCEL");
	

class CPalletDialog : public WindowImplBase
{
public:
	CPalletDialog(DWORD dwBKColor, LPCTSTR pstrBKImage);
	~CPalletDialog();

	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);
	DWORD GetSelectColor() ;

	void InitWindow();
	void Notify(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName) ;

	void SetBkColor(DWORD dwBackColor);
	DWORD GetBkColor() const;
	void SetBkImage(CDuiString strBkImage);
	LPCTSTR GetBkImage();

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CDuiString GetZIPFileName() const;
protected:
	//CSkinPickerDialog*m_pSkinPickerWnd;
	CColorPaletteUI	*m_pPallet;
	CButtonUI		*m_pBtnOk;
	CButtonUI		*m_pBtnCancel;
	DWORD			m_SelectedColor;
	DWORD m_InitBKColor;
	CDuiString m_InitBKImage;
};

#endif PALLET_DIALOG_H

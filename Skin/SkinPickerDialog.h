#ifndef SKIN_PICKER_H
#define SKIN_PICKER_H

const TCHAR kSkinPickerPicutreItem[] = _T("SkinPictureItem");
const TCHAR kSkinTab[] =		_T("Skin_Tab");
const TCHAR kOfficialSkin[] =	_T("optIn");
const TCHAR kMySkin[] =			_T("optMy");
const TCHAR kBtnRestore[] =		_T("Skin_BtnRestore");
const TCHAR kBtnListTrans[] =	_T("Skin_BtnListTrans");
const TCHAR kOptSkinTrans[] =	_T("Skin_OptSkinTrans");
const TCHAR kBtnSkinTrans[] =	_T("Skin_BtnSkinTrans");
const TCHAR kBtnPattet[] =		_T("Skin_BtnPattet");
const TCHAR kBtnCustom[] =		_T("Skin_BtnCustom");
const TCHAR kWndBackGround[] = _T("background");
const TCHAR kAlphaSlider[] = _T("sldAlpha");
const TCHAR kWidgetSlider[] = _T("Widget_slider");

const int	kMainWndDefaultListTrans = 77;

class CSkinPickerDialog : public WindowImplBase
{
	bool m_bColorSet = false;
	bool m_bBkImageSet = false;
public:
	CSkinPickerDialog();
	~CSkinPickerDialog();

	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);

	void Init();
	void Notify(TNotifyUI& msg);

	void AddBKImage(int index, const TCHAR * fileName);
	void AddCustomBKImage(const TCHAR * fileName);

	CControlUI* CreateControl(LPCTSTR pstrClass, LPCTSTR pstrName);

	void SetBkColor(DWORD dwBackColor);
	DWORD GetBkColor() const;
	void SetBkImage(CDuiString strBkImage);
	LPCTSTR GetBkImage();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual void InitWindow();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CDuiString GetZIPFileName() const;
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	//CWndShadow		 m_WndShadow;
	CAnimationTabLayoutUI *m_pSkinTab;
	CButtonUI		*m_pBtnRestore;
	CButtonUI		*m_pBtnListTrans;
	COptionUI		*m_pOptSkinTrans;
	CButtonUI		*m_pBtnSkinTrans;
	CButtonUI		*m_pBtnPattet;
	CButtonUI		*m_pBtnCustom;
};

#endif SKIN_PICKER_H

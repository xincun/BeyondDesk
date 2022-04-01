#pragma once
//////////////////////////////////////////////////////////////////////////

#include "DuiHelper.h"
#include "DLLInvoke.h"

class CVolWnd : public WindowImplBase
{
	static CVolWnd* g_pVolWnd ;

	

public:
	static void Init() {
		if (!g_pVolWnd) {
			g_pVolWnd = new CVolWnd();
			if (g_pVolWnd == NULL) return;

			g_pVolWnd->Create(0, L"BeyondDeskÒôÁ¿µ÷Õû", WS_POPUP, WS_EX_TOOLWINDOW, 0, 0, 0, 0);

			//g_pVolWnd = DuiHelper::NewPopWindow<CVolWnd>(L"ÎüÄÉ°É",  gUnify->App()->m_MainWnd);
		}
	}

	static void Show() {
	
		g_pVolWnd->CenterWindow();
		::ShowWindow(g_pVolWnd->m_hWnd, SW_SHOW);
		//SetForegroundWindow(g_pVolWnd->m_hWnd);
		::SetWindowPos(g_pVolWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//Util::Window::InsureForegroundWnd(g_pVolWnd->m_hWnd);
		g_pVolWnd->UpdateVol();
		
		g_pVolWnd->ResetTimer();

	}

	void ResetTimer()
	{
		gUnify->Core()->Timer()->StopTimer(this, 1);
		gUnify->Core()->Timer()->SetTimer(this, 1, 1500, &CVolWnd::OnTimer);
	}

	void OnTimer(void* pOwner, UINT uiTimerID)
	{
		
		if (pOwner == this && uiTimerID == 1)
		{
			::ShowWindow(g_pVolWnd->m_hWnd, SW_HIDE);
		}
	}

	void UpdateVol() {
		int n = DLLInvoker::GetVol();
		DuiHelper::SetSliderAndAssociateLabel(m_PaintManager, L"sldVol", L"lblVol", n);
	}

	CVolWnd();
	~CVolWnd(void)
	{
		gUnify->Core()->Timer()->StopTimer(this, 1);
	}

	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
	virtual CDuiString GetZIPFileName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
};

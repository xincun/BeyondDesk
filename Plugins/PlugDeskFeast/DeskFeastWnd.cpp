#include "stdafx.h"
#include "DeskFeastWnd.h"
#include "EventDefine.h"
#include "Utils/datehelper.h"


void CDeskFeastWnd::Notify(TNotifyUI& msg)
{
	//LOGCON(msg.sType.GetData());
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnFresh"))
		{

		}
		else if (msg.pSender->GetName() == _T("btnMore"))
		{

		}
	}

	//return CNotifyPump::NotifyPump(msg);
}

CDeskFeastWnd::~CDeskFeastWnd()
{
	
	gUnify->Core()->Event()->RemoveEventListen(EVENT_NEW_DAY, this, &CDeskFeastWnd::OnEvent);
	
}

LRESULT CDeskFeastWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDialogBuilder builder;

	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	//	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	styleValue |= WS_EX_TOOLWINDOW; //����ʾ��������
	::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

	m_pm.Init(m_hWnd);
	m_pm.AddResourcePath(gPlugInfo.Path.c_str()); //���ñ���·����RenderȡͼƬ�õ���duilib�޸�
	CControlUI *pRoot = builder.Create((gPlugInfo.Path + _T("Res\\PlugWndDeskFeast.xml")).c_str(), (UINT)0, this, &m_pm);
	//CControlUI *pRoot = builder.Create(_T("D:\\[Work]\\desktop\\BeyondDesk\\bin\\Debug\\Res\\XML\\DlgAbout.xml"), (UINT)0, this, &m_pm);

	ASSERT(pRoot && "Failed to parse XML");

	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	LRESULT lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

	gUnify->Core()->Event()->AddEventListen(EVENT_NEW_DAY, this, &CDeskFeastWnd::OnEvent);

	return lRet;
}

void CDeskFeastWnd::OnEvent(CEvent* e)
{
	if (e->EventID == EVENT_NEW_DAY) {
		Update();
	}
}

void CDeskFeastWnd::Update() {
	TString sSection = GetDBSection();

	LoadBack();

	DWORD dwColor = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("FontColor"), RGB(255, 255, 255));
	int R = (dwColor & 0xFF0000) >> 16;    //��ȡ����ʾ��ɫ����R����λ�õ���ʵ����ɫ����ֵ, ��Alphaͨ���й�
	int G = (dwColor & 0x00FF00) >> 8;
	int B = dwColor & 0x0000FF;
	dwColor = RGB(R, G, B);

	/*CLabelUI* pLbl = NULL;
	for (int i = 1; i <= 10; ++i) {
		CDuiString s;
		s.Format(L"lblFeast%d", i);
		pLbl = static_cast<CLabelUI*>(m_pm.FindControl(s));
		pLbl->SetTextColor(dwColor);
	}*/
	int numLimit = gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("edtNum"), 10);

	CVerticalLayoutUI* pPnl = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(L"pnlFeast"));
	if (pPnl)
	{
		CLunar lunar;
		pPnl->RemoveAll();
		int n = pPnl->GetHeight();
		n = n / 23;

		int nCount = 0;

		for (int i = 0; i <= 365; ++i)
		{
			if (nCount == numLimit) break;

			COleDateTime& dt = COleDateTime::GetCurrentTime();
			dt += COleDateTimeSpan(i, 0, 0, 0);

			lunar.Lunar(dt);

			//CString m_lunarFestival; //ũ������
			//CString m_solarFestival; //��������
			//CString m_solarTerms;    //����
			CString sJQ = lunar.m_solarTerms;
			CString sNL = lunar.m_lunarFestival;
			CString sGL = lunar.m_solarFestival;
			CString s;
			s.Format(_T("%d-%d %s %s %s"), dt.GetMonth(), dt.GetDay(), lunar.m_solarFestival, lunar.m_lunarFestival, lunar.m_solarTerms);

			if (sJQ.GetLength() > 0 || sNL.GetLength()>0 || sGL.GetLength()>0)
			{
				CLabelUI* pLbl = new CLabelUI;
				pLbl->SetFixedHeight(23);
				pLbl->SetFont(1);
				RECT rc;
				::GetClientRect(*this, &rc);
				pLbl->SetFixedWidth(rc.right-rc.left - 30);
				rc = { 20, 2, 0, 0 };
				pLbl->SetTextPadding(rc);
				pLbl->SetTextColor(dwColor);
				pLbl->SetAttribute(_T("endellipsis"), _T("true"));
				pLbl->SetText(s.GetBuffer());
				pLbl->SetToolTip(s.GetBuffer());
				s.ReleaseBuffer();

				pPnl->Add(pLbl);

				++nCount;
			}
		}
	}

	pPnl->Invalidate();
	pPnl->NeedUpdate();
	UpdateWindow(*this);
	

	//UINT n = 16;
	//if (gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("chkGL"), true))
	//	n += 23;
	//if (gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("chkNL"), true))
	//	n += 23;
	//if (gUnify->DB()->PlugDB()->GetInt(sSection.c_str(), _T("chkFeast"), true) && pnl->IsVisible())
	//	n += 23;

	//n += UpdateTodo();
	//n += UpdateTask();
	//n += UpdateSch();

	//n += 36;
	////UINT nHeight = 16 + 23 * 3 + 23 * 10 + nTodo + 23 * 10 + 16;

	//RECT rcClient;
	//::GetClientRect(*this, &rcClient);

	//SetWindowPos(m_hWnd, NULL, 0, 0, rcClient.right - rcClient.left, n, SWP_NOMOVE);
}

void CDeskFeastWnd::ApplyCfg()
{
	__super::ApplyCfg();

	LoadSize(320, 280);
	
	Update();

	UpdateWindow(m_hWnd);
}

LRESULT CDeskFeastWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE; //�������ϲ�

	{
		switch (uMsg) {
		case WM_EXITSIZEMOVE:
		{
			Update();
			break;
		}
		
		default:
			bHandled = FALSE;
		}
	}
	if (bHandled) return lRes;

	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

	return CDeskWnd::HandleMessage(uMsg, wParam, lParam);
}

void CDeskFeastWnd::SaveStat()
{
	__super::SaveStat();

	SaveSize(); //�ֿ�д���е����洰�ڣ�����ʱ�ӵȣ�����Ҫ����
}

void CDeskFeastWnd::LoadStat(LPCTSTR pstrSection)
{
	__super::LoadStat(pstrSection); //super�����applyacfg����������д��
}

void CDeskFeastWnd::OnConfig()
{
	ShowConfigWnd<CConfigWnd>(L"�������");
}

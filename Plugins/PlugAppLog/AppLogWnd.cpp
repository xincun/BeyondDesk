#include "stdafx.h"
#include "resource.h"
#include "AppLogWnd.h"
#include <shellapi.h>
#include <string>
#include<iterator>

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CAppLogWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
DUI_END_MESSAGE_MAP()

CAppLogWnd::CAppLogWnd(HWND hMainWnd) : m_pHisObj(NULL)
{
	m_hMainWnd = hMainWnd;
}

CAppLogWnd::~CAppLogWnd(void)
{
	
}

void CAppLogWnd::OnFinalMessage( HWND hWnd)
{
	gUnify->UI()->RemoveSkinChangeObserver(m_PaintManager);

	__super::OnFinalMessage(hWnd);
	
	delete this;
}

bool CAppLogWnd::UseCustomRes()
{
	return true;
}

DuiLib::CDuiString CAppLogWnd::GetSkinFolder()
{
#ifdef _DEBUG
	return _T("Res\\");
#else
	return _T("Res\\");
#endif
	
}

DuiLib::CDuiString CAppLogWnd::GetSkinFile()
{
	//m_PaintManager->AddResourcePath(gPlugInfo.Path.c_str());
	return _T("PlugWndAppLog.xml");
}

UILIB_RESOURCETYPE CAppLogWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	 return UILIB_FILE;// UILIB_ZIP;
#endif
}

DuiLib::CDuiString CAppLogWnd::GetZIPFileName() const
{
	return _T("Res.dll");
}

LPCTSTR CAppLogWnd::GetWindowClassName( void ) const
{
	return _T("UIAppLog");
}

void CAppLogWnd::OnClick( TNotifyUI &msg )
{
	__super::OnClick(msg);
	
	 if (msg.pSender->GetName() == _T("btnToday"))
	{
		

	}
}

void CAppLogWnd::OnSelectChanged( TNotifyUI &msg )
{

}

void CAppLogWnd::OnItemClick( TNotifyUI &msg )
{
	
}

void CAppLogWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

void CAppLogWnd::InitWindow()
{
	gUnify->UI()->AddSkinChangeObserver(m_PaintManager);
	//解决Comb下拉显示字体为默认宋体的问题
	m_PaintManager->SetDefaultFont(_T("微软雅黑"), 12, FALSE, FALSE, FALSE);
}

LRESULT CAppLogWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	/*演示键盘消息的处理*/
	/*TCHAR press_char = (TCHAR)wParam;
	if(press_char == VK_BACK)
	{
	MessageBox(NULL, _T("按下了回退键"), _T("DUILIB DEMO"), MB_OK);
	}
	else
	{
	bHandled = FALSE;
	}*/
	return 0;
}

LRESULT CAppLogWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//if (WM_NCLBUTTONDBLCLK != uMsg) //屏蔽双击标题栏
	//{
	//	return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
	//}

	 return __super::HandleMessage(uMsg, wParam, lParam);;
}


void CAppLogWnd::Navigate(LPCTSTR pstrURL)
{
	CRichEditUI* pEdit = FindControl<CRichEditUI>("edtLog");
	/*USES_CONVERSION;
	string sPath = T2A(pstrURL);

	wifstream in(sPath.c_str(), ios::in);*/
	/*wifstream in(pstrURL, ios::in);
	in.imbue(std::locale("chs"));
	if (!in) return;

	TCHAR buf[2048];
	TString sFile;
	int i = 0;
	
	while (in.getline(buf, sizeof(buf)))
	{		
		sFile += buf;
		sFile += '\r\n';
		
	}*/

	//读取string比wstring快快几倍
	//int n = GetTickCount();
	ifstream dataFile(pstrURL);
	istream_iterator<string> dataBegin(dataFile);
	istream_iterator<string> dataEnd;
	vector<string> vecData(dataBegin, dataEnd);

	//LOGCONSOLE(GetTickCount() - n);
	
//	n = GetTickCount();
	
	string sFile;
	TCHAR pszFile[20 * 1024];
	USES_CONVERSION;
	CLog::InsureConsole();
	for (auto& item : vecData)
	{
		sFile += item.c_str();

		if (sFile.length() > 1024 * 9)
		{
			memset(pszFile, 0, 20 * 1024);
		   _tcscpy(pszFile, A2T(sFile.c_str()));
			sFile.clear();
			wprintf(L"%s", pszFile);
			pEdit->AppendText(pszFile);
		}
		//printf(item.c_str());
	}
	//LOGCONSOLE(GetTickCount() - n);
}

void CAppLogWnd::OnItemSelect(TNotifyUI &msg)
{
	
}

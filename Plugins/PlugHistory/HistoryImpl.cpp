#include "stdafx.h"
#include "HistoryIntf.h"
#include "HistoryWnd.h"
#include "AppPath.h"
#include "Event.h"
#include "LogHelper.h"
#include "SimpleTimer.h"
#include "CmdLine.h"
#include <string>
#include "resource.h"

using namespace std;

IUnify *gUnify;
TPlugInfo gPlugInfo;

extern "C" PLUG_API  TPlugInfo* GetPlugInfo(IUnify *p)
{
	gUnify = p;

	gPlugInfo.Name = _T("History");
	gPlugInfo.Type = PLUG_TYPE_TIMETOOL;
	gPlugInfo.Desc = _T("历史今日插件");
	gPlugInfo.Order = 99;
	gPlugInfo.PlugIntfCount = 2;

	return &gPlugInfo;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf()
{
	IPlugBase *p = new CHistory;
	p->OnCreate();

	return p;
}

extern "C" PLUG_API IPlugBase*  GetPlugIntf2()
{
	IPlugBase *p = new CUIHisPlug;
	p->OnCreate();

	return p;
}

void CHistory::OnTimer(void *pOwner, UINT nTimerID)
{
//	LOGCON("on timer...");
}

CHistory::CHistory() : m_pWnd(NULL)
{
	m_sCaption = _T("历史上的今天");
	m_sHint = _T("历史上的今天");
	m_sIconFile = _T("icon.png");
}

CHistory::~CHistory()
{

}

void CHistory::OnCreate()
{
	//Window::ShowMessage(_T("History Plugin Created..."));
}

void CHistory::OnDestroy()
{
	
}

void CHistory::OnUnload(void *pSender)
{
	
}

void CHistory::OnLoad(void *pSender)
{
	//Window::ShowMessage(L"History Plugin Loaded...");

	//Window::ShowMessage(gUnify->App()->Path()->DBPath.c_str());
	//Window::ShowMessage(gUnify->App()->CmdLine()->GetParamByIndex(0).c_str());

	HisList.reserve(6000);

	// 查找资源文件中、加载资源到内存、得到资源大小
	HINSTANCE hInst = GetModuleHandle(gPlugInfo.FileName.c_str());
	HRSRC   hResInfo = FindResource(hInst, MAKEINTRESOURCE(IDR_STRINGLINES1), L"STRINGLINES");
	HGLOBAL hResData = LoadResource(hInst, hResInfo);
	LPVOID pvResData = LockResource(hResData);
	DWORD dwResSize = SizeofResource(hInst, hResInfo);

	char* szRes = (char*)pvResData;
	char* szPos = (char*)pvResData;
	char* szStart = (char*)pvResData;

	while (*szPos != NULL)
	{
		szPos++;
		if (*szPos == '\r' && *(szPos + 1) == '\n')
		{
			char szLine[1024];
			memcpy(szLine, szStart, szPos - szStart);
			szLine[szPos - szStart] = '\0';

			string sDate = String::GetSplitStringA(szLine, '=', 0);
			string sHis = String::GetSplitStringA(szLine, '=', 1);
			USES_CONVERSION;
			HisList.push_back(make_pair(A2T(sDate.c_str()), A2T(sHis.c_str())));

			szStart = szPos + 2; //换行预留
		}
	}

	//CloseHandle(hFile);
	UnlockResource(hResData);
	FreeResource(hResData);
}

TString CHistory::GetHis(const TString& sDate)
{
	TString sReturn;
	for (auto& item : HisList)
	{
		if (item.first == sDate)
		{
			sReturn += item.second;
			sReturn += '\r';
			sReturn += '\n';
		}
	}

	return sReturn;
}

void CHistory::OnInvoke(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	
}

TString CHistory::OnGet(void *pSender, int wParam /*= 0*/, const TString &lParam /*= L""*/, const TString &eParam /*= L""*/)
{
	if (wParam == 0)
	{
		return GetHis(lParam);
	}
}

void CHistory::OnClick(void *pSender /*= NULL*/)
{
	//Window::ShowMessage(L"clicked");

	if (!m_pWnd || !IsWindow(*m_pWnd))
	{
		m_pWnd = new CHistoryWnd(0);
		if (m_pWnd == NULL) return;

		m_pWnd->m_pHisObj = this;
		//gUnify->GetPaintManager()->GetPaintWindow()
		//m_pAboutWnd->Create(this->m_hWnd, _T("关于"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0); //任务栏无图标
		m_pWnd->Create(0, _T("历史上的今天"), UI_WNDSTYLE_DIALOG, WS_EX_APPWINDOW, 0, 0, 0, 0); //任务栏有图标
		m_pWnd->CenterWindow();
		m_pWnd->ShowWindow();
	}
	else
	{
		if (IsWindow(*m_pWnd))
		{
			SetForegroundWindow(*m_pWnd);
		}
	}
}

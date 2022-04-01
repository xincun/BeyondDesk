#include "StdAfx.h"
#include "NetManager.h"
#include <SensAPI.h>
#include "DownloadMgr.h"
#include "mshtml.h"

#pragma comment(lib, "Sensapi.lib")

CNetManager::CNetManager()
{
    
}

CNetManager::~CNetManager()
{
	
}

bool CNetManager::IsNetAlive()
{
	DWORD dw;

	if (!IsNetworkAlive(&dw))
	{
		DWORD const e = GetLastError();

		if (e == ERROR_SUCCESS)
		{
			//::MessageBox(0, _T("Net Disconnected"), NULL, MB_OK);
			LOGCON("Disconnected");
			return false;
		}
		else
		{
			//::MessageBox(0, _T("Net error"), NULL, MB_OK);
			LOGCON("Connection error");
			return false;
		}
	}
	else
	{
		//printf("Connected to network!\n");
		if (dw == NETWORK_ALIVE_LAN)
		{
			//::MessageBox(0, _T("Connectioned"), NULL, MB_OK);
			LOGCON("Connected by LAN");
			return true;
		}
		else if (dw == NETWORK_ALIVE_WAN)
		{
			LOGCON("Connected by WAN");
			return true;
		}
		else
		{
			LOGCON("Connected by VPN? or Something?");
			return true;
		}
	}
}

void CNetManager::WriteIECookie(CWebBrowserUI *pWBUI)
{
	//写入Cookie值
	IHTMLDocument2 *pDoc;
	BSTR title, source, content;
	IDispatch *WebDocument;// = static_cast<IDispatch*>();
	pWBUI->GetWebBrowser2()->get_Document(&WebDocument);
	pDoc = static_cast<IHTMLDocument2*>(WebDocument);

	BSTR cookie;
	pDoc->get_cookie(&cookie); //先调一次Cookie？？
	//LOGCONSOLE(cookie);

	wstring s;// = Pub::AnsiToUnicode(gMainGate->UserManager()->get_cur_user_info().access_token);
	TCHAR szCookie[MAX_PATH] = {};
	wsprintf(szCookie, _T("ACCESS_TOKEN = %s;"), s.c_str());
	//LOGCONSOLE(szCookie);
	//wsprintf(szCookie, _T("Expires = %s;"), _T("19-Aug-2019 07:52:19 GMT"));
	if (pDoc->put_cookie((BSTR)szCookie) != S_OK)
	{
		//ShowMessage("fail!");
		
	}


	/*InternetSetOption(0, 42, NULL, 0);
	bool bRet = InternetSetCookie(URL_EMBED_HALL, _T("ACCESS_TOKEN"), szCookie);*/

	if (pDoc) pDoc->Release();
}

void CNetManager::DisableIEScroll(CWebBrowserUI *pWBUI)
{
	HRESULT hr;
	IDispatch *pDisp;
	
	pWBUI->GetWebBrowser2()->get_Document(&pDisp);
	
	IHTMLDocument2 *pDocument = NULL;
	IHTMLElement*   pEl;
	IHTMLBodyElement   *   pBodyEl;

	hr = pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pDocument);
	if (SUCCEEDED(pDocument->get_body(&pEl)))
	{
		if (SUCCEEDED(pEl->QueryInterface(IID_IHTMLBodyElement, (void**)&pBodyEl)))
		{
			pBodyEl->put_scroll(L"no");    //去滚动条(网页中滚动条设置为no 那么不管IE窗口与网页是否匹配 都不会显示滚动条)
			pBodyEl->Release();
		}
		
		//IHTMLStyle   *phtmlStyle;
		//pEl->get_style(&phtmlStyle);
		//
		//if (phtmlStyle != NULL)
		//{
		//	phtmlStyle->put_overflow(L"hidden");
		//	phtmlStyle->put_border(L"none");    //去除边网页边框
		//	phtmlStyle->Release();
		
		//}

		pEl->Release();
	}
}
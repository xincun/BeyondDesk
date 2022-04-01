#pragma once

#include "StdAfx.h"

class CNetManager: public INetManager
{
	bool m_bNetAlive = false;
public:
	bool IsNetAlive();
	void WriteIECookie(CWebBrowserUI *pWBUI);
	void DisableIEScroll(CWebBrowserUI *pWBUI);
	void CNetManager::OnTimer(void * pOwnder, UINT iTimerID);

	CNetManager();
	~CNetManager();
};


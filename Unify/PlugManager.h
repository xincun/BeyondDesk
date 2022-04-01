#pragma once

#include "PlugDefine.h"
#include <map>

class CPlugManager : public IPlugManager
{
	typedef std::multimap<int, TPlugInfo*> TPlugList;
	TPlugList m_mapPlugs;

	//加载到管理器列表结构里
	bool LoadPlugIntfs(TPlugInfo* pPlugInfo);
public:
	CPlugManager();
	virtual ~CPlugManager();

	virtual void LoadPlugs(LPCTSTR szPlugPath);
	virtual void UnLoadPlugs();
	virtual void GetPlugsByType(TPlugInfoList& outVector, UINT plgType);
	virtual TPlugInfo* GetPlugInfoByName(LPCTSTR szName);
};


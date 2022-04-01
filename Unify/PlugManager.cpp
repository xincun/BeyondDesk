#include "stdafx.h"
#include "PlugManager.h"
#include <algorithm>

CPlugManager::CPlugManager()
{

}


CPlugManager::~CPlugManager()
{
}

void CPlugManager::LoadPlugs(LPCTSTR szPlugPath)
{
	
	vector<TString> vecFiles;
	File::RecruseDirFiles(vecFiles, szPlugPath, true,  _T("*.dll"));
	//LOGCON("%d", vecFiles.size());
	for_each(vecFiles.begin(), vecFiles.end(), 
		[&](const TString& sFile) //lambada
		{
		
		LOGCONW(L"Load Plugin: %s \n", sFile.c_str());
		//Pub::ShowMessage(sFile.c_str());
		
			HINSTANCE hDll = LoadLibrary(sFile.c_str());//加载DLL,需要将DLL放到工程目录下. 
			if (hDll != NULL)
			{
				lpFuncGetInfo func = (lpFuncGetInfo)GetProcAddress(hDll, "GetPlugInfo");

				if (func != NULL)
				{
					TPlugInfo *pPlugInfo = func(gUnify);
					pPlugInfo->Instance = hDll;
					pPlugInfo->FileName = (sFile);
					pPlugInfo->Path = (Path::GetParetnDir(sFile.c_str()));
					
					m_mapPlugs.insert(make_pair(pPlugInfo->Order, pPlugInfo)); //按Order排序
				}
				else
				{
					//printf("the calling is error");
				}
			}
		});
	
	auto iter = m_mapPlugs.begin();
	for (; iter!= m_mapPlugs.end();)
	{
		TPlugInfo* pPlugInfo = iter->second;
		LOGCONW(L"Load Plugin Interfaces: %s \n", pPlugInfo->Name.c_str());
		if (!LoadPlugIntfs(pPlugInfo))
		{
			FreeLibrary(pPlugInfo->Instance);
			iter = m_mapPlugs.erase(iter);
		}
		else
		    iter++;
	}
}

bool CPlugManager::LoadPlugIntfs(TPlugInfo* pPlugInfo)
{
	bool bRet = true;

	for (int i = 0; i < pPlugInfo->PlugIntfCount; i++)
	{
		lpFuncGetIntf funcGet = NULL;
		if (i > 0)
			funcGet = (lpFuncGetIntf)GetProcAddress(pPlugInfo->Instance, ("GetPlugIntf" + String::IntToStrA(i + 1)).c_str());
		else
			funcGet = (lpFuncGetIntf)GetProcAddress(pPlugInfo->Instance, "GetPlugIntf");

		if (!funcGet) continue;

		IPlugBase* pPlugIntf = funcGet(); //创建借口，借此创建插件内部对象
		
		if ((pPlugInfo->Type & PLUG_TYPE_BOOT) == PLUG_TYPE_BOOT &&
			dynamic_cast<IBootPlug*>(pPlugIntf)) //类型完全匹配，多个接口时
		{
			//case ptBoot:
			{
				pPlugIntf->OnLoad(NULL);

				if (!pPlugInfo->Persistent)
				{
					pPlugIntf->OnUnload(NULL);
					pPlugIntf->OnDestroy();

					delete pPlugIntf;
					bRet = false;
					continue;
				}

				//break;
			}
		}

		pPlugInfo->AddPlugIntf(pPlugIntf);
	}

	return bRet;
}

void CPlugManager::UnLoadPlugs()
{
	for_each(m_mapPlugs.begin(), m_mapPlugs.end(),
		[&](TPlugList::reference item) //lambada
		{
			TPlugInfo *pPlugInfo = item.second;
			HINSTANCE  hInstance = pPlugInfo->Instance;

			for (int i = 0; i < pPlugInfo->PlugIntfCount; i++)
			{
				IPlugBase* pPlug = pPlugInfo->GetPlugIntf(i);

				//switch (pPlugInfo->Type)
				if ((pPlugInfo->Type & PLUG_TYPE_BOOT) == PLUG_TYPE_BOOT&&
					dynamic_cast<IBootPlug*>(pPlug))
				{
				//case ptBoot:
					pPlug->OnUnload(NULL); //boot 插件在此调用OnUnload，其他可能在窗口上调用
					break;
				}

				pPlug->OnDestroy();
				delete pPlug;

			}
			
			FreeLibrary(hInstance);  //会调用插件里的FreePlugIntf --> OnDestroy
		});
}

void CPlugManager::GetPlugsByType(TPlugInfoList& outVector, UINT plgType)
{
	for_each(m_mapPlugs.begin(), m_mapPlugs.end(),
		[&](TPlugList::reference item) //lambada
	{
		TPlugInfo *pPlugInfo = item.second;
		if ((pPlugInfo->Type & plgType) == plgType)
		{
			outVector.push_back(pPlugInfo);
		}
	});
}

TPlugInfo* CPlugManager::GetPlugInfoByName(LPCTSTR szName)
{
	for (auto& item : m_mapPlugs)
	{
		TPlugInfo* pPlugInfo = item.second;
		if (pPlugInfo->Name == szName)
		{
			return pPlugInfo;
		}
	}

	return NULL;
}

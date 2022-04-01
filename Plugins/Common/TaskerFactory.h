#ifndef _TASKER_FACTORY_
#define  _TASKER_FACTORY_

#pragma once

#include "FactoryBase.h"
using namespace SimpleFactory;

//typedef CFactoryBase TaskerFactory;

class CTaskerFactory : public CFactoryBase
{
	struct TItem
	{
		CString Name;
		CString ParentName;
		std::string TypeID;
	};
public:
	//typedef tuple<CString, CString, int> TItem;


	vector<TItem> TaskerTypeList; //注册用//注册顺序就是显示顺序

	//注册Tasker描述，界面展示用, nPos插入位置，999=最后，0=最前，其它依次排列
	void RegisterTaskerDesc(const CString& sGroupName, const CString& sName, const std::string& nTypeID, int nPos = 999)
	{
		TItem item;
		item.Name = sName;
		item.ParentName = sGroupName;
		item.TypeID = nTypeID;

		TaskerTypeList.push_back(item);
	}

    //name, parentname
	tuple<CString, CString> GetTaskerDescByTypeID(const std::string& nTypeID)
	{
		for (auto& item : TaskerTypeList)
		{
			if (item.TypeID == nTypeID)
			{
				return std::make_tuple(item.Name, item.ParentName);
			}
		}
	}

	CString GetTaskerNameByTypeID(const std::string& nTypeID)
	{
		for (auto& item : TaskerTypeList)
		{
			if (item.TypeID == nTypeID)
			{
				return item.Name;
			}
		}
	}
};

struct CTaskerRegister
{
	CTaskerRegister();
};

extern CTaskerFactory gTaskerFactory;

#endif


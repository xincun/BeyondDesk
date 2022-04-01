#ifndef  _DBManager_H
#define _DBManager_H

#pragma once

#include "CppSqlite3U.h"
#include <iostream>

using namespace std;

class  DBManager
{
	CppSQLite3DB db;
	DBManager();
	static DBManager* gDB;
	TCHAR szSql[2048];
public:
	static DBManager* GetInstance() {
		if (gDB == nullptr) {
			gDB = new DBManager();
		}
		return gDB;
	}
	~DBManager();

	void Open(const TCHAR* szFile);

	void OpenMemroy(const TCHAR* szFile);

	void TryCreate();

	void Compat();
	void Clear();
	void Close();

	void SetConfig(LPCTSTR name, LPCTSTR value);

	void SetConfig(LPCTSTR name, int value);

	wstring GetConfigString(LPCTSTR name, LPCTSTR def = L"");

	int GetConfigInt(LPCTSTR name, int def = 0);

	void BeginTransaction();

	void CommitTransaction();

	void InsertSch(LPCTSTR uid, LPCTSTR rid, LPCTSTR strSch, LPCTSTR strDate);
	void UpdateSch(LPCTSTR uid, LPCTSTR rid, LPCTSTR strSch, LPCTSTR strDate);
	void DeleteSch(LPCTSTR uid);

	CppSQLite3Query Query(LPCTSTR strSql);
	CppSQLite3Query QueryMonth(int year, int mon);
	CppSQLite3Query QueryYear(int year);
};

DBManager* GetDB();

#endif
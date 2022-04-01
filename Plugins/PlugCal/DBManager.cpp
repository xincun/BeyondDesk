#include "stdafx.h"
#include "DBManager.h"


DBManager::DBManager()
{
}


DBManager* DBManager::gDB = nullptr;

DBManager* GetDB()
{
	return DBManager::GetInstance();
}

DBManager::~DBManager()
{
}

void DBManager::Open(const TCHAR* szFile)
{
	db.open(szFile);
}

void DBManager::OpenMemroy(const TCHAR* szFile)
{
	db.openMemroy(szFile);
}

void DBManager::TryCreate()
{
	if (!db.tableExists(L"sch")) {
		//db.execDML(L"CREATE TABLE files (id integer PRIMARY KEY, name char(260) NOT NULL, path char(260) NOT NULL);");
		db.execDML(L"CREATE TABLE sch (id integer PRIMARY KEY, uid char(256) NOT NULL, rid char(256), sch varchar NOT NULL, date char(32)  NOT NULL,  year integer NOT NULL,  mon integer NOT NULL, day integer NOT NULL);");
		db.execDML(L"CREATE INDEX sch_index ON sch (sch);");
	}

	if (!db.tableExists(L"config")) {
		db.execDML(L"CREATE TABLE config (id integer PRIMARY KEY, name char(260) NOT NULL, value char(260) NOT NULL) ;");
		//db.execDML(L"CREATE INDEX name_index ON files (name);");
	}
}

void DBManager::Compat() {
	db.execDML(L"VACUUM;");
}

void DBManager::Clear()
{
	if (db.tableExists(L"files")) {
		db.execDML(L"DROP TABLE files;");
		//db.execDML(L"delete from files;");
		db.execDML(L"VACUUM;");
	}
}

void DBManager::Close()
{
	db.close();
}

void DBManager::SetConfig(LPCTSTR name, LPCTSTR value)
{
	memset(szSql, 0, sizeof(szSql));
	swprintf(szSql, 2048,
		L"SELECT * FROM config WHERE name ='%s';",
		name);
	CppSQLite3Query q = Query(szSql);
	if (!q.eof()) {
		swprintf(szSql, 2048,
			L"UPDATE config SET value =  \"%s\" WHERE name = '%s';",
			value, name);
	}
	else
		swprintf(szSql, 2048,
			L"INSERT INTO config (name, value) VALUES (\"%s\", \"%s\");",
			name, value);
	db.execDML(szSql);
}

void DBManager::SetConfig(LPCTSTR name, int value)
{
	memset(szSql, 0, sizeof(szSql));
	swprintf(szSql, 2048,
		L"SELECT * FROM config WHERE name ='%s';",
		name);
	CppSQLite3Query q = Query(szSql);
	if (!q.eof()) {
		swprintf(szSql, 2048,
			L"UPDATE config SET value =  \"%d\" WHERE name = '%s';",
			value);
	}
	else
		swprintf(szSql, 2048,
			L"INSERT INTO config (name, value) VALUES (\"%s\", \"%d\");",
			name, value);
	db.execDML(szSql);
}

std::wstring DBManager::GetConfigString(LPCTSTR name, LPCTSTR def /*= L""*/)
{
	memset(szSql, 0, sizeof(szSql));
	swprintf(szSql, 2048,
		L"SELECT * FROM config WHERE name ='%s';",
		name);
	CppSQLite3Query q = Query(szSql);
	if (!q.eof()) {
		return q.fieldValue(2);
	}
	else
		return def;
}

void DBManager::BeginTransaction()
{
	db.execDML(L"begin transaction;");
}

void DBManager::CommitTransaction()
{
	db.execDML(L"commit transaction;");
}

void DBManager::InsertSch(LPCTSTR uid, LPCTSTR rid, LPCTSTR strSch, LPCTSTR strDate)
{
	//TCHAR *charTime = (TCHAR*)strDate;
	CString sFormat = _T("%d-%d-%d");
	//TCHAR *charFormat = (TCHAR*)(LPCTSTR)sFormat;
	int nYear;
	int nMonth;
	int nDay;
	/*int nHour;
	int nMin;
	int nSec;*/
	_stscanf(strDate, sFormat.GetBuffer(), &nYear, &nMonth, &nDay);

	memset(szSql, 0, sizeof(szSql));
	swprintf(szSql, 2048,
		L"INSERT INTO sch (uid, rid, sch, date, year, mon, day) VALUES (\"%s\", \"%s\", \"%s\", \"%s\", %d, %d, %d);",
		uid, rid, strSch, strDate, nYear, nMonth, nDay);
	/*swprintf(szSql, 2048,
		L"INSERT INTO files (name) VALUES (\"%s\");",
		strName);*/
	db.execDML(szSql);
}

void DBManager::UpdateSch(LPCTSTR uid, LPCTSTR rid, LPCTSTR strSch, LPCTSTR strDate)
{
	CString sFormat = _T("%d-%d-%d");
	//TCHAR *charFormat = (TCHAR*)(LPCTSTR)sFormat;
	int nYear;
	int nMonth;
	int nDay;
	/*int nHour;
	int nMin;
	int nSec;*/
	_stscanf(strDate, sFormat.GetBuffer(), &nYear, &nMonth, &nDay);

	memset(szSql, 0, sizeof(szSql));
	swprintf(szSql, 2048,
		L"SELECT * FROM sch WHERE uid ='%s';", uid);
	CppSQLite3Query q = Query(szSql);
	if (!q.eof()) {
		swprintf(szSql, 2048,
			L"UPDATE sch SET rid =  \"%s\", sch =  \"%s\", date =  \"%s\", year =  \"%d\", mon =  \"%d\",day =  \"%d\" WHERE uid = '%s';",
			rid, strSch, strDate, nYear, nMonth, nDay, uid);
	}

	db.execDML(szSql);
}

void DBManager::DeleteSch(LPCTSTR uid)
{
	memset(szSql, 0, sizeof(szSql));
	swprintf(szSql, 2048,
		L"DELETE FROM sch where uid='%s';", uid);

	db.execDML(szSql);
}

CppSQLite3Query DBManager::Query(LPCTSTR strSql)
{
	return db.execQuery(strSql);
}

CppSQLite3Query DBManager::QueryMonth(int year, int mon)
{
	memset(szSql, 0, sizeof(szSql));
	swprintf(szSql, 2048,
		L"SELECT * FROM sch where year=%d and mon=%d;",
		year, mon);
	/*swprintf(szSql, 2048,
	L"INSERT INTO files (name) VALUES (\"%s\");",
	strName);*/
	return db.execQuery(szSql);
}

CppSQLite3Query DBManager::QueryYear(int year)
{
	memset(szSql, 0, sizeof(szSql));
	swprintf(szSql, 2048,
		L"SELECT * FROM sch where year=%d;",
		year);
	
	return db.execQuery(szSql);
}

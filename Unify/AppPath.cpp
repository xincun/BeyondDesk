#include "stdafx.h"
#include "AppPath.h"

CAppPath* GlobalPath = new CAppPath;

CAppPath::CAppPath()
{
	ExePath = Util::Path::GetAppDir();
	ExeFullPath = Util::Path::GetAppPath();

	//UserPath = Util::Path::GetSysDir(CSIDL_APPDATA) + _T("\\BeyondDesk\\Data\\");
	//�̻����ݣ���װ�Դ�
	DataPath = ExePath + _T("Data\\");

	//�û���������
	UserPath = Util::Path::GetSysDir(CSIDL_LOCAL_APPDATA);
	UserPath += L"\\BeyondDesk\\User\\";

	LogPath += UserPath + L"\\Logs\\";
	//UserPath = ExePath + _T("User\\");
	////��װ�������õ��ļ���·��
	//CIniFile ini;
	//CString sPath = DataPath.c_str();
	//sPath += L"setup.ini";
	//ini.SetFileName(sPath);
	//CString sDir = ini.GetString(L"base", L"DataDir", L"");
	//TString strDir = sDir.GetBuffer();
	//Util::Path::InsurePathLastDelimiter(strDir);

	//if (sDir.IsEmpty())
	//    UserPath = ExePath + _T("User\\");
	//else {
	//	strDir += L"BeyondDesk\\User\\";
	//	if (!Util::File::CreateFolder(strDir.c_str())) {
	//		UserPath = Util::Path::GetSysDir(CSIDL_PERSONAL);
	//		UserPath += L"\\BeyondDesk\\User\\";
	//	}
	//}

	ResPath = ExePath + _T("Res\\");
	//TempPath = Util::Path::GetSysDir(CSIDL_APPDATA) + _T("\\BeyondDesk\\Temp\\");
	TempPath = UserPath + _T("Temp\\");
	SoundPath = DataPath + _T("Sound\\");

	PlugPath = ExePath + _T("Plugins\\");

	CustomSkinFilesPath = UserPath + _T("Custom\\Skins\\");

	if (!Util::File::FolderExist(DataPath.c_str()))
	{
		Util::File::CreateFolder(DataPath.c_str());
	}
	if (!Util::File::FolderExist(UserPath.c_str()))
	{
		Util::File::CreateFolder(UserPath.c_str());
	}
	if (!Util::File::FolderExist(LogPath.c_str()))
	{
		Util::File::CreateFolder(LogPath.c_str());
	}
	if (!Util::File::FolderExist(TempPath.c_str()))
	{
		Util::File::CreateFolder(TempPath.c_str());
	}
	if (!Util::File::FolderExist(CustomSkinFilesPath.c_str()))
	{
		Util::File::CreateFolder(CustomSkinFilesPath.c_str());
	}
}

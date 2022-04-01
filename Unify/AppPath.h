//************************************
//
//����ĳ���·��֧��
//Author: LX.g
//***

#pragma once

#include "Utility.h"
#include "IniFile.h"

class CAppPath
{
public:
	TString ExeFullPath; //exeȫ·������������
	TString ExePath;
	TString UserPath;
	TString LogPath;
	TString DataPath;
	TString ResPath;
	TString TempPath;
	TString SoundPath;
	TString PlugPath;
	TString CustomSkinFilesPath;

	CAppPath();

	//void create_user_private_path(const char *userUniqueId);
private:
	TString GetAppDataPath();
};

extern CAppPath* GlobalPath;
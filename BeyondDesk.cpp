#include "stdafx.h"
#include "MainFrame.h"
#include "resource.h"
#include "Img/GDIPHelper.h"
#include "AVCapWrapper.h"
#include "CmdLine.h"
#include "ThreadHelper.h"
#include "CodeTest.h"
#include "Unify/Frame.h"


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{	
	TString sPath = GlobalPath->LogPath + L"app.log";
	gLog.SetLogFile(sPath.c_str());

	CodeTest::test();

	LOGCON("Main App init, GDIPHelper and CWndShadow");
	GDIPHelper::Initialize();
	CWndShadow::Initialize(hInstance);
	
	CPaintManagerUI::SetInstance(hInstance);
	
	CCmdLine cmd;
	cmd.SetCmdLine(lpCmdLine);

	if (cmd.GetParamByIndex(0) == _T("videocap"))
	{
		//printf("AVCAP OPERATE......");
		//OutputDebugStringA("av cap inti opera...");

		return CVideoCapProc::Proc();
	}
	else if (cmd.GetParamByIndex(0) == _T("audiocap"))
	{
		return CAudioCapProc::Proc();
	}
	else if (cmd.GetParamByIndex(0) == _T("scrrec"))
	{
		return CScrRecProc::Proc();
	}
	
	LOGCONW(lpCmdLine);
	bool bCheckOne = true;
	if (cmd.GetParamByIndex(0) == _T("-restore")) //放这里防止多个实例判断退出
	{
		LOGCON("恢复数据");
		bCheckOne = false;
		Util::File::CopyDir(cmd.GetParamByIndex(1).c_str(), cmd.GetParamByIndex(2).c_str());
	}
	
	LOGCON("Main App Setp 0, Frame Init");
	if (!Frame::Init(lpCmdLine, MAIN_CLASS, MAIN_TITLE, bCheckOne)) return false;
	LOGCON("Main App Setp 1, Resotre Old Data");
	gUnify->DB()->SysConfig()->SetString(_T("Base"), _T("ver"), APP_VER);

	//恢复旧版数据位置到新位置
	TString OldUserPath = Util::Path::GetSysDir(CSIDL_APPDATA);
	OldUserPath += L"\\BeyondDesk\\";
	TString NewUserPath = gUnify->App()->Path()->UserPath;
	vector<TString> listFiles;
	File::RecruseDirFiles(listFiles, OldUserPath.c_str(), true);
	if (listFiles.size() > 0) { //旧版存在,拷贝到新版,删除旧版
		Util::File::CopyDir(OldUserPath.c_str(), NewUserPath.c_str());
		Util::File::DeleteDir(OldUserPath.c_str());
	}

	LOGCON("Main App Setp 2, try add init tasks");
	//第一次运行时，添加初始化任务
	if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("FirstRun"), true)) {
		LOGCON("首次运行");
		TString sFrom = gUnify->App()->Path()->DataPath + L"Hotkeys.init";
		TString sTo = gUnify->App()->Path()->UserPath + L"Hotkeys.dat";
		if (!File::FileExist(sTo.c_str()))
		    CopyFile(sFrom.c_str(), sTo.c_str(), false);
		 sFrom = gUnify->App()->Path()->DataPath + L"Tasks.init";
		 sTo = gUnify->App()->Path()->UserPath + L"Tasks.dat";
		if (!File::FileExist(sTo.c_str()))
			CopyFile(sFrom.c_str(), sTo.c_str(), false);
	}
	
	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr)) return 0;
	
	CMainFrameWnd* pFrame = new CMainFrameWnd();
	if (pFrame == NULL) return 0;
	LOGCON("Main App Setp 3, create main wnd");
	//pFrame->InstallTray();
	pFrame->setCanCheckReg(false); //否则此处会触发showwindow消息，检测注册，弹出提示窗口，和下面的启动时最小化冲突，启动时最小化不需要检测
	pFrame->Create(NULL, MAIN_TITLE, UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW , 0, 0, 10, 10);
	pFrame->setCanCheckReg(true);
	pFrame->SetIcon(IDR_MAINFRAME);
	//pFrame->CenterWindow();
	LOGCON("Main App Setp 4, show main wnd and tray");
	//if (gUnify->DB()->SysConfig()->GetBool(_T("Base"), _T("chkStartToTray"), false))
	if (gUnify->App()->CmdLine()->GetParamByIndex(0) == _T("-m"))
	{
		::ShowWindow(*pFrame, SW_HIDE);
	}
	else
	{
		::ShowWindow(*pFrame, SW_SHOW);
	}
#ifdef ENABLE_TRAY
	gUnify->App()->Tray()->Create(hInstance, pFrame->GetHWND(), MAIN_TITLE,
		::LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)), MAIN_TITLE);
#endif
	LOGCON("Main App Setp 4, enter msg loop");
	CPaintManagerUI::MessageLoop();

	LOGCON("Main App clear Setp 0, CAVCapDLL Clear");
	CAVCapDLL::Clear();

	//if (hRichEditDll != NULL)					
		//::FreeLibrary(hRichEditDll);
	LOGCON("Main App clear Setp 1, unload plugs");
#ifdef ENABLE_PLUG
	gUnify->PlugManager()->UnLoadPlugs();
#endif

	LOGCON("Main App clear Setp 2, GDIPHelper Shutdown");
	GDIPHelper::Shutdown();

	LOGCON("Main App clear Setp 3, CoUninitialize");
	::CoUninitialize();

	//LOGCON("Main App clear Setp 4, delete gUnify");
	//delete gUnify;

	LOGCON("Main App clear done!");

	return 0;
}

#include "stdafx.h"
#include "CodeTest.h"
//#include "../BeyondDesk/Reg/Reg.h"
//#include "../BeyondDesk/Reg/SimpleEncrypt.h"
//#include "../ZipWrapper/include/ZHead.h"
#include <regex>

class A
{
public:
	void dosth() {
		int n = 1;
		cout << n << endl;
	};
	void free() {
		delete this;
	}
};

void do1(std::shared_ptr<A> p)
{

}

void do2(std::shared_ptr<A> p)
{
	p->dosth();
}

void do11(A* p)
{
	//p->free();
	delete p;
}

void do21(A* p)
{
	p->dosth();
}




void CodeTest::test()
{
	
	
	int nnnn = 1;
	//Util::Hardware::GetMonitorNameWithHandleList();
	
	//gLog.InsureConsole();

	// DISPLAY_DEVICE dd;
	// dd.cb = sizeof(dd);
	// int deviceIndex = 0;
	// while (EnumDisplayDevices(0, deviceIndex, &dd, 0))
	// {
	//
	// 	std::wstring deviceName = dd.DeviceName;
	// 	int monitorIndex = 0;
	// 	while (EnumDisplayDevices(deviceName.c_str(), monitorIndex, &dd, 0))
	// 	{
	// 		std::wcout << dd.DeviceName << L", " << L", " << dd.DeviceString << L"\n";
	// 		++monitorIndex;
	// 	}
	// 	++deviceIndex;
	// }

	
	// POINT pt;
	// pt.x = 0;
	// pt.y = 0;
	// HMONITOR hMonitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
	//
	//
	// DWORD num;
	// GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &num);
	// int n = GetMonitorIndex(hMonitor);
	//
	// sEnumInfo info;
	// info.iIndex = 1;
	// info.hMonitor = NULL;
	//

	
	/*gLog.InsureConsole();

	CString s = getHDSN();
	

	char szAddr[20];*/
	/*GetMacAddress(szAddr);
	cout << "\nNet adapter MAC Address:" << szAddr << endl << endl;*/

	/*OSVERSIONINFO VersionInfo;

	ZeroMemory(&VersionInfo, sizeof(VersionInfo));
	VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);
	GetVersionEx(&VersionInfo);

	switch (VersionInfo.dwPlatformId) {
	case VER_PLATFORM_WIN32s:
		cout << "Win32s is not supported by this programm." << endl;
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		hdid9x();
		break;
	case VER_PLATFORM_WIN32_NT:
		hdidnt();
		break;
	}*/

	//char pCpuId[32] = ""; //和reg.cpp里简单获取一样结果
	//get_cpuId(pCpuId);
	//cout << pCpuId << endl;
	//system("pause");
	/*char vendor[12 + 1] = { 0 };
	{
		CpuInfo f0(0);
		*(uint32_t*)&vendor[4 * 0] = f0.ebx;
		*(uint32_t*)&vendor[4 * 1] = f0.edx;
		*(uint32_t*)&vendor[4 * 2] = f0.ecx;
	}
	vendor[sizeof(vendor) - 1] = 0;

	char brand[16 * 3 + 1] = { 0 };
	for (int i = 0; i < 3; ++i) {
		CpuInfo e(i + 0x80000002);
		*(uint32_t*)&brand[16 * i + 4 * 0] = e.eax;
		*(uint32_t*)&brand[16 * i + 4 * 1] = e.ebx;
		*(uint32_t*)&brand[16 * i + 4 * 2] = e.ecx;
		*(uint32_t*)&brand[16 * i + 4 * 3] = e.edx;
	};
	brand[sizeof(brand) - 1] = 0;

	printf("Vendor       : [%s]\n", vendor);
	printf("Brand String : [%s]\n", brand);

	CpuInfo f1(1);
	printf("CMOV         : %d\n", (f1.edx >> 15) & 1);
	printf("FMA          : %d\n", (f1.ecx >> 12) & 1);
	printf("AVX          : %d\n", (f1.ecx >> 28) & 1);
	printf("RDRAND       : %d\n", (f1.ecx >> 30) & 1);*/
	//volatile int i = 10;

	//unsigned eax, ebx, ecx, edx;

	//eax = 1; /* processor info and feature bits */
	//native_cpuid(&eax, &ebx, &ecx, &edx);

	//printf("stepping %d\n", eax & 0xF);
	//printf("model %d\n", (eax >> 4) & 0xF);
	//printf("family %d\n", (eax >> 8) & 0xF);
	//printf("processor type %d\n", (eax >> 12) & 0x3);
	//printf("extended model %d\n", (eax >> 16) & 0xF);
	//printf("extended family %d\n", (eax >> 20) & 0xFF);

	///* EDIT */
	//eax = 3; /* processor serial number */
	//native_cpuid(&eax, &ebx, &ecx, &edx);

	///** see the CPUID Wikipedia article on which models return the serial
	//number in which registers. The example here is for
	//Pentium III */
	//printf("serial number 0x%08x%08x\n", edx, ecx);

	//2551594259 bcdedit /set hypervisorlaunchtype off 32位
	//2953479059 bcdedit /set hypervisorlaunchtype off 64位
	//2351893201 bcdedit /set hypervisorlaunchtype auto 64位

	//CString sCode = CSimpleEncrypt::encrypt(L"4258544449"); //jr9Mcj42fqGtMuJO5CdcBHiG4rn=
	//CReg::showRegWnd();
	//CReg::showWaitWnd();
	/*std::shared_ptr<A> p(new A);
	do1(p);
	do2(p);*/

	//A* pp = nullptr;
	////do11(pp);
	////delete pp;
	//
	//do21(pp);

	//CCmdLine* pcmd;
	//pcmd->GetCmdLine();

	/*StaticTest::setN(11);
	int x = StaticTest::getN();
	StaticTest::setn(11);
	int y = StaticTest::getn();*/

	//wregex r(L"1师德师风_\\d{4}-\\d{2}-\\d{2}\\s{1}\\d{2}\\.\\d{2}\\.\\d{2}\\.zip");
	//bool b = regex_match(L"1师德师风2_2019-10-21 04.40.35.zip", r);
	//cout << boolalpha <<  << endl;

	//ZipDirectory(L"C:\\Users\\Administrator\\AppData\\Roaming\\BeyondDesk\\Weather", L"D:\\OneDrive\\桌面\\1.zip");
	//UnZipToDirectory(L"D:\\OneDrive\\桌面\\1111111.ktb", L"D:\\OneDrive\\桌面\\zip");
	//D7KN7lnmmn8p6d�mp%!KlsJzKL7W
	/*CString m = CReg::getMachineCode();
	CString s = CSimpleEncrypt::encrypt(m);
	CString ss = CSimpleEncrypt::decrypt(s);*/
	//CReg::getInstallDate();
	//installDate多放几个地方？放别的地方？
	//CRC校验？

	/*DLLInvoker::SetMute();
	Window::ShowMessage(L"已经静音");

	DLLInvoker::SetMute(false);
	Window::ShowMessage(L"已经恢复静音");*/

	/*DLLInvoker::ChangeVol(30);
	Window::ShowMessage(L"增加30音量");

	DLLInvoker::ChangeVol(30, false);
	Window::ShowMessage(L"减小30音量");

	DLLInvoker::ChangeVolTo(30);
	Window::ShowMessage(L"改到30音量");*/
	//DLLInvoker::GetVol();
	//DLLInvoker::SendMail("lixingang", "lixingang@mail.51vv.com", "neo", "lightlab@163.com", "te", "dll invok te", "mail.51vv.com", "25", "lixingang", "D42bVoEm");

	//CTestWnd* pWnd = NewFrameWindow<CTestWnd>(_T("XML\\DlgAbout.xml"), _T("XXX"));

	//   pWnd->CenterWindow();
	//::ShowWindow(*pWnd, SW_SHOW);
}

CodeTest::CodeTest()
{
	
}

//结果相同，dll和主程序各一份不同变量
//int CodeTestN = 0;
static int CodeTestN = 0;
int CodeTest::n = 0;

void CodeTest::setn(int n)
{
	CodeTest::n = n;
}

int CodeTest::getn()
{
	return CodeTest::n;
}

void CodeTest::setN(int n)
{
	CodeTestN = n;
}

int CodeTest::getN()
{
	return CodeTestN;
}



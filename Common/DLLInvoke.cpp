#include "stdafx.h"
#include "DLLInvoke.h"

bool DLLInvoker::g_bInitVol = false;

typedef void (FAR __stdcall *ProcShutDown)(BOOL isForce);
typedef void (FAR __stdcall *ProcSvc)(LPCSTR SvcName, BOOL Start);
typedef void (FAR __stdcall *ProcEncrypt)(LPCSTR Out, LPCSTR Src, LPCSTR Key, BOOL isEncrypt);
typedef void (FAR __stdcall *ProcStandby)();
typedef void (FAR __stdcall *ProcClearMem)();
typedef void (FAR __stdcall *ProcDeleteIECache)();
typedef void (FAR __stdcall *ProcEmptyRecycleBin)();
//typedef LPCSTR (FAR __stdcall *ProcSendMail)(LPCSTR SenderName, LPCSTR SenderAddr, LPCSTR ReceiverName, LPCSTR ReceiverAddr,
//	LPCSTR Title, LPCSTR Content, LPCSTR Host, LPCSTR Port, LPCSTR UserName, LPCSTR Password);



typedef void(_cdecl *ProcVolInit)();
typedef void(_cdecl *ProcVolUnInit)();
typedef bool(_cdecl *ProcSetMute)(bool);
typedef bool(_cdecl *ProcChangeVol)(UINT, bool);
typedef bool(_cdecl *ProcChangeVolTo)(UINT);
typedef UINT(_cdecl *ProcGetVol)();
typedef bool(_cdecl *ProcGetMute)();

#define SYS_UTIL_DLL_NAME  _T("SysUtil.dll")
#define  VOL_DLL_NAME  _T("Volume.dll")

//#define SEND_MAIL_DLL_NAME  _T("SendMail.dll")





//void DLLInvoker::InitVol()
//{
//	if (!g_bInitVol)
//	{
//		HINSTANCE hInst = LoadLibrary(VOL_DLL_NAME);
//		if (!hInst) return;
//
//		ProcVolInit p = (ProcVolInit)::GetProcAddress(hInst, "Initialize"); //
//		p();
//		::FreeLibrary(hInst);
//	}
//}
//
//void DLLInvoker::UnInitVol()
//{
//	
//
//	g_bInitVol = false;
//}

void DLLInvoker::SetMute(bool bMute /*= true*/)
{
	HINSTANCE hInst = LoadLibrary(VOL_DLL_NAME);
	if (!hInst) return;

	ProcSetMute p = (ProcSetMute)::GetProcAddress(hInst, "SetMute"); //
	p(bMute);
	::FreeLibrary(hInst);
}

bool DLLInvoker::GetMute()
{
	HINSTANCE hInst = LoadLibrary(VOL_DLL_NAME);
	if (!hInst) return false;

	ProcGetMute p = (ProcGetMute)::GetProcAddress(hInst, "GetMute"); //
	bool ret = p();
	::FreeLibrary(hInst);
	return ret;
}

void DLLInvoker::ChangeVol(UINT nVol, bool bAdd /*= true*/)
{
	HINSTANCE hInst = LoadLibrary(VOL_DLL_NAME);
	if (!hInst) return;

	ProcChangeVol p = (ProcChangeVol)::GetProcAddress(hInst, "ChangeVol"); //
	p(nVol, bAdd);
	::FreeLibrary(hInst);
}

void DLLInvoker::ChangeVolTo(UINT nVol)
{
	HINSTANCE hInst = LoadLibrary(VOL_DLL_NAME);
	if (!hInst) return;

	ProcChangeVolTo p = (ProcChangeVolTo)::GetProcAddress(hInst, "ChangeToVol"); //
	p(nVol);
	::FreeLibrary(hInst);
}

UINT DLLInvoker::GetVol()
{
	HINSTANCE hInst = LoadLibrary(VOL_DLL_NAME);
	if (!hInst) return 0;

	ProcGetVol p = (ProcGetVol)::GetProcAddress(hInst, "GetVol"); //
	UINT nVol = p();
	::FreeLibrary(hInst);
	return nVol;
}

void DLLInvoker::ClearMem()
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if (!hInst) return;

	ProcClearMem p = (ProcClearMem)::GetProcAddress(hInst, "ClearMem"); //
	p();
	::FreeLibrary(hInst);
}

void DLLInvoker::DeleteIECache()
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if (!hInst) return;

	ProcDeleteIECache p = (ProcDeleteIECache)::GetProcAddress(hInst, "DeleteIECache"); //
	p();
	::FreeLibrary(hInst);
}

void DLLInvoker::EmptyRecycleBin()
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if (!hInst) return;

	ProcEmptyRecycleBin p = (ProcEmptyRecycleBin)::GetProcAddress(hInst, "EmptyRecycleBin"); //
	p();
	::FreeLibrary(hInst);
}

//string DLLInvoker::SendMail(LPCSTR SenderName, LPCSTR SenderAddr, LPCSTR ReceiverName, LPCSTR ReceiverAddr,
//	LPCSTR Title, LPCSTR Content, LPCSTR Host, LPCSTR Port, LPCSTR UserName, LPCSTR Password)
//{
//	HINSTANCE hInst = LoadLibrary(SEND_MAIL_DLL_NAME);
//	if (!hInst) return false;
//
//	ProcSendMail p = (ProcSendMail)::GetProcAddress(hInst, "SendMail"); //
//	string s= p(SenderName, SenderAddr, ReceiverName, ReceiverAddr, Title, Content, Host, Port, UserName, Password);
//	::FreeLibrary(hInst);
//
//	return s;
//}

void DLLInvoker::StartSvc(LPCSTR szSvcName)
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if (!hInst) return;

	ProcSvc p = (ProcSvc)::GetProcAddress(hInst, "StartSvc"); //
	p(szSvcName, TRUE);
	::FreeLibrary(hInst);
}

void DLLInvoker::StopSvc(LPCSTR szSvcName)
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if (!hInst) return;

	ProcSvc p = (ProcSvc)::GetProcAddress(hInst, "StartSvc"); //
	p(szSvcName, FALSE);
	::FreeLibrary(hInst);
}

void DLLInvoker::EncryptEngine(LPCSTR Out, LPCSTR Src, LPCSTR Key, BOOL isEncrypt)
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if (!hInst) return;

	ProcEncrypt p = (ProcEncrypt)::GetProcAddress(hInst, "EncryptEngine"); //
	p(Out, Src, Key, isEncrypt);
	::FreeLibrary(hInst);
}

void DLLInvoker::LogOff( BOOL isForce /*= TRUE*/ )
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if(!hInst) return;
  
	ProcShutDown p = (ProcShutDown)::GetProcAddress(hInst,"LogOff"); //
	p(isForce); 
	::FreeLibrary(hInst);
}

void DLLInvoker::ShutDown( BOOL isForce /*= TRUE*/ )
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if(!hInst) return;

	ProcShutDown p = (ProcShutDown)::GetProcAddress(hInst,"ShutDown"); //
	p(isForce); 
	::FreeLibrary(hInst);
}

void DLLInvoker::Reboot( BOOL isForce /*= TRUE*/ )
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if(!hInst) return;

	ProcShutDown p = (ProcShutDown)::GetProcAddress(hInst,"Reboot"); //
	p(isForce); 
	::FreeLibrary(hInst);
}

void DLLInvoker::StandBy( BOOL isForce /*= TRUE*/ )
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if(!hInst) return;

	ProcStandby p = (ProcStandby)::GetProcAddress(hInst,"StandBy"); //
	p(); 
	::FreeLibrary(hInst);
}

void DLLInvoker::Hibernate( BOOL isForce /*= TRUE*/ )
{
	HINSTANCE hInst = LoadLibrary(SYS_UTIL_DLL_NAME);
	if(!hInst) return;

	ProcStandby p = (ProcStandby)::GetProcAddress(hInst,"Hibernate"); //
	p(); 
	::FreeLibrary(hInst);
}

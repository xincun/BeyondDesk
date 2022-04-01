#pragma once

class DLLInvoker
{
	static bool g_bInitVol;
public:
	static void LogOff(BOOL isForce = TRUE);
	static void Hibernate( BOOL isForce = TRUE);
	static void StandBy( BOOL isForce = TRUE );
	static void Reboot( BOOL isForce = TRUE );
	static void ShutDown( BOOL isForce = TRUE);
	static void StartSvc(LPCSTR szSvcName);
	static void StopSvc(LPCSTR szSvcName);
	static void EncryptEngine(LPCSTR Out, LPCSTR Src, LPCSTR Key, BOOL isEncrypt);
	static void ClearMem();
	static void DeleteIECache();
	static void EmptyRecycleBin();
	//static string SendMail(LPCSTR SenderName, LPCSTR SenderAddr, LPCSTR ReceiverName, LPCSTR ReceiverAddr,
	//	LPCSTR Title, LPCSTR Content, LPCSTR Host, LPCSTR Port, LPCSTR UserName, LPCSTR Password);

	/*static void InitVol();

	static void UnInitVol();*/

	static void SetMute(bool bMute = true);
	static bool GetMute();

	static void ChangeVol(UINT nVol, bool bAdd = true);

	static void ChangeVolTo(UINT nVol);

	static UINT GetVol();

	
};


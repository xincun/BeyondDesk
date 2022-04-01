#pragma once

class TrayClockProxy
{
	//HWND TrayClockMsgWnd = NULL;
	TrayClockProxy();


	void Load(HWND mainWnd);
	 void SetInt(LPCTSTR szSection, LPCTSTR szName, int nValue);
	 void SetStr(LPCTSTR szSection, LPCTSTR szName, LPCTSTR szValue);
	 static TrayClockProxy* _instance;
public:
	~TrayClockProxy();
	static TrayClockProxy* GetInstance() {
		if (!_instance) {
			_instance = new TrayClockProxy();
		}
		return _instance;
	}

	void OnTimer(void* pOwner, UINT uiTimerID);
	/*HWND GetTrayClockMsgWnd() const { return TrayClockMsgWnd; }
	void SetTrayClockMsgWnd(HWND val) { TrayClockMsgWnd = val; }*/

	void RefreshConfig();

	void Unload();

	void OnEvent(CEvent* e);
};


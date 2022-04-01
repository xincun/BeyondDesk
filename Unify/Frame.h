#pragma once

#include "stdafx.h"

using namespace  Util;

struct Frame
{
	static bool Init(LPCTSTR szCmdLine, LPCTSTR szMainWndClass, LPCTSTR szMainTitle, bool checkOneInstacne = true);

	static bool Clear();
private:
	
};


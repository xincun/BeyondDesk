//************************************
//
//程序的常用路径支持
//Author: LX.g
//***

#pragma once

#include "Utility.h"
#include <vector>

class IToolPlug;
class CDeskWnd;
struct TDeskWindow {
	IToolPlug* PlugObject;
	CDeskWnd* WndObject;
};

class CStatus
{
public:
	INT64 ResumeFromStandbyTime = 9999999999999999;
	INT64 ResumeFromHibernateTime = 9999999999999999;

	vector<TDeskWindow> DeskWindows;

	TString TodayWeatherDesc;
	TString WeatherImage;
	TString WeatherTemperature;
};

extern CStatus* GlobalStatus;
#ifndef __EVENTDEFINE_H__
#define __EVENTDEFINE_H__

#pragma once

///////////////////////////////////////////////////EVENT DEFINE///////////////////////////////

const int EVENT_HOLIDAY_GET_DONE = 1;
const int EVENT_MAIN_SHOW_DONE = 2;
const int EVENT_NEW_DAY = 3;
const int EVENT_CFG_FRESH = 4;

const int EVENT_WEATHER_UPDATE = 7;

const int EVENT_UI_LOG_NEW = 8;

//�ճ��Ѿ����
//��ͨ��
struct TSchItem
{
	TString DateTimeStr = L""; //�ճ�����
	TString Sch = L""; //�ճ�����
	TString OldSID = L""; //�ɵ��ճ�ID
	TString SID = L""; //�µ��ճ�ID
	TString RID = L""; //���������ID
};
const int EVENT_SCH_ADD = 101;
const int EVENT_SCH_UPDATE = 102;
const int EVENT_SCH_DELETE = 103;

const int EVENT_DESK_SCH_TODO_UPDATE = 104;
const int EVENT_DESK_SCH_TASK_UPDATE = 105;
const int EVENT_DESK_SCH_SCH_UPDATE = 106;

//�رչ���ʱ
const int EVENT_DESK_SCH_TODO_CLEAR = 107;
const int EVENT_DESK_SCH_TASK_CLEAR = 108;
const int EVENT_DESK_SCH_SCH_CLEAR = 109;

const int EVENT_DESK_WND_CLOSE = 110;

#endif // 


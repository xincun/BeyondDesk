#pragma  once;

#include "stdafx.h"
#include <map>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class CWeatherParser
{
	typedef map<CString, CString> TWeaDesc;
	
	lua_State * L = NULL;

	CString GetImg(const CString& sWea)
	{
		string sResult;
		USES_CONVERSION;
		string strWea = T2A(sWea);

		string sPath = T2A(gPlugInfo.Path.c_str());
		sPath += "img.lua";
		luaL_dofile(L, sPath.c_str());   

		lua_getglobal(L, "getImg");
		lua_pushstring(L, strWea.c_str());
		
		lua_call(L, 1, 1);
		
		sResult = lua_tostring(L, -1);

		/*if (strWea == "晴" ) sResult ="00";
		else if (strWea == "薄云" || strWea == "晴间多云") sResult = "01";
		else if (strWea == "少云" || strWea == "多云间晴") sResult = "00";
		else if (strWea == "多云") sResult = "02";
		else if (strWea == "热") sResult = "00";
		else if (strWea == "阴" || strWea.find("阴") != string::npos) sResult = "02";
		else if (strWea == "小雨") sResult = "07";
		else if (strWea == "雨" || strWea == "中雨" || strWea.find("雨") != string::npos) sResult = "08";
		else if (strWea == "大雨") sResult = "09";
		else if (strWea == "小阵雨") sResult = "03";
		else if (strWea == "阵雨" || strWea == "风雨") sResult = "03";
		else if (strWea == "大阵雨") sResult = "03";
		else if (strWea == "小雷雨") sResult = "04";
		else if (strWea == "雷雨") sResult = "04";
		else if (strWea == "雷暴(大雷雨)" || strWea == "雷暴" || strWea == "大雷雨" || strWea.find("雷") != string::npos) sResult = "04";
		else if (strWea == "小冰雨") sResult = "07";
		else if (strWea == "冰雨加雪(中冰雨)" || strWea == "冰雨加雪" || strWea == "中冰雨") sResult = "08";
		else if (strWea == "大冰雨") sResult = "08";
		else if (strWea == "雨加雪") sResult = "06";
		else if (strWea == "小雪" || strWea.find("小雪") != string::npos) sResult = "07";
		else if (strWea == "雪花") sResult = "08";
		else if (strWea == "大雪") sResult = "09";
		else if (strWea == "风雪") sResult = "10";
		else if (strWea.find("雪") != string::npos) sResult = "09";
		else if (strWea == "冰雹") sResult = "07";
		else if (strWea == "尘") sResult = "18";
		else if (strWea == "雾" || strWea.find("雾") != string::npos) sResult = "18";
		else if (strWea == "霾" || strWea.find("霾") != string::npos) sResult = "18";
		else if (strWea == "薄雾") sResult = "18";
		else if (strWea == "烟") sResult = "18";
		else if (strWea == "风") sResult = "18";
		else if (strWea == "寒冷") sResult = "18";*/

		////夜间
		//if (sResult == "00" || sResult == "01" || sResult == "03" || sResult == "18")
		//{
		//	CTime Now = CTime::GetCurrentTime();
		//	if (Now.GetHour() >= 18 || Now.GetHour() <= 7)
		//		sResult += "_2";
		//}

		CString s = A2T(sResult.c_str());
		s += _T(".png");
		return s;
	}
public:
	CWeatherParser(){
		L = luaL_newstate();        //创建lua运行环境
		luaopen_base(L);     // 加载Lua基本库
		luaopen_string(L);
		luaL_openlibs(L);     // 加载Lua通用扩展库
	}

	~CWeatherParser(){
		lua_close(L);
	}

	CString m_sUIRegion;
	TWeaDesc Now;
	vector<TWeaDesc> NowIndex;
	vector<TWeaDesc> Future;

	bool DoParse(const CString& sWeaData)
	{
		bool bSucc = false;

		USES_CONVERSION;
		string sJson = T2A(sWeaData);

		Json::Reader* reader = new Json::Reader;
		Json::Value *root = new Json::Value; //reader->ReadFromFile("D:\\[Work]\\desktop\\BeyondDesk\\bin\\Debug\\Data\\Weather\\辽宁\\朝阳\\Weather.dat");

		//if (root != NULL)
		if (reader->parse(sJson, *root) && root->size() > 0) //用size判断是否字符串不是标准json而出错
		{
			int err = (*root)["status"].asInt();
			if (err != 1000)
			{
				goto ParseEnd;
			}

			bSucc = true;

			Json::Value Data = (*root)["data"];
			//Json::Value Today = Data["today"];

			m_sUIRegion = Data["city"].asCString();
			int n = 1;
			CString sCurTemp = Data["wendu"].asCString();
			sCurTemp = sCurTemp + _T("°");

			Json::Value FutureData = Data["forecast"];

			Future.clear();
			auto it = FutureData.begin();
			CString sWind = +_T("风力 ");
		    sWind += (*it)["fengli"].asCString();
			CString sWea = (*it)["type"].asCString();
			CString sLowTemp = (*it)["low"].asCString();
			CString sHighTemp = (*it)["high"].asCString();
			sLowTemp.Replace(_T("低温 "), _T(""));
			sHighTemp.Replace(_T("高温 "), _T(""));

			Now[_T("Temp")] = sCurTemp;
			sWind.Replace(_T("<![CDATA["), _T(""));
			sWind.Replace(_T("]]>"), _T(""));
			Now[_T("Wind")] = sWind;
			Now[_T("Wea")] = sWea;

			Now[_T("LowTemp")] = sLowTemp;
			Now[_T("HighTemp")] = sHighTemp;
			Now[_T("Img")] = GetImg(sWea);

			++it;
			for (; it != FutureData.end(); ++it)
			{
				CString sWind = (*it)["fengli"].asCString();
				CString sWea = (*it)["type"].asCString();
				CString sLowTemp = (*it)["low"].asCString();
				CString sHighTemp = (*it)["high"].asCString();
				sLowTemp.Replace(_T("低温 "), _T(""));
				sHighTemp.Replace(_T("高温 "), _T(""));

				CString sDateAll = (*it)["date"].asCString();

				TWeaDesc wea;
				sWind.Replace(_T("<![CDATA["), _T(""));
				sWind.Replace(_T("]]>"), _T(""));
				wea[_T("Wind")] = sWind;
				wea[_T("Wea")] = sWea;
				wea[_T("LowTemp")] = sLowTemp;
				wea[_T("HighTemp")] = sHighTemp;
				wea[_T("Img")] = GetImg(sWea);

				CString sDate = sDateAll.Left(sDateAll.Find(_T("日")) + 1);
				CString sWeek = sDateAll.Right(sDateAll.GetLength() - sDateAll.Find(_T("日")) - 1);

				wea[_T("Date")] = sDate;
				wea[_T("Week")] = sWeek;

				Future.push_back(wea);
			}
			/*CString sWind = Today["fengli"].asCString();
			CString sWea = Today["type"].asCString();
			CString sLowTemp = Today["lowtemp"].asCString();
			CString sHighTemp = Today["hightemp"].asCString();

			if (sCurTemp.Right(1) == _T("℃"))
				sCurTemp = sCurTemp.Left(sCurTemp.GetLength() - 1) + _T("°");
			Now[_T("Temp")] = sCurTemp;
			if (sWind.Right(1) == _T("级"))
				sWind = sWind.Left(sWind.GetLength() - 1);
			Now[_T("Wind")] = sWind;
			Now[_T("Wea")] = sWea;
			if (sLowTemp.Right(1) == _T("℃"))
				sLowTemp = sLowTemp.Left(sLowTemp.GetLength() - 1) + _T("°");
			Now[_T("LowTemp")] = sLowTemp;
			if (sHighTemp.Right(1) == _T("℃"))
				sHighTemp = sHighTemp.Left(sHighTemp.GetLength() - 1) + _T("°");
			Now[_T("HighTemp")] = sHighTemp;
			Now[_T("Img")] = GetImg(sWea);

			NowIndex.clear();
			Json::Value Index = Today["index"];
			auto it = Index.begin();
			for (; it != Index.end(); ++it)
			{
				string strCode = (*it)["code"].asString();
				CString sCode;
				if (strCode.compare("gm") == 0)
					sCode = _T("感冒指数");
				else if (strCode.compare("fs") == 0)
					sCode = _T("辐射指数");
				else if (strCode.compare("ct") == 0)
					sCode = _T("穿衣指数");
				else if (strCode.compare("yd") == 0)
					sCode = _T("运动指数");
				else if (strCode.compare("xc") == 0)
					sCode = _T("洗车指数");
				else if (strCode.compare("ls") == 0)
					sCode = _T("晾晒指数");

				CString sIndex = (*it)["index"].asCString();
				CString sDetail = (*it)["details"].asCString();

				TWeaDesc weaIndex;
				weaIndex[_T("Name")] = sCode;
				weaIndex[_T("Index")] = sIndex;
				weaIndex[_T("Detail")] = sDetail;

				NowIndex.push_back(weaIndex);
			}

			Json::Value FutureData = Data["forecast"];

			Future.clear();
			it = FutureData.begin();
			for (; it != FutureData.end(); ++it)
			{
				CString sWind = (*it)["fengli"].asCString();
				CString sWea = (*it)["type"].asCString();
				CString sLowTemp = (*it)["lowtemp"].asCString();
				CString sHighTemp = (*it)["hightemp"].asCString();
				CString sDate = (*it)["date"].asCString();
				CString sWeek = (*it)["week"].asCString();

				TWeaDesc wea;
				if (sWind.Right(1) == _T("级"))
					sWind = sWind.Left(sWind.GetLength() - 1);
				wea[_T("Wind")] = sWind;
				wea[_T("Wea")] = sWea;
				if (sLowTemp.Right(1) == _T("℃"))
					sLowTemp = sLowTemp.Left(sLowTemp.GetLength() - 1) + _T("°");
				wea[_T("LowTemp")] = sLowTemp;
				if (sHighTemp.Right(1) == _T("℃"))
					sHighTemp = sHighTemp.Left(sHighTemp.GetLength() - 1) + _T("°");
				wea[_T("HighTemp")] = sHighTemp;
				wea[_T("Img")] = GetImg(sWea);
				wea[_T("Date")] = sDate;
				wea[_T("Week")] = sWeek;

				Future.push_back(wea);
			}*/
		}

	ParseEnd:
		delete root;
		delete reader;

		return bSucc;
	}
};
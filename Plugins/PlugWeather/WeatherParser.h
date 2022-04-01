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

		/*if (strWea == "��" ) sResult ="00";
		else if (strWea == "����" || strWea == "������") sResult = "01";
		else if (strWea == "����" || strWea == "���Ƽ���") sResult = "00";
		else if (strWea == "����") sResult = "02";
		else if (strWea == "��") sResult = "00";
		else if (strWea == "��" || strWea.find("��") != string::npos) sResult = "02";
		else if (strWea == "С��") sResult = "07";
		else if (strWea == "��" || strWea == "����" || strWea.find("��") != string::npos) sResult = "08";
		else if (strWea == "����") sResult = "09";
		else if (strWea == "С����") sResult = "03";
		else if (strWea == "����" || strWea == "����") sResult = "03";
		else if (strWea == "������") sResult = "03";
		else if (strWea == "С����") sResult = "04";
		else if (strWea == "����") sResult = "04";
		else if (strWea == "�ױ�(������)" || strWea == "�ױ�" || strWea == "������" || strWea.find("��") != string::npos) sResult = "04";
		else if (strWea == "С����") sResult = "07";
		else if (strWea == "�����ѩ(�б���)" || strWea == "�����ѩ" || strWea == "�б���") sResult = "08";
		else if (strWea == "�����") sResult = "08";
		else if (strWea == "���ѩ") sResult = "06";
		else if (strWea == "Сѩ" || strWea.find("Сѩ") != string::npos) sResult = "07";
		else if (strWea == "ѩ��") sResult = "08";
		else if (strWea == "��ѩ") sResult = "09";
		else if (strWea == "��ѩ") sResult = "10";
		else if (strWea.find("ѩ") != string::npos) sResult = "09";
		else if (strWea == "����") sResult = "07";
		else if (strWea == "��") sResult = "18";
		else if (strWea == "��" || strWea.find("��") != string::npos) sResult = "18";
		else if (strWea == "��" || strWea.find("��") != string::npos) sResult = "18";
		else if (strWea == "����") sResult = "18";
		else if (strWea == "��") sResult = "18";
		else if (strWea == "��") sResult = "18";
		else if (strWea == "����") sResult = "18";*/

		////ҹ��
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
		L = luaL_newstate();        //����lua���л���
		luaopen_base(L);     // ����Lua������
		luaopen_string(L);
		luaL_openlibs(L);     // ����Luaͨ����չ��
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
		Json::Value *root = new Json::Value; //reader->ReadFromFile("D:\\[Work]\\desktop\\BeyondDesk\\bin\\Debug\\Data\\Weather\\����\\����\\Weather.dat");

		//if (root != NULL)
		if (reader->parse(sJson, *root) && root->size() > 0) //��size�ж��Ƿ��ַ������Ǳ�׼json������
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
			sCurTemp = sCurTemp + _T("��");

			Json::Value FutureData = Data["forecast"];

			Future.clear();
			auto it = FutureData.begin();
			CString sWind = +_T("���� ");
		    sWind += (*it)["fengli"].asCString();
			CString sWea = (*it)["type"].asCString();
			CString sLowTemp = (*it)["low"].asCString();
			CString sHighTemp = (*it)["high"].asCString();
			sLowTemp.Replace(_T("���� "), _T(""));
			sHighTemp.Replace(_T("���� "), _T(""));

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
				sLowTemp.Replace(_T("���� "), _T(""));
				sHighTemp.Replace(_T("���� "), _T(""));

				CString sDateAll = (*it)["date"].asCString();

				TWeaDesc wea;
				sWind.Replace(_T("<![CDATA["), _T(""));
				sWind.Replace(_T("]]>"), _T(""));
				wea[_T("Wind")] = sWind;
				wea[_T("Wea")] = sWea;
				wea[_T("LowTemp")] = sLowTemp;
				wea[_T("HighTemp")] = sHighTemp;
				wea[_T("Img")] = GetImg(sWea);

				CString sDate = sDateAll.Left(sDateAll.Find(_T("��")) + 1);
				CString sWeek = sDateAll.Right(sDateAll.GetLength() - sDateAll.Find(_T("��")) - 1);

				wea[_T("Date")] = sDate;
				wea[_T("Week")] = sWeek;

				Future.push_back(wea);
			}
			/*CString sWind = Today["fengli"].asCString();
			CString sWea = Today["type"].asCString();
			CString sLowTemp = Today["lowtemp"].asCString();
			CString sHighTemp = Today["hightemp"].asCString();

			if (sCurTemp.Right(1) == _T("��"))
				sCurTemp = sCurTemp.Left(sCurTemp.GetLength() - 1) + _T("��");
			Now[_T("Temp")] = sCurTemp;
			if (sWind.Right(1) == _T("��"))
				sWind = sWind.Left(sWind.GetLength() - 1);
			Now[_T("Wind")] = sWind;
			Now[_T("Wea")] = sWea;
			if (sLowTemp.Right(1) == _T("��"))
				sLowTemp = sLowTemp.Left(sLowTemp.GetLength() - 1) + _T("��");
			Now[_T("LowTemp")] = sLowTemp;
			if (sHighTemp.Right(1) == _T("��"))
				sHighTemp = sHighTemp.Left(sHighTemp.GetLength() - 1) + _T("��");
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
					sCode = _T("��ðָ��");
				else if (strCode.compare("fs") == 0)
					sCode = _T("����ָ��");
				else if (strCode.compare("ct") == 0)
					sCode = _T("����ָ��");
				else if (strCode.compare("yd") == 0)
					sCode = _T("�˶�ָ��");
				else if (strCode.compare("xc") == 0)
					sCode = _T("ϴ��ָ��");
				else if (strCode.compare("ls") == 0)
					sCode = _T("��ɹָ��");

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
				if (sWind.Right(1) == _T("��"))
					sWind = sWind.Left(sWind.GetLength() - 1);
				wea[_T("Wind")] = sWind;
				wea[_T("Wea")] = sWea;
				if (sLowTemp.Right(1) == _T("��"))
					sLowTemp = sLowTemp.Left(sLowTemp.GetLength() - 1) + _T("��");
				wea[_T("LowTemp")] = sLowTemp;
				if (sHighTemp.Right(1) == _T("��"))
					sHighTemp = sHighTemp.Left(sHighTemp.GetLength() - 1) + _T("��");
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
#include "stringutils.h"
#include <sstream>
#include <algorithm>
#include <sstream>
#include "..\..\..\xbox\input\sysxboxinput.h"

typedef struct
{
	std::string strCtrlName;
	int iCtrlEnum;
} sControlNameMap;

// Used to convert between strings and enums
sControlNameMap ControlNameMap[] =
{
	{"xbox_a", K_XBOX_A},
	{"xbox_b", K_XBOX_B},
	{"xbox_x", K_XBOX_X},
	{"xbox_Y", K_XBOX_Y},
	{"xbox_back", K_XBOX_BACK},
	{"xbox_start", K_XBOX_START},
	{"xbox_black", K_XBOX_BLACK},
	{"xbox_white", K_XBOX_WHITE},
	{"xbox_dpad_up", K_XBOX_DPAD_UP},
	{"xbox_dpad_down", K_XBOX_DPAD_DOWN},
	{"xbox_dpad_left", K_XBOX_DPAD_LEFT},
	{"xbox_dpad_right", K_XBOX_DPAD_RIGHT},
	{"xbox_l_trig", K_XBOX_LTRIG},
	{"xbox_r_trig", K_XBOX_RTRIG},
	{"xbox_l_thumb", K_XBOX_LTHUMB},
	{"xbox_r_thumb", K_XBOX_RTHUMB},
};

void CStringUtils::AddSlashAtEnd(std::string& strFolder)
{
	if (!HasSlashAtEnd(strFolder))
      strFolder += '\\';
}

bool CStringUtils::HasSlashAtEnd(const std::string& strFile)
{
	if (strFile.size() == 0) return false;
	char kar = strFile.c_str()[strFile.size() - 1];

	if (kar == '/' || kar == '\\')
		return true;

	return false;
}

// Converts string to wide
void CStringUtils::StringtoWString(std::string strText, LPCWSTR &strResult)
{
	wchar_t* wtext = new wchar_t[strText.size()+1];
	mbstowcs(wtext, strText.c_str(), strlen(strText.c_str())+1);
	strResult = wtext;

	delete wtext;
}

int CStringUtils::SplitString(const std::string& strInput, CStringArray &results)
{
	std::istringstream ss(strInput);
	std::string token;

	while(std::getline(ss, token, '.'))
		results.push_back(token);
	
	return results.size();
}

void CStringUtils::ToLower(std::string &strText)
{
	std::transform(strText.begin(), strText.end(), strText.begin(), ::tolower);
}

std::string CStringUtils::IntToString(int iValue)
{
	std::stringstream ssTemp;
	ssTemp << iValue;
	std::string strTemp = ssTemp.str();
	return strTemp;
}

std::string CStringUtils::ControlEnumToString(int iControl)
{
	for(int i = 0; i < sizeof(ControlNameMap)/sizeof(ControlNameMap[0]); i++)
	{
		if(iControl == ControlNameMap[i].iCtrlEnum)
			return ControlNameMap[i].strCtrlName;
	}
	return "NA";
}

int CStringUtils::ControlStringToEnum(std::string strControl)
{
	for(int i = 0; i < sizeof(ControlNameMap)/sizeof(ControlNameMap[0]); i++)
	{
		if(strControl == ControlNameMap[i].strCtrlName)
			return ControlNameMap[i].iCtrlEnum;
	}
	return 0;
}
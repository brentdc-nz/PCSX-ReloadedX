#include "stringutils.h"
#include <sstream>
#include <algorithm>
#include <sstream>

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
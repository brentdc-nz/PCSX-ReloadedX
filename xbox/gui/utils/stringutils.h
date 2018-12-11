#ifndef H_CSTRINGUTILS
#define H_CSTRINGUTILS

#include <string>
#include <xtl.h>
#include <vector>

typedef std::vector<std::string> CStringArray;

class CStringUtils
{
public:
	static void AddSlashAtEnd(std::string& strFolder);
	static bool HasSlashAtEnd(const std::string& strFile);
	static void StringtoWString(std::string strText, LPCWSTR &strResult);
	static int SplitString(const std::string& strInput, CStringArray &results);
	static void ToLower(std::string &strText);
	static std::string IntToString(int iValue);
	static std::string ControlEnumToString(int iControl);
	static int ControlStringToEnum(std::string strControl);
};

#endif //H_CSTRINGUTILS
#ifndef H_CGUIXBOXCONFIGS
#define H_CGUIXBOXCONFIGS

#ifdef __cplusplus

#include <xtl.h>
#include <string>
#include <vector>
#include <map>

#include "..\tinyxml\tinyxml.h"
#include "utils\stringutils.h"

#define CDIMAGE_PATH "D:\\psxcds\\" // TODO: Store in CFG_STRING

enum CfgType
{ 
	GLOBAL = 0,
	GAME
};

enum CfgDataType
{ 
	CFG_BOOL = 0,
	CFG_INT,
	CFG_STRING
};

enum CfgCategory
{ 
	CFG_VIDEO = 0,
	CFG_SOUND,
	CFG_CORE,
	CFG_CONTROLS
};

#define MULTISAMPLE_NONE 0
#define MULTISAMPLE_2    2
#define MULTISAMPLE_4    4

#define VSYNC_OFF        0
#define VSYNC_ON         1
#define VSYNC_ADAPTIVE   2

class CConfig
{
public:
	CConfig(std::string strConfig, std::string strDisplayText, int iConfigType, int iDataType){ m_strSetting = strConfig; m_strDisplayText = strDisplayText; m_iConfigType = iConfigType; m_iDataType = iDataType; m_iGUIControlID = 0; };
	~CConfig(){};

	virtual std::string ToString() = 0;
	virtual void FromString(const std::string strText) = 0;

	int GetDataType() { return m_iDataType; };
	int GetConfigType() { return m_iConfigType; };
	std::string GetDisplayText(){ return m_strDisplayText; };
	std::string GetConfigName() { return m_strSetting; };
	void SetGUIControlID(int iCtrlID) { m_iGUIControlID = iCtrlID; };
	int GetGUIControlID() { return m_iGUIControlID; };

private:
	int m_iDataType;
	int m_iConfigType;
	int m_iGUIControlID;
	std::string m_strDisplayText;
	std::string m_strSetting;
};

class CConfigBool : public CConfig
{
public:
	CConfigBool(std::string strConfig, std::string strDisplayText, int iConfigType, bool bValue) : CConfig(strConfig, strDisplayText, iConfigType, CFG_BOOL) { m_bValue = bValue; };
	~CConfigBool(){};

	virtual bool GetValue(){ return m_bValue; };
	virtual void SetValue(bool bValue) { m_bValue = bValue; };
	virtual std::string ToString() { return m_bValue ? "true" : "false"; };
	virtual void FromString(const std::string strText) { if(strText == "true" || strText == "yes" ) m_bValue = true; else m_bValue = false; };

private:
	bool m_bValue;
};

class CConfigInt : public CConfig
{
public:
	CConfigInt(std::string strConfig, std::string strDisplayText, int iConfigType, int iValue) : CConfig(strConfig, strDisplayText, iConfigType, CFG_INT) { m_iValue = iValue; };
	~CConfigInt(){ m_OptionsMap.clear(); };

	virtual int GetValue(){ return m_iValue; };
	virtual void SetValue(int iValue) { m_iValue = iValue; };
	virtual std::string ToString() { return CStringUtils::IntToString(m_iValue); };
	virtual void FromString(const std::string strText) { m_iValue = atoi(strText.c_str()); };
	
	void AddOption(std::string strDisplayText, int iValue) { m_OptionsMap.insert(std::pair<std::string, int>(strDisplayText, iValue)); }
	const std::map<std::string, int> &GetOptions() const { return m_OptionsMap; }

	typedef std::map<std::string, int>::iterator mapIter;
	typedef std::map<std::string, int> OptionsMap;

private:
	int m_iValue;
	OptionsMap m_OptionsMap;
};

class CConfigString : public CConfig
{
public:
	CConfigString(std::string strConfig, std::string strDisplayText, int iConfigType, std::string strValue) : CConfig(strConfig, strDisplayText, iConfigType, CFG_STRING) { m_strValue = strValue; };
	~CConfigString(){ m_OptionsMap.clear(); };

	virtual std::string GetValue(){ return m_strValue; };
	virtual void SetValue(std::string strValue) { m_strValue = strValue; };
	virtual std::string ToString() { return m_strValue; };
	virtual void FromString(const std::string strText) { m_strValue = strText; };
	
	void AddOption(std::string strDisplayText, std::string strValue) { m_OptionsMap.insert(std::pair<std::string, std::string>(strDisplayText, strValue)); }
	const std::map<std::string, std::string> &GetOptions() const { return m_OptionsMap; }

	typedef std::map<std::string, std::string>::iterator mapIter;
	typedef std::map<std::string, std::string> OptionsMap;

private:
	std::string m_strValue;
	OptionsMap m_OptionsMap;
};

class CXboxConfigs
{
public:
	CXboxConfigs();
	~CXboxConfigs();

	bool Load();
	bool Save();
	void Clear();

	// CFG_BOOL
	void AddBool(std::string strConfig, std::string strDisplayText, bool bValue, int iCfgType);
	void SetBool(const char *strConfig, const bool bValue);
	bool GetBool(const char* strConfig);

	// CFG_INT
	void AddInt(std::string strConfig, std::string strDisplayText, int iValue, int iCfgType);
	void AddIntOption(std::string strConfig, std::string strDisplayText, int iValue);
	void SetInt(const char *strConfig, const int iValue);
	int GetInt(const char* strConfig);

	// CFG_STRING
	void AddString(std::string strConfig, std::string strDisplayText, std::string strValue, int iCfgType);
	void AddStringOption(std::string strConfig, std::string strDisplayText, std::string strValue);
	void SetString(const char *strConfig, const std::string strValue);
	std::string GetString(const char* strConfig);
	const char* GetStringC(const char* strConfig); // For C code

	bool SaveGlobalConfigs();
	bool SaveGameConfigs(std::string strGameFilename);
	bool LoadGameConfigs(std::string strGameFilename);
	const std::map<std::string, CConfig*> &GetConfigs() const { return m_ConfigsMap; }
	const std::map<std::string, CConfig*> GetConfigs(int iCategory) const;
	CConfig* GetConfig(const char* strConfig);

	typedef std::map<std::string, CConfig*>::iterator mapIter;
	typedef std::map<std::string, CConfig*> CfgMap;

private:
	void PopulateSkins();
	void PopulateBiosFiles();
	void AddControlOptions(std::string strConfig);
	bool LoadGlobalConfigs();

	CfgMap m_ConfigsMap;
};

extern CXboxConfigs g_XboxConfigs;

#endif

#ifdef __cplusplus
extern "C"
{
#endif

BOOL XboxConfigs_Load();
void XboxConfigs_Clear();
BOOL XboxConfigs_GetBool(const char *strConfig);
int XboxConfigs_GetInt(const char *strConfig);
const char* XboxConfigs_GetString(const char *strConfig);

#ifdef __cplusplus
}
#endif

#endif //H_CGUIXBOXCONFIGS
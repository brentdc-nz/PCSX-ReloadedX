#ifndef H_CGUICONTROLFACTORY
#define H_CGUICONTROLFACTORY

#include <xtl.h>
#include "guicontrol.h"
#include "..\tinyxml\tinyxml.h"

class CGUIControlFactory
{
public:
	CGUIControlFactory();
	~CGUIControlFactory();

	CGUIControl* Create(int iParentID, TiXmlElement* pControlNode);

private:
	bool GetString(const TiXmlNode* pRootNode, const char* strTag, std::string& strStringValue);
	bool GetInt(const TiXmlNode* pRootNode, const char* strTag, int& iIntValue);
	bool GetHex(const TiXmlNode* pRootNode, const char* strTag, DWORD& dwHexValue);
	bool GetBoolean(const TiXmlNode* pRootNode, const char* strTag, bool& bBoolValue);
};

#endif //H_CGUICONTROLFACTORY
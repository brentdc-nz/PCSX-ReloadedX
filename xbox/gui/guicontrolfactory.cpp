#include "guicontrolfactory.h"
#include "utils\stringutils.h"
#include "guiwindowkeys.h"

#include "controls\guicontrolimage.h"
#include "controls\guicontrollabel.h"
#include "controls\guicontrolbutton.h"
#include "controls\guicontrollist.h"
#include "controls\guicontrolcheckbox.h"
#include "controls\guicontrolspin.h"

using namespace std;

CGUIControlFactory::CGUIControlFactory()
{
}

CGUIControlFactory::~CGUIControlFactory()
{
}

CGUIControl* CGUIControlFactory::Create(int iParentID, TiXmlElement* pControlNode)
{
	// Get the control type
	string strType;
	GetString(pControlNode, "type", strType);

	CGUIControl* pControl = NULL;

	bool bVisible = false;

	int iPosX = 0, iPosY = 0;
	int iWidth = 0, iHeight = 0;

	int iID = UNKNOWN_ID;

	GetInt(pControlNode, "id", iID);

	GetInt(pControlNode, "posx", iPosX);
	GetInt(pControlNode, "posy", iPosY);
	
	GetInt(pControlNode, "width", iWidth);
	GetInt(pControlNode, "height", iHeight);

	int iLeft = 0, iRight = 0, iUp = 0, iDown = 0;

	if (!GetInt(pControlNode, "onup" , iUp ))
		iUp = iID - 1;

	if (!GetInt(pControlNode, "ondown" , iDown))
		iDown = iID + 1;

	if (!GetInt(pControlNode, "onleft" , iLeft ))
		iLeft = iID;

	if (!GetInt(pControlNode, "onright", iRight))
		iRight = iID;

	GetBoolean(pControlNode, "visible", bVisible);

	// Instantiate a new control using the properties gathered above

	if(strType == "image")
	{
			string strTexture;
			GetString(pControlNode, "texture", strTexture);
			pControl = new CGUIControlImage(iID, iParentID, iPosX, iPosY, iWidth, iHeight, strTexture);
	}
	else if(strType == "label")
	{
		int iSize = 15;
		string strFont, strText;
		DWORD dwColor = 0;

		GetInt(pControlNode, "size", iSize);
		GetString(pControlNode, "font", strFont);
		GetString(pControlNode, "label", strText);
		GetHex(pControlNode, "color", dwColor);

		pControl = new CGUIControlLabel(iID, iParentID, iPosX, iPosY, iWidth, iHeight, strFont, strText, dwColor, iSize);
	}
	else if(strType == "button")
	{
		int iSize = 15, iTextOffsetX = 0, iTextOffsetY = 0, iLinkedWindow = WINDOW_INVALID;
		string strFont, strText, strFocus, strNoFocus;
		DWORD dwColor = 0, dwSelectedColor = 0;

		GetString(pControlNode, "texturefocus", strFocus);
		GetString(pControlNode, "texturenofocus", strNoFocus);
		GetString(pControlNode, "label", strText);
		GetInt(pControlNode, "size", iSize);
		GetString(pControlNode, "font", strFont);
		GetHex(pControlNode, "color", dwColor);
		GetHex(pControlNode, "selectedcolor", dwSelectedColor);
		GetString(pControlNode, "label", strText);
		GetInt(pControlNode, "textoffsetx", iTextOffsetX);
		GetInt(pControlNode, "textoffsety", iTextOffsetY);
		GetInt(pControlNode, "linkedwindowid", iLinkedWindow);


		pControl = new CGUIControlButton(iID, iParentID, iPosX, iPosY, iWidth, iHeight, strFont, strText, dwColor, dwSelectedColor, iSize, strFocus, strNoFocus
			                             ,iTextOffsetX, iTextOffsetY, iLinkedWindow);
	}
	else if(strType == "list")
	{
		int iSize = 15;
		int iLineSpacing = 30;
		int iItemsPerPage = 10; // TODO: Calculate from item size and control height
		string strFont;
		DWORD dwColor = 0, dwSelectedColor = 0;

		GetHex(pControlNode, "color", dwColor);
		GetHex(pControlNode, "selectedcolor", dwSelectedColor);
		GetString(pControlNode, "font", strFont);
		GetInt(pControlNode, "size", iSize);
		GetInt(pControlNode, "linespacing", iLineSpacing);
		GetInt(pControlNode, "itemsperpage", iItemsPerPage);

		pControl = new CGUIControlList(iID, iParentID, iPosX, iPosY, iWidth, iHeight, iLineSpacing, iItemsPerPage, strFont, dwColor, iSize, dwSelectedColor);
	}
	else if(strType == "checkbox")
	{
		int iSize = 15, iImgOffsetX = 0;
		int iImgWidth = 0, iImgHeight = 0;
		string strFont = "", strText = "", strImageFocus = "-";
		string strImage = "", strImageSelected = "";
		DWORD dwColor = 0;

		GetString(pControlNode, "label", strText);
		GetString(pControlNode, "texturefocus", strImageFocus);

		GetString(pControlNode, "texture", strImage);
		GetString(pControlNode, "textureselected", strImageSelected);

		GetInt(pControlNode, "size", iSize);
		GetString(pControlNode, "font", strFont);
		GetHex(pControlNode, "color", dwColor);	
		GetInt(pControlNode, "imagewidth", iImgWidth);
		GetInt(pControlNode, "imageheight", iImgHeight);
		GetInt(pControlNode, "imageoffsetx", iImgOffsetX);

		pControl = new CGUIControlCheckbox(iID, iParentID, iPosX, iPosY, iWidth, iHeight, strFont, strText, dwColor, iSize, strImageFocus 
			                               ,iImgOffsetX, strImage, strImageSelected, iImgWidth, iImgHeight);
	}
	else if(strType == "spin")
	{
		int iSpinWidth = 0, iSpinHeight = 0;
		int iSize = 15;

		string strFont = "", strText = "", strImageFocus = "-";
		string strSpinUp = "", strSpinUpFocus = "";
		string strSpinDown = "", strSpinDownFocus = "";
		DWORD dwColor = 0;

		GetString(pControlNode, "label", strText);
		GetInt(pControlNode, "size", iSize);
		GetString(pControlNode, "font", strFont);
		GetHex(pControlNode, "color", dwColor);	

		GetInt(pControlNode, "spinwidth", iSpinWidth);
		GetInt(pControlNode, "spinheight", iSpinHeight);		

		GetString(pControlNode, "texturefocus", strImageFocus);

		GetString(pControlNode, "textureup", strSpinUp);
		GetString(pControlNode, "textureupfocus", strSpinUpFocus);
		GetString(pControlNode, "texturedown", strSpinDown);
		GetString(pControlNode, "texturedownfocus", strSpinDownFocus);

		pControl = new CGUIControlSpin(iID, iParentID, iPosX, iPosY, iWidth, iHeight, strFont, iSize, strText, dwColor, strImageFocus, strSpinUp, strSpinUpFocus
			                           ,strSpinDown, strSpinDownFocus, iSpinWidth, iSpinHeight);
	}

	if(pControl)
	{
		pControl->SetNavigation(iUp, iDown, iLeft, iRight);
		pControl->SetVisible(bVisible);
	}

	return pControl;
}

bool CGUIControlFactory::GetString(const TiXmlNode* pRootNode, const char* strTag, string& strStringValue)
{
	const TiXmlElement* pElement = pRootNode->FirstChildElement(strTag);

	if (!pElement) return false;
	
	const TiXmlNode* pNode = pElement->FirstChild();
	
	if (pNode != NULL)
	{
		strStringValue = pNode->Value();
		return true;
	}

	strStringValue.empty();
	return false;
}

bool CGUIControlFactory::GetInt(const TiXmlNode* pRootNode, const char* strTag, int& iIntValue)
{
	const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
	if (!pNode || !pNode->FirstChild()) return false;
		iIntValue = atoi(pNode->FirstChild()->Value());
  
	return true;
}

bool CGUIControlFactory::GetHex(const TiXmlNode* pRootNode, const char* strTag, DWORD& dwHexValue)
{
	const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
	if (!pNode || !pNode->FirstChild()) return false;
	sscanf(pNode->FirstChild()->Value(), "%x", &dwHexValue );

	return true;
}

bool CGUIControlFactory::GetBoolean(const TiXmlNode* pRootNode, const char* strTag, bool& bBoolValue)
{
	const TiXmlNode* pNode = pRootNode->FirstChild(strTag );
	if (!pNode || !pNode->FirstChild()) return false;
	string strEnabled = pNode->FirstChild()->Value();
	CStringUtils::ToLower(strEnabled);

	if (strEnabled == "off" || strEnabled == "no" || strEnabled == "disabled" || strEnabled == "false" || strEnabled == "0" )
		bBoolValue = false;
	else
	{
		bBoolValue = true;
		if (strEnabled != "on" && strEnabled != "yes" && strEnabled != "enabled" && strEnabled != "true")
			return false; // Invalid bool switch, it's probably some other string
	}

	return true;
}
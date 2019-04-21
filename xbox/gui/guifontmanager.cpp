#include "guifontmanager.h"
#include "xboxgui.h"
#include "..\..\libpcsxcore\system.h"

using namespace std;

CGUIFontManager::CGUIFontManager()
{
}

CGUIFontManager::~CGUIFontManager()
{
}

void CGUIFontManager::Initialize()
{
	string strFilename = g_XboxGUI.GetMediaDir();
	strFilename += "fonts.xml";

	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile(strFilename.c_str()))
	{
		SysMessage("Could'nt load fonts XML: %s\n", strFilename.c_str()); 
		return;
	}

	string strFontFileName, strFontName;
	DWORD dwStyle = XFONT_NORMAL;

	TiXmlElement* pRootElement =xmlDoc.RootElement();

	string strValue=pRootElement->Value();
	if (strValue!=string("fonts")) return;

	const TiXmlNode *pChild = pRootElement->FirstChild();
	
	while(pChild)
	{
		string strValue=pChild->Value();
		if(strValue=="font")
		{  
			const TiXmlNode *pNode = pChild->FirstChild("name");
			if (pNode)
			{
				strFontName=pNode->FirstChild()->Value();

				const TiXmlNode *pNode = pChild->FirstChild("filename");	
				if (pNode)
					strFontFileName=pNode->FirstChild()->Value();
/*
				const TiXmlNode *pNode2 = pChild->FirstChild("size"); //TODO
				if (pNode2)
				{
					iSize = atol(pNode2->FirstChild()->Value());
					if (iSize <= 0) iSize = 20;
				}
*/
				const TiXmlNode *pNode3 = pChild->FirstChild("style");
				if(pNode3)
				{
					string style = pNode3->FirstChild()->Value();
					if (style == "normal")
					dwStyle = XFONT_NORMAL;
					else if (style == "bold")
					dwStyle = XFONT_BOLD;
					else if (style == "italics")
					dwStyle = XFONT_ITALICS;
					else if (style == "bolditalics")
					dwStyle = XFONT_BOLDITALICS;
				}

				Load(strFontName, strFontFileName, dwStyle);
			}
		}
		pChild=pChild->NextSibling();  
	}
}

void CGUIFontManager::Load(const string& strFontName, const string& strFilename, DWORD dwStyles)
{
	// Check if font already exists
	CGUIFont* pFont = GetFont(strFontName);
	if (pFont) return;
 
	CGUIFont* pNewFont = new CGUIFont(strFontName, strFilename, dwStyles);
	if (pNewFont->Load())
	{
		// Font is loaded
		m_vecFonts.push_back(pNewFont);
		return;
	}

	delete pNewFont;
}

void CGUIFontManager::Cleanup()
{
	for(int i = 0; i < (int)m_vecFonts.size(); i++)
	{
		CGUIFont* pFont = NULL;
		pFont = m_vecFonts[i];

		pFont->FreeResources();

		delete pFont;
	}
	
	m_vecFonts.clear();
}

CGUIFont* CGUIFontManager::GetFont(std::string strFontName)
{
	for(int i = 0; i < (int)m_vecFonts.size(); i++)
	{
		CGUIFont* pFont = NULL;
		pFont = m_vecFonts[i];
		
		if(pFont->GetFontName() == strFontName)
			return pFont;
	}

	return NULL;
}
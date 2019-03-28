#include "guiconfigs.h"
#include "..\..\libpcsxcore\system.h"
#include "utils\stringutils.h"
#include "..\input\sysxboxinput.h"
#include "utils\fileutils.h"

#define GLOBAL_CONFIGS_PATH "D:\\configs\\global.xml"
#define GAME_CONFIGS_PATH   "D:\\configs\\games.xml"
#define BIOS_FILE_PATH      "D:\\bios\\"
#define SKINS_PATH          "D:\\skins\\"

CXboxConfigs g_XboxConfigs;

CXboxConfigs::CXboxConfigs()
{
}

CXboxConfigs::~CXboxConfigs()
{
}

bool CXboxConfigs::Load()
{
	// Add all our configs and initialize with default values
	// before we look for XML files to load values from

	/* Format: *type*, *Display Text*, *Default Value*, *Config Type* */

	//
	// Global configs
	//

	AddBool("core.showbiosscreen", "Show Bios Screen", true, GLOBAL_CFG);
	AddBool("video.showfps", "Show FPS Counter", false, GLOBAL_CFG);
	AddBool("video.keepaspectratio", "Keep Aspect Ratio", false, GLOBAL_CFG);

	AddInt("video.vsync", "VSync", VSYNC_ON, GLOBAL_CFG);
	AddIntOption("video.vsync", "Off", VSYNC_OFF);	
	AddIntOption("video.vsync", "On", VSYNC_ON);	
	AddIntOption("video.vsync", "Adaptive", VSYNC_ADAPTIVE);

	AddInt("video.resolution", "Resolution", RESOLUTION_480P, GLOBAL_CFG);
	AddIntOption("video.resolution", "480p", RESOLUTION_480P);
	AddIntOption("video.resolution", "720p", RESOLUTION_720P);
//	AddIntOption("video.resolution", "1080i", RESOLUTION_1080I); // Disabled as prob too slow and ram hungry

	AddBool("sound.stereo", "Stereo Sound", false, GLOBAL_CFG);

	AddString("gui.skin", "Skin", "pm3", GLOBAL_CFG);
	PopulateSkins(); // Look for skins and load them as options

	AddString("core.biosfile", "Bios File", "scph1001.bin", GLOBAL_CFG);
	PopulateBiosFiles();

	//
	// Individual game configs
	//

	// Core Configs

	AddBool("core.usehlebios", "Use HLE Bios", false, GAME_CFG);

	// Video Configs

	AddInt("video.antialiasing", "Anti-aliasing", 2, GAME_CFG);
	AddIntOption("video.antialiasing", "None", MULTISAMPLE_NONE);
	AddIntOption("video.antialiasing", "2x", MULTISAMPLE_2);
	AddIntOption("video.antialiasing", "4x", MULTISAMPLE_4);

	AddBool("video.frameskip", "Use Frameskip", false, GAME_CFG);

	AddInt("video.offscreendrawing", "Off-Sceen drawing", 2, GAME_CFG);
	AddIntOption("video.offscreendrawing", "Fastest(Most glicthes)", 0);
	AddIntOption("video.offscreendrawing", "Minimum(Missing screens)", 1);
	AddIntOption("video.offscreendrawing", "Standard(Ok for most games)", 2);
	AddIntOption("video.offscreendrawing", "Enhanced(Shows more stuff)", 3);
	AddIntOption("video.offscreendrawing", "Extended(Can cause garbage)", 4);

	AddInt("video.texturefiltering", "Texture Filtering", 2, GAME_CFG);
	AddIntOption("video.texturefiltering", "None", 0);
	AddIntOption("video.texturefiltering", "Standard(Glicthes can happen)", 1);
	AddIntOption("video.texturefiltering", "Extended(Removes back borders)", 2);
	AddIntOption("video.texturefiltering", "Standard(w/o Sprtes - Unfiltered 2D)", 3);
	AddIntOption("video.texturefiltering", "Extended(w/o Sprtes - Unfiltered 2D)", 4);
	AddIntOption("video.texturefiltering", "Standard + filtered sprites", 5);
	AddIntOption("video.texturefiltering", "Extended + filtered sprites", 6);

	AddInt("video.vramsize", "VRam Size", 0, GAME_CFG);
	AddIntOption("video.vramsize", "Autodetect", 0);
	AddIntOption("video.vramsize", "2mb", 2);
	AddIntOption("video.vramsize", "4mb", 4);
	AddIntOption("video.vramsize", "8mb", 8);
	AddIntOption("video.vramsize", "16mb", 16);
	AddIntOption("video.vramsize", "32mb", 32);

	AddBool("video.useframelimit", "Use frame limit", true, GAME_CFG);

	AddInt("video.hirestextures", "Hi-Res Textures", 0, GAME_CFG);
	AddIntOption("video.hirestextures", "None (standard)", 0);
	AddIntOption("video.hirestextures", "2xSal (alot of VRam required)", 1);
	AddIntOption("video.hirestextures", "Streteched (filtering required)", 2);

	AddBool("video.usefastmdec", "Use fast Mdec", true, GAME_CFG);

	AddInt("video.framebufferupload", "Framebuffer Upload", 0, GAME_CFG);
	AddIntOption("video.framebufferupload", "Minimum(fast detection)", 0);
	AddIntOption("video.framebufferupload", "Standard", 1);
	AddIntOption("video.framebufferupload", "Maximum(full VRam)", 2);

	// Sound Configs

	AddInt("sound.mode", "Mode", 1, GAME_CFG);
	AddIntOption("sound.mode", "Fast threaded mode", 0);
	AddIntOption("sound.mode", "High compatibility timer mode", 1);
	AddIntOption("sound.mode", "SPUAsync mode", 2);

	AddInt("sound.interpolation", "Interpolation", 2, GAME_CFG);
	AddIntOption("sound.interpolation", "None", 0);
	AddIntOption("sound.interpolation", "Simple", 1);
	AddIntOption("sound.interpolation", "Gaussian - Good quality", 2);
	AddIntOption("sound.interpolation", "Cubic- Better treble", 3);

	AddInt("sound.usereverb", "Reverb", 2, GAME_CFG);
	AddIntOption("sound.usereverb", "None", 0);
	AddIntOption("sound.usereverb", "Simple - Fakes common effects", 1);
	AddIntOption("sound.usereverb", "PSX Reverb", 2);

	AddBool("sound.spuirqwait", "SPUIRQ Wait", true, GAME_CFG);
	AddBool("sound.xapitch", "XA Pitch", false, GAME_CFG);
	AddBool("sound.freqresponse", "Freq Response", false, GAME_CFG);

	// Controls Configs

	AddInt("controls.cross", "Cross", K_XBOX_A, GAME_CFG);
	AddControlOptions("controls.cross");

	AddInt("controls.circle", "Circle", K_XBOX_B, GAME_CFG);
	AddControlOptions("controls.circle");

	AddInt("controls.triangle", "Triangle", K_XBOX_Y, GAME_CFG);
	AddControlOptions("controls.triangle");

	AddInt("controls.square", "Square", K_XBOX_X, GAME_CFG);
	AddControlOptions("controls.square");

	AddInt("controls.start", "Start", K_XBOX_START, GAME_CFG);
	AddControlOptions("controls.start");

	AddInt("controls.select", "Select", K_XBOX_BACK, GAME_CFG);
	AddControlOptions("controls.select");

	AddInt("controls.dpadup", "DPad Up", K_XBOX_DPAD_UP, GAME_CFG);
	AddControlOptions("controls.dpadup");

	AddInt("controls.dpaddown", "DPad Down", K_XBOX_DPAD_DOWN, GAME_CFG);
	AddControlOptions("controls.dpaddown");

	AddInt("controls.dpadleft", "DPad Left", K_XBOX_DPAD_LEFT, GAME_CFG);
	AddControlOptions("controls.dpadleft");

	AddInt("controls.dpadright", "DPad Right", K_XBOX_DPAD_RIGHT, GAME_CFG);
	AddControlOptions("controls.dpadright");

	AddInt("controls.l1", "L1", K_XBOX_LTRIG, GAME_CFG);
	AddControlOptions("controls.l1");

	AddInt("controls.l2", "L2", K_XBOX_WHITE, GAME_CFG);
	AddControlOptions("controls.l2");

	AddInt("controls.l3", "L3", K_XBOX_LTHUMB, GAME_CFG);
	AddControlOptions("controls.l3");

	AddInt("controls.r1", "R1", K_XBOX_RTRIG, GAME_CFG);
	AddControlOptions("controls.r1");

	AddInt("controls.r2", "R2", K_XBOX_BLACK, GAME_CFG);
	AddControlOptions("controls.r2");

	AddInt("controls.r3", "R3", K_XBOX_RTHUMB, GAME_CFG);
	AddControlOptions("controls.r3");

	LoadGlobalConfigs();

	return true;
}

void CXboxConfigs::Clear()
{
	for (mapIter it = m_ConfigsMap.begin(); it != m_ConfigsMap.end(); it++)
		delete (*it).second;

	m_ConfigsMap.clear();
}

void CXboxConfigs::AddBool(std::string strConfig, std::string strDisplayText, bool bValue, int iCfgType)
{
	CConfigBool* pConfig = new CConfigBool(strConfig, strDisplayText, iCfgType, bValue);

	if (!pConfig) return;

	m_ConfigsMap.insert(std::pair<std::string, CConfig*>(strConfig, pConfig));
}

void CXboxConfigs::SetBool(const char *strConfig, const bool bValue)
{
	mapIter it = m_ConfigsMap.find(strConfig);
	if (it != m_ConfigsMap.end())
	{
		((CConfigBool*)(*it).second)->SetValue(bValue);
		return;
	}

	SysMessage("CXboxConfigs::SetBool %s Not Found!\n", strConfig);
}

bool CXboxConfigs::GetBool(const char* strConfig)
{
	mapIter it = m_ConfigsMap.find(strConfig);
	
	if (it != m_ConfigsMap.end())
		return ((CConfigBool*)(*it).second)->GetValue();
	
	SysMessage("CXboxConfigs::GetBool %s Not Found!\n", strConfig);
	return 0;
}

void CXboxConfigs::AddInt(std::string strConfig, std::string strDisplayText, int iValue, int iCfgType)
{
	CConfigInt* pConfig = new CConfigInt(strConfig, strDisplayText, iCfgType, iValue);

	if (!pConfig) return;

	m_ConfigsMap.insert(std::pair<std::string, CConfig*>(strConfig, pConfig));
}

void CXboxConfigs::AddIntOption(std::string strConfig, std::string strDisplayText, int iValue)
{
	mapIter it = m_ConfigsMap.find(strConfig);

	if (it != m_ConfigsMap.end())
	{
		((CConfigInt*)(*it).second)->AddOption(strDisplayText, iValue);
		return;
	}

	SysMessage("CXboxConfigs::AddIntOption %s Not Found!\n", strConfig);
}

void CXboxConfigs::SetInt(const char *strConfig, const int iValue)
{
	mapIter it = m_ConfigsMap.find(strConfig);

	if (it != m_ConfigsMap.end())
	{
		((CConfigInt*)(*it).second)->SetValue(iValue);
		return;
	}

	SysMessage("CXboxConfigs::SetInt %s Not Found!\n", strConfig);
}

int CXboxConfigs::GetInt(const char* strConfig)
{
	mapIter it = m_ConfigsMap.find(strConfig);
	
	if (it != m_ConfigsMap.end())
		return ((CConfigInt*)(*it).second)->GetValue();
	
	SysMessage("CXboxConfigs::GetBool %s Not Found!\n", strConfig);
	return 0;
}

void CXboxConfigs::AddString(std::string strConfig, std::string strDisplayText, std::string strValue, int iCfgType)
{
	CConfigString* pConfig = new CConfigString(strConfig, strDisplayText, iCfgType, strValue);

	if (!pConfig) return;

	m_ConfigsMap.insert(std::pair<std::string, CConfig*>(strConfig, pConfig));
}

void CXboxConfigs::AddStringOption(std::string strConfig, std::string strDisplayText, std::string strValue)
{
	mapIter it = m_ConfigsMap.find(strConfig);

	if (it != m_ConfigsMap.end())
	{
		((CConfigString*)(*it).second)->AddOption(strDisplayText, strValue);
		return;
	}

	SysMessage("CXboxConfigs::AddIntOption %s Not Found!\n", strConfig);
}

void CXboxConfigs::SetString(const char *strConfig, const std::string strValue)
{
	mapIter it = m_ConfigsMap.find(strConfig);
	if (it != m_ConfigsMap.end())
	{
		((CConfigString*)(*it).second)->SetValue(strValue);
		return;
	}

	SysMessage("CXboxConfigs::SetBool %s Not Found!\n", strConfig);
}

std::string CXboxConfigs::GetString(const char* strConfig)
{
	mapIter it = m_ConfigsMap.find(strConfig);
	
	if (it != m_ConfigsMap.end())
		return ((CConfigString*)(*it).second)->GetValue();
	
	SysMessage("CXboxConfigs::GetBool %s Not Found!\n", strConfig);
	return "";
}


const char* CXboxConfigs::GetStringC(const char* strConfig) // For C code
{
	mapIter it = m_ConfigsMap.find(strConfig);
	
	if (it != m_ConfigsMap.end())
	{
		std::string strTmp = ((CConfigString*)(*it).second)->GetValue();
		char *cstr = new char[strTmp.length() + 1];
		strcpy(cstr, strTmp.c_str());

		return cstr;
	}
	
	SysMessage("CXboxConfigs::GetBool %s Not Found!\n", strConfig);
	return "";
}

CConfig* CXboxConfigs::GetConfig(const char* strConfig)
{
	mapIter it = m_ConfigsMap.find(strConfig);

	if (it != m_ConfigsMap.end())
		return (*it).second;
	
	SysMessage("CXboxConfigs::GetConfig %s Not Found!\n", strConfig);

	return NULL;
}

//==================================================================

bool CXboxConfigs::LoadGlobalConfigs()
{
	// Load the xml file // TODO: Move to own it's own function 
	TiXmlDocument xmlDoc;

	if (!xmlDoc.LoadFile(GLOBAL_CONFIGS_PATH))
	{
		SysMessage("Failed to load %s !\n", GLOBAL_CONFIGS_PATH);
		return false;
	}

	TiXmlElement* pRootElement = xmlDoc.RootElement();

	if (strcmpi(pRootElement->Value(), "globalconfigs") != 0)
	{
		SysMessage("%s Doesn't contain <globalconfigs>", GLOBAL_CONFIGS_PATH);
		return false;
	}

	for (mapIter it = m_ConfigsMap.begin(); it != m_ConfigsMap.end(); it++)
	{
		CStringArray strSplit;
		CStringUtils::SplitString((*it).first, strSplit);
	
		if (strSplit.size() > 1)
		{
			const TiXmlNode *pChild = pRootElement->FirstChild(strSplit[0].c_str());
			if (pChild)
			{
				const TiXmlNode *pGrandChild = pChild->FirstChild(strSplit[1].c_str());
				if (pGrandChild && pGrandChild->FirstChild())
				{
					std::string strValue = pGrandChild->FirstChild()->Value();
					if (strValue.size() )
					{
						if (strValue != "-")
						{
							CStringUtils::ToLower(strValue);
							(*it).second->FromString(strValue);
						}
					}
				}
			}
		}
	}

	return true;
}

bool CXboxConfigs::SaveGlobalConfigs()
{
	TiXmlDocument xmlDoc;
	TiXmlElement xmlRootElement("globalconfigs");

	TiXmlNode *pRootNode = xmlDoc.InsertEndChild(xmlRootElement);
	if (!pRootNode) return false;

	for (mapIter it = m_ConfigsMap.begin(); it != m_ConfigsMap.end(); it++)
	{
		if((*it).second->GetConfigType() != GLOBAL_CFG)
			continue;

		CStringArray strSplit;
		CStringUtils::SplitString((*it).first, strSplit);
		
		if (strSplit.size() > 1)
		{
			TiXmlNode *pChild = pRootNode->FirstChild(strSplit[0].c_str());
			if (!pChild)
			{
				TiXmlElement newElement(strSplit[0].c_str());
				pChild = pRootNode->InsertEndChild(newElement);
			}

			if (pChild)
			{
				TiXmlElement newElement(strSplit[1].c_str());
				TiXmlNode *pNewNode = pChild->InsertEndChild(newElement);
				if (pNewNode)
				{
					TiXmlText value((*it).second->ToString().c_str());
					pNewNode->InsertEndChild(value);
				}
			}
		}
	}

	return xmlDoc.SaveFile(GLOBAL_CONFIGS_PATH);
}

bool CXboxConfigs::LoadGameConfigs(std::string strGameFilename)
{
	TiXmlDocument doc(GAME_CONFIGS_PATH);

	if(!doc.LoadFile())
		return false;

	TiXmlElement *pRootElement = doc.RootElement();

	if(pRootElement)
	{
		std::string strRoot(pRootElement->Value());
		if(/*pRootElement->Value()*/strRoot != "gameconfigs") return false; // Had to put in std::string to use

		TiXmlNode *pGame = pRootElement->FirstChildElement("game");
 
		if(pGame)
		{
			while(pGame)
			{
				TiXmlElement *pFileElement = pGame->FirstChildElement("filename");

				std::string strFilename(pFileElement->GetText());
				if(strFilename == strGameFilename)
				{
					// Core Configs
					TiXmlElement *pCore = pFileElement->NextSiblingElement("core");
					if(pCore)
					{
						TiXmlElement *pCoreElemets = pCore->FirstChildElement();

						while(pCoreElemets)
						{
							std::string strElementName(pCoreElemets->Value());

							mapIter it = m_ConfigsMap.find("core." + strElementName);
	
							if (it != m_ConfigsMap.end() && ((*it).second->GetConfigType() == GAME_CFG))
								(*it).second->FromString(pCoreElemets->GetText());			

							pCoreElemets = pCoreElemets->NextSiblingElement();
						}
					}

					// Video Configs
					TiXmlElement *pVideo = pFileElement->NextSiblingElement("video");
					if(pVideo)
					{
						TiXmlElement *pVideoElemets = pVideo->FirstChildElement();
						
						while(pVideoElemets)
						{
							std::string strElementName(pVideoElemets->Value());

							mapIter it = m_ConfigsMap.find("video." + strElementName);
	
							if (it != m_ConfigsMap.end() && ((*it).second->GetConfigType() == GAME_CFG))
								(*it).second->FromString(pVideoElemets->GetText());	

							pVideoElemets = pVideoElemets->NextSiblingElement();
						}
					}

					// Sound Configs
					TiXmlElement *pSound = pFileElement->NextSiblingElement("sound");
					if(pSound)
					{
						TiXmlElement *pSoundElemets = pSound->FirstChildElement();

						while(pSoundElemets)
						{
							std::string strElementName(pSoundElemets->Value());

							mapIter it = m_ConfigsMap.find("sound." + strElementName);
	
							if (it != m_ConfigsMap.end() && ((*it).second->GetConfigType() == GAME_CFG))
								(*it).second->FromString(pSoundElemets->GetText());	

							pSoundElemets = pSoundElemets->NextSiblingElement();
						}
					}
					
					return true;
				}

				pGame = pGame->NextSiblingElement("game");
			}
		}
	}

	return false;
}

bool CXboxConfigs::SaveGameConfigs(std::string strGameFilename)
{
	TiXmlDocument doc;

	if(!doc.LoadFile(GAME_CONFIGS_PATH))
		return false;

	TiXmlElement *pRootElement = doc.RootElement();
	TiXmlNode *pGameNode = NULL;
	TiXmlNode *pExisitingGameNode = NULL;

	if(!pRootElement) return false;

	std::string strRoot(pRootElement->Value());
	if(/*pRootElement->Value()*/strRoot != "gameconfigs") return false; // Had to put in std::string to use

	pGameNode = pRootElement->FirstChildElement("game");
 
	if(pGameNode)
	{
		while(pGameNode)
		{
			TiXmlElement *pFileElement = pGameNode->FirstChildElement("filename");

			std::string strFilename(pFileElement->GetText());
			if(strFilename == strGameFilename)
			{
				pGameNode->Clear();
				pExisitingGameNode = pGameNode;
			}
			pGameNode = pGameNode->NextSiblingElement("game");
		}
	}

	std::vector<CConfig*> vecCoreCFG, vecVideoCFG, vecSoundCFG, vecControlsCFG;

	for (mapIter it = m_ConfigsMap.begin(); it != m_ConfigsMap.end(); it++)
	{
		if((*it).second->GetConfigType() != GAME_CFG)
			continue;

		CStringArray strSplit;
		CStringUtils::SplitString((*it).first, strSplit);
		
		// FIXME: Prob a much better way to do this, rather than store each cfg type!
		if (strSplit.size() > 1)
		{	
			if(strSplit[0] == "core")
				vecCoreCFG.push_back((*it).second);

			if(strSplit[0] == "video")
				vecVideoCFG.push_back((*it).second);

			if(strSplit[0] == "sound")
				vecSoundCFG.push_back((*it).second);

			if(strSplit[0] == "controls")
				vecControlsCFG.push_back((*it).second);
		}
	}

	if(pExisitingGameNode)
		pGameNode = pExisitingGameNode; // Reuse the cleared element
	else
		pGameNode = new TiXmlElement( "game" ); // If we didn't find an element to reuse create one

	TiXmlElement TitleElement("title");
	TiXmlNode *pNewNode = pGameNode->InsertEndChild(TitleElement);
	
	if (pNewNode)
	{
		TiXmlText value("GAME TITLE - WIP"); // TODO: Extract the title from ISO code
		pNewNode->InsertEndChild(value);
	}

	TiXmlElement FilenameElement("filename");
	pNewNode = pGameNode->InsertEndChild(FilenameElement);

	if (pNewNode)
	{
		TiXmlText value(strGameFilename.c_str());
		pNewNode->InsertEndChild(value);
	}
	
	// Core Configs
	TiXmlElement CoreElement("core");
	pNewNode = pGameNode->InsertEndChild(CoreElement);
	if (pNewNode)
	{
		for(int i = 0; i < (int)vecCoreCFG.size(); i++)
		{
			CConfig* pConfig = NULL;
			pConfig = vecCoreCFG[i];
				
				if(pConfig)
				{
					CStringArray strSplit;
					CStringUtils::SplitString(pConfig->GetConfigName(), strSplit);

					TiXmlElement value(strSplit[1].c_str());
					TiXmlNode *pNextNewNode = pNewNode->InsertEndChild(value);

					if(pNextNewNode)
					{
						TiXmlText value(pConfig->ToString().c_str());
						pNextNewNode->InsertEndChild(value);
					}
				}
			}
		}

		// Video Configs
		TiXmlElement VideoElement("video");
		pNewNode = pGameNode->InsertEndChild(VideoElement);
		if (pNewNode)
		{
			for(int i = 0; i < (int)vecVideoCFG.size(); i++)
			{
				CConfig* pConfig = NULL;
				pConfig = vecVideoCFG[i];
				
				if(pConfig)
				{
					CStringArray strSplit;
					CStringUtils::SplitString(pConfig->GetConfigName(), strSplit);

					TiXmlElement value(strSplit[1].c_str());
					TiXmlNode *pNextNewNode = pNewNode->InsertEndChild(value);

					if(pNextNewNode)
					{
						TiXmlText value(pConfig->ToString().c_str());
						pNextNewNode->InsertEndChild(value);
					}
				}
			}
		}

		// Sound Configs
		TiXmlElement SoundElement("sound");
		pNewNode = pGameNode->InsertEndChild(SoundElement);
		if (pNewNode)
		{
			for(int i = 0; i < (int)vecSoundCFG.size(); i++)
			{
				CConfig* pConfig = NULL;
				pConfig = vecSoundCFG[i];
				
				if(pConfig)
				{
					CStringArray strSplit;
					CStringUtils::SplitString(pConfig->GetConfigName(), strSplit);

					TiXmlElement value(strSplit[1].c_str());
					TiXmlNode *pNextNewNode = pNewNode->InsertEndChild(value);

					if(pNextNewNode)
					{
						TiXmlText value(pConfig->ToString().c_str());
						pNextNewNode->InsertEndChild(value);
					}
				}
			}
		}
	
		// Controls Configs
		TiXmlElement ControlsElement("controls");
		pNewNode = pGameNode->InsertEndChild(ControlsElement);
		if (pNewNode)
		{
			for(int i = 0; i < (int)vecControlsCFG.size(); i++)
			{
				CConfig* pConfig = NULL;
				pConfig = vecControlsCFG[i];
				
				if(pConfig)
				{
					CStringArray strSplit;
					CStringUtils::SplitString(pConfig->GetConfigName(), strSplit);

					TiXmlElement value(strSplit[1].c_str());
					TiXmlNode *pNextNewNode = pNewNode->InsertEndChild(value);

					if(pNextNewNode)
					{
						TiXmlText value(pConfig->ToString().c_str());
						pNextNewNode->InsertEndChild(value);
					}
				}
			}

		if(!pExisitingGameNode)
			pRootElement->LinkEndChild(pGameNode);  
	}

	return doc.SaveFile(GAME_CONFIGS_PATH);
}

const std::map<std::string, CConfig*> CXboxConfigs::GetConfigs(int iCategory) const
{
	CfgMap m_ConfigsCat;

	typedef CXboxConfigs::CfgMap::const_iterator mapIter;

	for (mapIter it = m_ConfigsMap.begin(); it != m_ConfigsMap.end(); it++)
	{
		CConfig* pConfig = (*it).second;
		if(pConfig)
		{
			CStringArray strSplit;
			CStringUtils::SplitString(pConfig->GetConfigName(), strSplit);
			
			switch(iCategory)
			{
				case CFG_VIDEO:
				{
					if(strSplit[0] == "video")
					m_ConfigsCat.insert(std::pair<std::string, CConfig*>(pConfig->GetConfigName(), pConfig));	
					break;
				}
				case CFG_SOUND:
				{
					if(strSplit[0] == "sound")
					m_ConfigsCat.insert(std::pair<std::string, CConfig*>(pConfig->GetConfigName(), pConfig));	
					break;
				}	
				case CFG_CORE:
				{
					if(strSplit[0] == "core")
					m_ConfigsCat.insert(std::pair<std::string, CConfig*>(pConfig->GetConfigName(), pConfig));
					break;
				}
				case CFG_CONTROLS:
				{
					if(strSplit[0] == "controls")
					m_ConfigsCat.insert(std::pair<std::string, CConfig*>(pConfig->GetConfigName(), pConfig));
					break;
				}
			}
		}
	}

	return m_ConfigsCat;
}

void CXboxConfigs::AddControlOptions(std::string strConfig)
{
	AddIntOption(strConfig, "XBOX A", K_XBOX_A);
	AddIntOption(strConfig, "XBOX B", K_XBOX_B);
	AddIntOption(strConfig, "XBOX X", K_XBOX_X);
	AddIntOption(strConfig, "XBOX Y", K_XBOX_Y);
	AddIntOption(strConfig, "XBOX Start", K_XBOX_START);
	AddIntOption(strConfig, "XBOX Back", K_XBOX_BACK);
	AddIntOption(strConfig, "XBOX Black", K_XBOX_BLACK);
	AddIntOption(strConfig, "XBOX White", K_XBOX_WHITE);
	AddIntOption(strConfig, "XBOX Dpad Up", K_XBOX_DPAD_UP);
	AddIntOption(strConfig, "XBOX Dpad Down", K_XBOX_DPAD_DOWN);
	AddIntOption(strConfig, "XBOX Dpad Left", K_XBOX_DPAD_LEFT);
	AddIntOption(strConfig, "XBOX Dpad Right", K_XBOX_DPAD_RIGHT);
	AddIntOption(strConfig, "XBOX Left Trigger", K_XBOX_LTRIG);
	AddIntOption(strConfig, "XBOX Right Trigger", K_XBOX_RTRIG);
	AddIntOption(strConfig, "XBOX Left Thumb", K_XBOX_LTHUMB);
	AddIntOption(strConfig, "XBOX Right Thumb", K_XBOX_RTHUMB);
}

void CXboxConfigs::GetScreenSize(int *iWidth, int *iHeight)
{
	switch(GetInt("video.resolution"))
	{
	case RESOLUTION_480P:
		*iWidth = 640;
		*iHeight = 480;
		break;

	case RESOLUTION_720P:
		*iWidth = 1280;
		*iHeight = 720;
		break;

	case RESOLUTION_1080I:
		*iWidth = 1920;
		*iHeight = 1080;
		break;

	default:
		*iWidth = 640;
		*iHeight = 480;
		break;
	}
}

void CXboxConfigs::PopulateSkins()
{
	std::vector<std::string> vecSkins;

	// Find all the directories in the skin folder
	CFileUtils::GetFilesInDirectory(SKINS_PATH, vecSkins, CFileUtils::DIRECTORY);	

	// Now load the skins into our options
	for(int i = 0; i < (int)vecSkins.size(); i++)
	{
		std::string strFilename;
		strFilename = SKINS_PATH+vecSkins[i]+"\\skininfo.xml";

		TiXmlDocument xmlDoc;
		if (!xmlDoc.LoadFile(strFilename.c_str()))
		{
			SysMessage("Could'nt load skin info XML: %s\n", strFilename.c_str()); 
			return;
		}

		TiXmlElement* pRootElement = xmlDoc.RootElement();

		std::string strValue = pRootElement->Value();

		if (strValue!=std::string("skininfo")) return;

		const TiXmlNode *pChild = pRootElement->FirstChild();
	
		std::string strName = pChild->Value();

		if(strName=="name")
			AddStringOption("gui.skin", pChild->FirstChild()->Value(), vecSkins[i]);
	}
	vecSkins.clear();
}

void CXboxConfigs::PopulateBiosFiles()
{
	std::vector<std::string> vecBiosFiles;
	CFileUtils::GetFilesInDirectory(BIOS_FILE_PATH, vecBiosFiles, CFileUtils::BIN_FILE);

	for(int i=0; i < (int)vecBiosFiles.size(); i++)
		AddStringOption("core.biosfile", vecBiosFiles[i], vecBiosFiles[i]);
}

//==================================================================================

BOOL XboxConfigs_Load()
{
	return (BOOL)g_XboxConfigs.Load();
}

void XboxConfigs_Clear()
{
	g_XboxConfigs.Clear();
}

BOOL XboxConfigs_GetBool(const char *strConfig)
{
	return (BOOL)g_XboxConfigs.GetBool(strConfig);
}

int XboxConfigs_GetInt(const char *strConfig)
{
	return g_XboxConfigs.GetInt(strConfig);
}

void XboxConfigs_SetInt(const char *strConfig, int iValue)
{
	g_XboxConfigs.SetInt(strConfig, iValue);
}

const char* XboxConfigs_GetString(const char *strConfig)
{
	return g_XboxConfigs.GetStringC(strConfig);
}

void XboxConfigs_GetScreenSize(int *iWidth, int *iHeight)
{
	g_XboxConfigs.GetScreenSize(iWidth, iHeight);
}

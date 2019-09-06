#ifndef H_CGUIINGAMEOSD
#define H_CGUIINGAMEOSD

#include "guifont.h"

class CGUIInGameOSD
{
public:
	CGUIInGameOSD();
	~CGUIInGameOSD();

	void Initialize();
	void Render(char* strFps);
	void DeInitialize();

private:
	int m_iPosX;
	int m_iPosY;
	unsigned int m_iFontSize;
	CGUIFont* m_pDebugFont;
	bool m_bAllocated;
	MEMORYSTATUS m_stat;
};

#endif //H_CGUIINGAMEOSD
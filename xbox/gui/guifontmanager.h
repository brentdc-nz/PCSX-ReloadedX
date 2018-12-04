#ifndef H_CGUIFONTMANAGER
#define H_CGUIFONTMANAGER

#include "guifont.h"
#include <vector>

class CGUIFontManager
{
public:
	CGUIFontManager();
	~CGUIFontManager();

	void Initialize();
	void Load(const std::string& strFontName, const std::string& strFilename, DWORD dwStyles);
	void Cleanup();
	CGUIFont* GetFont(std::string strFontName);

private:
	std::vector <CGUIFont*>m_vecFonts;
};

#endif //H_CGUIFONTMANAGER
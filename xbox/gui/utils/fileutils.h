#ifndef H_CFILEUTILS
#define H_CFILEUTILS

#include <vector>
#include <string>
#include <utility>

typedef std::pair<std::string, bool> PairStrBool;

class CFileUtils
{
public:
	static void GetFilesInDirectory(std::string strPath, std::vector<PairStrBool> &vecFiles, int iFileTypes);

	// TODO: Add more file types!
	enum FileTypes
	{
		DIRECTORY = 1<<0,
		CUE_FILE =  1<<1,
		BIN_FILE =  1<<2,
		IMG_FILE =  1<<3,
		ISO_FILE =  1<<4
	};
};

#endif //H_CFILEUTILS
#ifndef H_CFILEUTILS
#define H_CFILEUTILS

#include <vector>
#include <string>

class CFileUtils
{
public:
	static void GetFilesInDirectory(std::string strPath, std::vector<std::string> &vecFiles, int iFileTypes);

	// TODO: Add more file types!
	enum FileTypes
	{
		DIRECTORY = 1<<0,
		CUE_FILE =  1<<1,
		BIN_FILE =  1<<2,
		ISO_FILE =  1<<3
	};
};

#endif //H_CFILEUTILS
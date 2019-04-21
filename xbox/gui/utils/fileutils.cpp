#include "fileutils.h"
#include <algorithm>
#include <xtl.h>
#include "stringutils.h"

void CFileUtils::GetFilesInDirectory(std::string strPath, std::vector<PairStrBool> &vecFiles, int iFileTypes)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;

	memset(&wfd, 0, sizeof(wfd));
	CStringUtils::AddSlashAtEnd(strPath);

	std::string strSearchMask = strPath;
	strSearchMask += "*.*";

	hFind = FindFirstFile(strSearchMask.c_str(), &wfd);

	if(INVALID_HANDLE_VALUE != hFind) 
	{
		do
		{
			if (wfd.cFileName[0] != 0)
			{
				if ((iFileTypes & DIRECTORY) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					vecFiles.push_back(std::make_pair(wfd.cFileName, true));
				else
				{
					size_t len = strlen(wfd.cFileName);

					// Force the extensions to lower case
					for(int i = len -4; wfd.cFileName[i]; i++)
						wfd.cFileName[i] = tolower(wfd.cFileName[i]);

					if((iFileTypes & BIN_FILE) && (len >= 4 && !strcmp(wfd.cFileName + len - 4, ".bin")) // TODO: Add more file types!
					 ||(iFileTypes & ISO_FILE) && (len >= 4 && !strcmp(wfd.cFileName + len - 4, ".iso"))
					 ||(iFileTypes & IMG_FILE) && (len >= 4 && !strcmp(wfd.cFileName + len - 4, ".img"))
					 ||(iFileTypes & CUE_FILE) && (len >= 4 && !strcmp(wfd.cFileName + len - 4, ".cue")))
						vecFiles.push_back(std::make_pair(wfd.cFileName, false));
				}
			}
		}
		while (FindNextFile(hFind, &wfd) != 0);
	}

	FindClose(hFind);

	// Sort the strings alphabetically
	std::sort(vecFiles.begin(), vecFiles.end());
}
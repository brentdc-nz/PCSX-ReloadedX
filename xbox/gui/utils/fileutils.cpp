#include "fileutils.h"
#include <xtl.h>
#include "stringutils.h"

void CFileUtils::GetFilesInDirectory(std::string strPath, std::vector<std::string> &vecFiles, int iFileTypes)
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
					vecFiles.push_back(wfd.cFileName);
				else
				{
					size_t len = strlen(wfd.cFileName);

					// Force the extensions to lower case
					for(int i = len -4; wfd.cFileName[i]; i++)
						wfd.cFileName[i] = tolower(wfd.cFileName[i]);

					if((iFileTypes & BIN_FILE) && (len >= 4 && !strcmp(wfd.cFileName + len - 4, ".bin")) // TODO: Add more file types!
					 ||(iFileTypes & ISO_FILE) && (len >= 4 && !strcmp(wfd.cFileName + len - 4, ".iso"))
					 ||(iFileTypes & CUE_FILE) && (len >= 4 && !strcmp(wfd.cFileName + len - 4, ".cue")))		
						vecFiles.push_back(wfd.cFileName);
				}
			}
		}
		while (FindNextFile(hFind, &wfd) != 0);
	}

	FindClose(hFind);
}
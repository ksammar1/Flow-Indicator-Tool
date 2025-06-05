/*
	
*/

#ifndef __FILE_SYSTEM_ORE_TOOL_
#define __FILE_SYSTEM_ORE_TOOL_

#ifdef _WIN32
#pragma warning(disable: 4251) // disable warning C4521
#ifdef  common_logic_EXPORTS 
#define DLLEXPORT __declspec(dllexport)  
#else
#define DLLEXPORT __declspec(dllimport)  
#endif
#else
#define DLLEXPORT
#endif

// Include standard C++ headers.
#include <string>
#include <iostream>
#include <set>


// TODO(Anton) - Need to split the seperate file, also rename OreHelper to "Helpers"
namespace FileSystem
{
	bool DLLEXPORT IsExist(const std::string& fileNamePath);
	bool DLLEXPORT IsDirectory(const std::string& fileNamePath);
	bool DLLEXPORT CreateFolder(const std::string& folderNamePath);
	std::string DLLEXPORT GetCurrentPath();
	std::string DLLEXPORT MakePreferred(const std::string& path);

	std::set<std::string> DLLEXPORT ListFilesRecursively(const std::string& path);


	std::set<std::string> DLLEXPORT GetFileNamesRecursively(const std::string& path, std::set<std::string> foldersToSkipRecurse = std::set<std::string>());

	struct DirEntry
	{
		std::string m_fileName;
		std::string m_fullPath;
		std::string m_parentPath;

		int m_inode;

		bool operator<(const DirEntry& rhs) const
		{
			return this->m_fileName < rhs.m_fileName;
		}
	};

	std::set<DirEntry> DLLEXPORT GetFilesDataRecursively(const std::string& path, std::set<std::string> foldersToSkipRecurse = std::set<std::string>());
}

#endif
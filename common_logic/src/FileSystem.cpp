

#include <FileSystem.h>
#include <Advantest_VM.h>
#include <stdexcept>

#ifdef _WIN32
#include <filesystem>
namespace fs = std::filesystem;

#else

#ifndef __SMT7_ADVANTEST_VM_
	#include <experimental/filesystem> 
	namespace fs = std::experimental::filesystem;
#else
	// Linux APIs
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <linux/limits.h>
#endif

// Linux APIs
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>

#endif


bool FileSystem::IsExist(const std::string& fileNamePath)
{
#ifdef _WIN32
    return fs::exists(fileNamePath);
#else
#ifdef __SMT7_ADVANTEST_VM_
    struct stat buff;
    if (0 == stat(fileNamePath.c_str(), &buff))
    {
        return true;
    }
    return false;

#else // This is VLSI, may use filesystem API.
    return fs::exists(fileNamePath);
#endif
#endif
}

bool FileSystem::IsDirectory(const std::string& fileNamePath)
{
#ifdef _WIN32
    return fs::is_directory(fileNamePath);
#else
#ifdef __SMT7_ADVANTEST_VM_
    struct stat buff;
    if (0 == stat(fileNamePath.c_str(), &buff) && (buff.st_mode & S_IFMT) == S_IFDIR)
    {
        return true;
    }
#else
    return fs::is_directory(fileNamePath);
#endif
#endif
}

bool FileSystem::CreateFolder(const std::string& folderNamePath)
{
#ifdef _WIN32
    return fs::create_directory(folderNamePath);
#else
#ifdef __SMT7_ADVANTEST_VM_
    struct stat buff;
    if (-1 == stat(folderNamePath.c_str(), &buff))
    {
        mkdir(folderNamePath.c_str(), 0775);
        return true;
    }
    std::cerr << "Folder exist!.\n";
    return false;
#else
    return fs::create_directory(folderNamePath);
#endif
#endif
}

std::string DLLEXPORT FileSystem::GetCurrentPath()
{

#ifdef _WIN32
    return fs::current_path().string();
#else
#ifdef __SMT7_ADVANTEST_VM_
    char currentPath[PATH_MAX];
    if (nullptr != getcwd(currentPath, sizeof(currentPath)))
    {
        return std::string{ currentPath };
    }
    else
    {
        throw std::runtime_error("getcwd returned null.");;
    }

#else
	// VLSI
	return fs::current_path().string();
#endif
#endif
}

std::string FileSystem::MakePreferred(const std::string& path)
{
#ifdef _WIN32
    fs::path tmpPathObj{ path };
    return tmpPathObj.make_preferred().string();
#else

#ifdef __SMT7_ADVANTEST_VM_

    // TODO(Anton) - Need to implement this.
    throw std::invalid_argument("__NOT_IMPLEMTATED_YET___");
#else
    fs::path tmpPathObj{ path };
    return tmpPathObj.make_preferred().string();

#endif
#endif
}


#ifndef _WIN32

static void _ListFilesRecursively(const std::string& path, std::set<std::string>& fileList)
{
    DIR* dir = opendir(path.c_str());
    struct dirent* dirEntryPtr = nullptr;
    size_t fileCnt = 0;
    while (dirEntryPtr = readdir(dir))
    {
        if ("." == std::string(dirEntryPtr->d_name) || ".." == std::string(dirEntryPtr->d_name))
        {
            continue;
        }

        fileList.insert(std::string(dirEntryPtr->d_name));
        const std::string fileName = path + "/" + std::string(dirEntryPtr->d_name);
        // std::cout << '\t' << ++fileCnt << ") " << fileName << " ";
        struct stat buf;
        const int statStatus = stat(fileName.c_str(), &buf);
        if (0 != statStatus)
        {
            // std::cout << std::endl;
            continue;
        }

        if ((buf.st_mode & S_IFMT) == S_IFDIR)
        {
            // std::cout << "[DIR] " << std::endl;
            _ListFilesRecursively(fileName, fileList);
        }
        else
        {
            // std::cout << "[reg file] " << std::endl;
        }
    }
    // std::cout << "Closed dir: " << path << std::endl;
    closedir(dir);
}

#endif

std::set<std::string> FileSystem::ListFilesRecursively(const std::string& path)
{
    // std::cout << "Current path: " << path << std::endl;

    std::set<std::string> filesList;

#ifdef _WIN32
    for (auto dirEntIter = fs::recursive_directory_iterator{ path };
        dirEntIter != fs::recursive_directory_iterator();
        ++dirEntIter)
    {
        filesList.insert(dirEntIter->path().filename().string());
    }
#else

#ifdef __SMT7_ADVANTEST_VM_

    _ListFilesRecursively(path, filesList);

#else

    for (auto dirEntIter = fs::recursive_directory_iterator{ path };
        dirEntIter != fs::recursive_directory_iterator();
        ++dirEntIter)
    {
        filesList.insert(dirEntIter->path().filename().string());
    }

#endif
#endif

    return filesList;
}


#ifndef _WIN32

static void _ListFilesRecursively(const std::string& path,
	std::set<std::string>& fileList,
	std::set<std::string> foldersToSkipRecurse)
{
	DIR* dir = opendir(path.c_str());
	struct dirent* dirEntryPtr = nullptr;
	size_t fileCnt = 0;
	while (dirEntryPtr = readdir(dir))
	{
		if ("." == std::string(dirEntryPtr->d_name) || ".." == std::string(dirEntryPtr->d_name))
		{
			continue;
		}

		const std::string fileName = path + "/" + std::string(dirEntryPtr->d_name);
		// std::cout << '\t' << ++fileCnt << ") " << fileName << " ";


		fileList.insert(std::string(dirEntryPtr->d_name));

		struct stat buf;
		const int statStatus = stat(fileName.c_str(), &buf);
		if (0 != statStatus)
		{
			// std::cout << std::endl;
			continue;
		}

		if ((buf.st_mode & S_IFMT) == S_IFDIR)
		{
			// std::cout << "[DIR] " << std::endl;
			_ListFilesRecursively(fileName, fileList, foldersToSkipRecurse);
		}
		else
		{
			// std::cout << "[reg file] " << std::endl;
		}
	}
	// std::cout << "Closed dir: " << path << std::endl;
	closedir(dir);
}

#endif



std::set<std::string> FileSystem::GetFileNamesRecursively(const std::string& path,
	std::set<std::string> foldersToSkipRecurse)
{
	std::set<std::string> filesList;

#ifdef _WIN32
	for (auto dirEntIter = fs::recursive_directory_iterator{ path };
		dirEntIter != fs::recursive_directory_iterator();
		++dirEntIter)
	{
	/*	bool skip = false;
		for (auto& eachFolder : foldersToSkipRecurse)
		{
			if (std::string::npos == path.find(eachFolder))
			{
				skip = true;
			}
		}

		if (skip)
		{
			continue;
		}*/

		if (foldersToSkipRecurse.count(dirEntIter->path().filename().string()) > 0)
		{
			dirEntIter.disable_recursion_pending();
			continue;
		}
		filesList.insert(dirEntIter->path().filename().string());

	}
#else
#ifdef __SMT7_ADVANTEST_VM_
	_ListFilesRecursively(path, filesList, foldersToSkipRecurse);
#else
	for (auto dirEntIter = fs::recursive_directory_iterator{ path };
		dirEntIter != fs::recursive_directory_iterator();
		++dirEntIter)
	{
		/*bool skip = false;
		for (auto& eachFolder : foldersToSkipRecurse)
		{
			if (std::string::npos == path.find(eachFolder))
			{
				skip = true;
			}
		}

		if (skip)
		{
			continue;
		}*/
		if (foldersToSkipRecurse.count(dirEntIter->path().filename().string()) > 0)
		{
			dirEntIter.disable_recursion_pending();
			continue;
		}

		filesList.insert(dirEntIter->path().filename().string());

	}
#endif
#endif
	return filesList;
}



#ifndef _WIN32

static void _ListFilesDataRecursively(const std::string& path,
	std::set<FileSystem::DirEntry>& fileList,
	std::set<std::string> foldersToSkipRecurse)
{
	DIR* dir = opendir(path.c_str());
	struct dirent* dirEntryPtr = nullptr;
	size_t fileCnt = 0;
	while (dirEntryPtr = readdir(dir))
	{
		if ("." == std::string(dirEntryPtr->d_name) || ".." == std::string(dirEntryPtr->d_name) || foldersToSkipRecurse.count(dirEntryPtr->d_name) > 0 )
		{
			continue;
		}

		const std::string fileName = path + "/" + std::string(dirEntryPtr->d_name);
		// std::cout << '\t' << ++fileCnt << ") " << fileName << " ";

		FileSystem::DirEntry fileData;
		fileData.m_fileName = std::string(dirEntryPtr->d_name);
		fileData.m_fullPath = std::string(path + "/" + dirEntryPtr->d_name);
		fileData.m_parentPath = std::string(path);

		fileList.insert(fileData);

		struct stat buf;
		const int statStatus = stat(fileName.c_str(), &buf);
		if (0 != statStatus)
		{
			// std::cout << std::endl;
			continue;
		}

		if ((buf.st_mode & S_IFMT) == S_IFDIR)
		{
			// std::cout << "[DIR] " << std::endl;
			_ListFilesDataRecursively(fileName, fileList, foldersToSkipRecurse);
		}
		else
		{
			// std::cout << "[reg file] " << std::endl;
		}
	}
	// std::cout << "Closed dir: " << path << std::endl;
	closedir(dir);
}

#endif

std::set<FileSystem::DirEntry> FileSystem::GetFilesDataRecursively(const std::string& path,
	std::set<std::string> foldersToSkipRecurse)
{
	std::set<DirEntry> retSet;

#ifdef _WIN32
	for (auto itrDirEnt = fs::recursive_directory_iterator{ path };
		itrDirEnt != fs::recursive_directory_iterator{};
		++itrDirEnt)
	{
		/*if (fs::is_symlink(itrDirEnt->path()))
		{
			GetFilesDataRecursively(fs::read_symlink(itrDirEnt->path()),foldersToSkipRecurse)
		}*/
		if (foldersToSkipRecurse.count(itrDirEnt->path().filename().string()) > 0)
		{
			itrDirEnt.disable_recursion_pending();
			continue;
		}
		DirEntry fileData;
		fileData.m_fileName = itrDirEnt->path().filename().string();
		fileData.m_fullPath = itrDirEnt->path().string();
		fileData.m_parentPath = itrDirEnt->path().parent_path().string();

		retSet.insert(fileData);
	}

#else

#ifdef __SMT7_ADVANTEST_VM_
	_ListFilesDataRecursively(path, retSet, foldersToSkipRecurse);
#else

	for (auto itrDirEnt = fs::recursive_directory_iterator{ path };
		itrDirEnt != fs::recursive_directory_iterator{};
		++itrDirEnt)
	{
		if (foldersToSkipRecurse.count(itrDirEnt->path().filename().string()) > 0)
		{
			itrDirEnt.disable_recursion_pending();
			continue;
		}
		DirEntry fileData;
		fileData.m_fileName = itrDirEnt->path().filename().string();
		fileData.m_fullPath = itrDirEnt->path().string();
		fileData.m_parentPath = itrDirEnt->path().parent_path().string();


		retSet.insert(fileData);
	}

#endif
#endif
	return retSet;
}
#include "OsService.h"
//**********************************************************************************
void OsService::OpenDirectoryWindow(const std::string& pathTodir)
{
    const std::string pathToOpendir = "explorer \"" + pathTodir + "\"";

    if (OS_WINDOWS)
    {
        system(pathToOpendir.c_str());
    }
    else
    {
       // std::cout << "IT IS NOT SUPPORTED ON LINUX TO OPEN THE DIRECTORY" << std::endl;
    }
}
//**********************************************************************************
void OsService::CallPause() 
{
    if (OS_WINDOWS)
    {
        system("pause");
    }
    else
    {
       // std::cout << "IT IS NOT SUPPORTED ON LINUX TO keep the directory open" << std::endl;
    }

}
//**********************************************************************************
void OsService::PrintWithColor(const int clr)
{
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
#else
    // TODO(Karam) - Add color support for Linux. (CME have example)
#endif
 
}
//**********************************************************************************
void OsService::OpenDirWindowAndPause(const std::string& pathTodir)
{
    std::string path = pathTodir;
    std::replace_if(path.begin(), path.end(), [](char ch)
        {
            return '/' == ch;
        }, '\\');
    OsService::OpenDirectoryWindow(path);
    OsService::CallPause();
}
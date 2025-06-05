#ifndef OSSERVICE_H_
#define OSSERVICE_H_

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


//**********************************************************************************
#include <iostream>
#include <string>
#include <algorithm>
//**********************************************************************************
#ifdef _WIN32

#define OS_WINDOWS 1
#include <Windows.h>

#else

#define OS_WINDOWS 0

#endif
//**********************************************************************************
namespace OsService
{
	 void DLLEXPORT OpenDirectoryWindow(const std::string& pathTodir);
	 void DLLEXPORT PrintWithColor(const int clr);
	 void DLLEXPORT CallPause();
	 void DLLEXPORT OpenDirWindowAndPause(const std::string& pathTodir);
}
//**********************************************************************************
#endif

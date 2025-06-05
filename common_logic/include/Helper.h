#ifndef HELPER_H_
#define HELPER_H_


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

// Standard C++ headers.
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
#include <vector>
#include <cstdlib> 
#include <fstream>
#include <cerrno>
#include <sstream>
#include <set>
#include <exception>
#include <cctype>
#include <map>
#include <iterator>
#include <thread>
#include <cmath>
#include <stack>
// Local Project Headers.
#include <OsService.h>
#include <FileSystem.h>

//**********************************************************************************
//add namespace || capital and underscore
constexpr int DateLength = 26;
constexpr int LineLength = 53;
constexpr char PipToken = '|';
constexpr char Comma = ',';
const std::vector<std::string> TESTSUITE_MISSIONS = { "TEST_SETUP","SSN_SETUP", "TEST_PAYLOAD","SSN_END","TEST_END","_PL_"};
//put in cpp if you use them only there
//**********************************************************************************


//**********************************************************************************
namespace color
{
	const int Red = 4;
	const int White = 7;
	const int Green = 2;
};
//**********************************************************************************
typedef struct DLLEXPORT DataToUzipFile
{
	std::string testpPath;
	std::string path;
	std::string parent_path;
	std::string fileName;
} DataToUzipFile;

struct DLLEXPORT PatternData
{
	std::string patternName;
	std::string dieNumber;
	std::string portName;
	std::string revNumber;
};

// TODO(Anton) - Try to Cathegorize them into different namespaces\structs.
namespace Helpers
{
	std::string DLLEXPORT GetDate();

	std::pair<int, int> DLLEXPORT CheckValidNumberOrInterval(const std::string& substr, const int MaxNumber);
	
	std::vector<int> DLLEXPORT LineToIntVector(const std::string& line);
	
	bool DLLEXPORT CheckValidityAndInsert(std::set<int>& numbers, const std::string& substr, const int maxnumber);

	void DLLEXPORT GetPatternsFromBurstFolder(const std::string& pathToVecFolder, 
		                                      std::map<std::string, std::string>& burstAndItsFolder, 
		                                      std::map<std::string, std::vector<PatternData>>& burstAndItsPatterns);

	double DLLEXPORT UpdateValueAccordingToUnit(const std::string& strval, const std::string& unit);

	/*
		Anton - 11.28.2024
		
		The function CheckValidEntryAndInsertPatterns and IterateOverBurstFolder moved from helpers namespaces to global scope.
		and made it static in the C++ files, the reason is to avoid expositing it in the header as we would like to
		prevent from using fs = filesystem in the header files to make portable code for older revision of LINUX
		where there is compiler dependancy.

		As they are used by GetPatternsFromBurstFolder( ... )

	*/

	/*
	   bool CheckValidEntryAndInsertPatterns(const std::map<std::string, std::set<std::string>>& folderAndTheBurstsInside,
										  const fs::directory_entry& entry,
										  std::map<std::string, std::vector<PatternData>>& burstAndItsPatterns,
										  std::vector<DataToUzipFile>& dataforZippedFiles);

		void DLLEXPORT IterateOverBurstFolder(const std::string& testPath,
										  const std::map<std::string,
										  std::set<std::string>>& folderAndTheBurstsInside,
										  const fs::path& pathtoburstfolder,
										  std::map<std::string, std::vector<PatternData>>& burstAndItsPatterns,
										  std::vector<DataToUzipFile>& dataforZippedFiles);

	 */

	bool DLLEXPORT CalcTheStrExpressionWithOneOperator(const std::string& exprstr, double& res);
	//double DLLEXPORT CalcTheStringExpression(const std::string& expression);
	std::string DLLEXPORT GetTestSuiteOrPatternMission(const std::string& patternOrTestSuite);

	void DLLEXPORT GetPorts(const std::string& fileName);
	
	std::vector<int> DLLEXPORT ConvertDecimalToBinary(const int val);

}
//**********************************************************************************
namespace FileHelper
{
	std::vector<std::string> DLLEXPORT CreateNewDirectorywithDate(const std::string& tpPath, const std::string& folderThatContainresults, const std::string& resultFolderName = std::string());
	void DLLEXPORT SearchForPmflInVectorsFolder(const std::string& pmflFileName, const std::string& vectorsFolderPath,
		std::string& currPath, std::map<std::string, std::string>& burstAndItsFolder, std::set<std::string>& BurstsToSearch,const bool needAllpatterns);
	std::string DLLEXPORT unzipAndRead(const std::string& gzFilePath, const std::string& parentpath, const std::string& fileName);

	// TODO(Anton) - Where is the definition?
	void DLLEXPORT UnzipWithThread(std::map<std::string, std::vector<std::pair<std::string, std::string>>>& burstAndItsPatterns, const std::vector<DataToUzipFile>& dataforZippedFiles, const int iterationPerThread, const int intialIndex, std::set<std::string>& UnzippedFiles);

	void DLLEXPORT GetBurstsPath(const std::string& pmflFileName, const std::string& vectorsFolderPath, std::map<std::string,
		std::string>& burstAndItsFolder, std::set<std::string>& BurstsToSearch,const bool needAllpatterns);


	
	std::vector<PatternData> DLLEXPORT ReadBurstFile(const std::string& burstFile);
}
//**********************************************************************************
namespace ConsoleHelper
{
	void DLLEXPORT PrintLines(const char ch, const int length);
	std::string DLLEXPORT CreateFolderToContainResults(const std::string& TPpath, const std::string& folderThatContainresults);
	bool DLLEXPORT YesNoQuestion(const std::string& askmessage);
	std::string DLLEXPORT GetTestProgramPath(const std::string& folderEnrty = std::string(), const std::string& errormsg=std::string());

	void DLLEXPORT PrintTable(const std::map<int, std::string>& lines, const size_t rowsize);
	bool DLLEXPORT CheckValidTableindex(const std::map<int, std::string>& lines, const int index, const bool IndexFromZero);

	std::vector<std::string> DLLEXPORT GetTableIndex(const std::map<int, std::string>& input, 
													 const std::string& msg, 
													 const bool IndexFromZero);

	std::vector<std::string> DLLEXPORT GetNameWithMinus(const std::string& askmsg);
	void DLLEXPORT PrintErrorMessage(const std::string& msg);
	void DLLEXPORT PrintDirPathfromOre(const std::string& dirName);
	std::string DLLEXPORT GetFileNameFromCommandLine(const std::string& type, const std::string& errormessage, const std::string& cmdarg);
	std::set<int> DLLEXPORT GetNumberswithIntervals(const std::string& askmsg, const int maxVAL);
	std::vector<int> DLLEXPORT GetNumbersFromConsole(const std::string& askmsg, const int maxVAL);

	// TODO(Anton) - How come there is no definition?
	std::vector<int> DLLEXPORT GetNumbersWithMinus(const std::string& askmsg, const int maxVAL);
}
//**********************************************************************************
namespace StringHelper
{
	std::vector<std::string> DLLEXPORT TokenizeString(const std::string& str, const char del);
	bool DLLEXPORT AllDigitsOrAlphabet(std::string str);
	std::pair<int, int> DLLEXPORT GetStartEndIndexesOfInterval(const std::string& str);
	std::string DLLEXPORT ReadDataBetweenTags(const std::string& line, const std::string& opentag, const std::string& closetag);
	bool DLLEXPORT CheckValidNames(const std::string& name);
	std::string DLLEXPORT GetRevNumber(const std::string& pattern,const std::string& revstr);
	void DLLEXPORT GetSuitablePath(std::string& path);
	std::string DLLEXPORT GetBinaryStringFromHexString(const std::string& sHex);
}
//**********************************************************************************
#endif

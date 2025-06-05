

// Include Local project headers.
#include "Helper.h"
#include "OsService.h"

// TODO(Anton) -> This will be deleted later.
#ifdef _WIN32
#include <filesystem>
namespace fs = std::filesystem;

#else
#include <Advantest_VM.h>

#ifdef __SMT7_ADVANTEST_VM_
// Linux APIs
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>
#else

// VLSI
#include <experimental/filesystem> 
namespace fs = std::experimental::filesystem;

#endif
#endif



void ConsoleHelper::PrintLines(const char ch, const int length)
{
    std::cout << std::string(length, ch) << ch << std::endl;
}

std::pair<int, int> StringHelper::GetStartEndIndexesOfInterval(const std::string& str)
{
    const size_t found = str.find('-');
    if (found != str.npos)
    {
        std::pair<int, int> interval;
        std::string start, end;
        start = str.substr(0, found);
        end = str.substr(found + 1);
        if (!std::all_of(start.begin(), start.end(), ::isdigit) || !std::all_of(end.begin(), end.end(), ::isdigit) || start.empty()||end.empty())
        {
            return std::make_pair(-1, -1);
        }
        
        interval.first = std::stoi(start);
        interval.second = std::stoi(end);
        return interval;
    }
    else if(!std::all_of(str.begin(), str.end(), ::isdigit))
    {
        return std::make_pair(-1, -1);
    }
    const int qpos = std::stoi(str);
    return std::make_pair(qpos, qpos);
}

std::string StringHelper::GetBinaryStringFromHexString(const std::string& sHex)
{
    std::string sReturn = "";
    for (int i = 0; i < sHex.length(); ++i)
    {
        switch (sHex[i])
        {
        case '0': sReturn.append("0000"); break;
        case '1': sReturn.append("0001"); break;
        case '2': sReturn.append("0010"); break;
        case '3': sReturn.append("0011"); break;
        case '4': sReturn.append("0100"); break;
        case '5': sReturn.append("0101"); break;
        case '6': sReturn.append("0110"); break;
        case '7': sReturn.append("0111"); break;
        case '8': sReturn.append("1000"); break;
        case '9': sReturn.append("1001"); break;
        case 'a':
        case 'A':
            sReturn.append("1010");
            break;
        case 'b':
        case 'B':
            sReturn.append("1011");
            break;
        case 'c':
        case 'C':
            sReturn.append("1100");
            break;
        case 'd':
        case 'D':
            sReturn.append("1101");
            break;
        case 'e':
        case 'E':
            sReturn.append("1110");
            break;
        case 'f':
        case 'F':
            sReturn.append("1111");
            break;
        }
    }
    return sReturn;
}


std::string Helpers::GetDate()
{
    auto curretPoint = std::chrono::system_clock::now();
    const std::time_t curretPointTimeT = std::chrono::system_clock::to_time_t(curretPoint);

    std::string strdate = ctime(&curretPointTimeT);
    //std::string strdate = date;
    //the date end with /n we should remove it
    strdate.erase(std::remove(strdate.begin(), strdate.end(), '\n'), strdate.end());

    std::replace(strdate.begin(), strdate.end(), ' ', '_');
    std::replace(strdate.begin(), strdate.end(), ':', '_');
    return strdate;
}
std::string ConsoleHelper::CreateFolderToContainResults(const std::string& TPpath, const std::string& folderThatContainresults)
{
   // const std::string PathtoDir = TPpath + "\\" + strdate;
    const std::string PathtoDir = TPpath + "/" + folderThatContainresults;
    if (!(FileSystem::IsExist(TPpath+ "/" + folderThatContainresults) && FileSystem::IsDirectory(TPpath + "/" + folderThatContainresults)))
    {
        FileSystem::CreateFolder(TPpath + "/" + folderThatContainresults);
    }
    return PathtoDir;
}
//**********************************************************************************
void ConsoleHelper::PrintDirPathfromOre(const std::string&  dirName)
{
    OsService::PrintWithColor(color::Green);
    ConsoleHelper::PrintLines('#', LineLength);
    std::cout << "Path For OreConfig Field:" << std::endl;
    std::cout << "ore/" << dirName << "/OreXml.xml" << std::endl;
    ConsoleHelper::PrintLines('#', LineLength);
    OsService::PrintWithColor(color::White);

}
//**********************************************************************************
std::vector<std::string> StringHelper::TokenizeString(const std::string& str, const char del)
{
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string substr;
    while (std::getline(ss, substr, del))
    {
        res.push_back(substr);
    }
    return res;
}
//**********************************************************************************
std::string ConsoleHelper::GetTestProgramPath(const std::string& folderEnrty, const std::string& errormsg)
{
    std::string TPpath;

    std::cout << "Enter Path to Test Program: ";
   std::cin >> TPpath;
   std::string printmsg = errormsg.empty() ? "path not found" : errormsg;
   while (TPpath.empty() || !FileSystem::IsExist(TPpath)) {
       std::cout << printmsg << std::endl;
       std::cout << "Enter Path to Test Program: ";
       std::getline(std::cin, TPpath);
   }
    std::string entry{ TPpath };
    if (!folderEnrty.empty()) {
        entry.assign(TPpath + "/" + folderEnrty);
    }
    PrintLines('-', LineLength);
    return TPpath;
}
//**********************************************************************************
bool StringHelper::AllDigitsOrAlphabet(std::string str)
{
    return std::all_of(str.begin(), str.end(), [](char ch)
        {
            return (::isdigit(ch) || isalpha(ch)||'_'== ch);
        });
}
//**********************************************************************************
std::pair<int, int> Helpers::CheckValidNumberOrInterval(const std::string& substr, const int MaxNumber)
{
   auto error_pair = std::make_pair(-1, -1);
    const std::pair<int, int> interval = StringHelper::GetStartEndIndexesOfInterval(substr);
    if (-1 == interval.first)
    {
        ConsoleHelper::PrintErrorMessage("The interval/Number should be only from digits");
        return  error_pair;
    }
    if (interval.first > interval.second)
    {
        ConsoleHelper::PrintErrorMessage("Invalid Interval the left number bigger than the right one");
        return error_pair;
    }
    if (interval.first < 0 || interval.first  >MaxNumber || interval.second < 0 || interval.second  >MaxNumber)
    {
        ConsoleHelper::PrintErrorMessage("Invalid Interval OR Number,The Indexes must be between 0-" + MaxNumber);
        return error_pair;
    }
    return interval;
}
//**********************************************************************************
bool  ConsoleHelper::YesNoQuestion(const std::string& askmessage)
{
    std::string ans;

    while (1)
    {
        std::cout << askmessage;
        std::cin >> ans;
        if ("Y" == ans || "y" == ans)
        {
            return true;
        }
        else if ("N" == ans || "n" == ans)
        {
            return false;
        }
        else
        {
            PrintErrorMessage(" you should answer the question by typing Y or N (YES OR NO) ");
        }
    }
}
//**********************************************************************************
std::vector<std::string> FileHelper::CreateNewDirectorywithDate(const std::string& tpPath,const std::string& folderThatContainresults  ,const std::string& resultFolderName)
{
    std::string strdate = Helpers::GetDate();
    std::string pathtoDir = ConsoleHelper::CreateFolderToContainResults(tpPath, folderThatContainresults);
    try
    {
        FileSystem::CreateFolder(pathtoDir + "/" + resultFolderName + "_" + strdate);
    }
    catch (const std::exception& exc)
    {
        std::cout << exc.what() << std::endl;
    }

   std::replace(pathtoDir.begin(), pathtoDir.end(), '\\', '/');
    /*const size_t index = pathtoDir.find_last_of('/');
    if (std::string::npos != index)
    {
       // return { pathtoDir + "/" + resultFolderName + "_" + strdate, pathtoDir.substr(index + 1) };
        return { pathtoDir + "/" + resultFolderName + "_" + strdate,  resultFolderName + "_" + strdate };
    }*/
    return { pathtoDir + "/" + resultFolderName + "_" + strdate, resultFolderName + "_" + strdate };
}
//**********************************************************************************
void ConsoleHelper::PrintErrorMessage(const std::string& msg)
{
    //PrintLines('-', LineLength);
    OsService::PrintWithColor(color::Red);
    std::cout << msg << std::endl;
    OsService::PrintWithColor(color::White);
    PrintLines('-', LineLength);

}
void ConsoleHelper::PrintTable(const std::map<int, std::string>& lines, const size_t rowsize)
{
    ConsoleHelper::PrintLines('-', LineLength);

    for (const auto& index :lines)
    {
        size_t spaces = rowsize - index.second.size() + 2;
        std::cout << index.second;
        for (int j = 0; j < spaces; j++)
        {
            std::cout << " ";
        }
        std::cout << " | " <<index.first << " |" << std::endl;
    }
    ConsoleHelper::PrintLines('-', 35);

}
//**********************************************************************************
bool ConsoleHelper::CheckValidTableindex(const std::map<int, std::string>& lines,const int index, const bool IndexFromZero)
{
    if (IndexFromZero)
    {
        return (0 <= index && lines.size() > index);
    }
    else
    {
        return (0 < index && lines.size() >= index);
    }

}
//**********************************************************************************
std::vector<std::string> ConsoleHelper::GetTableIndex(const std::map<int, std::string>& input, const std::string& msg,const bool IndexFromZero)
{
    std::string temp;
    std::vector<std::string> indexes;
    std::vector<std::string> data;
    while (1)
    {
        bool valid = true;
        std::cout << msg;
        std::cin >> temp;
        indexes = StringHelper::TokenizeString(temp, Comma);
       
        for (const auto& index : indexes)
        {
            if (!std::all_of(index.begin(), index.end(), ::isdigit))
            {
                ConsoleHelper::PrintErrorMessage("you should type a number not string");
                valid = false;
                data.clear();
                break;
            }
            if(!ConsoleHelper::CheckValidTableindex(input, std::stoi(index),IndexFromZero))
            {
                ConsoleHelper::PrintErrorMessage("you should type a number from the printed table");
                valid = false;
                data.clear();
                break;
            }

            auto retIter = input.find(std::stoi(index));
            if (input.end() != retIter)
            {
                data.push_back(retIter->second);
            }

        }
        if (valid)
        {
            break;
        }
    }
    return data;
}
//**********************************************************************************
std::string ConsoleHelper::GetFileNameFromCommandLine(const std::string& type, const std::string& errormessage, const std::string& cmdarg)
{
    size_t found = cmdarg.find(type);
    if (found == std::string::npos)
    {
        std::ostringstream fmtMsg;
        fmtMsg << errormessage << std::endl;
        throw std::invalid_argument(fmtMsg.str());
    }
    std::string fileName = cmdarg;
    fileName = fileName.substr(found + type.size());
    return fileName;
}
//**********************************************************************************
std::string StringHelper::ReadDataBetweenTags(const std::string& line, const std::string& opentag, const std::string& closetag)
{
    std::string data;
    size_t foundCycle = line.find(opentag);
    if (std::string::npos != foundCycle)
    {

        data = line.substr(foundCycle + opentag.size());
        size_t foundEnd = data.find(closetag);
        if (std::string::npos != foundEnd)
        {
            data = data.substr(0, foundEnd);
        }
        else
        {
            ConsoleHelper::PrintErrorMessage("didnt found the name");
        }
    }
    return data;
}
//**********************************************************************************
std::vector<int> Helpers::LineToIntVector(const std::string& line)
{
    std::vector<std::string> cyclesstring = StringHelper::TokenizeString(line, Comma);
    std::vector<int> cyclesvec(cyclesstring.size());
    std::transform(cyclesstring.begin(), cyclesstring.end(), cyclesvec.begin(), [](const std::string& x)
        {
            return std::stoi(x.c_str());
        });
    return cyclesvec;
}
//**********************************************************************************
bool StringHelper::CheckValidNames(const std::string& name)
{
    if (isdigit(name.at(0)))
    {
        ConsoleHelper::PrintErrorMessage("name should begin with alphabet Not digit ");
        return false;
    }
    size_t found = name.find("-");
    std::string first, second;
    if (std::string::npos != found)
    {
        first = name.substr(0, found);
        second = name.substr(found + 1);
        if (StringHelper::AllDigitsOrAlphabet(first) && StringHelper::AllDigitsOrAlphabet(second))
        {
            return true;
        }
    }
    else if (islower(name.at(0)))
    {
        if (ConsoleHelper::YesNoQuestion("Are you sure that name begins with lowercase letter ?[Y/N] ") && StringHelper::AllDigitsOrAlphabet(name))
        {
            return true;
        }
    }
    else if (StringHelper::AllDigitsOrAlphabet(name))
    {
        return true;
    }
    else
    {
        ConsoleHelper::PrintErrorMessage("name can have only alphabetes ,digits ,Underscore ");
    }
    return false;
}
std::vector<std::string>  ConsoleHelper::GetNameWithMinus(const std::string& askmsg)
{
    std::string names;
    std::vector<std::string> namesvec;
    while (1)
    {
        bool valid = true;;
        std::cout << askmsg;
        std::cin >> names;
         namesvec = StringHelper::TokenizeString(names, Comma);
        
         for (const auto& name : namesvec)
        {
            if (!StringHelper::CheckValidNames(name))
            {
                valid = false;
                break;
            }
        }
        if(valid)
        {
            break;
        }
    }
    ConsoleHelper::PrintLines('-', LineLength);
    return namesvec;
}
//**********************************************************************************
bool Helpers::CheckValidityAndInsert(std::set<int>& numbers, const std::string& substr, const int maxnumber)
{
    std::pair<int, int>  interval = Helpers::CheckValidNumberOrInterval(substr, maxnumber);
    if (-1 == interval.first)
    {
        return false;
    }
    int currindex = interval.first;
    while (currindex <= interval.second)
    {
        if (numbers.count(currindex) > 0)
        {
            ConsoleHelper::PrintErrorMessage("Invalid Interval,The interval Intersect");
            return false;
        }
        numbers.insert(currindex);
        ++currindex;
    }
    return true;
}
//**********************************************************************************
std::set<int> ConsoleHelper::GetNumberswithIntervals(const std::string& askmsg ,const int maxVAL)
{
    std::string strNums, substr;
    std::set<int> numbers;
    while (true)
    {
        std::cout << askmsg;
        std::cin >> strNums;
        std::stringstream ss(strNums);
        bool isValid = true;
        while (std::getline(ss, substr, Comma))
        {
            if (!Helpers::CheckValidityAndInsert(numbers, substr, maxVAL))
            {
                numbers.clear();
                isValid = false;  // Mark as invalid if any part is invalid
                break;
            }
        }
        if (isValid)
        {
            break;
        }
    }
    ConsoleHelper::PrintLines('-', LineLength);
    return numbers;


}


std::vector<int> ConsoleHelper::GetNumbersFromConsole(const std::string& askmsg, const int maxVAL)
{
    std::string strNums, substr;
    std::vector<int> numbers;
    while (true)
    {
        std::cout << askmsg;
        std::cin >> strNums;
        std::stringstream ss(strNums);
        bool isValid = true;
        while (std::getline(ss, substr, Comma))
        {
            if (substr.empty())
            {
                std::cout << "you have extra comma" << std::endl;
                numbers.clear();
                isValid = false;  // Mark as invalid if any part is invalid
                break;
            }
            if (!std::all_of(substr.begin(), substr.end(), ::isdigit))
            {
                std::string tmp = substr.substr(1);
                if ('-' == substr.at(0) && std::all_of(tmp.begin(), tmp.end(), ::isdigit))
                {

                }
                else
                {
                    std::cout << "should enter Numbers with/without minus" << std::endl;
                    numbers.clear();
                    isValid = false;  // Mark as invalid if any part is invalid
                    break;
                }

            }
            numbers.push_back(std::stoi(substr));
        }
        if (isValid)
        {
            break;
        }
    }
    ConsoleHelper::PrintLines('-', LineLength);
    return numbers;
}

//**********************************************************************************
void FileHelper::SearchForPmflInVectorsFolder(const std::string& pmflFileName, const std::string& vectorsFolderPath,
    std::string& currPath, std::map<std::string, std::string>& burstAndItsFolder,std::set<std::string>& BurstsToSearch,const bool needAllpatterns)
{
    if (std::string::npos != pmflFileName.find("pmfl_old"))
    {
        return;
    }
    std::cout << " the pmfl file Name : " << pmflFileName << std::endl;
    std::ifstream file{ vectorsFolderPath + "/" + pmflFileName };
    if (!file)
    {
        throw std::runtime_error("Failed to open the Pmfl file : " + pmflFileName);
        return;
    }
    std::string line, Name;
    while (getline(file, line))
    {
        const std::string searchTagforPath = "../vectors/";
        const std::string searchTagforInclude = "#include";
        size_t foundPath = line.find(searchTagforPath);
        size_t foundFileName = line.find_first_of(".");
        size_t foundBurstAndPort = line.find("@");
        size_t foundInclude = line.find(searchTagforInclude);
        std::string comment = line.substr(0, 2);
        if ("--" == comment)
        {
            continue;
        }
        std::string Name;
        if (BurstsToSearch.empty()&&!needAllpatterns)
        {
            return;
        }
        if (std::string::npos != foundPath)
        {
            currPath = line.substr(foundPath);
            currPath = currPath.substr(searchTagforPath.size());
            StringHelper::GetSuitablePath(currPath);
        }
        std::string originalName = line;
        originalName.erase(remove_if(originalName.begin(), originalName.end(), isspace), originalName.end());
        if (BurstsToSearch.count(originalName) > 0)
        {
            burstAndItsFolder[originalName] = currPath;
            BurstsToSearch.erase(originalName);
        }
        else if (std::string::npos != foundInclude)
        {
            Name = line.substr(foundInclude);
            Name = Name.substr(searchTagforInclude.size());
            Name.erase(remove_if(Name.begin(), Name.end(), isspace), Name.end());
            //does the file will be closed 
            SearchForPmflInVectorsFolder(Name, vectorsFolderPath, currPath, burstAndItsFolder, BurstsToSearch,needAllpatterns);
        }
        else if (std::string::npos != foundFileName)
        {
            if (needAllpatterns)
            {
                Name = line;
                Name.erase(remove_if(Name.begin(), Name.end(), isspace), Name.end());
                burstAndItsFolder[Name] = currPath;
            }
            else
            {
                Name = line.substr(0, foundFileName);
                Name.erase(remove_if(Name.begin(), Name.end(), isspace), Name.end());
                if (BurstsToSearch.count(Name) > 0)
                {
                    burstAndItsFolder[Name] = currPath;
                    BurstsToSearch.erase(Name);
                }
            }

        }
        else if (std::string::npos != foundBurstAndPort)
        {
            Name = line;
            Name.erase(remove_if(Name.begin(), Name.end(), isspace), Name.end());
            if (needAllpatterns)
            {
                burstAndItsFolder[Name] = currPath;

            }
            else
            {
                Name = line.substr(0, foundBurstAndPort);
                Name.erase(remove_if(Name.begin(), Name.end(), isspace), Name.end());
                if (BurstsToSearch.count(Name) > 0)
                {
                    burstAndItsFolder[Name] = currPath;
                    BurstsToSearch.erase(Name);
                }
            }
        }
        else
        {
            Name = line;
            Name.erase(remove_if(Name.begin(), Name.end(), isspace), Name.end());
            if (BurstsToSearch.count(Name) > 0)
            {
                burstAndItsFolder[Name] = currPath;
                BurstsToSearch.erase(Name);
            }
            else if (needAllpatterns)
            {
                burstAndItsFolder[Name] = currPath;
            }
        }
    }
}
void FileHelper::GetBurstsPath(const std::string& pmflFileName, 
                               const std::string& vectorsFolderPath, 
                               std::map<std::string, std::string>& burstAndItsFolder, 
                               std::set<std::string>& BurstsToSearch,const bool needAllpatterns)
{
    std::string currPath;
   
    if (!FileSystem::IsExist(vectorsFolderPath))
    {
        throw std::runtime_error("PATH NOT VALID: " + vectorsFolderPath);
        return;
    }

    SearchForPmflInVectorsFolder(pmflFileName, 
                                 vectorsFolderPath, 
                                 currPath, 
                                 burstAndItsFolder, 
                                 BurstsToSearch, needAllpatterns);
}

static bool CheckValidEntryAndInsertPatterns(const std::map<std::string, std::set<std::string>>& folderAndTheBurstsInside,
                                             const FileSystem::DirEntry& entry,
                                             std::map<std::string, std::vector<PatternData>>& burstAndItsPatterns,
                                             std::vector<DataToUzipFile>& dataforZippedFiles)
{
    std::vector<PatternData> patternsData;
    std::string fileName;

    std::string folderName = entry.m_fileName;
    size_t foundZippedFile = entry.m_fileName.find(".binl.gz");
    size_t foundBurstFile = entry.m_fileName.find(".burst");
    size_t foundBurstAndPort = entry.m_fileName.find("@");
    std::string key = entry.m_parentPath;

    key = key.substr(key.find("vectors"));
    std::cout << key << std::endl;
    if (folderAndTheBurstsInside.end() == folderAndTheBurstsInside.find(key))
    {
        return false;
    }

    std::set<std::string> bursts = folderAndTheBurstsInside.find(key)->second;
    // std::cout << "found folder " << key <<  std::endl;
    if (std::string::npos != foundZippedFile)
    {
        fileName = folderName.substr(0, foundZippedFile);
        //do find and then get it with itaror
        if (bursts.count(fileName))
        {
            /* DataToUzipFile data;
             data.path= entry.path().string();
             data.parent_path = entry.path().parent_path().string();
             data.fileName = fileName;
             dataforZippedFiles.push_back(data);
             */
            std::cout << "found zipped file" << std::endl;
            std::string path = entry.m_fullPath;
            const std::string  zippedfile = entry.m_parentPath + "/" + fileName + ".binl";
            std::string unzippedfilePath = FileHelper::unzipAndRead(path, entry.m_parentPath, fileName + ".binl");
            StringHelper::GetSuitablePath(unzippedfilePath);
            std::cout << unzippedfilePath << std::endl;
            patternsData = FileHelper::ReadBurstFile(unzippedfilePath);
            burstAndItsPatterns[fileName].insert(burstAndItsPatterns[fileName].begin(), patternsData.begin(), patternsData.end());
            for (auto patt :patternsData)
            {
                std::cout << patt.patternName << std::endl;
            }
            std::remove(unzippedfilePath.c_str());
            return true;

        }
    }
    else if (std::string::npos != foundBurstFile)
    {
        // std::cout << "found burst" << std::endl;
        fileName = folderName.substr(0, foundBurstFile);
        if (bursts.count(fileName))
        {
            std::string path = entry.m_fullPath;
            StringHelper::GetSuitablePath(path);
            patternsData = FileHelper::ReadBurstFile(path);
            burstAndItsPatterns[fileName].insert(burstAndItsPatterns[fileName].begin(), patternsData.begin(), patternsData.end());
            return true;

        }
    }
    else if (std::string::npos != foundBurstAndPort)
    {
        // std::cout << "found burst and port" << std::endl;
        fileName = folderName.substr(0, foundBurstAndPort);
        if (bursts.count(fileName))
        {
            //std::cout << " 33333333333" << std::endl;
            std::string path = entry.m_fullPath;
            StringHelper::GetSuitablePath(path);
            patternsData = FileHelper::ReadBurstFile(path);
            burstAndItsPatterns[fileName].insert(burstAndItsPatterns[fileName].begin(), patternsData.begin(), patternsData.end());
            return true;
        }
    }
    return false;
}

static void IterateOverBurstFolder(const std::string& testPath,
                                   const std::map<std::string, std::set<std::string>>& folderAndTheBurstsInside,
                                   const std::string& pathtoburstfolder,
                                   std::map<std::string, std::vector<PatternData>>& burstAndItsPatterns,
                                   std::vector<DataToUzipFile>& dataforZippedFiles)
{
    const std::set<std::string> filters{ ".cache" , "old" };
     std::string burstFolderPath = testPath + "/" + pathtoburstfolder;
     StringHelper::GetSuitablePath(burstFolderPath);
     std::cout << "burst folder path: " << burstFolderPath << std::endl;
     auto entryFiles = FileSystem::GetFilesDataRecursively(burstFolderPath, filters);
    /* for (auto dir_entry = fs::recursive_directory_iterator(testPath + "/" + pathtoburstfolder);
        dir_entry != fs::recursive_directory_iterator();
        ++dir_entry) */
    for(auto& dir_entry : entryFiles)
    {
        std::string path = dir_entry.m_fullPath;
        StringHelper::GetSuitablePath(path);
        std::string parent_path= dir_entry.m_parentPath;
        StringHelper::GetSuitablePath(parent_path);
      //  dir_entry.m_fullPath = path;
        FileSystem::DirEntry tmp;
        tmp.m_fullPath = path;
        tmp.m_parentPath = parent_path;
        tmp.m_inode = dir_entry.m_inode;
        tmp.m_fileName = dir_entry.m_fileName;
        std::cout << "Searching in entry: " << path << std::endl;
        ::CheckValidEntryAndInsertPatterns(folderAndTheBurstsInside, tmp, burstAndItsPatterns, dataforZippedFiles);
    }
}

void Helpers::GetPatternsFromBurstFolder(const std::string& pathToVecFolder, 
                                         std::map<std::string, std::string>& burstAndItsFolder, 
                                         std::map<std::string, std::vector<PatternData>>& burstAndItsPatterns)
{
    std::map<std::string, std::set<std::string>> folderAndTheBurstsInside;

    for (const auto& burst : burstAndItsFolder)
    {
        std::string pathtoburstfolder = "vectors/" + burst.second;
        StringHelper::GetSuitablePath(pathtoburstfolder);
        folderAndTheBurstsInside[pathtoburstfolder].insert(burst.first);
        std::cout << "path from pmfl" << pathtoburstfolder << std::endl;
        //SearchForBurstInFolder(burst.first, pathtoburstfolder);
    }

    std::vector<DataToUzipFile> dataforZippedFiles;
    for (auto folder : folderAndTheBurstsInside)
    {
         ::IterateOverBurstFolder(pathToVecFolder,folderAndTheBurstsInside, folder.first,burstAndItsPatterns, dataforZippedFiles);
    }
    /*
    std::set<std::string> unzippedFiles;
    unsigned int n = std::thread::hardware_concurrency();
    const size_t iterationPerThread = dataforZippedFiles.size() / n;
    std::cout << "iterations "<<iterationPerThread << std::endl;
    std::cout << "number of zipped " << dataforZippedFiles.size() << std::endl;
    if (!iterationPerThread)
    {
        Helpers::UnzipWithThread(burstAndItsPatterns, dataforZippedFiles, dataforZippedFiles.size(), 0,unzippedFiles);
    }
    else
    {
        std::vector<std::thread> threads;
        for (unsigned int i = 0; i < n; i++)
        {
        //    std::thread th(Helpers::UnzipWithThread,burstAndItsPatterns, dataforZippedFiles, iterationPerThread, i * iterationPerThread, unzippedFiles);
          //  threads.push_back(std::move(th));
        }
        for (int i = 0; i < n; i++)
        {
            threads.at(i).join();
        }
        if (dataforZippedFiles.size() % n)
        {
            std::cout << "55555" << std::endl;
            Helpers::UnzipWithThread(burstAndItsPatterns, dataforZippedFiles, dataforZippedFiles.size() - iterationPerThread * n, iterationPerThread * n, unzippedFiles);
        }


    }
    for (auto file : unzippedFiles)
    {
        std::remove(file.c_str());
    }
    std::cout << unzippedFiles.size() << std::endl;
    //deal with zero iterationPerThread */
    }
/*
void FileHelper::UnzipWithThread(std::map<std::string, std::vector<patternData>>& burstAndItsPatterns,const std::vector<DataToUzipFile>& dataforZippedFiles, const int iterationPerThread,const int intialIndex,std::set<std::string>& UnzippedFiles)
{
    for (int i = intialIndex; i < iterationPerThread + intialIndex; ++i)
    {
        std::string fileName = dataforZippedFiles[i].fileName;
        std::vector<PatternData> patternData;
        FileHelper::unzipAndRead(dataforZippedFiles[i].path,dataforZippedFiles[i].parent_path,fileName);
        std::string unzippedfile = dataforZippedFiles[i].path.substr(0,dataforZippedFiles[i].path.find_first_of(".")) + ".binl";
        std::replace_if(unzippedfile.begin(), unzippedfile.end(), [](char ch)
            {
                return '\\' == ch;
            }, '/');
        patternData = FileHelper::ReadBurstFile(unzippedfile);
        UnzippedFiles.insert(unzippedfile);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(2));
        std::remove(unzippedfile.c_str());
        burstAndItsPatterns[fileName].insert(burstAndItsPatterns[fileName].begin(), patternData.begin(), patternData.end());
    }
}*/

std::string FileHelper::unzipAndRead(const std::string& gzFilePath, 
                                     const std::string& parentpath, 
                                     const std::string& fileName)
{
    // Ensure paths are in the correct format
    std::string unzipcommand;

#ifdef _WIN32
    std::string mgzFilePath = FileSystem::MakePreferred(gzFilePath);
    std::string mparentpath = FileSystem::MakePreferred(parentpath);
    const std::string currentPath = FileSystem::GetCurrentPath();
    const std::string pathtoFolder = ConsoleHelper::CreateFolderToContainResults(currentPath, "unzippedFiles");
    const std::string setPathFor7zCmd = "powershell -Command \"$env:Path +=';C:\\Program Files\\7-Zip'\"";
    //system return zero if it succeseded
    if (system(setPathFor7zCmd.c_str()))
    {
        std::cout << "[ERROR] Cant Update path for 7z tool" << std::endl;
    }

    //std::filesystem::path mgzFilePath = "C:\\dev\\testburstfolder\\mm\\MPB_SCAN_HBM3_INT_AC_MCPB_FT_SSN_END_DUAL.binl.gz";
    //std::filesystem::path mparentpath = "C:\\dev\\testburstfolder\\mm";
    // Use PowerShell with proper quoting
      // unzipcommand = "powershell -Command \"& \\\"C:\\Program Files\\7-Zip\\7z.exe\\\" x \\\"" +
       // mgzFilePath.string() + "\\\" -o\\\"" + mparentpath.string() + "\\\"\"";
         unzipcommand = "powershell -Command \"& \\\"C:\\Program Files\\7-Zip\\7z.exe\\\" x \\\"" +
          mgzFilePath + "\\\" -o\\\"" + pathtoFolder + "\\\"\"";
     int result = system(unzipcommand.c_str());
     if (result != 0)
     {
         std::cerr << "[ERROR] unzipping file: " << gzFilePath << std::endl;
         return "";
     }

     // return mparentpath.string() + fileName;
     return pathtoFolder +"\\" + fileName;

#elif __linux__
    std::string mgzFilePath = gzFilePath;
    std::string mparentpath = parentpath;
   // unzipcommand = "7z x \"" + mgzFilePath + "\" -o\"" + mparentpath.string() + "\"";
    std::string pathtoFolder=ConsoleHelper::CreateFolderToContainResults(FileSystem::GetCurrentPath(), "unzippedFiles");
    std::ofstream unzipped(pathtoFolder + "/" + fileName);
    if (!unzipped)
    {
        throw std::runtime_error("Failed to create a new " + fileName);
        return "";
    }
    unzipcommand = "gunzip -c " + mgzFilePath + " > /" + pathtoFolder + "/" + fileName;
    int result = system(unzipcommand.c_str());
    if (result != 0) {
        std::cerr << "[ERROR] unzipping file: " << gzFilePath << std::endl;
        return "";
    }
    return pathtoFolder + "/" + fileName;
#endif
    // TODO(Anton) - consider to use exceptions.
    return "";
}
std::vector<PatternData> FileHelper::ReadBurstFile(const std::string& burstFile)
{
    std::ifstream file{ burstFile };
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open the burst file : " + burstFile);
        return std::vector<PatternData>();
    }
    std::vector<PatternData> patternsData;
    std::string line, word;
    bool itsPatternFile = false;
    while (getline(file, line))
    {
        size_t foundPattern = line.find("CALL,,");
        size_t foundPatternFile = line.find("SQLA LBL,\"");
        size_t foundPortName = line.find("\",,(");
        std::string patternName;
        std::string portName;
        if (std::string::npos != foundPattern)
        {
            patternName = StringHelper::ReadDataBetweenTags(line, "CALL,,\"", "\"");
            if (std::string::npos != foundPortName)
            {
                portName = StringHelper::ReadDataBetweenTags(line, "\",,(", ")");
            }
        }
        if (std::string::npos != foundPatternFile)
        {
            patternName = StringHelper::ReadDataBetweenTags(line, "SQLA LBL,\"", "\",");
            std::cout << "patt "<<patternName << std::endl;
            if (std::string::npos != foundPortName)
            {
                portName = StringHelper::ReadDataBetweenTags(line, "\",(", ")");
            }
            itsPatternFile = true;
        }
        //checking if the pattern have one of the wanted ports
        /*if (!DESIREDPORTS.count(portName))
        {
            patternName = std::string();
        }*/
        if (!patternName.empty())
        {
            PatternData data;
            data.patternName = patternName;
            data.portName = portName;
            data.revNumber = StringHelper::GetRevNumber(patternName,"_rev");
            if (std::string::npos != patternName.find("_Die0") || 
                std::string::npos != patternName.find("_D0") || 
                std::string::npos != patternName.find("_DIE0"))
            {
                data.dieNumber = "DIE0";
                patternsData.push_back(data);
            }
            else if (std::string::npos != patternName.find("_Die1") || std::string::npos != patternName.find("_D1") || std::string::npos != patternName.find("_DIE1"))
            {
                data.dieNumber = "DIE1";
                patternsData.push_back(data);
            }
            //because PATTERN NAME IN PATTERN FILE appears once
            if (itsPatternFile)
            {
                break;
            }
        }
    }
    file.close();
    return patternsData;
}


std::string Helpers::GetTestSuiteOrPatternMission(const std::string& patternOrTestSuite)
{
    for (const auto mission : TESTSUITE_MISSIONS)
    {
        std::string tmp = mission;
        if (std::string::npos != patternOrTestSuite.find(mission))
        {
            return mission;
        }
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
        if (std::string::npos != patternOrTestSuite.find(tmp))
        {
            return mission;
        }
    }

    // TODO(Anton) - Consider to use exceptions.
    return std::string();
}


void Helpers::GetPorts(const std::string& fileName)
{
    std::ifstream file{ fileName };
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open theport file ");
        return;
    }

    std::ofstream ports("AllPorts.txt");
    if (!ports)
    {
        throw std::runtime_error("Failed to create a new ports File");
        return;
    }
    std::stringstream ss;
    std::string line, port;
    bool itsPatternFile = false;
    while (getline(file, line))
    {
        size_t foundPort = line.find("),(");
        if (std::string::npos != foundPort)
        {
            port = StringHelper::ReadDataBetweenTags(line, "),(", ")");
            ss << port << std::endl;
        }
    }
    ports << ss.str();
}


std::vector<int> Helpers::ConvertDecimalToBinary(const int val)
{
    std::vector<int> NumberBits;
    int value = val;
    while (value)
    {
        NumberBits.push_back(value % 2);
        value /= 2;
    }
    return NumberBits;
}


std::string StringHelper::GetRevNumber(const std::string& pattern,const std::string& revstr)
{
    // TODO(Anton) - Consider instead of hardcoding _rev use some const std::string in a designated location.
    const size_t foundRev = pattern.find(revstr);
    if (std::string::npos != foundRev)
    {
        std::string revNumber = pattern.substr(foundRev + 4);
        revNumber = revNumber.substr(0, revNumber.find_first_of("_"));
        return revNumber;
    }
    return std::string();
}


void StringHelper::GetSuitablePath(std::string& path)
{

#ifdef _WIN32
    std::replace_if(path.begin(), path.end(), [](char ch)
        {
            return '/' == ch;
        }, '\\');
#else
    std::replace_if(path.begin(), path.end(), [](char ch)
        {
            return '\\' == ch;
        }, '/');
#endif
}

double Helpers::UpdateValueAccordingToUnit(const std::string& strval, const std::string& unit)
{
    double val;
   // std::cout <<"str val : "<< strval << std::endl;
    std::string res = strval;
    if (unit.empty() && std::string::npos != strval.find("m"))
    {
        res = res.substr(0, res.find("m"));
        val = std::stod(res);
        val *= std::pow(10, -3);
    }
    else if (unit.empty() && std::string::npos != strval.find("u"))
    {
        res = res.substr(0, res.find("u"));
        val = std::stod(res);
        val *= std::pow(10, -6);
    }
    else if (unit.empty() && std::string::npos != strval.find("P"))
    {
        res = res.substr(0, res.find("P"));
        val = std::stod(res);
        val *= std::pow(10, -12);
    }
    else if (unit.empty() && std::string::npos != strval.find("M"))
    {
        res = res.substr(0, res.find("M"));
        val = std::stod(res);
        val *= std::pow(10, 6);
    }
    else if(!strval.empty())
    {
        val = std::stod(res);
    }

    if (!unit.empty() && std::string::npos != unit.find("m"))
    {
        val *= std::pow(10, -3);
    }
    else if (!unit.empty() && std::string::npos != unit.find("u"))
    {
        val *= std::pow(10, -6);
    }
    else if (!unit.empty() && std::string::npos != unit.find("P"))
    {
        val *= std::pow(10, -12);
    }
    else if (!unit.empty() && std::string::npos != unit.find("M"))
    {
        val *= std::pow(10, 6);
    }
 //   std::cout << "double val " << val << std::endl;
    return val;
}
bool Helpers::CalcTheStrExpressionWithOneOperator(const std::string& exprstr,double& res)
{
    std::string expr = exprstr;
    expr.erase(remove_if(expr.begin(), expr.end(), isspace), expr.end());
    if (std::any_of(exprstr.begin(), exprstr.end(), isalpha))
    {
        return false;
    }
    std::vector<std::string> expVec;
    if (std::string::npos != expr.find("*"))
    {
        expVec = StringHelper::TokenizeString(expr, '*');
        res = std::stod(expVec.at(0)) * std::stod(expVec.at(1));
    }
    else if (std::string::npos != expr.find("-"))
    {
        expVec = StringHelper::TokenizeString(expr, '-');
        res = std::stod(expVec.at(0)) - std::stod(expVec.at(1));
    }
    else if (std::string::npos != expr.find("+"))
    {
        expVec = StringHelper::TokenizeString(expr, '+');
        res = std::stod(expVec.at(0)) + std::stod(expVec.at(1));
    }
    else
    {
        return false;
    }
    return true;
}
/*
double Helpers::CalcTheStringExpression(const std::string& expression)
{
    //not complited
    std::stack<double> st;
    int index = 0;
    double num = 0;
    std::string numStr;
    while (index != expression.size())
    {
        if ('+' == expression[index] || '-' == expression[index] ||
            '/' == expression[index] || '*' == expression[index])
        {
            double a, b, c;
            a = st.top();
            st.pop();
            b = st.top();
            st.pop();
            if ('+' == expression[index])
            {
                c = a + b;
            }
            else if ('*' == expression[index])
            {
                c = a * b;
            }
            else if ('-' == expression[index])
            {
                c = b - a;
            }
            else
            {
                c = b / a;
            }

            st.push(c);
        }
        else
        {
            numStr += expression.at(index);
        }


    }




}*/
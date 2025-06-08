#include "FlowIndicator.h"
#include <string>



void ProgramManager::Start(const int m_argc, const char** m_argv)
{
    if (2 == m_argc && "--help" == std::string(m_argv[1]))
    {
        //PrintHowToOperate();
        return;
    }
    std::string prog;
    if (2 == m_argc)
    {
        FlowIndicatorPlistPreCheck stage1;
        const std::string argv1 = m_argv[1];
        if (std::string::npos != argv1.find("-PLIST:"))
        {
            stage1.RunflwIndPlistStage(m_argc, argv1);
        }
    }
}

void FlowIndicatorPlistPreCheck::RunflwIndPlistStage(const int& argc, const std::string& fileName)
{

    std::string pListPath;
    const std::string askForFilemsg = "Enter name of Plist file like this: -PLIST:plist_example.plist";
    //change name it not only for files //put it in string helper
    pListPath = ConsoleHelper::GetFileNameFromCommandLine(std::string("-PLIST:"), askForFilemsg, fileName);
  //  PListParser(pListPath);
}

void FlowIndicatorPlistPreCheck::PListParser(const std::string& pListPath)
{
    std::ifstream file(pListPath); // Replace with your actual file name
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << pListPath << std::endl;
        return;
    }
    while (std::getline(file, line)) {
        size_t pos = line.find("Pat");
        if (pos != std::string::npos) {
            // Find the start of the name after "Pat"
            size_t start = line.find_first_not_of(" \t", pos + 3);
            if (start != std::string::npos) {
                // Find the end of the name using space as delimiter
                size_t end = line.find_first_of(" \t;", start);
                std::string name = line.substr(start, end - start);

                // Remove semicolon at the end if present
                if (!name.empty() && name.back() == ';') {
                    name.pop_back();
                }

                m_patNames.push_back(name);
            }
        }
    }
    file.close();
    // Output the names
    std::cout << "Extracted Pat names:" << std::endl;
    for (const auto& name : m_patNames) {
        std::cout << name << std::endl;
    }
}
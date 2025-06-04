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
        std::string argv1 = m_argv[1];
        if (std::string::npos != argv1.find("-PLIST:"))
        {
            stage.RunflwIndPlistStage(m_argc, m_argv)
        }
    }
}
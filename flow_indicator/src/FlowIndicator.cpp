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
        std::string argv1 = m_argv[1];
        if (std::string::npos != argv1.find("-PLIST:"))
        {
            stage1.RunflwIndPlistStage(m_argc, argv1);
        }
    }
}

void FlowIndicatorPlistPreCheck::RunflwIndPlistStage(const int& argc,const std::string& argv)
{


const std::string askForFlowFilemsg = "Enter name of flow file like this: -FLOW:flow_example.flw";
//change name it not only for files //put it in string helper
flowFile = ConsoleHelper::GetFileNameFromCommandLine("-FLOW:", askForFlowFilemsg, argv[1]);


]
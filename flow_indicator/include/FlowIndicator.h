#ifndef FLOW_INDICATOR_H_
#define FLOW_INDICATOR_H_

#include "Helper.h"
#include <vector>
class ProgramManager
{
public:
    ProgramManager() = default;
    ProgramManager(const ProgramManager& rhs) = delete;
    ProgramManager& operator=(const ProgramManager& rhs) = delete;

    void Start(const int argc, const char** argv);
  //  void PrintHowToOperate();
};


class FlowIndicatorPlistPreCheck
{
public:
    void RunflwIndPlistStage(const int& argc,const std::string& fileName);
    void PListParser(const std::string& pListPath);
private:
    std::vector<std::string> m_patNames;

};

#endif
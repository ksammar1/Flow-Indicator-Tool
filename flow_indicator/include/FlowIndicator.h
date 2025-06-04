#ifndef FLOW_INDICATOR_H_
#define FLOW_INDICATOR_H_

class ProgramManager
{
public:
    ProgramManager() = default;
    ProgramManager(const ProgramManager& rhs) = delete;
    ProgramManager& operator=(const ProgramManager& rhs) = delete;

    void Start(const int argc, const char** argv);
    void PrintHowToOperate();
};


class FlowIndicatorPlistPreCheck
{
public:



};

#endif
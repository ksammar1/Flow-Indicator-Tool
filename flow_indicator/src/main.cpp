#include <iostream>
#include <FlowIndicator.h>



int main(const int argc, const char** argv)
{
    try
    {
        ProgramManager prog;
        prog.Start(argc, argv);
        return 0;
    }
    catch (const std::exception& exc)
    {
        std::cout << exc.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        return 2;
    }
}
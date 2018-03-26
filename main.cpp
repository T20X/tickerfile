#include <iostream>
#include <string.h>

#include "ITickerplant.h"
#include "ICommandFactory.h"
#include "CmdProcessor.h"

int main(int argc, char* argv[])
{
    CmdProcessor::OptimizeOptions o = CmdProcessor::OptimizePrint;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-OptimizePrint"))
            o = CmdProcessor::OptimizePrint;

        if (!strcmp(argv[i], "-OptimizeProduct"))
            o = CmdProcessor::OptimizeProduct;
    }

    try
    {
        CmdProcessor p(o);
        p.go();
    }
    catch (const std::exception& e)
    {
        std::cout << "Caught an exception in the main thread -> "
                  << e.what() << std::endl;
    }
    
    return 0;
}
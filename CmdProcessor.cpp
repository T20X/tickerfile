#include <iterator>
#include <sstream>
#include <iostream>
#include <vector>

#include "TickerplantPrintOptimize.h"
#include "TickerplantProductOptimize.h"
#include "CommandFactory.h"
#include "CmdProcessor.h"
#include "Utils.h"

null_out_stream cnul;       // test null stream.

/* --------------------------------------------------------------
 */
CmdProcessor::CmdProcessor(OptimizeOptions option)
{    
    switch (option)
    {
        case CmdProcessor::OptimizePrint:
            m_tickerplant.reset(new TickerplantPrintOptimize());
            std::cout << "OptimizePrint selected!" << std::endl;
            break;

        case CmdProcessor::OptimizeProduct:
            m_tickerplant.reset(new TickerplantProductOptimize());
            std::cout << "OptimizeProduct selected!" << std::endl;
            break;

        default:
            std::stringstream ss;
            ss << "Unknown optimize option -> " << static_cast<int>(option) << std::endl;
            throw std::runtime_error(ss.str());
    }

    m_cmdFactory.reset(new CommandFactory());
}

/* --------------------------------------------------------------
 */
void CmdProcessor::go()
{
    std::string input;

    do
    {
        input.clear();

        try
        {      
            if (!std::getline(std::cin, input))
                continue;
            
            std::vector<std::string> cmdParams = Utils::split(input, "\\s+");
            if (cmdParams.empty())
               continue;

            Command::Ptr c = m_cmdFactory->create(cmdParams);
            if (c)
            {
                c->setTickerplant(m_tickerplant.get());
                c->setCmdFactory(m_cmdFactory.get());
                c->execute(std::cout);
            }           
        }
        catch (const std::exception& e)
        {
            std::cout << "[Caught an exception while processing a command] -> " 
                      << e.what() << std::endl;
        }
    } 
    while (input != "exit");
}


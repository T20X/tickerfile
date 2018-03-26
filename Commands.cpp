#include <fstream>

#include "ITickerplant.h"
#include "ICommandFactory.h"
#include "Utils.h"
#include "Commands.h"

/* --------------------------------------------------------------
 */
void LoadTickefileCommand::execute(std::ostream& o)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    o << std::endl;

    std::ifstream file;
    file.exceptions(std::ifstream::badbit);

    try
    {   
        getTickerplant().clear();

        std::string line;
        file.open(filename, std::ios::in);
        while (std::getline(file, line))
        {
            if (file.bad())
                throw std::runtime_error("tickerfile became bad!");

            try
            {
                std::vector<std::string> cmdParams = Utils::split(line, ",");
                if (cmdParams.empty())
                    continue;

                Command::Ptr c = getCmdFactory().create(cmdParams);
                c->setTickerplant(&getTickerplant());
                c->execute(o);
            }
            catch (const std::exception& e)
            {
                o << "[Failed to process a record from tickerfile] - >" 
                  << e.what() << " [at line] ->" << line << "\n";
            }
        }

        getTickerplant().shrinkToFit();
    }
    catch (const std::exception& e)
    {
        throw;
    }

    if (file.is_open())
        file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    o << "Command took ->" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
      << " milliseconds to execute" << std::endl;
}
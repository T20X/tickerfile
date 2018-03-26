#include <vector>
#include <iterator>

#include "Utils.h"
#include "CommandFactory.h"

/* --------------------------------------------------------------
 */
class TickerfileCommandMaker
{
public:
    Command::Ptr operator()(const std::vector<std::string>& cmdParams)
    {
        if (cmdParams.at(0) != "tickerfile")
            return Command::Ptr();

        LoadTickefileCommand* c = new LoadTickefileCommand();
        Command::Ptr r(c);
        {
            c->filename = cmdParams.at(1);
        }

        return r;
    }
};

/* --------------------------------------------------------------
 */
class AddRecordCommandMaker
{
public:
    Command::Ptr operator()(const std::vector<std::string>& cmdParams)
    {
        AddRecordCommand* c = new AddRecordCommand();
        Command::Ptr r(c);
        {
            c->timestamp = Utils::strToTime(cmdParams.at(0));
            c->sym = cmdParams.at(1);

            size_t pos = 2;
            auto it = cmdParams.begin();
            std::advance(it, 2);

            while (it != cmdParams.end())
            {
                std::string fieldName = cmdParams.at(pos);
                std::string fieldValue = cmdParams.at(pos + 1);//keeps manipulations with it safe!

                c->fields.push_back({ fieldName, Utils::strToDouble(fieldValue) });

                std::advance(it, 2);
                pos += 2;
            }
        }

        return r;
    }
};

/* --------------------------------------------------------------
 */
class ParsePrintCommandMaker
{
public:
    Command::Ptr operator()(const std::vector<std::string>& cmdParams)
    {
        if (cmdParams.at(0) != "print")
            return Command::Ptr();

        PrintCommand* c = new PrintCommand();
        Command::Ptr r(c);
        {
            c->from = Utils::strToTime(cmdParams.at(1));
            c->to = Utils::strToTime(cmdParams.at(2));
            c->sym = cmdParams.at(3);
        }

        return r;
    }
};

/* --------------------------------------------------------------
 */
class ParseProductCommandMaker
{
public:
    Command::Ptr operator()(const std::vector<std::string>& cmdParams)
    {
        if (cmdParams.at(0) != "product")
            return Command::Ptr();

        ProductCommand* c = new ProductCommand();
        Command::Ptr r(c);
        {
            c->from = Utils::strToTime(cmdParams.at(1));
            c->to = Utils::strToTime(cmdParams.at(2));
            c->sym = cmdParams.at(3);
            c->field1 = cmdParams.at(4);
            c->field2 = cmdParams.at(5);
        }

        return r;
    }
};

/* --------------------------------------------------------------
*/
CommandFactory::CommandFactory()
{    
    m_makers.push_back(TickerfileCommandMaker());
    m_makers.push_back(ParsePrintCommandMaker());
    m_makers.push_back(ParseProductCommandMaker());
    m_makers.push_back(AddRecordCommandMaker());
}

/* --------------------------------------------------------------
 */
Command::Ptr CommandFactory::create(const std::vector<std::string>& cmdParams)
{
    Command::Ptr c;
 
    for (const auto& handler : m_makers)
    {
        c = handler(cmdParams);
        if (c)
        {
            return c;
        }
    }

    return c;
}


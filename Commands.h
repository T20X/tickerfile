#pragma once

#include <string>
#include <list>
#include <chrono>
#include <memory>
#include <iostream>

#include "ITickerplant.h"

/* --------------------------------------------------------------
 */
class ICommandFactory;
class Command
{
public:
    typedef std::shared_ptr<Command> Ptr;
    
    Command(ITickerplant* tp = nullptr):m_tickerplant(tp) {}
    virtual ~Command() {}

    void setTickerplant(ITickerplant* tp) { m_tickerplant = tp; }
    void setCmdFactory(ICommandFactory* cmdFactory) { m_cmdFactory = cmdFactory; }

    virtual void execute(std::ostream& o) = 0; 

protected: 
    ITickerplant& getTickerplant() 
    {
        if (!m_tickerplant)
            throw std::runtime_error("tickerplant not set!");

        return *m_tickerplant;  
    }

    ICommandFactory& getCmdFactory()
    {
        if (!m_tickerplant)
            throw std::runtime_error("Cmd Factory not set!");

        return *m_cmdFactory;  
    }

private:
     ITickerplant* m_tickerplant; // command reciever
     ICommandFactory* m_cmdFactory; // ability for commands to spwan another commands
};

/* --------------------------------------------------------------
 */
class LoadTickefileCommand : public Command
{
public:
    using Command::Command;
    std::string filename;

    void execute(std::ostream& o);
};

/* --------------------------------------------------------------
 */
class AddRecordCommand : public Command
{
public:
    using Command::Command;

    std::string sym;
    time_t timestamp;

    struct Field
    {
        std::string name;
        double value;
    };

    std::list<Field> fields;

    void execute(std::ostream& o)
    {
        getTickerplant().add(*this);
    }
};

/* --------------------------------------------------------------
 */
class PrintCommand : public Command
{
public:
    using Command::Command;

    std::string sym;
    time_t from;
    time_t to;

    void execute(std::ostream& o)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        o << std::endl;

        getTickerplant().print(*this, o);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        o << "Command took ->" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
          << " microseconds to execute" << std::endl;
    }
};

/* --------------------------------------------------------------
 */
class ProductCommand : public Command
{
public:
    using Command::Command;

    std::string sym;
    time_t from;
    time_t to;
    std::string field1;
    std::string field2;

    void execute(std::ostream& o)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        o << std::endl;

        getTickerplant().product(*this, o);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        o << "Command took ->" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
          << " microseconds to execute" << std::endl;
    }
};


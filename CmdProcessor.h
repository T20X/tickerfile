#pragma once

#include <memory>

class CmdProcessor
{
public:
    enum OptimizeOptions
    {
        OptimizePrint,
        OptimizeProduct
    };

    CmdProcessor(OptimizeOptions option);
    void go();

private:
    CmdProcessor(CmdProcessor&) = delete;
    CmdProcessor(CmdProcessor&&) = delete;
    CmdProcessor  operator=(CmdProcessor&) = delete;
    CmdProcessor  operator=(CmdProcessor&&) = delete;

private:
    std::unique_ptr<class ITickerplant> m_tickerplant;
    std::unique_ptr<class ICommandFactory> m_cmdFactory;


};

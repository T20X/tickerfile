#pragma once

#include <functional>
#include "ICommandFactory.h"

class CommandFactory : public ICommandFactory
{
public:
    CommandFactory();
    Command::Ptr create(const std::vector<std::string>& cmdParams);

private:
    std::vector<std::function<Command::Ptr(std::vector<std::string>)>> m_makers;
};

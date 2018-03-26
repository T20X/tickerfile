#pragma once

#include <vector>
#include "Commands.h"

class ICommandFactory
{
public:
    virtual Command::Ptr create(const std::vector<std::string>& cmdParams) = 0;
    virtual ~ICommandFactory() {}
};
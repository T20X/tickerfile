#pragma once

class AddRecordCommand;
class PrintCommand;
class ProductCommand;

class ITickerplant
{
public:    
    virtual void clear() = 0;
    virtual void shrinkToFit() = 0;

    virtual void add(const AddRecordCommand& a) = 0;
    virtual void print(const PrintCommand& a, std::ostream& o) = 0;
    virtual void product(const ProductCommand& a, std::ostream& o) = 0;

    virtual ~ITickerplant() {}
};
#pragma once

#include <string>
#include <map>
#include <vector>
#include <ostream>

#include "Commands.h"
#include "ITickerplant.h"

class TickerplantProductOptimize : public ITickerplant
{
public:
    void clear();
    void shrinkToFit();
    void add(const AddRecordCommand& a);        
    void print(const PrintCommand& a, std::ostream& o);
    void product(const ProductCommand& a, std::ostream& o);

private:

    struct DataPoint
    {
        time_t time;
        double value;
    };

    typedef std::vector<DataPoint> Timeseries;
    typedef std::map<std::string/*field*/, Timeseries> Table;
    typedef std::map<std::string/*sym*/, Table> Storage;

    Storage m_storage;

    friend bool operator< (const DataPoint& left, const DataPoint& right);    
};
#pragma once

#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <forward_list>

#include "Commands.h"
#include "ITickerplant.h"

/* For each symbol it maintains a vector of fields from tickerfile. Effiency is best achieved when the output
   is done through hifgh-performance I/O rather then just standart output.
*/
class TickerplantPrintOptimize : public ITickerplant
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
        std::string name;
        double value;
    };
        
    typedef std::vector<DataPoint> Timeseries;
    typedef std::map<std::string/*sym*/, Timeseries> Storage;

    Storage m_storage;

    friend bool operator< (const DataPoint& left, const DataPoint& right);
};
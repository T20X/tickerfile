#include <algorithm>
#include <iostream>

#include "TickerplantPrintOptimize.h"

/* --------------------------------------------------------------
 */
bool operator< (const TickerplantPrintOptimize::DataPoint& left, const TickerplantPrintOptimize::DataPoint& right)
{
    return left.time < right.time;
}

/* --------------------------------------------------------------
 */
void TickerplantPrintOptimize::clear()
{
    m_storage.clear();
}

/* --------------------------------------------------------------
 */
void TickerplantPrintOptimize::shrinkToFit()
{
    for (auto& timeseries : m_storage)
    {
        timeseries.second.shrink_to_fit();
    }
}

/* --------------------------------------------------------------
 */
void TickerplantPrintOptimize::add(const AddRecordCommand& cmd)
{
    Timeseries& t = m_storage[cmd.sym];

    DataPoint p;
    {        
        for (const auto& field : cmd.fields)
        { 
            t.push_back({ cmd.timestamp, field.name, field.value });
        }      
    }    
}

/* --------------------------------------------------------------
 */
void TickerplantPrintOptimize::print(const PrintCommand& cmd, std::ostream& o)
{   
    Timeseries& t = m_storage[cmd.sym];

    DataPoint p;
    p.time = cmd.from;

    Timeseries::iterator currentIt = std::lower_bound(
        t.begin(),
        t.end(),
        p);

    p.time = cmd.to;
    Timeseries::const_iterator upperBoundIt = std::upper_bound(
        t.begin(),
        t.end(),
        p);

    while (currentIt != upperBoundIt)
    {        
         o << currentIt->name << ":"
           << currentIt->value << "\n";
          
        currentIt++;
    }   
}

/* --------------------------------------------------------------
 */
void TickerplantPrintOptimize::product(const ProductCommand& cmd, std::ostream& o)
{
    Timeseries& t = m_storage[cmd.sym];

    DataPoint p;
    p.time = cmd.from;

    Timeseries::iterator currentIt = std::lower_bound(
        t.begin(),
        t.end(),
        p);

    p.time = cmd.to;
    Timeseries::const_iterator upperBoundIt = std::upper_bound(
        t.begin(),
        t.end(),
        p);

    double product = 0; 
    time_t prevTimestamp = 0;
    double field1Val = 0, field2Val = 0;
    while (currentIt != upperBoundIt)
    {   
        if (currentIt->time != prevTimestamp)
        {
            product += field1Val * field2Val;
            field1Val = 0;
            field2Val = 0;
        }

        if (currentIt->name == cmd.field1)
            field1Val = currentIt->value;

        if (currentIt->name == cmd.field2)
            field2Val = currentIt->value;

        prevTimestamp = currentIt->time;
        currentIt++;
    }

    product += field1Val * field2Val; 

    //overflows are not reported!
    o << product << "\n";
}

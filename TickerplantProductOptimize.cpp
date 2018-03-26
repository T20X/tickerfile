#include <algorithm>
#include <iostream>

#include "TickerplantProductOptimize.h"

/* --------------------------------------------------------------
 */
bool operator< (const TickerplantProductOptimize::DataPoint& left, const TickerplantProductOptimize::DataPoint& right)
{
    return left.time < right.time;
}

/* --------------------------------------------------------------
 */
void TickerplantProductOptimize::clear()
{
    m_storage.clear();
}

/* --------------------------------------------------------------
 */
void TickerplantProductOptimize::shrinkToFit()
{
    for (auto& table : m_storage)
    {        
        for (auto& timeseries : table.second)
        {
            timeseries.second.shrink_to_fit();
        }
    }
}

/* --------------------------------------------------------------
 */
void TickerplantProductOptimize::add(const AddRecordCommand& cmd)
{
    Table& t = m_storage[cmd.sym];
    for (auto const& field : cmd.fields)
    {
        Timeseries& ts = t[field.name];
        ts.push_back({ cmd.timestamp, field.value });        
    }
}

/* --------------------------------------------------------------
 */
void TickerplantProductOptimize::print(const PrintCommand& cmd, std::ostream& o)
{ 
    struct TimeseriesPrintTracker
    {
        Timeseries::iterator current;
        Timeseries::const_iterator upperBound;
        std::string field;
    };

    std::vector<TimeseriesPrintTracker> trackers;      

    Table& table = m_storage[cmd.sym];
    for (auto& timeseries : table)
    {  
        TimeseriesPrintTracker details;
        {
            details.field = timeseries.first;

            details.current = std::lower_bound(
                timeseries.second.begin(),
                timeseries.second.end(),
                DataPoint{ cmd.from, 0.0 });

            details.upperBound = std::upper_bound(
                timeseries.second.begin(),
                timeseries.second.end(),
                DataPoint{ cmd.to, 0.0 });

            trackers.push_back(details);
        }           
    }

    time_t currentTimestamp = 0; 
    do 
    {
        // re-evaluate the current timestamp
        currentTimestamp = cmd.to;
        for (std::vector<TimeseriesPrintTracker>::iterator tracker = trackers.begin();
            tracker != trackers.end();)
        {
            if (tracker->current == tracker->upperBound)
            {
                tracker = trackers.erase(tracker);
                continue;
            }
            else if (currentTimestamp > tracker->current->time)
            {
                currentTimestamp = tracker->current->time;
            }

            tracker++;
        }

        // print all fields and values for current timestamp
        for (auto& details : trackers)
        {
            if (details.current->time == currentTimestamp)
            {
                 o << details.field << ":"
                   << details.current->value << "\n";    

                details.current++;
            }            
        } 
    }
    while (currentTimestamp < cmd.to);    
}

/* --------------------------------------------------------------
 */
void TickerplantProductOptimize::product(const ProductCommand& cmd, std::ostream& o)
{
    Table& table = m_storage[cmd.sym];

    Timeseries& field1Timeseries = table.at(cmd.field1);

    Timeseries::iterator field1CurrentIt = std::lower_bound(
        field1Timeseries.begin(),
        field1Timeseries.end(),
        DataPoint{ cmd.from, 0.0 });
   
    Timeseries::const_iterator field1UpperBoundIt = std::upper_bound(
         field1Timeseries.begin(),
         field1Timeseries.end(),
         DataPoint{ cmd.to, 0.0 });

    Timeseries& field2Timeseries = table.at(cmd.field2);

    Timeseries::iterator field2CurrentIt = std::lower_bound(
        field2Timeseries.begin(),
        field2Timeseries.end(),
        DataPoint{ cmd.from, 0.0 });

    Timeseries::const_iterator field2UpperBoundIt = std::upper_bound(
        field2Timeseries.begin(),
        field2Timeseries.end(),
        DataPoint{ cmd.to, 0.0 });
    
    double product = 0.0;

    while (field1CurrentIt != field1UpperBoundIt &&
           field2CurrentIt != field2UpperBoundIt)
    {
        if (field1CurrentIt->time == field2CurrentIt->time)
        {
            product += field1CurrentIt->value *
                       field2CurrentIt->value;

            field1CurrentIt++;
            field2CurrentIt++;
        }
        else if (field1CurrentIt->time < field2CurrentIt->time)
        {
            field1CurrentIt++;
        }
        else if (field2CurrentIt->time < field1CurrentIt->time)
        {
            field2CurrentIt++;
        }
    }   

    o << product << "\n";
}

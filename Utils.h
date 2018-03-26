#pragma once

#include <vector>
#include <string>
#include <regex>

class Utils
{
public:
    static std::vector<std::string> split(const std::string& input, const std::string& regexPattern)
    {
        std::regex r(regexPattern);
        std::sregex_token_iterator first{ input.begin(), input.end(), r, -1 }, last;
        return{ first, last };
    }

    static time_t strToTime(const std::string& str)
    {
        unsigned long int val = strtoul(str.c_str(), 0, 10);
        if (val == 0 ||
            val >= ULONG_MAX ||
            val >= UINT_MAX)
        {
            throw std::runtime_error("invalid str to time_t conversion");
        }

        return (time_t)val;
    }

    static double strToDouble(const std::string& str)
    {
        char* strEnd;
        double val = strtod(str.c_str(), &strEnd);

        if (!val && str.c_str() != strEnd && *strEnd == '\0')
        {
            return val;
        }
        else if (isnormal(val))
        {
            return val;
        }
        else
        {
            throw std::runtime_error("invalid str to double conversion");
        }        
    }
};

class null_out_buf : public std::streambuf
{
public:
    virtual std::streamsize xsputn(const char * s, std::streamsize n)
    {
        return n;
    }

    virtual int overflow(int c)
    {
        return 1;
    }
};

class null_out_stream : public std::ostream
{
public:
    null_out_stream():std::ostream(&buf) {}

private:
    null_out_buf buf;
};

extern null_out_stream cnul;       // null stream.

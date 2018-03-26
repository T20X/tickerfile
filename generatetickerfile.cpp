#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>
#include <random>
#include <algorithm>
#include <set>

std::string generateRandomSymbol()
{
    int symInteger = rand() % 25;
    char sym = 65 + symInteger;
    return std::string({ (char)sym });
}

size_t generateRandomFieldNum()
{  
    size_t num = rand() % 7;
    return num + 1;
}

std::string generateRandomField(const std::set<std::string>& fieldsInUse)
{
    std::vector<std::string> fields = 
        { "bidPx",
          "askPx",
          "bidSize",
          "askSize",
          "tickId",
          "region"
          "lastTrade",
          "counterparty",
          "attempt"
        
        };

    for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end();)
    {
        if (fieldsInUse.find(*it) != fieldsInUse.end())
        {
            it = fields.erase(it);
        }
        else
        {
            it++;
        }
    }
        
    size_t pos = rand() % (fields.size() - 1);
    return fields.at(pos);
}

double generateRandomDouble()
{
    double min = 0.1;
    double max = 100;
    double r = (((double)rand() / (double)RAND_MAX) * (max - min)) + min;
    return r;
}

int main(int argc, char* argv[])
{
    size_t N = 0;
    if (argc == 2)
    	N = atoi(argv[1]);

    std::ofstream out("tp");
    srand(time(NULL));

    for (size_t i = 1; i <= N; i++)
    {
        std::stringstream ss;
        {
            ss << i << ",";
            ss << generateRandomSymbol() << ",";

            std::set<std::string> fieldsInUse;
            size_t fieldNum = generateRandomFieldNum();
            for (size_t j = 0; j < fieldNum; j++)
            {   
                std::string fieldName = generateRandomField(fieldsInUse);
                ss << fieldName << ",";
                fieldsInUse.insert(fieldName);

                if (j == (fieldNum - 1)) //last iteration
                {
                    ss << generateRandomDouble();
                }
                else
                {
                    ss << generateRandomDouble() << ",";
                }
            }
        }

        out << ss.str() << "\n";
    }
    
    out.close();
    return 0;
}

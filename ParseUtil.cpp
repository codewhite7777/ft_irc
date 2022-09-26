#include "Define.hpp"
#include <vector>
#include <sstream>
VECTOR<STRING> split(STRING input, char delimiter) {
    VECTOR<STRING> answer;
    std::stringstream ss(input);
    STRING temp;
 
    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }
 
    return answer;
}

void ParsePacket(const STRING & str, const char & delimeter, VECTOR<STRING> &_v)
{
    unsigned long prev_index = 0;
    unsigned long deli_index = str.find(delimeter);
    while (deli_index != std::string::npos)
    {
        std::string substring = str.substr(prev_index, deli_index - prev_index);
        if (substring != "")
            _v.push_back(substring);
        prev_index = deli_index + 1;
        deli_index = str.find(delimeter, prev_index);
    }
    std::string substring = str.substr(prev_index, deli_index - prev_index);
    if (substring != "")
        _v.push_back(substring);
}

void ParsePacket(const STRING & str, STRING & delimeter, VECTOR<STRING> &_v)
{
    unsigned long prev_index = 0;
    unsigned long deli_index = str.find(delimeter);
    while (deli_index != std::string::npos)
    {
        std::string substring = str.substr(prev_index, deli_index - prev_index);
        if (substring != "")
            _v.push_back(substring);
        prev_index = deli_index + 2;
        deli_index = str.find(delimeter, prev_index);
    }
    std::string substring = str.substr(prev_index, deli_index - prev_index);
    if (substring != "")
        _v.push_back(substring);
}
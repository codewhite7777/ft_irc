#ifndef _PARSE_UTIL_HPP_
#define _PARSE_UTIL_HPP_

#define STRING std::string
#define VECTOR std::vector
std::vector<std::string> split(std::string input, char delimiter);
void ParsePacket(const STRING & str, const char &delimeter, VECTOR<STRING> &_v);
void ParsePacket(const STRING & str, STRING &delimeter, VECTOR<STRING> &_v);

#endif
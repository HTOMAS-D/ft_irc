#pragma once
#include <vector>

class Parser{
    private:
    public:
        static std::vector<std::string> divideString(const std::string &str, char delim);
        static int isAction(const std::string &cmd);
        static std::string toUpper(const std::string &str);
};
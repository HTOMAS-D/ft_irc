#pragma once
#include <vector>

class Client;

class Parser{
    private:
    public:
        static std::vector<std::string> divideString(const std::string &str, char delim);
        static int isAction(const std::string &cmd, int i);
        static std::string toUpper(const std::string &str);
        static int nickParse(Client &client);
        static int inviteParse(Client &client);
        static int kickParse(Client &client);
};
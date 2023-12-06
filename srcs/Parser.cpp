#include "../includes/Socket.hpp"


std::vector<std::string> Parser::divideString(const std::string &str, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }

    return tokens;
}

int Parser::isAction(const std::string &command, int i){
    if(Manager::getActionMap().find(command) != Manager::getActionMap().end())
        return 1;
    Manager::getClientByID(i)->setregularCommand(Manager::getClientBuffer(i).str());
    return 0;
}

std::string Parser::toUpper(const std::string &str){
    std::string result = str;

    for (std::string::iterator it = result.begin(); it != result.end(); ++it) {
        *it = static_cast<char>(toupper(static_cast<unsigned char>(*it)));
    }

    return result;
}

int Parser::nickParse(int client, std::string nick) {
    if (nick.size() > MAXNAME || !nick.size()) {
        send(client, "Wrong nickname size\n", 20, 0);
        return 0;
    }
    for (int i = 0; i < (int)Manager::getClient().size(); i++) {
        if (Manager::getClient()[i].getNickName() == nick) {
            send(client, "Nickname already in use\n", 24, 0);
            return 0;
        }
    }
    return 1;
}

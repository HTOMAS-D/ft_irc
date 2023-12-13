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

int Parser::nickParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2 || command[1].size() == 1) {
        Manager::sendIrcMessage("431 NICK :No nick given", client.getId());
        return (0);
    }
    std::string nick = command[1];
    nick.pop_back();
    //check if it is too long, starts with special cha or has space
    if (nick.size() > 9 || nick.c_str()[0] == '#' || nick.c_str()[0] == ':' || (nick.find(" ") < nick.size() && (int)nick.find(" ") != -1)) {
        Manager::sendIrcMessage("432 NICK :Erroneus nickname", client.getId());
        return (0);
    }
    for (int i = 0; i < (int)Manager::getClient().size(); i++) {
        if (Manager::getClient()[i].getNickName() == nick) {
                Manager::sendIrcMessage("433 NICK :Nickname is already in use", client.getId());
            return 0;
        }
    }
    return 1;
}

int Parser::inviteParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    // std::cout << "cmd = " << command[1] << std::endl << "find == " << (int)command[1].find(" ") << std::endl;
    if (command.size() < 2 || (int)command[1].find(" ") < 0) {
        Manager::sendIrcMessage("461 INVITE :Not enough parameters", client.getId());
        return (0);
    }
    std::string channelName = command[1].substr(command[1].find(" ") + 1, command[1].size());
    std::string nickName = command[1].substr(0, command[1].find(" "));
    channelName.pop_back();
    int target = Manager::getIDbyNick(nickName);

    // no such user
    if (target == -1) {
        Manager::sendIrcMessage("401 :No such nick", client.getId());
        return (0);
    }

    //if channel doesnt exist reject
    if (Manager::getChannels().find(channelName) == Manager::getChannels().end()){
        Manager::sendIrcMessage("403 :No such channel", client.getId());
        return (0);
    }

    //check if it is on channel
    if (!Manager::getChannels().find(channelName)->second.checkClient(client.getId())) {
        Manager::sendIrcMessage("442 :You're not on that channel", client.getId());
        return (0);
    }

    //check I mode on and if is op
    if (Manager::getChannels().find(channelName)->second.getModeI() && \
    !Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage("482 :You're not channel operator", client.getId());
        return (0);
    }

    //if user already in channel
    if (Manager::getChannels().find(channelName)->second.checkClient(target)) {
        Manager::sendIrcMessage("443 :is already on channel", client.getId());
        return (0);
    }
    return (1);
}

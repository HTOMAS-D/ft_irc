#include "../../includes/Manager.hpp"

void Manager::kickAction(Client &client) {
    if(Parser::kickParse(client)) {
        std::vector<std::string> command = client.getCommand();
        std::string channelName = command[1].substr(0, command[1].find(" "));
        std::string user = command[1].substr(command[1].find(" ") + 1, command[1].size());
        std::string comment = "";

        if ((int)user.find(":") > 0) {
            comment = user.substr(user.find(":") + 1, command[1].size());
            user = user.substr(0, user.find(" "));
        }
        std::stringstream kickMsg;
        kickMsg << ":" << client.getNickName() << " KICK " << _channels.find(channelName)->second.getChannelId() \
        << " " << user;
        if (comment.empty()) {
            kickMsg << " :No reason given.";
        }
        else
            kickMsg << " :" << comment;
        kickMsg << "\r\n";
        _channels.find(channelName)->second.channelMessage(kickMsg.str().c_str());
        _channels.find(channelName)->second.removeClient(Manager::getIDbyNick(user));
    }
}
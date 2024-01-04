#include "../../includes/Manager.hpp"

void Manager::inviteAction(Client &client) {
    if (Parser::inviteParse(client)) {
        std::vector<std::string> command = client.getCommand();
        std::string nick = command[1].substr(0, command[1].find(" "));
        int id = Manager::getIDbyNick(nick);
        std::string channel = command[1].substr(command[1].find(" ") + 1, command[1].size());
        
        //add invited id to channel's list
        Manager::getChannels().find(channel)->second.addInvited(id);
        sendIrcMessage(client.getId(), formatMessage(client, INVITING) + " " + nick  + " " + channel);
        
        //send notice
        sendIrcMessage(id, formatMessage(client) + " NOTICE " + nick + " you have been invited to join " + channel);
    }
}
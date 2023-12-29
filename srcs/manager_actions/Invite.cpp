#include "../../includes/Manager.hpp"

void Manager::inviteAction(Client &client) {
    if (Parser::inviteParse(client)) {
        std::vector<std::string> command = client.getCommand();
        std::string nick = command[1].substr(0, command[1].find(" "));
        int id = Manager::getIDbyNick(nick);
        std::string channel = command[1].substr(command[1].find(" ") + 1, command[1].size());
        
        //prepare msg for user
        std::stringstream invMessage;
        invMessage << ":" << client.getNickName() << "!" << client.getUserName() << "@" << hostName \
        << " INVITE " << nick << " " << channel << "\r\n";

        //add invited id to channel's list
        Manager::getChannels().find(channel)->second.addInvited(id);
        send(client.getId(), invMessage.str().c_str(), invMessage.str().size(), 0);
        
        //prepare msg for invited user
        std::stringstream invNotif;
        invNotif << ":" << client.getNickName() << "!" << client.getUserName() << "@" << hostName \
        << " NOTICE " << nick << " you have been invited to join " << channel << "\r\n";
        send(id, invNotif.str().c_str(), invNotif.str().size(), 0);
    }
}
#include "../../includes/Manager.hpp"

void Manager::whoAction(Client &client) {
    if (Parser::whoParse(client)) {
        std::vector<std::string> command = client.getCommand();
        std::string channelName;
        std::string arg = "";
        if (command.size() > 1 && (int)command[1].find(" ") > 0) {
            channelName = command[1].substr(0, command[1].find(" "));
            arg = command[1].substr(command[1].find(" ") + 1, command[1].size());
        }
        else if (command.size() > 1)
            channelName = command[1];
        if (command.size() == 1) {
            //send literally everyone
            sendWhoMessage(getAllClientsIds(), client, "*");
        }
        else if (arg != "") {
            sendWhoMessage(_channels.find(channelName)->second.getOps(), client, channelName);
        }
        else {
            //send everyone in that channel
            sendWhoMessage(_channels.find(channelName)->second.getClients(), client, channelName);
        }
    }
}
#include "../../includes/Manager.hpp"

void Manager::joinAction(Client &client){
    std::vector<std::string> command = client.getCommand();
    std::string channelName = command[1];
    if (Parser::joinParse(client)){
        // if (!validChannel(channelName)) {
        //     joinChannel(channelName, client);
        //     return ;
        // }
        //Check if the client is already in the channel
        if (_channels[channelName].checkClient(client.getId())) {
            sendIrcMessage(client.getId(), formatMessage(client, ERR_ALREADYREGISTERED) + " " + channelName + " :You're already in that channel");
            return;
        }
        //Check if the channel exists, create if not
        if (_channels.find(channelName) != _channels.end()) {
            _channels[channelName] = Channel(channelName);
        }
        _channels[channelName].addClient(client.getId());
        sendIrcMessage(client.getId(), formatMessage(client) + " JOIN " + channelName);
        sendNamesList(channelName, client);
    }
}

void Manager::sendNamesList(const std::string &channelName, Client &client) {
    std::string _serverName = "my_server";
    std::vector<std::string> namesList = _channels[channelName].getNamesList();
    std::string namesMessage = Manager::formatMessage(client, NAMREPLY) + " = " + channelName + " :";
    for (unsigned long i = 0; i < namesList.size(); i++) {
        namesMessage += namesList[i];
        if (i != namesList.size() - 1)
            namesMessage += " ";
    }
    sendIrcMessage(client.getId(), namesMessage);
    // Send end of NAMES list
    sendIrcMessage(client.getId(), formatMessage(client, ENDOFNAMES) + " " + channelName + " :End of /NAMES list");
}
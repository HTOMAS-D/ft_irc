#include "../../includes/Manager.hpp"

void Manager::joinAction(Client &client){
    std::vector<std::string> command = client.getCommand();
    std::string channelName = command[1];
    if (Parser::joinParse(client)) {
        std::cout << "channel name = " << channelName << std::endl;
        // if (!validChannel(channelName)) {
        //     joinChannel(channelName, client);
        //     return ;
        // }
        //Check if the channel exists, create if not
        if (_channels.find(channelName) == _channels.end()) {
            std::cout << "hello" << std::endl;
            _channels[channelName] = Channel(channelName);
        }
        sendIrcMessage(client.getId(), formatMessage(client) + " JOIN " + channelName);
        //if there is topic send topic
        if (_channels.find(channelName)->second.getTopic() != "") {
            sendIrcMessage(client.getId(), formatMessage(client, TOPIC_CHANNEL) + " " + channelName + " :" + _channels.find(channelName)->second.getTopic());
        }
        _channels[channelName].addClient(client.getId());
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
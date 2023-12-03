#include "../includes/Manager.hpp"

void Manager::createMap(void) {
    _actionMap.insert(std::pair<std::string, eventFunction>("JOIN", &joinAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("KICK", &kickAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("INVITE", &inviteAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("TOPIC", &topicAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("MODE", &modeAction));
}

/* This function is to be used in the joinAction function to get the channel name and key*/
// std::map<std::string, std::string> Manager::getChannelMap(std::string cmd){
//     std::map<std::string, std::string> result;
//     std::string channel, key;
//     std::istringstream channelStream(cmd.substr(cmd.find(" "), cmd.end()));

// }
void Manager::joinAction(Client &client){
    std::string cmd = client.getCommand();  
    std::cout << "cmd = " << cmd << std::endl;
    if (cmd.size() < 2){
        Manager::sendIrcMessage("461 JOIN :Not enough parameters", client.getId());
        return;
    }
}

int	Manager::sendIrcMessage(std::string message, int clientId)
{
	message = message + "\r\n";
	std::cout << "Sending message: " << message << std::endl;
	if (send(clientId, message.c_str(), message.length(), 0) == -1)
		exit(4);
	return 0;
}

void Manager::runActions(Client &client){
    createMap();
    std::string cmd = client.getCommand();
    std::string action = cmd.substr(0, cmd.find(" "));
    std::cout << "Action = " << action << std::endl;
    std::map<std::string, eventFunction>::iterator it = _actionMap.find(action);
    if (it != _actionMap.end()){
        it->second(client);
    }
    else{
        Manager::sendIrcMessage("421 :Unknown command", client.getId());
    }
}
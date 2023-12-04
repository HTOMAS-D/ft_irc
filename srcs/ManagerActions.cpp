#include "../includes/Manager.hpp"

void Manager::createMap(void) {
    _actionMap.insert(std::make_pair<std::string, eventFunction>("JOIN", &joinAction));
    _actionMap.insert(std::make_pair<std::string, eventFunction>("KICK", &joinAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("INVITE", &inviteAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("TOPIC", &topicAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("MODE", &modeAction));
}

/* This function is to be used in the joinAction function to get the channel name and key*/
// std::map<std::string, std::string> Manager::getChannelMap(std::string cmd){
//     std::map<std::string, std::string> result;
//     std::string channel, key;

    
// }
// }
void Manager::joinAction(Client &client){
    std::string cmd = client.getCommand()[0];  

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
    (void) client;
    std::cout << "entered run" << std::endl;
    // std::cout << "it = " << (_actionMap.find("JOIN"))->first << std::endl;
    std::string cmd = client.getCommand()[0];
    std::cout << "cmd = " << cmd << std::endl;
    std::string action = cmd;
    std::cout << "size: " << action.size() << std::endl;
    std::cout << "Action =" << action << std::endl;
    // if(Parser::isAction(cmd)){
    //     return ;
    // }
    // std::map<std::string, eventFunction>::iterator it = _actionMap.find(action);
    // std::cout << "First" << it->first << std::endl;
    // if (_actionMap.find(action) != _actionMap.end()){
    //     std::cout << "Found action" << std::endl;
    //     // it->second(client);
    // }
    // else{
    //     Manager::sendIrcMessage("421 :Unknown command", client.getId());
    // }
}
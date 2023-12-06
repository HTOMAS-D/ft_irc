#include "../includes/Manager.hpp"

void Manager::nickAction(Client &client){
    std::string temp = client.getCommand()[1];
    // temp.pop_back();
    if (Parser::nickParse(client.getId(), temp))
        client.setNickName(temp);
    std::cout << "nickname set to " << client.getNickName(); 
}

void Manager::createMap(void) {
    _actionMap.insert(std::make_pair<std::string, eventFunction>("JOIN", &joinAction));
    _actionMap.insert(std::make_pair<std::string, eventFunction>("KICK", &joinAction));
    _actionMap.insert(std::pair<std::string, eventFunction>("INVITE", &joinAction));
    _actionMap.insert(std::pair<std::string, eventFunction>("TOPIC", &joinAction));
    _actionMap.insert(std::pair<std::string, eventFunction>("MODE", &joinAction));
    _actionMap.insert(std::pair<std::string, eventFunction>("NICK", &nickAction));

}


void Manager::joinAction(Client &client){
    if (client.getCommand().size() < 2){
        Manager::sendIrcMessage("461 JOIN :Not enough parameters", client.getId());
        return;
    }
    if (_channels.find(client.getCommand()[1]) != _channels.end())
        _channels.find(client.getCommand()[1])->second.addClient(client.getId());
    else
        send(client.getId(), "No such channel\n", 16, 0);
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
    // std::cout << "entered run" << std::endl;
    // std::cout << "it = " << (_actionMap.find("JOIN"))->first << std::endl;
    std::string cmd = client.getCommand()[0];
    // std::cout << "cmd = " << cmd << std::endl;
    std::string action = cmd;
    // std::cout << "size: " << action.size() << std::endl;
    // std::cout << "Action =" << action << std::endl;
    std::map<std::string, eventFunction>::iterator it = _actionMap.find(action);
    // std::cout << "First" << it->first << std::endl;
    if (_actionMap.find(action) != _actionMap.end()){
    //     std::cout << "Found action" << std::endl;
        it->second(client);
    }
    else{
        Manager::sendIrcMessage("421 :Unknown command", client.getId());
    }
}
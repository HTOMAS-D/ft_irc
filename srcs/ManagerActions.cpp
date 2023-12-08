#include "../includes/Manager.hpp"

void Manager::nickAction(Client &client){
    std::string temp = client.getCommand()[1];
    // temp.pop_back();
    if (Parser::nickParse(client.getId(), temp))
        client.setNickName(temp);
    std::cout << "nickname set to " << client.getNickName(); 
}

void Manager::createMap(void) {
    _actionMap["JOIN"] = joinAction;
    _actionMap["NICK"] = nickAction;
    //_actionMap["KICK"] = kickAction;
    // _actionMap["INVITE"] = inviteAction;
    // _actionMap["TOPIC"] = topicAction;
    // _actionMap["MODE"] = modeAction;
    //_actionMap.insert(std::make_pair<std::string, eventFunction>("JOIN", &joinAction));
    //_actionMap.insert(std::make_pair<std::string, eventFunction>("KICK", &joinAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("INVITE", &joinAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("TOPIC", &joinAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("MODE", &joinAction));
    // _actionMap.insert(std::pair<std::string, eventFunction>("NICK", &nickAction));
}

//command[1] its the channel name
void Manager::joinAction(Client &client){
    std::vector<std::string> command = client.getCommand();
    if (client.getCommand().size() < 2){
        Manager::sendIrcMessage("461 JOIN :Not enough parameters", client.getId());
        return;
    }
    std::string channelName = command[1];
    if (channelName[0] != '#'){
        Manager::sendIrcMessage("403 :No such channel", client.getId());
        return;
    }
    //Check if the channel exists, create if not
    if (_channels.find(channelName) == _channels.end())
        _channels.insert(std::make_pair<std::string, Channel>(client.getCommand()[1], Channel(client.getCommand()[1], "", "")));
    
    //Check if the client is already in the channel
    if (_channels.find(channelName)->second.checkClient(client.getId())) {
        Manager::sendIrcMessage("442 :You're already in that channel", client.getId());
        return;
    }
    _channels[channelName].addClient(client.getId());
    std::string joinMessage = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName()
                        + " JOIN :" + channelName + "\r\n";
    send(client.getId(), joinMessage.c_str(), joinMessage.size(), 0);
    sendNamesList(channelName, client);
    // if (_channels.find(command[1]) != _channels.end())
    //     _channels.find(command[1])->second.addClient(client.getId());
    // else{
    //     send(client.getId(), "No such channel\n", 16, 0);
    //     //Manager::sendIrcMessage("475 :Bad channel key", client.getId());
    //     //return;
    // }
}

void Manager::sendNamesList(const std::string &channelName, Client &client) {
    std::string _serverName = "my_server";
    std::vector<std::string> namesList = _channels[channelName].getNamesList();
    std::string namesMessage = ":" + _serverName + " 353 " + \
        client.getClientPrefix() + " = " + channelName + " :";
    
    for (unsigned long i = 0; i < namesList.size(); i++) {
        namesMessage += namesList[i];
        if (i != namesList.size() - 1)
            namesMessage += " ";
    }
    namesMessage += "\r\n";
    send(client.getId(), namesMessage.c_str(), namesMessage.size(), 0);
    // Send end of NAMES list
    std::string endOfNamesMessage = ":" + _serverName + " 366 " + \
        client.getClientPrefix()+ " " + channelName +
                                    " :End of /NAMES list\r\n";
    send(client.getId(), endOfNamesMessage.c_str(), endOfNamesMessage.size(), 0);
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

O join já ta funcional no hexchat! Tive de sacar os hostnames dos clients no ficheiro do socket, por isso o construtor do cliente tb recebe isso agora, pq é algo necessario para mandar para o protocolo irc
Depois, o problema foi arranjar no join uma lista com todos os nicknames que tao num dado canal para mandar a ultimas duas mensagens do protocolo irc necessarias e para isso inventei umas funcoes no channel e no client.
Por isso, o codigo ficou um bcd uma mess pq queria mesmo fazer isto antes de dormir, mas vou dar push já e amanha arrumo o codigo todo. Amanha vou tentar acabar o resto das actions com protocolo
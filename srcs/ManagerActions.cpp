#include "../includes/Manager.hpp"

// void Manager::modeAction(Client &client) {
//     if (Parser::modeParse(client)) {

//     }
// }

void Manager::topicAction(Client &client) {
    if (Parser::topicParse(client)) {
        std::vector<std::string> command = client.getCommand();
        std::string channelName;
        std::string topic;
        std::stringstream topicMsg;
        //if there is topic in cmd
        if ((int)command[1].find(":") > 0) {
            topic = command[1].substr(command[1].find(":") + 1, command[1].size());
            channelName = command[1].substr(0, command[1].find(" "));
        }
        else
            channelName = command[1];
        topicMsg << ":" << client.getHostName() << " ";
        if ((int)command[1].find(":") < 0 && _channels.find(channelName)->second.getTopic().empty()) {
            topicMsg << "331 " << client.getNickName() << " :No topic is set" << "\r\n";
            send(client.getId(), topicMsg.str().c_str(), topicMsg.str().size(), 0);
        }
        else if ((int)command[1].find(":") < 0) {
            topicMsg << "332 " << client.getNickName() << " :" << _channels.find(channelName)->second.getTopic() << "\r\n";
            send(client.getId(), topicMsg.str().c_str(), topicMsg.str().size(), 0);
        }
        else if (_channels.find(channelName)->second.getModeT() && !_channels.find(channelName)->second.IsOp(client.getId())) {
            topicMsg << "482 " << client.getNickName() << " :Your're not channel operator" << "\r\n";
            send(client.getId(), topicMsg.str().c_str(), topicMsg.str().size(), 0);
        }
        else {
            _channels.find(channelName)->second.setTopic(topic);
            topicMsg << "332 " << client.getNickName() << " " << _channels.find(channelName)->second.getChannelId() <<  " :"  << _channels.find(channelName)->second.getTopic() << "\r\n";
            std::cout << "msg topic->" << topicMsg.str();
            _channels.find(channelName)->second.channelMessage(topicMsg.str().c_str());
        }
    }
}

void Manager::kickAction(Client &client) {
    if(Parser::kickParse(client)) {
        std::vector<std::string> command = client.getCommand();
        std::string channelName = command[1].substr(0, command[1].find(" "));
        std::string user = command[1].substr(command[1].find(" ") + 1, command[1].size());
        std::string comment = "";
        if ((int)user.find(":") > 0) {
            std::cout << "found comment" << std::endl;
            comment = user.substr(user.find(":") + 1, command[1].size());
            user = user.substr(0, user.find(" "));
        }
        std::stringstream kickMsg;
        kickMsg << client.getNickName() << " KICK " << _channels.find(channelName)->second.getChannelId() \
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

void Manager::nickAction(Client &client){
    if (Parser::nickParse(client)) {
        std::string temp = client.getCommand()[1];
        //preparing msg of successfull change
        std::stringstream nickMsg;
        nickMsg << ":" << client.getNickName() << " NICK :" << temp << "\r\n";
        send(client.getId(), nickMsg.str().c_str(), nickMsg.str().size(), 0);
        client.setNickName(temp);
    }
}

void Manager::inviteAction(Client &client) {
    if (Parser::inviteParse(client)) {
        //prepare arguments
        std::vector<std::string> command = client.getCommand();
        std::string nick = command[1].substr(0, command[1].find(" "));
        int id = Manager::getIDbyNick(nick);
        std::string channel = command[1].substr(command[1].find(" ") + 1, command[1].size());
        //prepare msg for user
        std::stringstream invMessage;
        invMessage << ":" << client.getNickName() << "!" << client.getUserName() << "@" << client.getHostName() \
        << " INVITE " << nick << " " << channel << "\r\n";
        //add invited id to channel's list
        Manager::getChannels().find(channel)->second.addInvited(id);
        send(client.getId(), invMessage.str().c_str(), invMessage.str().size(), 0);
        //prepare msg for invited user
        std::stringstream invNotif;
        invNotif << ":" << client.getNickName() << "!" << client.getUserName() << "@" << client.getHostName() \
        << " NOTICE " << nick << " you have been invited to join " << channel << "\r\n";
        send(id, invNotif.str().c_str(), invNotif.str().size(), 0);
    }
}

void Manager::createMap(void) {
    _actionMap["JOIN"] = joinAction;
    _actionMap["NICK"] = nickAction;
    _actionMap["INVITE"] = inviteAction;
    _actionMap["KICK"] = kickAction;
    _actionMap["TOPIC"] = topicAction;
    //_actionMap["MODE"] = modeAction;
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
    //channelName.pop_back();
    if (channelName[0] != '#'){
        Manager::sendIrcMessage("403 :No such channel", client.getId());
        return;
    }
    //Check if the channel exists, create if not
    if (_channels.find(channelName) == _channels.end()) {
        std::cout << "aqui 4" << std::endl;
        _channels.insert(std::make_pair<std::string, Channel>(channelName, Channel(channelName, "", "")));
    }
    
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
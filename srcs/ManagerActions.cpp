#include "../includes/Manager.hpp"

void Manager::createMap(void) {
    _actionMap["JOIN"] = joinAction;
    _actionMap["NICK"] = nickAction;
    _actionMap["INVITE"] = inviteAction;
    _actionMap["KICK"] = kickAction;
    _actionMap["TOPIC"] = topicAction;
    _actionMap["MODE"] = modeAction;
	_actionMap["PRIVMSG"] = privmsgAction;

}

//PRIVMSG channel msg
void Manager::privmsgAction(Client &client)
{
    std::vector<std::string> command = client.getCommand();
    std::string channelName = command[1].substr(0, command[1].find(" "));
    std::string msg = command[1].substr(command[1].find(":"), command[1].size());
    std::cout << "channel name = " << channelName << "; msg = " << msg << ";\n";
    if (command.size() < 2) {
		sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", client.getId());
		return;
	}
    // Extract the target and the message

    // Check if the target is a valid channel or user
    // if (target[0] != '#' && !Manager::isUserInChannel(target, client.getChannel())) {
    //     sendIrcMessage(formatMessage(client, ERR_NOSUCHNICK) + " " + target + " :No such nick/channel", client.getId());
    //     return;
    // }

    // If everything is okay, send the message to the target
    if (channelName[0] == '#') {
        //Broadcast the message to all members of the channel
        Channel &channel = Manager::getChannels().find(channelName)->second;
        channel.channelMessage(formatMessage(client) + " PRIVMSG " + channelName + " :" + msg);
    } 
    else {
        // Target is a user, send the message directly to the user
        int targetId = Manager::getIDbyNick(channelName);
        if (targetId != -1) {
            std::string ircMessage = formatMessage(client) + " PRIVMSG " + channelName + " :" + msg;
            sendIrcMessage(ircMessage, targetId);
        } else {
            // Handle the case where the target user does not exist
            sendIrcMessage(formatMessage(client, NOSUCHNICK) + " " + channelName + " :No such nick", client.getId());
        }
    }
}



std::string Manager::formatMessage(Client &client) {
	return (":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName());
}

std::string Manager::formatMessage(Client &client, std::string message)
{
	return (":" + client.getHostName() + " " + message + " " + client.getNickName());
}

void Manager::modeAction(Client &client) {
    if (Parser::modeParse(client)) {
        
    }
}

// std::string Manager::formatMessage(Channel &_channel,  std::string &message)
// {
// 	return (":" + _channel.getHostName() + " " + message + " " + _channel.getName());
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

void Manager::nickAction(Client &client)
{
    if (Parser::nickParse(client)) 
    {
        sendIrcMessage(":" + client.getNickName() + " NICK :" + client.getCommand()[1], client.getId());
        client.setNickName(client.getCommand()[1]);
    }
}

void Manager::joinAction(Client &client){
    std::vector<std::string> command = client.getCommand();
    std::string channelName = command[1];
    if (Parser::joinParse(client)){
        //Check if the client is already in the channel
        // if (_channels.find(channelName)->second.checkClient(client.getId())) {
        //     sendIrcMessage(Manager::formatMessage(client, NOTONCHANNEL), client.getId());
        //     return ;
        // }
        //Check if the channel exists, create if not
        if (_channels.find(channelName) == _channels.end()) {
            _channels.insert(std::make_pair<std::string, Channel>(channelName, Channel(channelName, "", "")));
        }
        _channels[channelName].addClient(client.getId());
        sendIrcMessage(formatMessage(client) + " JOIN " + channelName, client.getId());
        sendNamesList(channelName, client);
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


void Manager::sendNamesList(const std::string &channelName, Client &client) {
    std::string _serverName = "my_server";
    std::vector<std::string> namesList = _channels[channelName].getNamesList();
    std::string namesMessage = Manager::formatMessage(client, NAMREPLY) + " = " + channelName + " :";
    for (unsigned long i = 0; i < namesList.size(); i++) {
        namesMessage += namesList[i];
        if (i != namesList.size() - 1)
            namesMessage += " ";
    }
    sendIrcMessage(namesMessage, client.getId());
    // Send end of NAMES list
    std::string endOfNamesMessage = ":" + _serverName + " 366 " + \
        Manager::formatMessage(client) + " " + channelName + " :End of /NAMES list";
    sendIrcMessage(endOfNamesMessage, client.getId());
}

int	Manager::sendIrcMessage(std::string message, int clientId)
{
	message = message + "\r\n";
	std::cout << "Sending message: " << message;
	if (send(clientId, message.c_str(), message.length(), 0) == -1)
		exit(4);
	return 0;
}

void Manager::runActions(Client &client){
    (void) client;
    std::string cmd = client.getCommand()[0];
    std::string action = cmd;
    std::map<std::string, eventFunction>::iterator it = _actionMap.find(action);
    if (_actionMap.find(action) != _actionMap.end()){
        it->second(client);
    }
    else{
        Manager::sendIrcMessage("421 :Unknown command", client.getId());
    }
}
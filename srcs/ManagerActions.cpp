#include "../includes/Manager.hpp"

// how we receive them:
// * JOIN group
// * KICK user group
// * INVITE group user
// * TOPIC user :topico
// * MODE action

void Manager::createMap(void) {
    _actionMap["JOIN"] = joinAction;
    _actionMap["NICK"] = nickAction;
    _actionMap["INVITE"] = inviteAction;
    _actionMap["KICK"] = kickAction;
    _actionMap["TOPIC"] = topicAction;
    _actionMap["MODE"] = modeAction;
	_actionMap["PRIVMSG"] = privmsgAction;
}

std::string Manager::formatMessage(Client &client) {
	return (":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName());
}

std::string Manager::formatMessage(Client &client, std::string message)
{
	return (":" + client.getHostName() + " " + message + " " + client.getNickName());
}

// std::string Manager::formatMessage(Channel &_channel,  std::string &message)
// {
// 	return (":" + _channel.getChannelId() + " " + message + " " + _channel.getName());
// }

void Manager::joinAction(Client &client){
    std::vector<std::string> command = client.getCommand();
    std::string channelName = command[1];
    if (Parser::joinParse(client)){
        //Check if the client is already in the channel
        // if (_channels.find(channelName)->second.checkClient(client.getId())) {
        //     sendIrcMessage(client.getId(), Manager::formatMessage(client, NOTONCHANNEL));
        //     return ;
        // }
        //Check if the channel exists, create if not
        if (_channels.find(channelName) == _channels.end()) {
            //std::cout << "OLA" << std::endl;
            // _channels.insert(std::make_pair<std::string, Channel>(channelName, Channel(channelName)));
            _channels[channelName] = Channel(channelName);
        }
        _channels[channelName].addClient(client.getId());
        sendIrcMessage(client.getId(), formatMessage(client) + " JOIN " + channelName);
        sendNamesList(channelName, client);
    }
}

void Manager::nickAction(Client &client)
{
    if (Parser::nickParse(client)) 
    {
        sendIrcMessage(client.getId(), ":" + client.getNickName() + " NICK :" + client.getCommand()[1]);
        client.setNickName(client.getCommand()[1]);
    }
}

void Manager::topicAction(Client &client) {
    if (Parser::topicParse(client)) {
        std::vector<std::string> command = client.getCommand();
        std::stringstream topicMsg;
        std::string topic;
        std::string channelName;
        //if there is topic in cmd
        if ((int)command[1].find(":") > 0) {
            topic = command[1].substr(command[1].find(":") + 1, command[1].size());
            channelName = command[1].substr(0, command[1].find(" "));
        }
        topicMsg << ":" << client.getHostName() << " ";
        if ((int)command[1].find(":") < 0 && _channels.find(channelName)->second.getTopic().empty()) {
            sendIrcMessage(client.getId(), formatMessage(client, NOTOPIC) + " " + channelName + " :No topic is set");
        }
        else if ((int)command[1].find(":") < 0) {
            sendIrcMessage(client.getId(), formatMessage(client, TOPIC_CHANNEL) + " " + channelName + " :" + _channels.find(channelName)->second.getTopic());
        }
        else if (_channels.find(channelName)->second.getModeT() && !_channels.find(channelName)->second.IsOp(client.getId())) {
            sendIrcMessage(client.getId(), formatMessage(client, NOTCHANOP) + " " + channelName + " :You're not channel operator");
        }   
        else {
            _channels.find(channelName)->second.setTopic(topic);
            sendIrcMessage(client.getId(), formatMessage(client, TOPIC_CHANNEL) + " " + channelName + " :" + _channels.find(channelName)->second.getTopic());
            _channels.find(channelName)->second.channelMessage(topicMsg.str().c_str());
        }
    }
}

void Manager::inviteAction(Client &client) {
    if (Parser::inviteParse(client)) {
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

void Manager::privmsgAction(Client &client)
{
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2 || (int)command[1].find(":") < 0) {
		sendIrcMessage(client.getId(), formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters");
		return;
	}
    std::string targetName = command[1].substr(0, command[1].find(" "));
    std::string msg = command[1].substr(command[1].find(":") + 1, command[1].size());
    // Extract the target and the message

    // Check if the target is a valid channel or user
    // if (target[0] != '#' && !Manager::isUserInChannel(target, client.getChannel())) {
    //     sendIrcMessage(client.getId(), formatMessage(client, ERR_NOSUCHNICK) + " " + target + " :No such nick/channel");
    //     return;
    // }

    // If everything is okay, send the message to the target
    if (targetName[0] == '#') {
        std::string channelName = targetName;
        std::cout << "pre msg send to " << channelName << std::endl;
        //Broadcast the message to all members of the channel
        std::string formatmsg = formatMessage(client) + " PRIVMSG " + targetName + " :" + msg;
        if (_channels[channelName].checkClient(client.getId()))
            _channels[channelName].clientMessage(client.getId(), formatmsg.c_str());
        else
            sendIrcMessage(client.getId(), formatMessage(client, NOTONCHANNEL) + " " + targetName + " :Not in channel");
    } 
    else {
        // Target is a user, send the message directly to the user
        int targetId = Manager::getIDbyNick(targetName);
        if (targetId != -1) {
            std::string ircMessage = formatMessage(client) + " PRIVMSG " + targetName + " :" + msg;
            sendIrcMessage(targetId, ircMessage);
        } else {
            // Handle the case where the target user does not exist
            sendIrcMessage(client.getId(), formatMessage(client, NOSUCHNICK) + " " + targetName + " :No such nick");
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
    std::string endOfNamesMessage = ":" + _serverName + " 366 " + \
        Manager::formatMessage(client) + " " + channelName + " :End of /NAMES list";
    sendIrcMessage(client.getId(), endOfNamesMessage);
}

int	Manager::sendIrcMessage(int clientId, std::string message)
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
        Manager::sendIrcMessage(client.getId(), "421 :Unknown command");
    }
}
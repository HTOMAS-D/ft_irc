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
    _actionMap["WHO"] = whoAction;
    _actionMap["USER"] = userAction;
    _actionMap["PASS"] = passAction;
    _actionMap["CAP"] = capAction;
}

std::string Manager::formatMessage(Client &client) {
	return (":" + client.getNickName() + "!" + client.getUserName() + "@" + hostName);
}

std::string Manager::formatMessage(Client &client, std::string message)
{
	return (":" + hostName + " " + message + " " + client.getNickName());
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
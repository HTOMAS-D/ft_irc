#include "../includes/Manager.hpp"
#include "manager_actions/Join.cpp"
#include "manager_actions/Invite.cpp"
#include "manager_actions/Kick.cpp"
#include "manager_actions/Quit.cpp"
#include "manager_actions/Topic.cpp"
#include "manager_actions/Privmsg.cpp"
#include "manager_actions/Who.cpp"
#include "manager_actions/Mode.cpp"
#include "manager_actions/OtherActions.cpp"

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
    _actionMap["QUIT"] = quitAction;
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
        sendIrcMessage(client.getId(), formatMessage(client, UNKNOWNCOMMAND) + " :Unknown command");
    }
}
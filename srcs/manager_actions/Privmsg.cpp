#include "../../includes/Manager.hpp"

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
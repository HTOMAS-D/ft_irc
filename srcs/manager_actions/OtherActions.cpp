#include "../../includes/Manager.hpp"


void Manager::passAction(Client &client) {
    std::vector<std::string> command = client.getCommand();
    std::string password = command[1];
    setPassword(password);
    //sendIrcMessage(client.getId(), formatMessage(client, WELCOME_MESSAGE) + " :Welcome to the Internet Relay Network ");
}

//The cap command is used to negotiate capabilities between clients and servers.
void Manager::capAction(Client &client) {
    std::vector<std::string> command = client.getCommand();
    std::string cap = command[1];
    if (cap == "LS") {
        sendIrcMessage(client.getId(), formatMessage(client) + " CAP * LS :multi-prefix sasl");
    }
    else if (cap == "REQ") {
        sendIrcMessage(client.getId(), formatMessage(client) + " CAP * ACK :multi-prefix sasl");
    }
    else if (cap == "END") {
        sendIrcMessage(client.getId(), formatMessage(client) + " CAP * ACK :multi-prefix sasl");
    }
}

void Manager::sendWhoMessage(const std::vector<int> &list, Client &client, std::string const &channelName) {
    for (int i = 0; i < (int)list.size(); i++) {
        Client &temp = *Manager::getClientByID(list[i]);
        std::string status;
        if (channelName != "*")
            status = _channels.find(channelName)->second.IsOp(temp.getId()) ? "@" : "+";
        sendIrcMessage(client.getId(), formatMessage(client, RPL_WHOREPLY) + " " + channelName + " localhost ft_irc " + temp.getNickName() + " H" + status + " :1 " + temp.getUserName());
    }
    sendIrcMessage(client.getId(), formatMessage(client, RPL_ENDOFWHO) + " " + channelName + " :End of WHO list");
}

void Manager::nickAction(Client &client)
{
    if (Parser::nickParse(client)) 
    {
        //1 for normal cases, 2 for cases that tried a nick and didnt work
        if (client.getNickName() != "")
            sendIrcMessage(client.getId(), ":" + client.getNickName() + " NICK :" + client.getCommand()[1]);
        else
            sendIrcMessage(client.getId(), ":" + client.getLastNick() + " NICK :" + client.getCommand()[1]);
        client.setNickName(client.getCommand()[1]);
    }
}

void Manager::userAction(Client &client) {
    if (Parser::userParse(client)) {
        std::vector<std::string> command = client.getCommand();
        std::string username = command[1].substr(0, command[1].find(" "));
        client.setUserName(username);
    }
}
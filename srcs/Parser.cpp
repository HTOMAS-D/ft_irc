#include "../includes/Socket.hpp"


std::vector<std::string> Parser::divideString(const std::string &str, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

int Parser::isAction(const std::string &command, int i){
    if(Manager::getActionMap().find(command) != Manager::getActionMap().end())
        return 1;
    Manager::getClientByID(i)->setregularCommand(Manager::getClientBuffer(i).str());
    return 0;
}

std::string Parser::toUpper(const std::string &str){
    std::string result = str;

    for (std::string::iterator it = result.begin(); it != result.end(); ++it) {
        *it = static_cast<char>(toupper(static_cast<unsigned char>(*it)));
    }

    return result;
}

int Parser::nickParse(Client &client) {

    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2) {
	    Manager::sendIrcMessage(Manager::formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", client.getId());
	    return 0;
	}
    if (command[1].size() == 1) {
        Manager::sendIrcMessage(Manager::formatMessage(client, NONICKNAMEGIVEN), client.getId());
        return 0;
    }
    std::string nick = command[1];
    //check if it is too long, starts with special cha or has space
    if (nick.size() > 9 || nick.c_str()[0] == '#' || nick.c_str()[0] == ':' || ((int)nick.find(" ") && (int)nick.find(" ") != -1)) {
        Manager::sendIrcMessage(Manager::formatMessage(client, ERRONEUSNICKNAME) + " " + nick + " :Erroneous nickname", client.getId());
        return 0;
    }
    for (int i = 0; i < (int)Manager::getClient().size(); i++) {
        if (Manager::getClient()[i].getNickName() == nick) {
                Manager::sendIrcMessage(Manager::formatMessage(client, NICKNAMEINUSE) + " " + nick + " :Nickname is already in use", client.getId());
            return 0;
        }
    }
    return 1;
}

int Parser::joinParse(Client &client)
{
    if (client.getCommand().size() < 2){
        Manager::sendIrcMessage(Manager::formatMessage(client, NEEDMOREPARAMS), client.getId());
        return 0;
    }
    if (client.getCommand()[1][0] != '#'){
        Manager::sendIrcMessage(Manager::formatMessage(client, NEEDMOREPARAMS), client.getId());
        return 0;
    }
    return 1;
}

int Parser::inviteParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    // std::cout << "cmd = " << command[1] << std::endl << "find == " << (int)command[1].find(" ") << std::endl;
    if (command.size() < 2 || (int)command[1].find(" ") < 0) {
        Manager::sendIrcMessage("461 INVITE :Not enough parameters", client.getId());
        return (0);
    }
    std::string channelName = command[1].substr(command[1].find(" ") + 1, command[1].size());
    std::string nickName = command[1].substr(0, command[1].find(" "));
    //channelName.pop_back();
    int target = Manager::getIDbyNick(nickName);

    // no such user
    if (target == -1) {
        Manager::sendIrcMessage("401 :No such nick", client.getId());
        return (0);
    }

    //if channel doesnt exist reject
    if (Manager::getChannels().find(channelName) == Manager::getChannels().end()){
        Manager::sendIrcMessage("403 :No such channel", client.getId());
        return (0);
    }

    //check if it is on channel
    if (!Manager::getChannels().find(channelName)->second.checkClient(client.getId())) {
        Manager::sendIrcMessage("442 :You're not on that channel", client.getId());
        return (0);
    }

    //check I mode on and if is op
    if (Manager::getChannels().find(channelName)->second.getModeI() && \
    !Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage("482 :You're not channel operator", client.getId());
        return (0);
    }

    //if user already in channel
    if (Manager::getChannels().find(channelName)->second.checkClient(target)) {
        Manager::sendIrcMessage("443 :is already on channel", client.getId());
        return (0);
    }
    return (1);
}

int Parser::kickParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2 || (int)command[1].find(" ") < 0) {
        Manager::sendIrcMessage("461 KICK :Not enough parameters", client.getId());
        return (0);
    }
    //preparing variables
    std::string channelName = command[1].substr(0, command[1].find(" "));
    std::string user = command[1].substr(command[1].find(" ") + 1, command[1].size());
    //user.pop_back();
    std::string comment = "";
    if ((int)user.find(":") > 0) {
        comment = user.substr(user.find(":") + 1, command[1].size());
        user = user.substr(0, user.find(" "));
    }
    //check if user exists
    int target = Manager::getIDbyNick(user);
    if (target == -1) {
        Manager::sendIrcMessage("401 :No such nick", client.getId());
        return (0);
    }

    //if channel doesnt exist reject
    if (Manager::getChannels().find(channelName) == Manager::getChannels().end()){
        Manager::sendIrcMessage("403 :No such channel", client.getId());
        return (0);
    }

    //is op to kick
    if (!Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage("482 :Your're not channel operator", client.getId());
        return (0);
    }

    //if user not in channel
    if (!Manager::getChannels().find(channelName)->second.checkClient(target)) {
        Manager::sendIrcMessage("441 :They aren't on that channel", client.getId());
        return (0);
    }

    //cannot kick yourself
    if (client.getId() == target) {
        Manager::sendIrcMessage("481 :Cannot kick yourself", client.getId());
        return (0);
    }
    return (1);
}

int Parser::topicParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2) {
        Manager::sendIrcMessage("461 TOPIC :Not enough parameters", client.getId());
        return (0);
    }
    std::string channelName;
    //if there is topic in cmd
    if ((int)command[1].find(":") > 0)
        channelName = command[1].substr(0, command[1].find(" "));
    else
        channelName = command[1];
    //if channel doesnt exist reject
    if (Manager::getChannels().find(channelName) == Manager::getChannels().end()){
        Manager::sendIrcMessage("403 :No such channel", client.getId());
        return (0);
    }

    //if user not in channel
    if (!Manager::getChannels().find(channelName)->second.checkClient(client.getId())) {
        Manager::sendIrcMessage("442 :You're not on that channel", client.getId());
        return (0);
    }

    //is op and mode topic is on
    if (Manager::getChannels().find(channelName)->second.getModeT() && \
    !Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage("482 :Your're not channel operator", client.getId());
        return (0);
    }
    return (1);
}

int Parser::modeParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2) {
        Manager::sendIrcMessage("461 MODE :Not enough parameters", client.getId());
        return (0);
    }

    std::string channelName;
    std::string flag = "";
    std::string arg = "";
    if ((int)command[1].find(" ") > 0) {
        channelName = command[1].substr(0, command[1].find(" "));
        flag = command[1].substr(command[1].find(" ") + 1, command[1].size());
        if ((int)flag.find(" ") > 0) {
            arg = flag.substr(flag.find(" ") + 1, flag.size());
            flag = flag.substr(0, flag.find(" "));
        }
    }
    else
        channelName = command[1];
    //if channel doesnt exist reject
    if (Manager::getChannels().find(channelName) == Manager::getChannels().end()){
        Manager::sendIrcMessage("403 :No such channel", client.getId());
        return (0);
    }
    //if channel exists there is no flag we send modes to user
    if (flag.empty()){
        Manager::sendIrcMessage("324 :" + channelName + ": " + Manager::getChannels().find(channelName)->second.getChannelModes(), client.getId());
        return (0);
    }
    //check if flag format is correct
    if (!flag.empty() && (flag.size() != 2 || (flag[0] != '+' && flag[0] != '-'))) {
        Manager::sendIrcMessage("501 :Unknown MODE flag", client.getId());
        return (0);
    }
    //check if it is op
    if (!Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage("482 :Your're not channel operator", client.getId());
        return (0);
    }
    //check if there is such flag
    if (flag[1] != 'i' && flag[1] != 't' && flag[1] != 'k' && flag[1] != 'o' && flag[1] != 'l') {
        Manager::sendIrcMessage("421 :Unknown command", client.getId());
        return (0);
    }
    // std::cout << "flag = " << flag << "; arg = " << arg << "; e empty ta = " << (((flag[1] == 'k' && flag[0] == '+') || flag[1] != 'o') && arg.empty()) << std::endl;
    if (((flag[1] == 'k' && flag[0] == '+') || flag[1] == 'o') && arg.empty()) {
        Manager::sendIrcMessage("461 MODE :Not enough parameters", client.getId());
        return (0);
    }

    if (flag[1] == 'o') {
        //check if user exists
        int target = Manager::getIDbyNick(arg);
        if (target == -1) {
            Manager::sendIrcMessage("401 :No such nick", client.getId());
            return (0);
        }
    }
    return (1);
}

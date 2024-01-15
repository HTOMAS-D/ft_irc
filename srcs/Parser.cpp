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
	    Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " NICK ERROR :Not enough parameters");
	    return 0;
	}
    if (command[1].size() == 0) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NONICKNAMEGIVEN));
        return 0;
    }
    std::string nick = command[1];
    //check if it is too long, starts with special cha or has space
    if (nick.size() > 9 || nick.c_str()[0] == '#' || nick.c_str()[0] == ':' || ((int)nick.find(" ") && (int)nick.find(" ") != -1)) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERRONEUSNICKNAME) + " " + nick + " :Erroneous nickname");
        return 0;
    }
    for (int i = 0; i < (int)Manager::getClient().size(); i++) {
        if (Manager::getClient()[i].getNickName() == nick) {
                client.setLastNick(nick);
                Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NICKNAMEINUSE) + nick + " :Nickname is already in use");
            return 0;
        }
    }
    return 1;
}

int Parser::joinParse(Client &client)
{
    std::vector<std::string> command = client.getCommand();
    std::string channelName = "";
    std::string pass = "";
    if (command.size() < 2){
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " JOIN ERROR :Not enough parameters");
        return 0;
    }
    if ((int)command[1].find(" ") >= 0) {
        pass = command[1].substr(command[1].find(" ") + 1, command[1].size());
        channelName = command[1].substr(0, command[1].find(" "));
    }
    else
        channelName = command[1];


    // too many channels
    if ((int)command[1].find(",") != -1){
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, TOOMANYCHANNELS) + " :Too many channels");
        return 0;
    }
    //check channel name for # and spaces
    if (command[1][0] != '#' || (int)channelName.find(" ") >= 0){
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, BADCHANNELNAME) + " :Bad channel name");
        return 0;
    }
    //no nick cant join
    if (client.getNickName() == ""){
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERRONEUSNICKNAME) + " :Can't join without Nickname");
        return 0;
    }
    if (Manager::getChannels().find(channelName) != Manager::getChannels().end()){
        //check modes
        //Check if the client is already in the channel
        if (Manager::getChannels().find(channelName)->second.checkClient(client.getId())) {
            Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_ALREADYREGISTERED) + " " + channelName + " :You're already in that channel");
            return 0;
        }
        Channel &target = Manager::getChannels().find(channelName)->second;
        //check if pass is correct
        if (target.getKey().size() && target.getKey() != pass) {
            std::cout << "pass = " << target.getKey() << "; trying pass: " << pass << std::endl;
            Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, BADCHANNELKEY) + " :Bad channel key");
            return 0;
        }
        //check if is invited
        if (target.getModeI() && !target.IsInvited(client.getId())) {
            Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, INVITEONLYCHAN) + " :Invited only channel");
            return 0;
        }
        //check if is full
        if (target.getModeL() && target.howManyClients() >= target.getModeL()) {
            Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, CHANNELISFULL) + " :Channel is full");
            return 0;
        }
    }
    return 1;
}

int Parser::inviteParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    // std::cout << "cmd = " << command[1] << std::endl << "find == " << (int)command[1].find(" ") << std::endl;
    if (command.size() < 2 || (int)command[1].find(" ") < 0) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " INVITE ERROR :Not enough parameters");
        return (0);
    }
    std::string channelName = command[1].substr(command[1].find(" ") + 1, command[1].size());
    std::string nickName = command[1].substr(0, command[1].find(" "));
    //channelName.pop_back();
    int target = Manager::getIDbyNick(nickName);

    // no such user
    if (target == -1) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOSUCHNICK) + " " + nickName + " :No such nick");
        return (0);
    }

    //if channel doesnt exist reject
    if (Manager::getChannels().find(channelName) == Manager::getChannels().end()){
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel");
        return (0);
    }

    //check if it is on channel
    if (!Manager::getChannels().find(channelName)->second.checkClient(client.getId())) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, USERNOTINCHANNEL) + " " + channelName + " :You're not on that channel");
        return (0);
    }

    //check I mode on and if is op
    if (Manager::getChannels().find(channelName)->second.getModeI() && \
    !Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOTCHANOP) + " " + channelName + " :You're not channel operator");
        return (0);
    }

    //if user already in channel
    if (Manager::getChannels().find(channelName)->second.checkClient(target)) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_USERONCHANNEL) + " " + nickName + " " + channelName + " :is already on channel");
        return (0);
    }
    return (1);
}

int Parser::kickParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2 || (int)command[1].find(" ") < 0) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " KICK ERROR :Not enough parameters");
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
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOSUCHNICK) + " " + user + " :No such nick");
        return (0);
    }

    //if channel doesnt exist reject
    if (Manager::getChannels().find(channelName) == Manager::getChannels().end()){
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel");
        return (0);
    }

    //is op to kick
    if (!Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOTCHANOP) + " " + channelName + " :You're not channel operator");
        return (0);
    }

    //if user not in channel
    if (!Manager::getChannels().find(channelName)->second.checkClient(target)) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, USERNOTINCHANNEL) + " " + user + " " + channelName + " :They aren't on that channel");
        return (0);
    }

    //cannot kick yourself
    if (client.getId() == target) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOPRIVILEGES) + " :Cannot kick yourself");
        Manager::sendIrcMessage(client.getId(), "481 :Cannot kick yourself");
        return (0);
    }
    return (1);
}

int Parser::topicParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " TOPIC ERROR :Not enough parameters");
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
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel");
        return (0);
    }

    //if user not in channel
    if (!Manager::getChannels().find(channelName)->second.checkClient(client.getId())) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOTONCHANNEL) + " " + channelName + " :You're not on that channel");
        return (0);
    }

    //is op and mode topic is on
    if (Manager::getChannels().find(channelName)->second.getModeT() && \
    !Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOTCHANOP) + " " + channelName + " :You're not channel operator");
        return (0);
    }
    return (1);
}

int Parser::modeParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " MODE ERROR :Not enough parameters");
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
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel");
        return (0);
    }
    //if channel exists there is no flag we send modes to user
    if (flag.empty()){
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, CHANNELMODEIS) + " " + channelName + ": " + Manager::getChannels().find(channelName)->second.getChannelModes());
        return (0);
    }
    //check if flag format is correct
    if (!flag.empty() && (flag.size() != 2 || (flag[0] != '+' && flag[0] != '-'))) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_NOSUCHCHANNEL) + " :Unknown MODE flag");
        return (0);
    }
    //check if it is op
    if (!Manager::getChannels().find(channelName)->second.IsOp(client.getId())) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOTCHANOP) + " " + channelName + " :You're not channel operator");
        return (0);
    }
    //check if there is such flag
    if (flag[1] != 'i' && flag[1] != 't' && flag[1] != 'k' && flag[1] != 'o' && flag[1] != 'l') {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, UNKNOWNCOMMAND) + " " + flag[1] + " :Unknown mode flag");
        return (0);
    }
    // std::cout << "flag = " << flag << "; arg = " << arg << "; e empty ta = " << (((flag[1] == 'k' && flag[0] == '+') || flag[1] != 'o') && arg.empty()) << std::endl;
    //check if several flags have their arguments
    if (((flag[1] == 'k' && flag[0] == '+') || (flag[1] == 'l' && flag[0] == '+') || flag[1] == 'o') && arg.empty()) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " MODE ERROR :Not enough parameters");
        return (0);
    }
    if (flag[1] == 'o') {
        //check if user exists
        int target = Manager::getIDbyNick(arg);
        if (target == -1) {
            Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NOSUCHNICK) + " " + arg + " :No such nick");
            return (0);
        }
    }
    // f flag has to be numeric +
    if (flag[1] == 'l' && (int)arg.find_first_not_of("0123456789", 0) >= 0) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " MODE ERROR :Numeric only");
            return (0);
    }
    return (1);
}

int Parser::whoParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    //normal who without mask
    if (command.size() < 2) {
        return (1);
    }
    std::string channelName;
    std::string arg = "";
    if ((int)command[1].find(" ") > 0) {
        channelName = command[1].substr(0, command[1].find(" "));
        arg = command[1].substr(command[1].find(" ") + 1, command[1].size());
    }
    else
        channelName = command[1];
    //if channel doesnt exist reject
    if (Manager::getChannels().find(channelName) == Manager::getChannels().end()){
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel");
        return (0);
    }
    //check for wrong who usage
    if (arg != "" && arg != "o") {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, UNKNOWNCOMMAND) + " : USAGE: WHO [<mask> [<o>]]");
        return (0);
    }
    return (1);
}

int Parser::userParse(Client &client) {
    std::vector<std::string> command = client.getCommand();
    if (command.size() < 2 || (int)command[1].find(":") < 0) {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, NEEDMOREPARAMS) + " USER ERROR :Not enough parameters");
        return (0);
    }
    std::string username = command[1].substr(0, command[1].find(" "));
    std::string theRest = command[1].substr(command[1].find(" ") + 1, 3);
    if (theRest != "0 *") {
        Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, UNKNOWNCOMMAND) + " : USAGE: USER <username> 0 * :<realname>");
    }
    for (int i = 0; i < (int)Manager::getClient().size(); i++) {
        if (Manager::getClient()[i].getUserName() == username) {
                Manager::sendIrcMessage(client.getId(), Manager::formatMessage(client, ERR_ALREADYREGISTERED) + ":You are already registered");
            return 0;
        }
    }
    std::string realname = command[1].substr(command[1].find(":") + 1, command[1].size());
    //wtf do i do with this shit
    return (1);
}

std::string& Parser::sanitizeBuffer(std::string& str) {
    static const std::string whitespace = "\t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        str.clear();
        return str;
    }
    str.erase(0, start);
    size_t end = str.find_last_not_of(whitespace);
    str.erase(end + 1);
    return str;
}
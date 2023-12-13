#include "../includes/Client.hpp"

Client::Client(int id, std::string hostname){
    _clientID = id;
    _nickName = "";
    _userName = "";
    _channel = "";
    _hostname = hostname;
    std::cout << "Client created with id: " << id << std::endl;
    std::cout << "nick == " << _nickName << std::endl;
}

Client::~Client(){}

Client &Client::operator=(const Client &src) {
    _clientID = src._clientID;
    _nickName = src._nickName;
    _userName = src._userName;
    _channel = src._channel;
    _cmd = src._cmd;
    return *this;
}


Client::Client(const Client &src) {
    *this = src;
}

int &Client::getId(){
    return _clientID;
}

std::stringstream &Client::getBuffer() {
	return (_buffer);
}

std::string &Client::getNickName() {
    return (_nickName);
}

void Client::setNickName(std::string nickName) {
    _nickName = nickName;
}

std::string &Client::getUserName() {
    return (_userName);
}

void Client::setUserName(std::string userName) {
    _userName = userName;
}

std::string &Client::getHostName() {
    return _hostname;
}

void Client::setHostName(std::string hostname) {
    _hostname = hostname;
}

void Client::setCommand(std::string cmd) {
    //cmd.pop_back();
    _cmd.clear();
    if (cmd[0] == '/')
        cmd.erase(0, 1);
    std::string holder = Parser::toUpper(cmd.substr(0, cmd.find(" ")));
    _cmd.push_back(holder);
    _cmd.push_back(cmd.substr(cmd.find(" ") + 1, cmd.size()));
    // for (unsigned long i = 0; i < _cmd.size(); i++) {
    //     std::cout << _cmd[i] << std::endl;
    // }
}

void Client::setregularCommand(std::string cmd) {
    _cmd.clear();
    if (cmd[0] == '/')
        cmd.erase(0, 1);
    // std::cout << "antes cmd = " << cmd << std::endl;
    // std::string holder = Parser::toUpper(cmd.substr(0, cmd.find(" ")));
    _cmd.push_back(cmd);
    // _cmd.push_back(cmd.substr(cmd.find(" ") + 1, cmd.size()));
    // std::cout << "after not entering: " << std::endl;
    // for (unsigned long i = 0; i < _cmd.size(); i++) {
    //     std::cout << _cmd[i] << std::endl;
    // }
}

std::vector<std::string> Client::getCommand(void) const {
   return (_cmd);
}

std::string &Client::getChannel() {
    return _channel;
}
void Client::setChannel(std::string channel) {
    _channel = channel;
}

std::string Client::getClientPrefix() {
    std::string clientPrefix = getNickName() + "!" + \
        getUserName() + "@" + getHostName();
    return clientPrefix;
}
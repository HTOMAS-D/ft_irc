#include "../includes/Client.hpp"

Client::Client(int id){
    _clientID = id;
    std::cout << "Client created with id: " << id << std::endl;
}

Client::~Client(){}

int &Client::getId(){
    return _clientID;
}

std::stringstream &Client::getBuffer() {
	return (_buffer);
}

std::string Client::getNickName() {
    return (_nickName);
}

void Client::setNickName(std::string nickName) {
    _nickName = nickName;
}

std::string Client::getUserName() {
    return (_userName);
}

void Client::setUserName(std::string userName) {
    _userName = userName;
}


void Client::setCommand(std::string cmd) {
    _cmd.clear();
    if (cmd[0] == '/')
        cmd.erase(0, 1);
    std::string holder = Parser::toUpper(cmd.substr(0, cmd.find(" ")));
    _cmd.push_back(holder);
    _cmd.push_back(cmd.substr(cmd.find(" ") + 1, cmd.size()));
    for (unsigned long i = 0; i < _cmd.size(); i++) {
        std::cout << _cmd[i] << std::endl;
    }
}

void Client::setregularCommand(std::string cmd) {
    _cmd.clear();
    if (cmd[0] == '/')
        cmd.erase(0, 1);
    // std::cout << "antes cmd = " << cmd << std::endl;
    // std::string holder = Parser::toUpper(cmd.substr(0, cmd.find(" ")));
    _cmd.push_back(cmd);
    // _cmd.push_back(cmd.substr(cmd.find(" ") + 1, cmd.size()));
    std::cout << "after not entering: " << std::endl;
    for (unsigned long i = 0; i < _cmd.size(); i++) {
        std::cout << _cmd[i] << std::endl;
    }
}

std::vector<std::string> Client::getCommand(void) const {
   return (_cmd);
}
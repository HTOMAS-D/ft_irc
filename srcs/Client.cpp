#include "../includes/Client.hpp"

Client::Client(int id){
    _clientID = id;
    _nickName = "";
    _userName = "";
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
    if (cmd[0] == '/')
        cmd.erase(0, 1);
    std::cout << "antes cmd = " << cmd << std::endl;
    _cmd.push_back(cmd.substr(0, cmd.find(" ")));
    std::cout << "cmd = " << _cmd[0] << std::endl;
    _cmd.push_back(cmd.substr(cmd.find(" ") + 1, cmd.size()));
    std::cout << "cmd1 = " << _cmd[1] << std::endl;

}

std::vector<std::string> Client::getCommand(void) const {
   return (_cmd);
}
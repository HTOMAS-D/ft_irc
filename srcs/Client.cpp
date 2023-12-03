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
	_cmd = cmd;
}

std::string Client::getCommand(void) const {
   return (_cmd);
}
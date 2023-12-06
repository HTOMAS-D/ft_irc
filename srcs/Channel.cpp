#include "../includes/Channel.hpp"

Channel::Channel(std::string id){
    _channelId = id;
    _key = "";
    std::cout << "Channel created with id: " << id << std::endl;
}

Channel::Channel(std::string id, std::string pass){
    _channelId = id;
    _key = pass;
    std::cout << "Channel created with id: " << id << std::endl;
}

Channel::Channel(std::string id, std::string pass, std::string topic){
    _channelId = id;
    _key = pass;
    _topic = topic;
    std::cout << "Channel created with id: " << id << std::endl \
    << "With topic = " << _topic << std::endl;
}

Channel::~Channel(){}

void    Channel::addClient(int newClient) {
    std::stringstream temp;
    for(int i = 0; i < (int)_Clients.size(); i++){
        if(_Clients[i] == newClient){
           return ;
        }
    }
    _Clients.push_back(newClient);
    Manager::getClientByID(newClient)->setChannel(_channelId);
    temp << Manager::getClientByID(newClient)->getNickName() << " has been added to the channel!";
    channelMessage(temp.str().c_str());
}

void    Channel::addClientToOp(int newOp) {
    std::string temp = "You have been promoted to Operator";
    for(int i = 0; i < (int)_ClientOperators.size(); i++){
        if(_ClientOperators[i] == newOp){
           return ;
        }
    }
    _ClientOperators.push_back(newOp);
    send(newOp, temp.c_str(), temp.size(), 0);
}

void    Channel::removeClient(int id) { //SERA QUE DEVO RETIRAR TAMBEM JA DOS OPS???
    for(int i = 0; i < (int)_Clients.size(); i++){
        if(_Clients[i] == id){
            _Clients.erase(_Clients.begin() + i);
        }
    }
}

void    Channel::removeOp(int id) {
    std::string temp = "You have been demoted to normie\n";
    for(int i = 0; i < (int)_ClientOperators.size(); i++){
        if(_ClientOperators[i] == id){
            _ClientOperators.erase(_ClientOperators.begin() + i);
        }
    }
    send(id, temp.c_str(), temp.size(), 0);
}

void	Channel::channelMessage(const char *msg) {
    std::stringstream temp;
    temp << "Channel " << _channelId << ": "<< msg << std::endl;
	for(int i = 0; i < (int)_Clients.size(); i++){
		if (send(_Clients[i], temp.str().c_str(), temp.str().size(), 0) == -1)
			std::cout << "error sending" << std::endl;
    }
}

void    Channel::clientMessage(int client, const char *msg) {
    std::stringstream temp;
    temp << Manager::getClientByID(client)->getNickName() << ": "<< msg;
	for(int i = 0; i < (int)_Clients.size(); i++){
		if (i != client && send(_Clients[i], temp.str().c_str(), temp.str().size(), 0) == -1)
			std::cout << "error sending" << std::endl;
    }
}

std::string &Channel::getChannelId() {
    return _channelId;
}

std::string &Channel::getKey() {
    return _key;
}

std::string &Channel::getTopic() {
    return _topic;
}

int &Channel::getModeI() {
    return _ModeI;
}

int &Channel::getModeT() {
    return _ModeT;
}

int Channel::IsOp(int id) {
	for(int i = 0; i < (int)_ClientOperators.size(); i++){
        if(_ClientOperators[i] == id){
            return (1);
        }
    }
	return 0;
}

 std::vector<int> &Channel::getClients() {
    return _Clients;
 }

void Channel::setModeI() {
    if (_ModeI)
        _ModeI = 0;
    else
        _ModeI = 1;
}

void Channel::setModeT() {
    if (_ModeT)
        _ModeT = 0;
    else
        _ModeT = 1;
}
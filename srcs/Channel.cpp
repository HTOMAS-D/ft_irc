#include "../includes/Channel.hpp"

Channel::Channel(std::string id){
    _channelId = id;
    _key = "";
    _topic = "";
    _ModeI = 0;
    _ModeT = 0;
    std::cout << "Channel created with id: " << id << std::endl;
}

Channel::Channel(std::string id, std::string pass){
    _channelId = id;
    _key = pass;
    _topic = "";
    _ModeI = 0;
    _ModeT = 0;
    std::cout << "Channel created with id: " << id << std::endl;
}

Channel::Channel(std::string id, std::string pass, std::string topic){
    std::cout << "aqui 3" << std::endl;
    _channelId = id;
    _key = pass;
    _topic = topic;
    _ModeI = 0;
    _ModeT = 0;
    std::cout << "Channel created with id: " << id << std::endl \
    << "Topic = " << _topic << std::endl;
    // << "Pass = " << _key << std::endl 
    // << "ModeI = " << _ModeI << std::endl
    // << "ModeT = " << _ModeT << std::endl;
}

Channel::Channel(const Channel &src) {
    std::cout << "aqui 1" << std::endl;
    _channelId = src._channelId;
    _key = src._key;
    _topic = src._topic;
    _ModeI = src._ModeI;
    _ModeT = src._ModeT;
}

Channel &Channel::operator=(const Channel &src) {
    std::cout << "aqui 2" << std::endl;
    _channelId = src._channelId;
    _key = src._key;
    _topic = src._topic;
    _ModeI = src._ModeI;
    _ModeT = src._ModeT;
    return (*this);
}

Channel::~Channel(){}

void    Channel::addClient(int newClient) {
    std::stringstream temp;
    // for(int i = 0; i < (int)_Clients.size(); i++){
    //     if(_Clients[i] == newClient){
    //        return ;
    //     }
    // }
    _Clients.push_back(newClient);
    removeInvited(newClient);
    Manager::getClientByID(newClient)->setChannel(_channelId);
    temp << Manager::getClientByID(newClient)->getNickName() << " has been added to the channel!" << std::endl;
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

void    Channel::addInvited(int newInvited) {
    for(int i = 0; i < (int)_invited.size(); i++){
        if(_invited[i] == newInvited){
           return ;
        }
    }
    _invited.push_back(newInvited);
}

void    Channel::removeClient(int id) { //SERA QUE DEVO RETIRAR TAMBEM JA DOS OPS???
    for(int i = 0; i < (int)_Clients.size(); i++){
        if(_Clients[i] == id){
            _Clients.erase(_Clients.begin() + i);
        }
    }
    Manager::getClientByID(id)->setChannel("");
    removeOp(id);
}

void    Channel::removeOp(int id) {
    for(int i = 0; i < (int)_ClientOperators.size(); i++){
        if(_ClientOperators[i] == id){
            _ClientOperators.erase(_ClientOperators.begin() + i);
        }
    }
}

void    Channel::removeInvited(int id) {
    for(int i = 0; i < (int)_invited.size(); i++){
        if(_invited[i] == id){
            _invited.erase(_invited.begin() + i);
        }
    }
}

void	Channel::channelMessage(std::string msg) {
    for(int i = 0; i < (int)_Clients.size(); i++){
        if (send(_Clients[i], msg.c_str(), msg.size(), 0) == -1)
            std::cout << "error sending" << std::endl;
    }
}

void    Channel::clientMessage(int client, const char *msg) {
    std::stringstream temp;
    temp << Manager::getClientByID(client)->getNickName() << ": "<< msg;
	for(int i = 0; i < (int)_Clients.size(); i++){
        if (_Clients[i] != client && send(_Clients[i], temp.str().c_str(), temp.str().size(), 0) == -1)
            std::cout << "error sending" << std::endl;
    }
}

bool Channel::checkClient(int clientId) const {
    // Iterate through the clients and check if the client is already in the channel
    for (std::vector<int>::const_iterator it = _Clients.begin(); it != _Clients.end(); ++it) {
        if (*it == clientId) {
            return true; // Client is already in the channel
        }
    }
    return false; // Client is not in the channel
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

std::string Channel::getChannelModes() {
    std::string modes;

    if (_ModeI)
        modes += "+i ";
    else
        modes += "-i ";
    if (_ModeT)
        modes += "+t ";
    else
        modes += "-t ";
    if (!_key.empty())
        modes += "+k";
    else
        modes += "-k";
    return (modes);
}

int Channel::IsOp(int id) {
	for(int i = 0; i < (int)_ClientOperators.size(); i++){
        if(_ClientOperators[i] == id){
            return (1);
        }
    }
	return 0;
}

int Channel::IsInvited(int id) {
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

std::vector<std::string> Channel::getNamesList() {
    std::vector<std::string> namesList;

    for (int i = 0; i < (int)_Clients.size(); i++) {
        int clientId = _Clients[i];
        std::string nickname = Manager::getNickbyID(clientId);
        namesList.push_back(nickname);
    }

    return namesList;
}

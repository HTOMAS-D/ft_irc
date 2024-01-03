#include "../includes/Channel.hpp"

Channel::Channel(std::string id){
    _channelId = id;
    _key = "";
    _topic = "";
    _ModeI = 0;
    _ModeT = 0;
    _limit = 0;
    std::cout << "Channel created with id: " << id << std::endl;
    Manager::addChannelName(id);
}

// Channel::Channel(const Channel &src) {
//     // std::cout << "aqui 1" << std::endl;
//     _channelId = src._channelId;
//     _key = src._key;
//     _topic = src._topic;
//     _ModeI = src._ModeI;
//     _ModeT = src._ModeT;
//     _limit = src._limit;
// }

Channel &Channel::operator=(const Channel &src) {
    _channelId = src._channelId;
    _key = src._key;
    _topic = src._topic;
    _ModeI = src._ModeI;
    _ModeT = src._ModeT;
    _limit = src._limit;
    return (*this);
}

Channel::~Channel(){
}

void    Channel::addClient(int newClient) {
    std::stringstream temp;
    //check if limit flag is up and restrict
    if (_limit && (int)_Clients.size() == _limit) //Talvez mudar isto po parse do join ?
        return ;
    _Clients.push_back(newClient);
    removeInvited(newClient);
    Manager::getClientByID(newClient)->setChannel(_channelId);
    if (_ClientOperators.size() == 0) {
        addClientToOp(newClient);
    }
    else {
        updateList();
    }
}

void    Channel::addClientToOp(int newOp) {
    for(int i = 0; i < (int)_ClientOperators.size(); i++){
        if(_ClientOperators[i] == newOp){
           return ;
        }
    }
    _ClientOperators.push_back(newOp);
        updateList();
}

void    Channel::addInvited(int newInvited) {
    for(int i = 0; i < (int)_invited.size(); i++){
        if(_invited[i] == newInvited){
           return ;
        }
    }
    _invited.push_back(newInvited);
}

void    Channel::removeClient(int id) {
    for(int i = 0; i < (int)_Clients.size(); i++){
        if(_Clients[i] == id){
            _Clients.erase(_Clients.begin() + i);
        }
    }
    Manager::getClientByID(id)->setChannel(""); //problematico se client tiver em varios channels
    if (IsOp(id)) {
        removeOp(id);
    }
    else
        updateList();
}

void    Channel::removeOp(int id) {
    for(int i = 0; i < (int)_ClientOperators.size(); i++){
        if(_ClientOperators[i] == id){
            _ClientOperators.erase(_ClientOperators.begin() + i);
        }
    }
    if ((int)_ClientOperators.size() == 0 && (int)_Clients.size()) {
        addClientToOp(_Clients[0]);
    }
    else
        updateList();
}

void    Channel::removeInvited(int id) {
    for(int i = 0; i < (int)_invited.size(); i++){
        if(_invited[i] == id){
            _invited.erase(_invited.begin() + i);
        }
    }
}

//Broadcast message to all clients in the channel
void	Channel::channelMessage(std::string msg) {
    for(int i = 0; i < (int)_Clients.size(); i++){
        std::cout << i << std::endl; 
        if (send(_Clients[i], msg.c_str(), msg.size(), 0) == -1)
            std::cout << "error sending" << std::endl;
    }
}

//Send message to all clients in the channel except the sender
void    Channel::clientMessage(int client, const char *msg) {
	for(int i = 0; i < (int)_Clients.size(); i++){
        if (_Clients[i] != client && Manager::sendIrcMessage((int)_Clients[i], msg) == -1) {
            std::cout << "error sending" << std::endl;
        }
    }
}

bool Channel::checkClient(int clientId) const {
    for (int i = 0; i < (int)_Clients.size(); i++) {
        if (_Clients[i] == clientId)
            return (true);
    }
    return (false);
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

int &Channel::getModeL() {
    return _limit;
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
        modes += "+k ";
    else
        modes += "-k ";
    if (!_limit)
        modes += "-l";
    else
        modes += "+l";
    return (modes);
}

int Channel::howManyClients() {
    return(_Clients.size());
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
	for(int i = 0; i < (int)_invited.size(); i++){
        if(_invited[i] == id){
            return (1);
        }
    }
	return 0;
}

std::vector<int> &Channel::getClients() {
    return _Clients;
}

std::vector<int> &Channel::getOps() {
    return _ClientOperators;
}


void Channel::setModeI(int flag) {
    if (flag)
        _ModeI = 1;
    else
        _ModeI = 0;
}

void Channel::setModeT(int flag) {
    if (flag)
        _ModeT = 1;
    else
        _ModeT = 0;
}

void Channel::setModeL(int flag) {
    _limit = flag;
}

std::vector<std::string> Channel::getNamesList() {
    std::vector<std::string> namesList;

    for (int i = 0; i < (int)_Clients.size(); i++) {
        int clientId = _Clients[i];
        std::string nickname;
        if (IsOp(clientId))
            nickname = "@" + Manager::getNickbyID(clientId);
        else
            nickname = "%" +Manager::getNickbyID(clientId);
        namesList.push_back(nickname);
    }

    return namesList;
}

void Channel::updateList() {
    for (int i = 0; i < (int)_Clients.size(); i++) {
        Manager::sendNamesList(_channelId, *Manager::getClientByID(_Clients[i]));
    }
}
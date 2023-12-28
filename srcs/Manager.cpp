#include "../includes/Manager.hpp"

std::vector<Client> Manager::_clients;
std::map<std::string, eventFunction> Manager::_actionMap;
std::map<std::string, Channel> Manager::_channels;
std::string hostName = "localhost";

void Manager::addClient(int id){
    _clients.push_back(Client(id));
    std::cout << "Client " << id << " added to Clients" << std::endl;
}

void Manager::removeClient(int id){
    for(int i = 0; i < (int)_clients.size(); i++){
        if(_clients[i].getId() == id){
            _clients.erase(_clients.begin() + i);
            std::cout << "Client " << id << " removed from Clients" << std::endl;
        }
    }
}

std::vector<Client> &Manager::getClient() {
	return (_clients);
}

std::stringstream &Manager::getClientBuffer(int i) {
	std::vector<Client>::iterator it = _clients.begin();

	for ( ; it != _clients.end() ; it++) {
		if (it->getId() == i)
			break;
	}
	return (it->getBuffer());
}

std::vector<Client>::iterator Manager::getClientByID(int i) {
	std::vector<Client>::iterator it = _clients.begin();

	for ( ; it != _clients.end() ; it++) {
		if (it->getId() == i)
			break;
	}
	return (it);
}


std::map<std::string, eventFunction> &Manager::getActionMap(){
	return _actionMap;
}

std::map<std::string, Channel> &Manager::getChannels() {
	return _channels;
}

std::string Manager::getNickbyID(int id) {
	std::vector<Client>::iterator it = _clients.begin();
	for ( ; it != _clients.end() ; it++) {
		if (it->getId() == id)
			break;
	}
	return (it->getNickName());
}

int Manager::getIDbyNick(std::string nick) {
	std::vector<Client>::iterator it = _clients.begin();
	for ( ; it != _clients.end() ; it++) {
		if (it->getNickName() == nick)
			return (it->getId());
	}
	return (-1);
}

std::vector<int> Manager::getAllClientsIds() {
	std::vector<int> res;
	for(int i = 0; i < (int)_clients.size(); i++) {
		res.push_back(_clients[i].getId());
	}
	return (res);
}

int Manager::normalMsg(Client &client) {
	client.getBuffer().str("PRIVMSG " + client.getChannel() + " :" + client.getBuffer().str());
	client.setCommand(client.getBuffer().str());
	return (1);
}
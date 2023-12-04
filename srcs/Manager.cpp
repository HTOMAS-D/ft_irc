#include "../includes/Manager.hpp"

std::vector<Client> Manager::_clients;
std::map<std::string, eventFunction> Manager::_actionMap;
std::map<std::string, Channel> Manager::_channels;


void Manager::createChannels() {
	_channels.insert(std::make_pair<std::string, Channel>("#123", Channel("#123", "", "Exemplo")));
}

void Manager::addClient(int id){
    _clients.push_back(Client(id));
    std::cout << "Client " << id << " added to Clients" << std::endl;
	for (int i = 0; i < (int)_clients.size(); i++) {
		if(_clients[i].getId() == id) {
			_channels.find("#123")->second.addClient(_clients[i]); //HARDCODED FOR TESTING
		}
	}
}

void Manager::removeClient(int id){
    for(int i = 0; i < (int)_clients.size(); i++){
        if(_clients[i].getId() == id){
            _clients.erase(_clients.begin() + i);
			_channels.find("#123")->second.removeClient(id); //HARDCODED FOR TESTING
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
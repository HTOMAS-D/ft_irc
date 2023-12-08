#include "../includes/Manager.hpp"

std::vector<Client> Manager::_clients;
std::map<std::string, eventFunction> Manager::_actionMap;
std::map<std::string, Channel> Manager::_channels;


void Manager::createChannels() {
	_channels.insert(std::make_pair<std::string, Channel>("#123", Channel("#123", "", "Exemplo")));
	_channels.insert(std::make_pair<std::string, Channel>("#567", Channel("#567", "", "Other")));

}

void Manager::addClient(int id, std::string hostname){
    _clients.push_back(Client(id, hostname));
    std::cout << "Client " << id << " added to Clients" << std::endl;
	//_channels.find("#123")->second.addClient(id); //HARDCODED FOR TESTING
}

void Manager::removeClient(int id){
    for(int i = 0; i < (int)_clients.size(); i++){
        if(_clients[i].getId() == id){
            _clients.erase(_clients.begin() + i);
            std::cout << "Client " << id << " removed from Clients" << std::endl;
        }
    }
	//HARDCODED FOR TESTING
	for (int i = 0; i < (int)_channels.find("#123")->second.getClients().size(); i++) {
		if (_channels.find("#123")->second.getClients()[i] == id)
			_channels.find("#123")->second.removeClient(id);
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
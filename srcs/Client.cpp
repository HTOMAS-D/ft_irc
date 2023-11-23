#include "../includes/Client.hpp"

Client::Client(int id){
    _clientID = id;
    std::cout << "Client created with id: " << id << std::endl;
}

Client::~Client(){}

int Client::getId(){
    return _clientID;
}
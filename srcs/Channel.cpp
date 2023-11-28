#include "../includes/Channel.hpp"

Channel::Channel(int id){
    _channelId = id;
    std::cout << "Channel created with id: " << id << std::endl;
}

Channel::~Channel(){}

int &Channel::getChannelId(){
    return _channelId;
}

int Channel::IsOp(int id) {
	for(int i = 0; i < (int)_ClientOperators.size(); i++){
        if(_ClientOperators[i].getId() == id){
            return (1);
        }
    }
	return 0;
}

void	Channel::sendMessage(char *msg) {
	for(int i = 0; i < (int)_Clients.size(); i++){
		if (send(_Clients[i].getId(), msg, strlen(msg), 0) == -1)
			std::cout << "error sending" << std::endl;
    }
}
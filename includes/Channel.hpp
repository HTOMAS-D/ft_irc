#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Socket.hpp"

class Client;

class Channel{
    private:
		int					_channelId; // channel name?
        std::vector<Client> _ClientOperators; //talvez mapa
        std::vector<Client> _Clients; // Tem todos os clients incluindo ops, para mandar msg a todos
    public:
		Channel(int id);
		Channel(const Channel &src) {_channelId = src._channelId;}
		Channel &operator=(const Channel &src) {_channelId = src._channelId; return *this;}
        ~Channel();

		//add client to op
		//add client ENTROU
		//kick someone 
		//other channel cmds

		void sendMessage(char *msg);

		//GETS
		int &getChannelId();
		int IsOp(int i);
};

#endif
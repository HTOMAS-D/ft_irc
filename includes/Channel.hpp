#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Socket.hpp"

class Client;

class Channel{
    private:
		std::string			_channelId; // channel name?
        std::vector<int> _ClientOperators; //talvez mapa
        std::vector<int> _Clients; // Tem todos os clients incluindo ops, para mandar msg a todos
		std::string			_key; //password used has flag too
		std::string			_topic; // descricao do channel
		//flags de MODES
		int					_ModeI; // Invite only
		int					_ModeT; // restrictions of the topic cmd to channel ops
    public:
		Channel() {;}
		Channel(std::string id);
		Channel(std::string id, std::string pass);
		Channel(std::string id, std::string pass, std::string topic);
		Channel(const Channel &src) {_channelId = src._channelId;}
		Channel &operator=(const Channel &src) {_channelId = src._channelId; return *this;}
        ~Channel();

		//add client ENTROU
		void addClient(int newClient);
		//add client to op
		void addClientToOp(int newOp);
		//remove someone
		void removeClient(int id);
		//remove a client from op
		void removeOp(int id);
		//other channel cmds

		void channelMessage(const char *msg);
		void clientMessage(int client, const char *msg);
		//GETS

		std::string &getChannelId();
		std::string &getKey();
		std::string &getTopic();
		int &getModeI();
		int &getModeT();
		int IsOp(int i);
		std::vector<int> &getClients();
		//SETS
		void setKey(std::string pass) {_key = pass;}
		void setTopic(std::string topic) {_topic = topic;}
		void setModeI();
		void setModeT();

		bool checkClient(int clientId) const;
		std::vector<std::string> getNamesList();
};

#endif
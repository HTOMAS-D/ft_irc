#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Socket.hpp"

class Client;

class Channel{
    private:
		std::string			_channelId; // channel name?
        std::vector<int> _ClientOperators; //talvez mapa
        std::vector<int> _Clients; // Tem todos os clients incluindo ops, para mandar msg a todos
		std::vector<int> _invited; //users invited to channel
		//flags de MODES
		std::string			_key; //password used has flag too
		std::string			_topic; // descricao do channel
		int					_ModeI; // Invite only
		int					_ModeT; // restrictions of the topic cmd to channel ops
		int					_limit; // limit restriction
    public:
		Channel() {;}
		Channel(std::string id);
		//Channel(const Channel &src);
		Channel &operator=(const Channel &src);
        ~Channel();

		//add client ENTROU
		void addClient(int newClient);
		//add client to op
		void addClientToOp(int newOp);
		//add client to invited
		void addInvited(int newInvited);
		//remove someone
		void removeClient(int id);
		//remove a client from op
		void removeOp(int id);
		//remove invited
		void removeInvited(int id);
		//other channel cmds

		//update list of users in channel to everyone
		void updateList();

		void channelMessage(std::string msg);
		void clientMessage(int client, const char *msg);
		//GETS

		std::string &getChannelId();
		std::string &getKey();
		std::string &getTopic();
		int &getModeI();
		int &getModeT();
		int &getModeL();
		std::string getChannelModes();
		int IsOp(int i);
		int IsInvited(int i);
		std::vector<int> &getClients();
		std::vector<int> &getOps();
		int howManyClients();
		//SETS
		void setKey(std::string pass) {_key = pass;}
		void setTopic(std::string topic) {_topic = topic;}
		void setModeI(int flag);
		void setModeT(int flag);
		void setModeL(int flag);

		bool checkClient(int clientId) const;
		std::vector<std::string> getNamesList();
};

#endif
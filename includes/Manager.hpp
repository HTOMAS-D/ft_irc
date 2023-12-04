#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Socket;
class Client;

typedef void (*eventFunction)(Client& client);

class Manager{
    private:
        static std::vector<Client> _clients;
        //static std::vector<Channel> _channels;
        static std::map<std::string, eventFunction> _actionMap;

    public:
        static void addClient(int id);
        static void removeClient(int id);
		static std::vector<Client> &getClient();
		static std::vector<Client>::iterator getClientByID(int i);
		static std::stringstream &getClientBuffer(int i);

        static void createMap();
        static void joinAction(Client &client);
        static void kickAction(Client &client );
        static void inviteAction(Client &client );
        static void topicAction(Client &client );
        static void modeAction(Client &client );
        static void runActions(Client &client);
        static int	sendIrcMessage(std::string message, int clientId);
        static std::map<std::string, std::string> getChannelMap(std::string cmd);
        static std::map<std::string, eventFunction> getActionMap();
};

#endif
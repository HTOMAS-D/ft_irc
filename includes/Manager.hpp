#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Socket;
class Client;
class Channel;

typedef void (*eventFunction)(Client& client);

class Manager{
    private:
        static std::vector<Client> _clients;
        static std::map<std::string, Channel> _channels;
        static std::map<std::string, eventFunction> _actionMap;

    public:
        static void addClient(int id, std::string hostname);
        static void removeClient(int id);
		static std::vector<Client> &getClient();
		static std::vector<Client>::iterator getClientByID(int i);
		static std::stringstream &getClientBuffer(int i);
        static std::string getNickbyID(int id);
        static int getIDbyNick(std::string nick);

        static void createChannels();

        static void createMap(void);
        static void joinAction(Client &client);
        static void kickAction(Client &client );
        static void inviteAction(Client &client );
        static void topicAction(Client &client );
        static void modeAction(Client &client );
        static void nickAction(Client &client);
        static void runActions(Client &client);
        static int	sendIrcMessage(std::string message, int clientId);
        static std::map<std::string, eventFunction> &getActionMap();
        static std::map<std::string, Channel> &getChannels();

        static void sendNamesList(const std::string &channelName, Client &client);

};
#endif
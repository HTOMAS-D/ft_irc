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
        static std::string _password;
        static std::vector<Client> _clients;
        static std::vector<std::string> _channelNames;
        static std::map<std::string, Channel> _channels;
        static std::map<std::string, eventFunction> _actionMap;

    public:
        static std::string hostName;
        static void addClient(int id);
        static void removeClient(int id);
		static std::vector<Client> &getClient();
		static std::vector<Client>::iterator getClientByID(int i);
		static std::stringstream &getClientBuffer(int i);
        static std::string getNickbyID(int id);
        static int getIDbyNick(std::string nick);
        static std::vector<int> getAllClientsIds();
        static std::vector<std::string> getChannelNames();
        static void addChannelName(std::string name);

        static void createMap(void);
        static void joinAction(Client &client);
        static void kickAction(Client &client );
        static void inviteAction(Client &client );
        static void topicAction(Client &client );
        static void modeAction(Client &client );
        static void nickAction(Client &client);
        static void whoAction(Client &client);
        static void userAction(Client &client);
        static void runActions(Client &client);
        static void privmsgAction(Client &client);
        static void passAction(Client &client);
        static void quitAction(Client &client);
        static void capAction(Client &client);
        static int	sendIrcMessage(int clientId, std::string message);
        static void sendWhoMessage(const std::vector<int> &list, Client &client, std::string const &channelName);
        static std::map<std::string, eventFunction> &getActionMap();
        static std::map<std::string, Channel> &getChannels();
        static int normalMsg(Client &client);

        static void removeClientFromChannels(Client &client);
        static void sendNamesList(const std::string &channelName, Client &client);
        static std::string formatMessage(Client &client);
        static std::string formatMessage(Client &client, std::string message);
        //static std::string formatMessage(Channel &_channel, std::string &message);

        static int isClient(int id);

        static std::string &getPassword();
        static void setPassword(std::string password);
};

#endif
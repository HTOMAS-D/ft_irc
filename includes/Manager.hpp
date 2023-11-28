#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "Socket.hpp"

class Socket;
class Client;

class Manager{
    private:
        static std::vector<Client> _clients;
    public:
        static void addClient(int id);
        static void removeClient(int id);
		static std::vector<Client> &getClient();
		static std::vector<Client>::iterator getClientByID(int i);
		static std::stringstream &getClientBuffer(int i);
};

#endif
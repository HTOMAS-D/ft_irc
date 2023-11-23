#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <sstream>

#include "Socket.hpp"

class Client{
    private:
		std::stringstream _buffer;
		int _clientID;
    public:
        Client(int id);
		Client(const Client &src) {_clientID = src._clientID;}
		Client &operator=(const Client &src) {_clientID = src._clientID; return *this;}
        ~Client();

        //GETTERS
        int getId();
		std::stringstream &getBuffer();
};


#endif
#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Socket.hpp"

class Client{
    private:
        int _clientID;
    public:
        Client(int id);
        ~Client();

        //GETTERS
        int getId();
};


#endif
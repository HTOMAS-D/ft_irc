#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Socket.hpp"

class Client{
    private:
		std::stringstream _buffer;
		int _clientID;
		std::string _nickName;
		std::string _userName;
		std::vector<std::string> _cmd;
    public:
		Client(){};
        Client(int id);
		Client(const Client &src) {_clientID = src._clientID;}
		Client &operator=(const Client &src) {_clientID = src._clientID; return *this;}
        ~Client();

        //GETTERS
		std::string getNickName();
		void setNickName(std::string nickName);
		std::string getUserName();
		void setUserName(std::string userName);
        int &getId();
		std::stringstream &getBuffer();

		std::vector<std::string> getCommand( void ) const;
		// void setCommand(std::vector <std::string> cmd);
		void setCommand(std::string cmd);
		void setregularCommand(std::string cmd);

		
};


#endif
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
		std::string _channel;
		std::string _hostname;
        //GETTERS

    public:
		Client(){};
        Client(int id, std::string hostname);
		Client(const Client &src);
		Client &operator=(const Client &src);
        ~Client();


		std::string &getChannel();
		void setChannel(std::string channel);
		std::string &getNickName();
		void setNickName(std::string nickName);
		std::string &getUserName();
		void setUserName(std::string userName);
        int &getId();
		std::string &getHostName();
		void setHostName(std::string hostname);
		std::stringstream &getBuffer();
		
		std::string getClientPrefix() ;

		std::vector<std::string> getCommand( void ) const;
		// void setCommand(std::vector <std::string> cmd);
		void setCommand(std::string cmd);
		void setregularCommand(std::string cmd);


		
};


#endif
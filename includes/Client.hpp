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

    public:
		Client(){};
        Client(int id, std::string hostname);
		Client(const Client &src);
		Client &operator=(const Client &src);
        ~Client();

		//Getters
		std::string       &getChannel() ;
		std::string       &getNickName() ;
		std::string 	  &getUserName() ;
		std::string       &getHostName() ;
        int 			  getId() ;
		std::stringstream &getBuffer() ;
		std::string 	  getClientPrefix() ;
		std::vector<std::string> getCommand( void ) ;

		//Setters
		void setChannel(std::string channel);
		void setNickName(std::string nickName);
		void setUserName(std::string userName);
		void setHostName(std::string hostname);
		void setCommand(std::string cmd);
		void setregularCommand(std::string cmd);
};


#endif
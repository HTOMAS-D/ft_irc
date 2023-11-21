#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "FileManage.hpp"
#define BACKLOG 10 // connections waiting to be accepted ()
#define MAX_PORT 65535 // The maximum port number for TCP and UDP in the Internet Assigned Numbers Authority (IANA) range is 65535.
#define MIN_PORT 1023  // ports in the range from 0 to 1023 are known as well-known ports and are reserved for specific services. 
#define BUFFER_SIZE 2048 //number of bytes to handle the client received from the clients
// Ports from 1024 to 49151 are registered ports, and ports from 49152 to 65535 are dynamic or private ports. 
// Ideally it should be used in the range 49152 to 65535
class FileManage;

class Socket {
    private:
        std::string _port;
        std::string _password;
        fd_set _master; // main fd list with *listening* fd also there 
        fd_set _temp; //temp list for select()
        FileManage manager;
        int _socketFd;
        int _maxFd;
        void parseSocket();
        void initSocket();
        struct addrinfo * socketAddress();
        void startMainLoop();

    public:
        Socket(std::string port, std::string password);
        ~Socket(); 

        //GETTER AND SETTERS
        std::string &getPort();
        std::string &getPassword();
};


#endif
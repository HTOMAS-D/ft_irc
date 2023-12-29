#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Error.hpp"
#include "Manager.hpp"
#include "Parser.hpp"
#include "Client.hpp"

#define BACKLOG 10 // connections waiting to be accepted ()
#define MAX_PORT 65535 // The maximum port number for TCP and UDP in the Internet Assigned Numbers Authority (IANA) range is 65535.
#define MIN_PORT 1023  // ports in the range from 0 to 1023 are known as well-known ports and are reserved for specific services. 
#define BUFFER_SIZE 1000//number of bytes to handle the client received from the clients
// Ports from 1024 to 49151 are registered ports, and ports from 49152 to 65535 are dynamic or private ports. 
// Ideally it should be used in the range 49152 to 65535

#define MAXNAME 20

class FileManage;

class Socket {
    private:
        std::string _port;
        fd_set _master; // main fd list with *listening* fd also there 
        fd_set _temp; //temp list for select()
        int _socketFd;
        int _maxFd;
        void parsePortPass();
        void initSocket();
        struct addrinfo * socketAddress();
        void startMainLoop();
        void getData();
        void acceptedConnection(int newFd);
		void handleData(int i);
        void handleMessage(int i);

    public:
        Socket(std::string port, std::string password);
        ~Socket(); 

        void parsePortPass(std::string port, std::string password);
		std::vector<std::string> divideString(const std::string &str, char delim);
};


#endif
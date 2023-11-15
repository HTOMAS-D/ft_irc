#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <string>
#include <iostream>
#define BACKLOG 10 // connections waiting to be accepted ()
#define MAX_PORT 65535 // The maximum port number for TCP and UDP in the Internet Assigned Numbers Authority (IANA) range is 65535.
#define MIN_PORT 1023  // ports in the range from 0 to 1023 are known as well-known ports and are reserved for specific services. 

// Ports from 1024 to 49151 are registered ports, and ports from 49152 to 65535 are dynamic or private ports. 
// Ideally it should be used in the range 49152 to 65535


class Socket {
    private:
        std::string _port;
        std::string _password;
        void parseSocket();
        void initSocket();

    public:
        Socket(std::string port, std::string password);
        ~Socket(); 

        //GETTER AND SETTERS
        std::string &getPort();
        std::string &getPassword();
};


#endif
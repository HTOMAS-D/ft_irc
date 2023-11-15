#include "../includes/Socket.hpp"

// int socket(int domain, int type, int protocol);
// Domain:
//     AF_INET: IPv4 Internet protocols.
//     AF_INET6: IPv6 Internet protocols.

// Type:
//     SOCK_STREAM: Provides a reliable, connection-oriented, stream-based service (TCP).
//     SOCK_DGRAM: Provides an unreliable, connectionless, datagram-based service (UDP).

// Protocol:
//     0: The system selects a suitable protocol based on the combination of domain and type.
//     IPPROTO_TCP: TCP protocol (used with SOCK_STREAM).
//     IPPROTO_UDP: UDP protocol (used with SOCK_DGRAM).


// MAIN ORDER OF SETTING UP:
// getaddrinfo();
// socket();
// bind();
// listen();
// accept();  -> accepting will return a new socket_fd for this connection (you get the first fd from socket() and this one for send() and recv())


//CONSTRUCTORS AND DESTRUCTORS
Socket::Socket(std::string port, std::string password) : _port(port), _password(password){
    std::cout << "Socket Constructor called" << std::endl;
    this->parseSocket();
    this->initSocket();
}

Socket::~Socket(){
    std::cout << "Socket destroyed" << std::endl;
}


//GETTERS AND SETTERS
std::string &Socket::getPort(){return this->_port;}
std::string &Socket::getPassword(){return this->_password;}


//SOCKET FUNCTIONS
void Socket::parseSocket(){
    if(_port.length() <= 0 || atoi(_port.c_str()) <= MIN_PORT || atoi(_port.c_str()) > MAX_PORT){
        std::cout << "Invalid port" << std::endl;
        exit(1);
    }
    if(_password.length() == 0){
        std::cout << "Invalid Password" << std::endl;
        exit(1);
    }
}

void Socket::initSocket(){
    return ;
}
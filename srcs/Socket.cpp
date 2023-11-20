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

struct addrinfo *Socket::socketAddress(){
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    
    memset(&hints, 0, sizeof(hints)); // clear the array
    hints.ai_family = AF_UNSPEC; // IPv4   ///// may be : AF_UNSPEC (unspecified ipv4 or ipv6) You can set it to AF_INET or AF_INET6 if you want ipv4 or ipv6 respectivelly
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // ; this tells getaddrinfo() to assign the address of my local host to the socket structures.
    
    if ((status = getaddrinfo(NULL, _port.c_str(), &hints, &servinfo)) != 0) {
        std::cout << "getaddrinfo error: " <<  gai_strerror(status) << std::endl;
        exit(1);
    }
    // servinfo now points to a linked list of 1 or more struct addrinfos
    return(servinfo);
}

void Socket::initSocket(){
    struct addrinfo *server_socket, *p;
    int check_returns = 0;
    int yes = 1;


    server_socket = socketAddress();
    for (p = server_socket; p != NULL; p = p->ai_next){
        _socketFd = socket(server_socket->ai_family, server_socket->ai_socktype, server_socket->ai_protocol);
        if(_socketFd < 0){
            std::cout << "couldnt open the socket" << std::endl;
            continue;
        }
        // address already in use
        setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        check_returns = bind(_socketFd, server_socket->ai_addr, server_socket->ai_addrlen);
        if(check_returns < 0){
            std::cout << "couldnt bind the socket" << std::endl;
            close(_socketFd);
        }
        break ;
    }
    check_returns = listen(_socketFd, BACKLOG);
    if(check_returns < 0){
        std::cout << "couldnt listen" << std::endl;
        exit (-1);
    }
    std::cout << "back to init socket" << std::endl;
    freeaddrinfo(server_socket); // free the linked-list
}
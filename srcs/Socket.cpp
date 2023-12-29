#include "../includes/Socket.hpp"

//CONSTRUCTORS AND DESTRUCTORS
Socket::Socket(std::string port, std::string password) : _port(port){
    std::cout << "Socket Constructor called" << std::endl;
    FD_ZERO(&_master); // init fd list
    FD_ZERO(&_temp); // init fd list
    Manager::setPassword(password);
    parsePortPass(port, password);
    initSocket();
    startMainLoop();
}

Socket::~Socket(){
    std::cout << "Socket destroyed" << std::endl;
}

//SOCKET FUNCTIONS
void Socket::parsePortPass(std::string port, std::string password){
    if(port.length() <= 0 || atoi(port.c_str()) <= MIN_PORT || atoi(port.c_str()) > MAX_PORT){
        std::cout << "Invalid port" << std::endl;
        exit(1);
    }
    if(password.length() == 0){
        std::cout << "Invalid Password" << std::endl;
        exit(1);
    }
}

/* This function will return the servinfo that points to a linked list 
of 1 or more struct addrinfos each of which contains a 
struct sockaddr that contains the ip address. 
In brief, is used to obtain address information based on the provided hints 
to setup the socket communication.*/
struct addrinfo *Socket::socketAddress(){
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    
    memset(&hints, 0, sizeof(hints)); // clear the array
    hints.ai_family = AF_UNSPEC; // IPv4   ///// may be : AF_UNSPEC (unspecified ipv4 or ipv6) You can set it to AF_INET or AF_INET6 if you want ipv4 or ipv6 respectivelly
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // ; this tells getaddrinfo() to assign the address of my local host to the socket structures.
    
    // getaddrinfo() returns a list of address structures because 
    // it's common for a single host to have multiple ip addresses
    // meaning that can be connected to multiple networks at the same time
    if ((status = getaddrinfo(NULL, _port.c_str(), &hints, &servinfo)) != 0) {
        std::cout << "getaddrinfo error: " <<  gai_strerror(status) << std::endl;
        exit(1);
    }
    return(servinfo);
}

/* This function will create a socket and bind it to the port */
void Socket::initSocket(){
    struct addrinfo *server_socket, *p;
    int check_returns = 0;
    int yes = 1;

    server_socket = socketAddress(); 
    for (p = server_socket; p != NULL; p = p->ai_next){
        //create a socket for the server to listen to
        _socketFd = socket(server_socket->ai_family, server_socket->ai_socktype, server_socket->ai_protocol);
        if (_socketFd < 0) {
            std::cout << "couldnt open the socket" << std::endl;
            continue;
        }
        //avoid address already in use message
        setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        check_returns = bind(_socketFd, server_socket->ai_addr, server_socket->ai_addrlen);
        if(check_returns < 0) {
            std::cout << "couldnt bind the socket" << std::endl;
            close(_socketFd);
        }
        break;
    }
    if (p == NULL){
        std::cout << "Unable to bind" << std::endl;
        exit(1);
    }
    freeaddrinfo(server_socket); // free the linked-list
    check_returns = listen(_socketFd, BACKLOG);
    if (check_returns < 0){
        std::cout << "couldnt listen" << std::endl;
        exit (1);
    }
    std::cout << _socketFd << std::endl;
    FD_ZERO(&_master); //need it ?
    FD_SET(_socketFd, &_master);
    _maxFd =  _socketFd + 1; // Save the highest fd, and for the moment is this one
}

void Socket::startMainLoop(){
    Manager::createMap();
    while(1){
        _temp = _master; //start by copying the master to the temp fd_set
        //Monitor multiple fd's and will block until activity is 
        //detected on any of the descriptors or an error occurs.
        if (select((_maxFd+1), &_temp, NULL, NULL, NULL) == -1){
            std::cout << "Error selecting()" << std::endl;
            exit(4);
        }
		getData(); //process any incoming data from clients.
    }
}

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

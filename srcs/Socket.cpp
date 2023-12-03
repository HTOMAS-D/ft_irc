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
    FD_ZERO(&_master); // init fd list
    FD_ZERO(&_temp); // init fd list
    parsePortPass();
    initSocket();
    startMainLoop();
}

Socket::~Socket(){
    std::cout << "Socket destroyed" << std::endl;
}

//GETTERS AND SETTERS
std::string &Socket::getPort(){return this->_port;}
std::string &Socket::getPassword(){return this->_password;}

//SOCKET FUNCTIONS
void Socket::parsePortPass(){
    if(_port.length() <= 0 || atoi(_port.c_str()) <= MIN_PORT || atoi(_port.c_str()) > MAX_PORT){
        std::cout << "Invalid port" << std::endl;
        exit(1);
    }
    if(_password.length() == 0){
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

/* Iterate through successful connections to get data to read */
void	Socket::getData() {

    struct sockaddr_storage address; //struct to hold to store socket address (large enough to handle any struct, ipv4, ipv6 or other family)
    socklen_t addrlen; // used to represent the lenght of or size of socket structs
    int newFd;
	
	for(int i = 0; i < _maxFd; i++){
		if (FD_ISSET(i, &_temp)) // check if the fd is in the temp fd list
		{
			if(i == _socketFd){
				// this means we got a new connecion
				addrlen = sizeof(address);
				newFd = accept(_socketFd, (struct sockaddr *) &address, &addrlen);
				if (newFd == -1){
					std::cout << "Error accepting connection" << std::endl;
				}
				else{
					std::cout << "connection accepted" << std::endl;
					FD_SET(newFd, &_master); // add new fd to master fd list
					if (newFd > _socketFd){ // check if theres a new max FD
						_maxFd = newFd + 1;
					}
                    Manager::addClient(newFd);
				} 
			}
			//handle data from client
			else
				handleData(i);
		}
	}
}

/* Handle data from client */
void	Socket::handleData(int i) {
    int nbrBytes;
	char buffer[BUFFER_SIZE];

	bzero(buffer, sizeof(buffer));
	//start by checking if the activity is either error or client closed connection
	nbrBytes = recv(i, &buffer, sizeof(buffer) - 1, 0);
	if(nbrBytes <= 0){
		if (nbrBytes == 0){
			std::cout << "Client with socket " << i << " disconnected" << std::endl;
            Manager::removeClient(i);
		}
		else{
			std::cout << "Random receive error" << std::endl;
		}
		close(i);
		FD_CLR(i, &_master);
	}
	else{
		Manager::getClientBuffer(i).str(Manager::getClientBuffer(i).str() + buffer);
		//std::cout << "client buffer before while :" << Manager::getClientBuffer(i).str() << std::endl;
		int newLine = Manager::getClientBuffer(i).str().find('\n');
		while (newLine >= 0) {
			//std::cout << "new line = " << newLine << "|| buffer = " << Manager::getClientBuffer(i).str() << std::endl;
			std::string temp = Manager::getClientBuffer(i).str().substr(newLine + 1, Manager::getClientBuffer(i).str().size());
			//std::cout << "temp = " << temp << std::endl;
			//std::cout << "Manager para executar = " << Manager::getClientBuffer(i).str().substr(0, newLine + 1) << std::endl;
			Manager::getClientBuffer(i).str(Manager::getClientBuffer(i).str().substr(0, newLine + 1));
			std::cout << "[" << i << "]" << Manager::getClientBuffer(i).str();
			handleMessage(i); //handle message info ex. cmds usr info
			Manager::getClientBuffer(i).str(temp);
			newLine = Manager::getClientBuffer(i).str().find('\n');
			//std::cout << "New client buffer = " << Manager::getClientBuffer(i).str() << std::endl;
		}
	}
}

void    Socket::handleMessage(int i){
	std::stringstream temp;
	//temp is equal to USER_x: "msg" in send we use size to get nbr of bytes in the total msg
	temp << "USER_" << i << ": " << Manager::getClientBuffer(i).str();
    std::cout << "temp = " << temp.str() << std::endl;
    Manager::getClientByID(i)->setCommand(Manager::getClientBuffer(i).str());
    for (int j = 0; j < _maxFd; j++) {
		if (FD_ISSET(j, &_master)) {
            std::vector<Client>::iterator iter = Manager::getClientByID(i);
			// except the listener and ourselves
            if (iter != Manager::getClient().end() && j != _socketFd && j != i) {
                Manager::runActions(*iter);
            }
			// if (j != _socketFd && j != i) {
			// 	send(j, temp.str().c_str(), temp.str().size(), 0);
			// }
		}
	} 
}
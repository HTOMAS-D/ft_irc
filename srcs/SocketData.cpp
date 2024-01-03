#include "../includes/Socket.hpp"

/* Iterate through successful connections to get data to read */
void	Socket::getData() {

    struct sockaddr_storage address; //struct to hold to store socket address (large enough to handle any struct, ipv4, ipv6 or other family)
    socklen_t addrlen; // used to represent the lenght of or size of socket structs
    int newFd;
	
	for(int i = 0; i < _maxFd; i++){
		if (FD_ISSET(i, &_temp)) // check if the fd is in the temp fd list
		{
			if(i == _socketFd){
				// This means we got a new connecion
				addrlen = sizeof(address);
				newFd = accept(_socketFd, (struct sockaddr *) &address, &addrlen);
				if (newFd == -1){
					std::cout << "Error accepting connection" << std::endl;
				}
				else{
					FD_SET(newFd, &_master); // add new fd to master fd list
					if (newFd > _socketFd){ // check if theres a new max FD
						_maxFd = newFd + 1;
					}
                    acceptedConnection(newFd);
				} 
			}
			else
				handleData(i);
		}
	}
}

/* This function get the hostname for each client */
void    Socket::acceptedConnection(int newFd){
    // Use getpeername to obtain the address of the connecting client
    struct sockaddr_storage peerAddress;
    socklen_t peerAddrlen = sizeof(peerAddress);
    getpeername(newFd, (struct sockaddr*)&peerAddress, &peerAddrlen);

    char host[NI_MAXHOST];

    // Use inet_ntoa to convert the address to a string
    std::string ipAddress = inet_ntoa(((struct sockaddr_in*)&peerAddress)->sin_addr);

    // Use gethostbyname to obtain the hostname
    struct hostent* hostEntry = gethostbyname(ipAddress.c_str());
    if (hostEntry) {
        strncpy(host, hostEntry->h_name, NI_MAXHOST);
        host[NI_MAXHOST - 1] = '\0';
        std::cout << "Connection accepted from " << host << std::endl;
        Manager::addClient(newFd);
    } else {
        std::cerr << "gethostbyname failed" << std::endl;
    }
}

/* Handle data from client by fill in the buffer */
void	Socket::handleData(int i) {
    int nbrBytes;
	char buffer[BUFFER_SIZE];

	bzero(buffer, sizeof(buffer));

	//Client by nc has quit
	if (!Manager::isClient(i)) {
		close(i);
		FD_CLR(i, &_master);
		return ;
	}
	
    //Start by checking if the activity is either error or client closed connection
	nbrBytes = recv(i, &buffer, sizeof(buffer) - 1, 0);
	if(nbrBytes <= 0){
		if (nbrBytes == 0){
			std::cout << "Client with socket " << i << " disconnected" << std::endl;
			Manager::removeClientFromChannels(*Manager::getClientByID(i));
            Manager::removeClient(i);
		}
		else{
			std::cout << "Random receive error" << std::endl;
		}
		close(i);
		FD_CLR(i, &_master);
	}
	else{
        
        //Fill the client buffer with the data received
		Manager::getClientBuffer(i).str(Manager::getClientBuffer(i).str() + buffer);
		int newLine = Manager::getClientBuffer(i).str().find('\n');
		while (newLine >= 0) {
			std::string temp = Manager::getClientBuffer(i).str().substr(newLine + 1, Manager::getClientBuffer(i).str().size());
			Manager::getClientBuffer(i).str(Manager::getClientBuffer(i).str().substr(0, newLine + 1));
			std::cout << "[" << i << "]" <<  Manager::getClientBuffer(i).str();
			//Manager::sendIrcMessage(":PRIVMSG "+ Manager::getClientByID(i)->getChannel() + " :" + Manager::getClientBuffer(i).str() ,i);
			handleMessage(i); //handle message info ex. cmds usr info
			Manager::getClientBuffer(i).str(temp);
			newLine = Manager::getClientBuffer(i).str().find('\n');
		}
	}
}

void    Socket::handleMessage(int i){
	//to take out /r and /n
	std::string  temp = Manager::getClientBuffer(i).str();
	// std::cout << "Msg antes:" << temp << "; nr de cha:" << temp.size() << std::endl;
	Manager::getClientBuffer(i).str(Parser::sanitizeBuffer(temp));
	// std::cout << "Msg depois:" << Manager::getClientBuffer(i).str() << "; nr de cha:" << Manager::getClientBuffer(i).str().size() << std::endl;
    std::vector<Client>::iterator iter = Manager::getClientByID(i);
    Client &temporary = *Manager::getClientByID(i);
	temporary.setCommand(Manager::getClientBuffer(i).str());
    if (Parser::isAction(temporary.getCommand()[0], i) || (Manager::getClientByID(i)->getChannel().size() && Manager::normalMsg(temporary))) {
        Manager::runActions(*iter);
    }
    else {
        // if (Manager::getClientByID(i)->getChannel().size()) {
		// 	Manager::getClientBuffer(i).str("PRIVMSG " + temporary.getChannel() + " :" + Manager::getClientBuffer(i).str());
		// 	temporary.setCommand(Manager::getClientBuffer(i).str());
		// 	std::cout << Manager::getClientBuffer(i).str() << std::endl;
		// 	Manager::runActions(*iter);
        // }
        // else {
			std::cout << "sending regular your not in channel msg" << std::endl;
            send(i, "You are not in a channel, please join a channel!\n", 49, 0);
        // }
    }
}
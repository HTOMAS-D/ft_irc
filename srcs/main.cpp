#include "../includes/Socket.hpp"

int main(int ac, char **av){
    (void) av;
    if(ac != 3){
        std::cout << "Wrong number of arguments, try: ./ircserv <port> <password>" << std::endl;
        exit (1);
    }
    Socket socket_serv(av[1], av[2]);
}
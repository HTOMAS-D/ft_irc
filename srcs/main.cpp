#include <iostream>
#include "../includes/Socket.hpp"

int main(int ac, char **av){
    (void) av;
    if(ac != 2){
        std::cout << "Wrong number of arguments, try: ./ircserv <port> <password>" << std::endl;
        exit (1);
    }
}
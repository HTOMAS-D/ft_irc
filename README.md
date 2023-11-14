# ft_irc

Tutorial Link for Server Build:

https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf

https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands

https://www.youtube.com/watch?v=BDV8zK6Y8EU


# Concepts

IRC (Internet Relay Chat):

IRC is a text-based communication protocol used for real-time chat and messaging.
It allows users to join channels, send private messages, and participate in group discussions.
IRC Server:

An IRC server is a program that manages connections between IRC clients. It facilitates the exchange of messages among clients.
IRC Client:

An IRC client is a program or application that connects to an IRC server to send and receive messages. Users interact with the IRC server through the client.
Socket:

A socket is a communication endpoint that allows processes on different devices to communicate over a network. It is identified by an IP address and a port number.
Socket Programming:

Socket programming involves using sockets to establish communication between processes over a network. In C++, the <sys/socket.h> and <arpa/inet.h> headers are commonly used for socket programming.
Port:

A port is a 16-bit unsigned integer that identifies a specific process to which a message is to be delivered on a host. IRC servers typically listen on a specific port (e.g., 6667).
IP Address:

An IP (Internet Protocol) address is a numerical label assigned to each device connected to a computer network that uses the Internet Protocol for communication. IPv4 addresses are typically used in IRC (e.g., "192.168.1.1").
Listening:

When a server is "listening," it means it's actively waiting for incoming connections from clients.
Accepting Connections:

The server uses the accept() system call to accept incoming connection requests from clients.

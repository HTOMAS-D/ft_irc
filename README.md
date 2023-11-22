# ft_irc

Tutorial Link for Server Build:

https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf

https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands

https://www.youtube.com/watch?v=BDV8zK6Y8EU

https://modern.ircdocs.horse

# Concepts

- IRC (Internet Relay Chat):

  IRC is a text-based communication protocol used for real-time chat and messaging.
  It allows users to join channels, send private messages, and participate in group discussions.


- IRC Server:

  An IRC server is a program that manages connections between IRC clients. It facilitates the exchange of messages among clients.

- IRC Client:

  An IRC client is a program or application that connects to an IRC server to send and receive messages. Users interact with the IRC server through the client.

- Socket:

  A socket is a communication endpoint that allows processes on different devices to communicate over a network. It is identified by an IP address and a port number.

- Socket Programming:

  Socket programming involves using sockets to establish communication between processes over a network. In C++, the <sys/socket.h> and <arpa/inet.h> headers are commonly used for socket programming.

- Port:

  A port is a 16-bit unsigned integer that identifies a specific process to which a message is to be delivered on a host. IRC servers typically listen on a specific port (e.g., 6667).

- IP Address:

  An IP (Internet Protocol) address is a numerical label assigned to each device connected to a computer network that uses the Internet Protocol for communication. IPv4 addresses are typically used in IRC (e.g., "192.168.1.1").

- Listening:

  When a server is "listening," it means it's actively waiting for incoming connections from clients.

- Accepting Connections:

  The server uses the accept() system call to accept incoming connection requests from clients.


# Functions

- The socket() function is part of the sockets API and is commonly used for creating a socket, which is a communication endpoint for sending or receiving data over a network. 

- The close(serverSocket) function is used to close the socket when it is no longer needed.

- The setsockopt() function is used in C and C++ to set options on a socket. It allows you to configure various aspects of socket behavior.


- The getsockname() function is used in C and C++ to retrieve the local address to which a socket is bound. It's commonly used on a server-side socket after the socket has been bound to an address using bind.

- The getprotobyname() function is used in C and C++ to retrieve protocol information by specifying its name.

- The gethostbyname() function is a legacy function used in C and C++ to retrieve host information by specifying its name. It's important to note that gethostbyname() is considered deprecated, and its use is discouraged in favor of more modern and robust alternatives, such as getaddrinfo().

- The getaddrinfo() is a more modern and flexible function for resolving hostnames and service names to socket addresses. It is recommended over older functions like gethostbyname() and provides support for both IPv4 and IPv6. Additionally, getaddrinfo() can handle multiple addresses for a given host and supports service name resolution.

- The freeaddrinfo() function is used to free the memory allocated by the getaddrinfo() function when it's used to resolve hostnames and service names to socket addresses. When you call getaddrinfo(), it dynamically allocates memory to store information about the addresses it retrieves. It is your responsibility to release this memory when you are done using the results.


- The bind() function in C and C++ is used to associate a socket with a specific local address and port. This is a crucial step, especially for servers, as it allows them to "bind" to a specific network interface and port on the machine.

- The listen() function in C and C++ is used on a server-side socket to set it in a passive mode, allowing it to accept incoming connections from clients. After calling listen(), the server can use the accept() function to handle incoming connection requests.

- The accept() function in C and C++ is used on a server-side socket to accept an incoming connection from a client. It is typically used in a loop, where the server continuously accepts connections from clients and processes their requests.


- htons() stands for "host to network short." It is a function used in C and C++ programming, especially in network programming, to convert a 16-bit quantity (e.g., a port number) from host byte order to network byte order.
    Different computer architectures have different byte orders (endianness). Network protocols, however, typically use a standardized byte order called network byte order, which is big-endian. The htons function ensures that a 16-bit quantity is represented in the byte order expected by network protocols.

- htonl() stands for "host to network long." It is a function used in C and C++ programming, particularly in network programming, to convert a 32-bit quantity (e.g., an IPv4 address or a 32-bit integer) from host byte order to network byte order. As with htons() for 16-bit quantities, htonl() ensures that a 32-bit quantity is represented in the byte order expected by network protocols, which is big-endian.

- ntohs() stands for "network to host short." It is a function used in C and C++ programming, especially in network programming, to convert a 16-bit quantity (e.g., a port number) from network byte order to host byte order.
    Different computer architectures have different byte orders (endianness), and network protocols typically use a standardized byte order called network byte order, which is big-endian. The ntohs function ensures that a 16-bit quantity received from the network is correctly interpreted on a host with its native byte order.

- ntohl() stands for "network to host long." It is a function used in C and C++ programming, particularly in network programming, to convert a 32-bit quantity (e.g., an IPv4 address or a 32-bit integer) from network byte order to host byte order. Just like ntohs for 16-bit quantities, ntohl ensures that a 32-bit quantity received from the network is correctly interpreted on a host with its native byte order.


- inet_addr() is a function in C and C++ that converts an IPv4 address in dot-decimal notation (e.g., "192.168.1.1") into its binary representation as a 32-bit unsigned integer in network byte order.

- inet_ntoa is a function in C and C++ that converts a 32-bit IPv4 address in network byte order to a string in dot-decimal notation. Dot-decimal notation represents the IPv4 address as four decimal numbers separated by dots (e.g., "192.168.1.1").


- The send() function is used in C and C++ for sending data over a connected socket. It's commonly used in network programming to transmit data from one end of a connection (usually a socket) to the other.

- The recv() function is used in C and C++ for receiving data from a connected socket. It's commonly used in network programming to retrieve data sent from the other end of a connection, such as a server receiving data from a client.


- The signal() function is part of the C and C++ standard libraries and is used to handle signals in a program. Signals are a form of inter-process communication used by operating systems to notify processes about specific events or conditions. Common signals include things like SIGINT (sent when the user presses Ctrl+C) and SIGSEGV (sent when a segmentation fault occurs).

- sigaction() is a function used in C and C++ for more advanced and reliable signal handling compared to the older signal function. The sigaction function allows for more fine-grained control over how signals are handled, including specifying additional options and behaviors.


- lseek() is a system call in Unix-like operating systems that is used to change the file offset associated with a given file descriptor. The file offset is essentially the position in the file where the next read or write operation will occur. lseek is part of the standard I/O library in C.

- The fstat function is a system call in Unix-like operating systems that retrieves information about an open file descriptor, such as its size, permissions, and other attributes. It is often used to obtain information about a file before performing certain operations.

- The fcntl() (file control) function is a system call in Unix-like operating systems that is used for various operations on open file descriptors. It provides a way to modify or obtain information about file descriptor properties, such as file status flags and file locks.

- The poll() function is a system call in Unix-like operating systems that is used for monitoring multiple file descriptors to see if they are ready for I/O operations. It is often used as an alternative to the older select system call and provides similar functionality.


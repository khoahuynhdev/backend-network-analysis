#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

//maximum application buffer
#define APP_MAX_BUFFER 1024
#define PORT 8080

int main () {
  // define the server and client file descriptors
  int server_fd, client_fd;
  // the client_fd is only get created when a connection is established to that socket
  // a client_fd == a connection
  // server_fd 1-m client_fd

  // define the socket address
  struct sockaddr_in address;
  int address_len = sizeof(address);

  // define the application buffer where we receives the requests
  // data will be moved from received buffer to here
  char buffer[APP_MAX_BUFFER] = {0}; // the entire buffer will be init to zero

  // create socket
  // AF_INET or AF_INET4 is for IPv4, AF_INET6 is for IPv6
  // SOCK_STREAM means TCP, SOCK_DGRAM means UDP
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    // if it failed to create a file, an fd, just exit
    // why can it fail? maybe memory not enough
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  // bind socket
  address.sin_family = AF_INET; // ipv4
  address.sin_addr.s_addr = INADDR_ANY; // listen 0.0.0.0 interfaces
  address.sin_port = htons(PORT);

  // finally we bind the socket
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    // the bind fail
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  // create the queues (to handle connection)
  // listen for clients, with 10 backlogs (10 connections in accept queue)
  if (listen(server_fd, 10) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  // @NOTE: the kernel will finish the handshake
  // [x,x,x,x,x,x,x,x,x,x]
  // [c,x,x,x,x,x,x,x,x,x] -> have 1 connection
  // [x,x,x,x,x,x,x,x,x,x] -> when the backend call accept, it removes the connection
  // [c,c,c,c,c,c,c,c,c,c] -> the backend never call accept, the queue is full -> connection error
  return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
 
// This is HTML responce on the 404 not found error
char response[] = "HTTP/1.1 404 Not Found\n"
      "Content-type: text/html\n"
      "\n"
      "<html>\n"
      " <body>\n"
      "  <h1>Error 404</h1>\n"
      "  <h2> Not Found </h2> \n"
      "  <p>The requested URL was not found on this server.</p>\n"
      " </body>\n"
      "</html>\n";
;

// Actual length of the buffer
char buf[512];

//use send() & receive() instead of read() and write()
 
int main()
{
  int one = 1, client_fd, request_len;

  // Creating two structures of instance sockaddr_in
  // in order to implement web socket connection
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t sin_len = sizeof(cli_addr);
  
  // Creating & init a new socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    err(1, "can't open socket");
 
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
 
  int port = 8080;
  //Setting up for IPv4
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);
 
  //Binding and checking it (binding socket to the local port
  if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
    close(sock);
    err(1, "Can't bind");
  }
 
  // Listener for the specified socket and specified queue length
  listen(sock, 5);
  while (1) {
    // accept returns new variable, which represents a new port in order to communicate
    // with newly connected client. the old socket is now carrying on listening and handling
    // next clients waiting in the queue
    client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
    printf("got connection\n");
 
    // Handling accept returning -1 (in case of errors)
    if (client_fd == -1) {
      perror("Can't accept");
      continue;
    }
    
    request_len = recv(client_fd, buf, sizeof(buf), 0);
    if(request_len<1){// Including -1 and 0
      // Receive message is fault
      // Skip the current loop iteration
      perror("Can't receive");
      continue;
    }

    //write(client_fd, response, sizeof(response) - 1); /*-1:'\0'*/
    send(client_fd, response, sizeof(response), 0);
    close(client_fd);
  }
}
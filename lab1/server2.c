#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUF_SIZE 256

char buf[BUF_SIZE];

int main(int argc, char ** argv)
{    
    printf("Starting server...\n" );


    char outBuff[BUF_SIZE];


    int sock, newsock, port;    
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clen;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0)
    {
      perror("socket failure\n");
      return EXIT_FAILURE;
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    
    port = 8080;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
      perror("socket bind failure\n");
      return EXIT_FAILURE;
    }
    
    // listen to the socket with max numer of connection
    listen(sock, 5);

    while(1){
        clen = sizeof(cli_addr);
        // accept - establishing connection for the connection as the response for the client's request
        // accept returns new socket so old sokcet can be used to listen to clinets
        newsock = accept(sock, (struct sockaddr *) &cli_addr, &clen);
        if (newsock < 0) 
        {
            perror("accept failed\n");
            continue;
        }
    
        // clear buffer 
        memset(buf, 0, BUF_SIZE);
        if(recv(newsock, buf, BUF_SIZE-1, 0)<1){
           perror("receive failed");
           continue;
        }    
        printf("buf size: [%lu]\n", strlen(buf));
        printf("buf: [%s]\n", buf);
        
        char fPath[BUF_SIZE];
        memset(fPath, 0, BUF_SIZE);
        // get seond parameter (filename)
        sscanf(buf,"%*s %s",fPath);
        
        if (fPath == NULL ){
            perror("Error opening a file\n");
            close(newsock);
            continue;
        }

        printf("selected file: %s\n", fPath);

       // get file
       FILE *file;

       // removed 1st symbol (which is '/')
       char *trimmedPath = &(fPath[1]);
       file = fopen(trimmedPath,"r");
       if (file == NULL ){
            perror("Error opening a file\n");
            close(newsock);
            continue;
       }

       // clear buffer
       memset(outBuff,0,BUF_SIZE);

       char c = fgetc(file);
       int count = 0;
       // write file content to the file into outBufer
       while ( (c!=EOF) && (count<BUF_SIZE) ){
           outBuff[count]=c;
           count++;
           c=fgetc(file);
       }

       if(send(newsock, outBuff, BUF_SIZE,0)==-1){
           perror("send failure");
           continue;
       }

       close(newsock);
       printf("Client gone");
 }
}














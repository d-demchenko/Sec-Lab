#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUF_SIZE 10
#define OUT_BUF_SIZE 256


//char buf[BUF_SIZE];
//char *buf;
//int *accessGroup;


int main(int argc, char ** argv)
{    


    // char *buf = (char*) malloc(sizeof(char)*BUF_SIZE);
    // int *accessGroup = (char*)malloc(1);
    // *accessGroup = 'g';


    char *buf = (char*) malloc(sizeof(char)*BUF_SIZE);
    char *accessGroup = (char*)malloc(sizeof(char));
    *accessGroup = 'u';

    char *pageForbiden = "<html><body><h1>You have no access</h1></body></html>";
    char *pageWelcome = "<html><body><h1>Access granted</h1></body></html>";
    printf("Starting server...\n" );


    //printf("addr difference = %ld\n", ((long)&accessGroup)-((long)&buf[0]));
    printf("%p,%p,%p\n",accessGroup, buf, buf+16);
    printf("%04x\n", *accessGroup);
    //char *p1 = &buf[0];
   // int *p2 = &accessGroup;
    //int x = p1-p2;
    
    // for (int i = -300; i < x+300; ++i)
    // {
    //   printf("%c\n", (char) *(p2+i)) ;
    // }




    char outBuff[OUT_BUF_SIZE];


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
        *accessGroup='u';
        
        if(recv(newsock, buf, OUT_BUF_SIZE, 0)<1){
           perror("receive failed");
           continue;
        }

        // checking addresses
        for (int i = 0-10; i < BUF_SIZE+10; ++i)
        {
            printf("%p,%04x,%c\n", (&buf[0])+i, *(&buf[0])+i,*(&buf[0]+i) );
        }
        printf("access addr: %p,%04x,%c\n", &accessGroup,*accessGroup,*accessGroup);

       // printf("addr difference = %ld\n", &accessGroup-&buf[BUF_SIZE]);

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

        // clear buffer
        memset(outBuff,0,OUT_BUF_SIZE);       

        if (*accessGroup=='a')
        {
            strncpy(outBuff,pageWelcome,strlen(pageWelcome));
        }else{
            strncpy(outBuff,pageForbiden,strlen(pageForbiden));
        }        
              

        if(send(newsock, outBuff, OUT_BUF_SIZE,0)==-1){
            perror("send failure");
            continue;
        }

        close(newsock);
        printf("Client gone");
 }
}














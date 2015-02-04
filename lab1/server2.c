#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUF_SIZE 256

int main(int argc, char ** argv)
{    
     printf("Starting server\n" );
     int sock, newsock, port;
     char buf[BUF_SIZE];
     char outBuff[BUF_SIZE];
     struct sockaddr_in serv_addr, cli_addr;
     socklen_t clen;
     // if (argc < 2) 
     // {
     //     fprintf(stderr,"usage: %s <port_number>\n", argv[0]);
     //     return EXIT_FAILURE;
     // }
     
     sock = socket(AF_INET, SOCK_STREAM, 0);

     if (socket < 0)
     {
       printf("socket() failed: %d\n", errno);
       return EXIT_FAILURE;
     }

     memset((char *) &serv_addr, 0, sizeof(serv_addr));
     
     port = 8080;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);

     if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     {
       printf("bind() failed: %d\n", errno);
       return EXIT_FAILURE;
     }
     
     // listen to the socket with max numer of connection
     listen(sock, 5);
     while(1){
     clen = sizeof(cli_addr);
     // accept - establishing connection for the connection as the response for the client's request
     // accept returens new socket so old oskcet cna e used to listen to clinets
     newsock = accept(sock, (struct sockaddr *) &cli_addr, &clen);
     if (newsock < 0) 
     {
       printf("accept() failed: %d\n", errno);
       return EXIT_FAILURE;
     }

     // clear buffer 
     memset(buf, 0, BUF_SIZE);
     if(recv(newsock, buf, BUF_SIZE, 0)<1){
        perror("Can't receive");
        continue;
     }
     

    printf("shan we copy: \n\n");
    int count = 0;
     char c = buf[count];
     char lookupFileName[BUF_SIZE];


     while ( (buf[count]!=EOF) && (buf[count]!='\n') && (count<BUF_SIZE) ){    
        printf("%c", buf[count]);
        lookupFileName[count]=buf[count];
        count++;        
     }
     printf("\n\n");
     printf("CLIENT: %s\n", buf);


     // GET THE NAME OF THE REQUESTED FILE
     char strBuf[BUF_SIZE];
     strncpy(strBuf, buf, BUF_SIZE);
     char * fPath;
     fPath = strtok(strBuf," ");

    for (int i = 0; i < 1; ++i)
    {
          fPath = strtok (NULL, " ");
    }




    char *pstr1;
    pstr1 = &(fPath[1]);

     // get file



     FILE *file;
     file = fopen(pstr1,"r");
    

    if (file == NULL ){
         printf("Error opening a file\n");
         return EXIT_FAILURE;
     }

     memset(outBuff,0,BUF_SIZE);
     printf("File content: \n\n" );

     c = fgetc(file);
     count = 0;
     while ( (c!=EOF) && (count<BUF_SIZE) ){
        printf("%c", c);
        outBuff[count]=c;
        count++;
        c=fgetc(file);
     }
     printf("\n\n End of file \n" );


     if(send(newsock, outBuff, BUF_SIZE,0)==-1){
        perror("Can't send");
        continue;
     }

     close(newsock);
     printf("Client gone");
 }
     // close(sock);
     // printf("\nStopping server..\n");
}














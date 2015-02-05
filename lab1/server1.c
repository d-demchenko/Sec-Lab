#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define PATH_MAX 4096
#define BUF_SIZE 256

char buf[BUF_SIZE];
char* pwd;
char pwd_buff[PATH_MAX + 1];
char error_404[] = "HTTP/1.1 404 Not Found\n"
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

    // Fetching the full address of the current location 
    // (as pwd command on UNIX)
    pwd = getcwd( pwd_buff, PATH_MAX + 1 );
    printf("Curent server location is: %s\n", pwd);

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
    
    // listen to the socket with max number of connections in the queue of 5
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
        printf("\n=========\n");
        printf("buf size: [%lu]\n", strlen(buf));
        printf("buf: [%s]\n", buf);
        
        char fPath[BUF_SIZE];
        memset(fPath, 0, BUF_SIZE);
        // get seond parameter (filename)
        sscanf(buf,"%*s %s",fPath);
        
        if (fPath == NULL ){
            perror("Error opening a file\n");
            send(newsock, error_404, sizeof(error_404),0);
            close(newsock);
            continue;
        }

       // get file
       FILE *file;

       // Resolving with /.. in UNIX
       // As we don't want user to get an access 
       // any file outside the directory
       // Only internal files (and files of children) are allowed

       char *ptr;
       char ptr_buff[PATH_MAX + 1];
       ptr = realpath(fPath,ptr_buff);
       // Concatinating realPath of the file and current directory path
       strcat(pwd, ptr_buff);
       
       printf("selected file: %s\n", pwd);
       file = fopen(pwd,"r");
       if (file == NULL ){
            perror("Error opening a file\n");
            send(newsock, error_404, sizeof(error_404),0);
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
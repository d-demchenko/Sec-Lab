#include <stdio.h>
#include <string.h>
#include <stdlib.h>

    int t1=-1;
    int t2=-2;  
    // char buff[6];
    char *buff2;// = (char*) malloc(sizeof(char)*6);
    // char *buff = (char*) malloc(sizeof(char)*6);
    int t3=-3;
    int pass = 0;
    int t4=-4;


int main(void)
{
   *buff2 = (char*) malloc(sizeof(char)*6);
    printf("\n Enter the password : \n");
    gets(buff);
    printf("%d + %s \n", pass, buff);

    // strcpy(buff2, buff);

    printf("%d + %s", pass, buff);
    printf("%d + %d + %d + %d", t1,t2,t3,t4);


    //buff = &buff2;

    if(strcmp(buff, "123456"))
    {
        printf ("\n Wrong Password \n");
    }
    else
    {
        printf ("\n Correct Password \n");
        pass = 1;
    }

    if(pass)
    {
       /* Now Give root or admin rights to user*/
        printf ("\n Root privileges given to the user \n");
    }

    return 0;
}
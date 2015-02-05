#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//gcc -fstack-protector -masm=intel -S test.c
    
    

int main(void)
{
    char *getter;
    char buff[2];
    // char *buff2// = (char*) malloc(sizeof(char)*6);
    // char *buff = (char*) malloc(sizeof(char)*6);
    int pass = 0;
    
    getter = (char*) malloc(sizeof(char)*6);
    printf("\n Enter the password : \n");
    printf("%lu + %s \n", sizeof(buff), buff);
    gets(getter);
    strcpy(buff, getter);
    printf("============\nPassword is %d \n============\n",pass);
    
    printf("%lu + %s \n", sizeof(buff), buff);

    // strcpy(buff2, buff);

    printf("%lu + %s\n", sizeof(buff), buff);


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
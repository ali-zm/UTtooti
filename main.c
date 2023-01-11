#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "functions.h"
int main()
{
    int count_command;
    int count_username;
    int count_password;
    int numOfUsers=0;
    char *command_text;
    char *usernameText;
    char *passwordText;
    accounts* head=(accounts*)malloc(sizeof(accounts));
    head->nextAccount=NULL;
    while (1)
    {
        /*user can only choose to signup or login*/
        command_text= detectWhatUserTyped(command_text,&count_command);
        if(!strcmp(command_text,"signup"))
        {
            usernameText= detectWhatUserTyped(usernameText,&count_username);
            passwordText= detectWhatUserTyped(passwordText,&count_password);/*getting pass and username*/
            signup(&numOfUsers,usernameText,passwordText,head,count_username,count_password);
            createAccountFile(head);
            free(command_text);
        }
        else if(!strcmp(command_text,"login"))
        {
            free(command_text);
            usernameText=detectWhatUserTyped(usernameText,&count_username);
            passwordText=detectWhatUserTyped(passwordText,&count_password);
            int check=checkIfLoginAllowed(head,usernameText,passwordText,count_username,count_password);
            if(check==0)/*username and pass are valid both*/
            {
                printf("successfully logged in!\n");
                login(head,usernameText,count_username);
                createAccountFile(head);
                createPostsFile(head);
            }
            else if(check==1)/*pass not valid*/
            {
                printf("password not valid!\n");
                continue;
            }
            else if(check==2)/*username not valid*/
            {
                printf("user not found!\n");
            }
        }
        else
        {
            printf("command not valid!\n");
        }
        fflush(stdin);
    }
}
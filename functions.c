#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ACCOUNT_FILE_TEXT "accounts.txt"
#define POST_FILE_TEXT "posts.txt"
char* detectWhatUserTyped(char *typed, int *count)
{
    typed=(char*)malloc(sizeof(char));     
    char read;
    *count=0;
    while (1)
    {
        read=getchar();
        if(read==' ' || read=='\n')/*getting chars continues till it reaches to end of line or space*/
        {
            *(typed+(*count))='\0';
            return typed;
        }
        *(typed+(*count))=read;
        (*count)++;
        typed=realloc(typed,sizeof(char)*((*count)+1));  /*increasing space*/
    }
    return NULL;
}
char* detectWhatUserTypedForPost(char *typed, int *count)
{
    typed=(char*)malloc(sizeof(char));     
    char read;
    *count=0;
    while (1)
    {
        read=getchar();
        if(read=='\n')/*getting chars continues till it reaches to end of line */
        {
            *(typed+(*count))='\0';
            return typed;
        }
        *(typed+(*count))=read;
        (*count)++;
        typed=realloc(typed,sizeof(char)*((*count)+1));  
    }
    return NULL;
}
void store_signup_info(accounts *new_account,char *usernameText,char *passwordText,int count_username,int count_password)
{
    new_account->username=(char*)malloc(sizeof(char)*count_username);
    (new_account->password)=(char*)malloc(sizeof(char)*count_password);
    strcpy(new_account->username,usernameText);
    strcpy(new_account->password,passwordText); /*loading the data from dynamic arrays into new node*/
    *((new_account->username)+count_username)='\0';
    *((new_account->password)+count_password)='\0';
    new_account->nextAccount=NULL;
    free(usernameText);
    free(passwordText);
}
int checkIfLoginAllowed(accounts *head,char *usernameText,char *passwordText,int count_username,int count_password)
{
    accounts *cur=head->nextAccount;
    while (cur != NULL)
    {
            if(!strcmp(usernameText,cur->username))
            {
               if(!strcmp(passwordText,cur->password))/*comparing the stored pass with the pass user had entered*/
                {
                    return 0;
                }
                else
                    return 1;
            }
            cur=cur->nextAccount;
    }
    return 2;
}

int ifUserHadLikedBefore(post_type *cur_post,char *usernameText,int count_username)
{
    if(cur_post->NumberOfLikes==0)
    {
        cur_post->usersWhoHadLiked=(char**)malloc(sizeof(char*));
        cur_post->usersWhoHadLiked[cur_post->NumberOfLikes]=(char*)malloc(sizeof(char)*count_username);
        strcpy(cur_post->usersWhoHadLiked[cur_post->NumberOfLikes],usernameText);
        return 0;
    }
    else
    {
        for(int i=0;i<cur_post->NumberOfLikes;i++)
        {
            if(!strcmp(usernameText,cur_post->usersWhoHadLiked[i]))/*user have liked before*/
            {
                return 1;
            }
            if((i<cur_post->NumberOfLikes)-1)
                cur_post=cur_post->nextPost;
        }
        cur_post->usersWhoHadLiked=realloc(cur_post->usersWhoHadLiked,sizeof(char*)*(cur_post->NumberOfLikes+1));
        cur_post->usersWhoHadLiked[cur_post->NumberOfLikes]=(char*)malloc(sizeof(char)*count_username);
        strcpy(cur_post->usersWhoHadLiked[cur_post->NumberOfLikes],usernameText);/*user haven't liked before we store the username so he can't like again in future*/
        return 0;
        
            
    }
    
}
int liking(accounts *head_account,int postID_destination,char *username_destination,int count_userDestination,char* usernameText,int count_username)
{
    accounts *cur_account;
    post_type *cur_post;
    cur_account=head_account->nextAccount;
    int flag_username=0;
    int flag_postID=0;
    int flag_hadBeenLikedBefore=0;
    while(cur_account != NULL)
    {
        if(!strcmp(cur_account->username,username_destination))/*the user whose post is going to be liked*/
        {
            cur_post=cur_account->post;
            while(cur_post != NULL)
            {
                if(cur_post->postId==postID_destination)/*the post which is going to be liked*/
                {
                    int check=ifUserHadLikedBefore(cur_post,usernameText,count_username);
                    if(check==0)
                    {
                        cur_post->NumberOfLikes++;
                        flag_postID=1;
                        printf("successfully liked!\n");
                        break;
                    }
                    else
                    {
                        printf("U have liked this post before!!!!!\n");
                        return 0;
                    }
                }
                cur_post=cur_post->nextPost;
            }
            flag_username=1;
            break;
        }
        cur_account=cur_account->nextAccount;
    }
    if(flag_username==0)
    {
        printf("this user doesn't exist!\n");
        return 0;
    }
    if(flag_postID==0)
    {
        printf("this user doesn't have any post with this postID!\n");
        return 0;
    }
}
int deleting(accounts *head_account,int postID_destination,char *usernameText,int count_username)
{
    accounts *cur_acoount;
    post_type *cur_post;
    int flag=0;
    cur_acoount=head_account->nextAccount;
    while(cur_acoount != NULL)
    {
        if(!strcmp(cur_acoount->username,usernameText))/*finding the user whose post is going to be deleted*/
        {
            cur_post=cur_acoount->post->nextPost;
            if(cur_post->postId==postID_destination)
            {
                cur_acoount->post=cur_post->nextPost;
                flag=1;
                printf("post has been deleted successfully\n");
                break;
            }
            while(cur_post != NULL)
            {
                if(cur_post->nextPost!=NULL)
                {
                    if(cur_post->nextPost->postId==postID_destination)/*finding the post which should be  deleted*/
                    {
                        post_type *temp=cur_post->nextPost;
                        cur_post->nextPost=cur_post->nextPost->nextPost;
                        free(temp);
                        free(temp->usersWhoHadLiked);
                        free(temp->postContent);
                        printf("post has been deleted successfully\n");
                        flag=1;
                        return 0;
                    }
                }
                cur_post=cur_post->nextPost;
            }
            break;
        }
        cur_acoount=cur_acoount->nextAccount;
    }
    if(flag==0)
    {
        printf("there is no post with such postID\n");
        return 1;
    }
}
int find_user(accounts *head_account,char* usernameDestination,int count_username)
{
    accounts *cur_account;
    cur_account=head_account->nextAccount;
    post_type *cur_post;
    while(cur_account != NULL)
    {
        if(!strcmp(usernameDestination,cur_account->username))
        {
            cur_post=cur_account->post;
            printf("username:%s\n",usernameDestination);
            while(cur_post != NULL)
            {
                if(cur_post->postContent!=NULL)
                {
                    printf("post ID:%d\tpost content:%s\t\tnumber of likes:%d\n",cur_post->postId,cur_post->postContent,cur_post->NumberOfLikes);
                }
                cur_post=cur_post->nextPost;
            }
            return 0;
        }
        cur_account=cur_account->nextAccount;
    }
    printf("user not found!\n");
}
void info(char *usernameText,int count_username,accounts *head_account)
{
    accounts *cur_account;
    cur_account=head_account->nextAccount;
     post_type *cur_post;
    while(cur_account != NULL)
    {
        if(!strcmp(usernameText,cur_account->username))
        {
            cur_post=cur_account->post;
            printf("username:%s\tpassword:%s\n",usernameText,cur_account->password);
                while(cur_post != NULL)
                {
                    if(cur_post->postContent!=NULL)
                    {
                        printf("post ID:%d\tpost content:%s\t\tnumber of likes:%d\n",cur_post->postId,cur_post->postContent,cur_post->NumberOfLikes);
                    }
                    cur_post=cur_post->nextPost;
                }
                break;
        }
        cur_account=cur_account->nextAccount;
    }
}
post_type* posting(accounts *head_account,post_type *head_post,char *usernameText,int count_username,char *text,int count_text,int *num_of_posts)
{
    accounts *cur_account=head_account->nextAccount;
    post_type *cur_post;
    post_type *new_post=(post_type*)malloc(sizeof(post_type));
    while (cur_account != NULL)
    {
        if(!strcmp(usernameText,cur_account->username))
        {
            cur_post=cur_account->post;
            if(*(num_of_posts)==0)/*creating the first node*/
            {
                head_post->nextPost=new_post;
                new_post->nextPost=NULL;
                new_post->postContent=(char*)malloc(sizeof(char)*count_text);
                strcpy(new_post->postContent,text); 
                new_post->NumberOfLikes=0;
                new_post->postId=(*num_of_posts);
            }
            else
            {
                while (cur_post != NULL)/*atttaching the post into the last poat*/
                {
                    if(cur_post->nextPost==NULL)
                    {
                        cur_post->nextPost=new_post;
                        new_post->nextPost=NULL;
                        new_post->postContent=(char*)malloc(sizeof(char)*count_text);
                        strcpy(new_post->postContent,text); 
                        new_post->NumberOfLikes=0;
                        new_post->postId=(*num_of_posts);
                        break;
                    }
                    cur_post=cur_post->nextPost;
                }
            }
            (*num_of_posts)++;
        }
        cur_account=cur_account->nextAccount;
    }
    return head_post;
}
post_type *whichAccount(accounts *head_account,char *usernameText,int count_username)
{
    accounts *cur_account=(accounts*)malloc(sizeof(accounts));
    cur_account=head_account->nextAccount;
    while(cur_account != NULL)
    {
        if(!strcmp(usernameText,cur_account->username))
        {
            return cur_account->post;
        }
        cur_account=cur_account->nextAccount;
    }
}
int login(accounts *head_account,char *usernameText,int count_username)
{
    fflush(stdin);
    int count_command=0;
    char *command_text=NULL;
    int num_of_posts=0;
    post_type *head_post=whichAccount(head_account,usernameText,count_username);
    while ((1))/*user should be logged in first then he can write the command now*/
    {
        command_text=detectWhatUserTyped(command_text,&count_command);
        if(!strcmp(command_text,"post"))
        {
            char *text=(char*)malloc(sizeof(char));
            int count_text;
            text=detectWhatUserTypedForPost(text,&count_text);
            head_post = posting(head_account,head_post,usernameText,count_username,text,count_text,&num_of_posts);
            printf("post added successfully!\n");
            free(text);
        }
        else if(!strcmp(command_text,"like"))
        {
            int count_userDestination=0;
            int postID_destination;
            char *user_destination=(char*)malloc(sizeof(char));
            user_destination=detectWhatUserTyped(user_destination,&count_userDestination);
            scanf("%d",&postID_destination);
            fflush(stdin);
            liking(head_account,postID_destination,user_destination,count_userDestination,usernameText,count_username);
            free(user_destination);
        }
        else if(!strcmp(command_text,"delete"))
        {
            int postID_destination;
            scanf("%d",&postID_destination);
            fflush(stdin);
            deleting(head_account,postID_destination,usernameText,count_username);
        }    
        else if(!strcmp(command_text,"logout"))
        {
            printf("successfully logged out!\n");
            return 0;
        }
        else if(!strcmp(command_text,"find_user"))
        {
            int count_userDestination=0;
            char *usernameDestination=(char*)malloc(sizeof(char));
            usernameDestination=detectWhatUserTyped(usernameDestination,&count_userDestination);
            find_user(head_account,usernameDestination,count_userDestination);
        }
        else if(!strcmp(command_text,"info"))
        {
            info(usernameText,count_username,head_account);
        }
        else
        {
            printf("command not valid!!\n");
        }
        fflush(stdin);
        free(command_text);
    }
}
void signup(int *numOfUsers,char *usernameText,char *passwordText,accounts *head,int count_username,int count_password)
{
    int flag=0;
    accounts *cur=head->nextAccount;
    accounts *new_account=(accounts*)malloc(sizeof(accounts));
    if((*numOfUsers)==0)
    {
        flag=1;
        head->nextAccount=new_account;
        head->nextAccount->post=(post_type*)malloc(sizeof(post_type));
        head->nextAccount->post->nextPost=NULL;
        head->nextAccount->post->postContent=NULL;
        store_signup_info(new_account,usernameText,passwordText,count_username,count_password);
    }
    else
    {
        while (cur != NULL)
        {
            if(!strcmp(usernameText,cur->username))
            {
                printf("sorry,username has been used before!\n");
                free(usernameText);
                free(passwordText);
                break;
            }
            if(cur->nextAccount==NULL)
            {
                cur->nextAccount=new_account;
                new_account->post=(post_type*)malloc(sizeof(post_type));
                new_account->post->postContent=NULL;
                new_account->post->nextPost=NULL;
                new_account->post->postContent=NULL;
                store_signup_info(new_account,usernameText,passwordText,count_username,count_password);
                flag=1;
                break;
            }
            cur=cur->nextAccount;
        }
    }
    if(flag==1)
    {
       (*numOfUsers)++;
        printf("account is added successfully!\n");
    }
}
void createAccountFile(accounts *head_account)
{
    accounts *cur_account;
    cur_account=head_account->nextAccount;
    FILE* ptr=fopen(ACCOUNT_FILE_TEXT,"w");
    int num_of_posts;
    while(cur_account != NULL)
    {
        num_of_posts=0;
        post_type *cur_post =cur_account->post;
        while(cur_post != NULL)
        {
            num_of_posts++;
            cur_post=cur_post->nextPost;
        }
        fwrite(cur_account->username,sizeof(char),strlen(cur_account->username),ptr);
        fprintf(ptr," ");
        fwrite(cur_account->password,sizeof(char),strlen(cur_account->password),ptr);
        fprintf(ptr," %d\n",num_of_posts-1);
        cur_account=cur_account->nextAccount;    
    }
    fclose(ptr);
}
void createPostsFile(accounts *head_account)
{
    accounts *cur_account;
    cur_account=head_account->nextAccount;
    FILE* ptr=fopen(POST_FILE_TEXT,"w");
    while(cur_account != NULL)
    {
        post_type *cur_post =cur_account->post;
        while(cur_post != NULL)
        {
            if(cur_post->postContent != NULL)
            {
                fwrite(cur_post->postContent,sizeof(char),strlen(cur_post->postContent),ptr);
                fprintf(ptr," ");
                fwrite(cur_account->username,sizeof(char),strlen(cur_account->username),ptr);
                fprintf(ptr," %d\n",cur_post->NumberOfLikes);
            }
            cur_post=cur_post->nextPost;
        }
        cur_account=cur_account->nextAccount;    
    }
    fclose(ptr);
}
#include <stdio.h>
#include <stdlib.h>
typedef struct posts post_type;
struct posts{
    char *username;
    char **usersWhoHadLiked;
    int postId;
    int NumberOfLikes;
    char *postContent;
    struct posts *nextPost;
};
typedef struct accounts accounts;
struct accounts
{
    char *username;
    char *password;
    struct posts *post;
    accounts *nextAccount;
};
char* detectWhatUserTyped(char *typed, int *count);
char* detectWhatUserTypedForPost(char *typed, int *count);
void store_signup_info(accounts *new_account,char *usernameText,char *passwordText,int count_username,int count_password);
int checkIfLoginAllowed(accounts *head,char *usernameText,char *passwordText,int count_username,int count_password);
int ifUserHadLikedBefore(post_type *cur_post,char *usernameText,int count_username);
int liking(accounts *head_account,int postID_destination,char *username_destination,int count_userDestination,char* usernameText,int count_username);
int deleting(accounts *head_account,int postID_destination,char *usernameText,int count_username);
int find_user(accounts *head_account,char* usernameDestination,int count_username);
void info(char *usernameText,int count_username,accounts *head_account);
post_type* posting(accounts *head_account,post_type *head_post,char *usernameText,int count_username,char *text,int count_text,int *num_of_posts);
post_type *whichAccount(accounts *head_account,char *usernameText,int count_username);
int login(accounts *head_account,char *usernameText,int count_username);
void signup(int *numOfUsers,char *usernameText,char *passwordText,accounts *head,int count_username,int count_password);
void createAccountFile(accounts *head_account);
void createPostsFile(accounts *head_account);

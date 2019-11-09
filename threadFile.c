#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

void ls_func(){
    printf("\nls was called");
}
void mkdir_func(){
    printf("\nmkdir was called");
}
int main(){
    char command[20];
    char *api_extension = "_func";
    void *avail_command_ptr[2] = {&ls_func, &mkdir_func};
    char *avail_command_name[2] = {"ls_func", "mkdir_func"};
    unsigned int command_count = 2;
    printf("Welcome to CMD");
    while(1){
        printf("\nEnter command>>$$>>:");
        gets(command);
        if(strcmp(command, "exit") == 0)
            break;
        //Master process to maintain the CMD prompt
        //Child process to run the command entered by the user.
        if(fork() == 0){
            strcat(command, api_extension);
            for(int i = 0; i < command_count; i++){
                if(strcmp(command, avail_command_name[i]) == 0){
                    void (*func_ptr)(void) = avail_command_ptr[i];
                    (*func_ptr)();
                    break;
                }
            }
        }
    }
    getchar();
}
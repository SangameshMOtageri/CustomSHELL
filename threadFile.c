  
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

void ls_func(){
    clock_t c = clock();
    printf("CP");
    while(clock() < c + 100000)
	printf("..");
    printf("\nls was called");
}
void mkdir_func(){
    printf("\nmkdir was called");
}
int main(){
    char command[20];
    char *api_extension = "_func";
    char success_msg[10] =  "ChildDone";
    char read_success_msg[10];
    void *avail_command_ptr[2] = {&ls_func, &mkdir_func};
    char *avail_command_name[2] = {"ls_func", "mkdir_func"};
    unsigned int command_count = 2;
    int pipeID[2];

    printf("Welcome to CMD");
    while(1){
	pipe(pipeID);
        printf("\n>>$$>>:");
        gets(command);
        if(strcmp(command, "exit") == 0)
            break;
        //Master process to maintain the CMD prompt
        //Child process to run the command entered by the user.
        if(fork() == 0){
	    //Close the read end of pipe
	    close(pipeID[0]);
            strcat(command, api_extension);
            for(int i = 0; i < command_count; i++){
                if(strcmp(command, avail_command_name[i]) == 0){
                    void (*func_ptr)(void) = avail_command_ptr[i];
                    (*func_ptr)();
                    break;
                }
            }
	    write(pipeID[1], success_msg, sizeof(success_msg));
	    close(pipeID[1]);
        break;
        }
        else{
	    printf("PP@");
	    //Close the write end of pipe
	    close(pipeID[1]);
	    while(strcmp(read_success_msg, "ChildDone")){
		printf("\n PP");
		read(pipeID[0], read_success_msg, sizeof(read_success_msg));
	    }
	    close(pipeID[0]);
	    wait(NULL);
        }
     }
}

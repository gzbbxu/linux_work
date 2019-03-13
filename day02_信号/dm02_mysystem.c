#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<errno.h>
int my_system(const char* command);
int main()
{
	//system("ls -l | wc -w");
	my_system("ls - l | wc -w");
	return 0;
}
int my_system(const char* command){
	pid_t pid;
	int status;
	if(command == NULL){
		return 1;
	}
	if((pid = fork()) < 0){
		status = -1;// error 
	}else if(pid ==0){
		//child 
		execl("/bin/sh","sh","-c",command,NULL);
		exit(127);
	}else{
		while(waitpid(pid,&status,0)<0){
			if(errno == EINTR){
				continue;
			}
			status = -1;
			break;
		}
	}
	return status;
}

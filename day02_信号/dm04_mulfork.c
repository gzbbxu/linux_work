#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void TestFunc(int loopnum)
{
	printf("loopnum:%d\n", loopnum);	
}

int main(void )
{
	int ret = 0;	
	int procnum = 10;
	int loopnum = 100;
	

	int i = 0, j = 0;
	printf("please enter you procNum : \n");
	scanf("%d", &procnum);
	
	printf("please enter you loopnum :\n");
	scanf("%d", &loopnum);
	
	pid_t pid;
	
	for (i=0; i<procnum; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			for (j=0; j<loopnum; j ++)
			{
				TestFunc(j);
			}
			exit(0);
		}
	
	}
	//检测到所有子进程退出，父进程才退出
	while(1){
		ret = wait(NULL);//让父进程阻塞，阻塞过程中与偶可能被别的信号中断，，需要再做异常处理
		if(ret == -1){
			if(errno == EINTR){
				continue;
			}
			printf("break 中断");
			break;	
		}
		
	}
		
	printf("父进程退出\n");
	return 0; 
} 







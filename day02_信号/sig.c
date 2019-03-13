#include <stdio.h>
#include <signal.h>
#include<unistd.h>
void handler(int signo)
{
	printf("signo = %d \n",signo);
	
}
int main()
{
	signal(SIGINT,handler);//ctrl + c
	signal(SIGQUIT,handler);//ctrl + \
	while(1)
	{
		pause();
	}
	return 0;
}

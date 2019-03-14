#include<stdio.h>
#include <stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
void handler(int signal)
{
	int i = 1/0;
	printf("handler %d \n",signal);
}
int main()
{
	if(SIG_ERR == signal(SIGINT,handler))	
	{
		perror("signal error");	
		exit(0);
	}
	while(1){
		pause();
	}

	return 0;
}
int main11()
{
	char tempchar;
    signal(SIGINT,handler);	
	printf("please input a ");
	while((tempchar = getchar())!='a'){
		pause();
		printf("pppppppppp\n");
	}
	signal(SIGINT,SIG_DFL);
	while(1)
	{
		pause();
	}
	return 0;
}

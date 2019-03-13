#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>
#include <errno.h>
#include <signal.h>


int main(void )
{
	pid_t pid;

	pid = fork(); //errno
	if (pid == -1)	
	{
		perror("tile");
		return -1;
	}
	if (pid > 0)
	{
		exit(0);
	}
	else if (pid == 0)
	{
		
	}

	printf("fork after....\n");
	return 0;
}

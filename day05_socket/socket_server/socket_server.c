#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
/* struct sockaddr_in {
 sa_family_t    sin_family;  address family: AF_INET
 in_port_t      sin_port;    port in network byte order
 struct in_addr sin_addr;    internet address
 };

 Internet address.
 struct in_addr {
 uint32_t       s_addr;      address in network byte order
 };*/
int main() {
	printf("hello world");
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("fun socket fddd\n");
		exit(0);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8001);
	addr.sin_addr.s_addr = inet_addr("192.168.91.128");
//	addr.sin_addr.s_addr = INADDR_ANY; //renyi
	//const struct sockaddr *
	if (bind(sockfd, &addr, sizeof(addr)) < 0) {
		perror("bin error");
		exit(0);
	}

	return 0;
}

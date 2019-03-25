/*
 * socket_cli.c
 *
 *  Created on: Mar 22, 2019
 *      Author: zhouke
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
int main() {
	printf("hello world client");
	int sockfd = 0;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("fun socket\n");
		exit(0);
	}
	//设置客服端链接的tcpip结构体信息
	struct sockaddr_in srvaddr;
	//协议族
	srvaddr.sin_family = AF_INET;
	//设置端口
	srvaddr.sin_port = htons(8001);
	//设置ip
	srvaddr.sin_addr.s_addr = inet_addr("192.168.91.128"); //127.0.0.1
	//srvaddr.sin_addr.s_addr = inet_addr(INADDR_ANY); //绑定本机的任意一个地址
	//客服端链接
	if (connect(sockfd, (struct sockaddr*) (&srvaddr), sizeof(srvaddr)) < 0) {
		perror("fun socket\n");
		exit(0);
	}
	//设置接受和发送的缓存。
	char revbuf[1024] = { 0 };
	char sendbuf[1024] = { 0 };
	//
	//char *fgets(char *s, int size, FILE *stream); 从stream 读取size-1大小的数据存入s,最后加'\0'
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
		//向服务写数据
		//ssize_t write(int fd, const void *buf, size_t count);
		// 从buf中读取count大小的数据存入文件描述符为fd的文件中。
		write(sockfd, sendbuf, strlen(sendbuf)); //服务器端回发信息
		//从服务器读数据
		//ssize_t read(int fd, void *buf, size_t count);
		//从文件描述符为fd的文件中读取大小为count的数据存入buf中。
		read(sockfd, revbuf, sizeof(revbuf));
		fputs(revbuf, stdout); //从服务器收到数据，打印屏幕
		memset(revbuf, 0, sizeof(revbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
	}
	close(sockfd);
	return 0;
}


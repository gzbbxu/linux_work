/*
 * server.c
 *
 *  Created on: Mar 26, 2019
 *      Author: zhouke
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)
//
struct packet {
	int len;
	char buf[1024];
};
ssize_t readn(int fd, void * buf, size_t count) {
	size_t nleft = count;
	ssize_t nread;
	char * bufpb = (char*) buf;
	while (nleft > 0) {
		if ((nread = read(fd, bufpb, nleft)) < 0) {
			if (errno == EINTR) {
				continue;
			}
			return -1;
		} else if (nread == 0) { //client close
			return count - nleft;
		}
		bufpb += nread;
		nleft -= nread;
	}
}

ssize_t writen(int fd, const void * buf, size_t count) {

	size_t nleft = count;
	size_t nwritten;
	char * bufp = (char*) buf;
	while (nleft > 0) {
		if (nwritten = write(fd, bufp, nleft) < 0) {
			if ((nwritten = write(fd, bufp, nleft)) < 0) {
				if (errno == EINTR) {
					continue;
				}
				return -1;
			} else if (nwritten == 0) {
				continue;
			}
		}
		bufp += nwritten;
		nleft -= nwritten;
	}
}
int main() {
	return 0;
}


#include <cstdint>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fcntl.h>
#include <iostream>

const int BUF_SIZE = 42 * 4096;

typedef struct s_client {
	int	id;
    char message[BUF_SIZE];
}   t_client;

t_client    client[1024];
int id = 0, maxfd = 0;
char sendBuff[BUF_SIZE];
char recvBuff[BUF_SIZE];


void	print_error(const char * msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int sockfd, connfd, len, fd;
	struct sockaddr_in servaddr, cli; 

	if (argc != 2)
		print_error("Wrong number of arguments\n");
    memset(client, 0, sizeof(client));

	
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		print_error("Fatal error\n");
	int reuse = 1;
    // setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    memset(&servaddr, 0, sizeof(servaddr));
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		print_error("Fatal error\n"); 
	if (listen(sockfd, 128) != 0)
		print_error("Fatal error\n");

	

	bzero(recvBuff, BUF_SIZE);

	for (int k(0); k < 4; ++k) {
		pid_t	childFd = fork();

		if (childFd > 0) {
			std::cout << (k + 1) << "process pid is: " << childFd << std::endl;
		}
		if (childFd == 0) {
			int kq = kqueue();
			if (kq == -1)
				print_error("Fatal error\n");
			std::cout << "kqueue is : " << kq << std::endl;

			struct kevent change_list[10];
			std::vector<struct kevent> event_list;

			struct kevent server;
			EV_SET(&server, sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
			event_list.push_back(server);

			while (1) 
			{
				int retEvents = kevent(kq, &event_list[0], event_list.size(), change_list, 10, NULL);
				event_list.clear();

				for (int i = 0; i < retEvents; ++i) {
					if (change_list[i].filter == EVFILT_READ) {

						if (change_list[i].ident == sockfd) {
							connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t*)&len);
							if (connfd < 0)
								print_error("Fatal error\n");
							printf("New client connected\n");
							struct kevent event;
							// fcntl(connfd, F_SETFL, O_NONBLOCK);
							// EV_SET(&event, connfd, EVFILT_READ, EV_ONESHOT, 0, 0, NULL);
							EV_SET(&event, connfd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
							event_list.push_back(event);
							EV_SET(&event, connfd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, NULL);
							event_list.push_back(event);
						}
						else {
							char tmpBuf[4];
							bzero(tmpBuf, sizeof(tmpBuf));
							int n = read(change_list[i].ident, tmpBuf, 4);
							if (n <= 0) {
								struct kevent event;
								EV_SET(&event, change_list[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
								close(change_list[i].ident);
								continue;
							}
							strncat(recvBuff, tmpBuf, n);
							printf("Message from %dclient: %s\n", (int)change_list[i].ident, recvBuff);
							printf("===== %d =====\n", n);
							struct kevent event;
							if (n == 4) {
								EV_SET(&event, change_list[i].ident, EVFILT_READ, EV_CLEAR, 0, 0, NULL);
							}
							else {
								EV_SET(&event, change_list[i].ident, EVFILT_WRITE, EV_ADD | EV_CLEAR , 0, 0, NULL);
							}
							// EV_SET(&event, change_list[i].ident, EVFILT_WRITE, EV_ENABLE , 0, 0, NULL);
							// event_list.push_back(event);
							// EV_SET(&event, change_list[i].ident, EVFILT_READ, EV_CLEAR, 0, 0, NULL);
							// event_list.push_back(event);
							// bzero(tmpBuf, 4);
							// bzero(recvBuff, BUF_SIZE);
						}
					} else if (change_list[i].filter == EVFILT_WRITE) {
						std::string	tmpBuf = "test\n";
						int	n = send(change_list[i].ident, tmpBuf.c_str(), tmpBuf.length(), 0);
						if (n <= 0) {
							close(change_list[i].ident);
						}
						struct kevent event;
						EV_SET(&event, change_list[i].ident, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
						event_list.push_back(event);
					}
				}
			}
		}
	}
	for (int i = 0; i < 4; ++i) {
		wait(NULL);
	}
}
// #pragma omp ciritical

#include <cstdint>
#include <cstdio>
#include <sys/_types/_socklen_t.h>
#include <sys/event.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sstream>
#include <semaphore.h>
#include <pthread.h>
#include <vector>
#include <set>
#include <signal.h>
#include "../../Utils/Color.hpp"

#define FORK_NUM 4

int	main() {
	signal(SIGPIPE, SIG_IGN);
	sem_unlink("/testSem");
	sem_t*	sem = sem_open("/testSem", O_CREAT, 0644, 1);
	if (sem == SEM_FAILED) {
		std::cerr << "sem failed\n";
		exit(EXIT_FAILURE);
	}
	sem_unlink("/testSem2");
	sem_t*	sem2 = sem_open("/testSem2", O_CREAT, 0644, 1);
	if (sem2 == SEM_FAILED) {
		std::cerr << "sem2 failed\n";
		exit(EXIT_FAILURE);
	}

	std::set<int>	clientList;

	int	fd = socket(AF_INET, SOCK_STREAM, 0);

	int	fd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0 || fd2 < 0) {
		std::cerr << "socket init fail" << std::endl;
		return (0);
	}

	fcntl(fd, F_SETFL, O_NONBLOCK);
	fcntl(fd2, F_SETFL, O_NONBLOCK);
	struct sockaddr_in	addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(0);
	addr.sin_port = htons(8080);

	struct sockaddr_in	addr2;
	bzero(&addr2, sizeof(addr2));
	addr2.sin_family = AF_INET;
	addr2.sin_addr.s_addr = htonl(0);
	addr2.sin_port = htons(8081);

	// int	reuse = 1, reuse2 = 1;
	// setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_NOSIGPIPE, &reuse, sizeof(reuse));
	// setsockopt(fd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_NOSIGPIPE, &reuse2, sizeof(reuse2));

	if (bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
		std::cerr << "bind fail" << std::endl;
		return (0);
	}

	if (listen(fd, 128) < 0) {
		std::cerr << "listen fail" << std::endl;
		return (0);
	}

	if (bind(fd2, reinterpret_cast<const sockaddr*>(&addr2), sizeof(addr2)) < 0) {
		std::cerr << "bind fail" << std::endl;
		return (0);
	}

	if (listen(fd2, 128) < 0) {
		std::cerr << "listen fail" << std::endl;
		return (0);
	}


#ifdef FORK
	for (int k(0); k < FORK_NUM; ++k) {
		pid_t	childFd = fork();

		if (childFd > 0) {
			std::cout << (k + 1) << "process pid is: " << childFd << std::endl;
		}
		if (childFd == 0) {
#endif
			
			int kq = kqueue();
			if (kq < 0) {
				std::cerr << "epoll fail" << std::endl;
				close(fd);
				close(fd2);
				return (0);
			}
			std::vector<struct kevent>	changes(2);

			EV_SET(&changes[0], fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
			EV_SET(&changes[1], fd2, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);


			while (1) {
				struct kevent	events[1024];
				std::vector<struct kevent>	newEvents;
				int	nev = kevent(kq, &changes[0], changes.size(), &events[0], 1024, NULL);
				for (int nEvents(0); nEvents < nev; ++nEvents) {
					newEvents.push_back(events[nEvents]);
				}
				changes.clear();
				// std::cout << "Number of Events: " << nev << std::endl;
				if (nev < 0) {
					std::cerr << "kevent error" << std::endl;
					return (0);
				}

				for (int i(0); i < nev; ++i) {
					const int	eventFd = newEvents[i].ident;
					if (newEvents[i].filter == EVFILT_READ) {

						if (eventFd == fd) {
							struct sockaddr_in	clientAddr;
							socklen_t	clientLen = sizeof(clientAddr);
							int	clientFd;

						// std::cout << "B Accept Error Number: " << errno << std::endl;
						
							if (sem_trywait(sem) == 0) {
								if (clientList.size() > 2) {
									printf("%d max client reached", (int)clientList.size());
									sem_post(sem);
									continue;
								}
								clientFd = accept(fd, (struct sockaddr*)&clientAddr, &clientLen);
								if (clientFd < 0) {
									printf("accept fail server1: %d %d\n", getpid(), errno);
									sem_post(sem);
									continue ;
								}
								// printf("accept: client[%d] %d\n", eventFd, getpid());
								clientList.insert(clientFd);
								usleep(1000);
								sem_post(sem);
							} else {
								if (errno == EAGAIN) {
									continue ;
								}
							}
							// std::cout << "A Accept Error Number: " << errno << std::endl;

							if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
								printf("nonblock set fail 1 %d\n", clientFd);
							}

							struct kevent newEvent;
							EV_SET(&newEvent, clientFd, EVFILT_READ, EV_ADD | EV_CLEAR | EV_ONESHOT, 0, 0, NULL);
							changes.push_back(newEvent);
						} else if (eventFd == fd2) {
							struct sockaddr_in	clientAddr;
							socklen_t	clientLen = sizeof(clientAddr);
							int	clientFd;

							// std::cout << "B Accept Error Number: " << errno << std::endl;
							
							if (sem_trywait(sem2) == 0) {
								if (clientList.size() > 512) {
									printf("%d max client reached\n", (int)clientList.size());
									sem_post(sem2);
									continue;
								}
								clientFd = accept(fd2, (struct sockaddr*)&clientAddr, &clientLen);
								if (clientFd < 0) {
									printf("accept fail server2: %d %d\n", getpid(), errno);
									sem_post(sem2);
									continue ;
								}
								// printf("accept: client[%d] %d\n", eventFd, getpid());
								clientList.insert(clientFd);
								usleep(1000);
								sem_post(sem2);
							} else {
								if (errno == EAGAIN) {
									continue ;
								}
							}
							// std::cout << "A Accept Error Number: " << errno << std::endl;

							if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
								printf("nonblock set fail 2 %d\n", clientFd);
							}

							struct kevent newEvent;
							EV_SET(&newEvent, clientFd, EVFILT_READ, EV_ADD | EV_CLEAR | EV_ONESHOT, 0, 0, NULL);
							changes.push_back(newEvent);
						} else {
							char	data[4096];
							bzero(&data, sizeof(data));
							// std::cout << "Before Error Number: " << errno << std::endl;
							// const int	len = recv(eventFd, &data, sizeof(data), 0);
							const int	len = read(eventFd, &data, sizeof(data));
							// std::cout << "After Error Number: " << errno << std::endl;
							std::string	tmp(data);
							std::cout << BOLDRED << tmp << RESET << std::endl;

							printf("%d, %d client read %d\n", (int)getpid(), eventFd, len);
							std::cout << BOLDGREEN;
							for (int i = 0; i < 10; i++) {
								std::cout << (int)data[i] << " ";
							}
							std::cout << RESET << std::endl;
							// printf("%d client list size\n", (int)clientList.size());

							if (len <= 0) {
								// send(eventFd, "", 0, 0);
								std::cout << eventFd << ": Client Close\n";
								clientList.erase(eventFd);
								struct kevent	delEvent;
								EV_SET(&delEvent, fd, EVFILT_READ, EV_CLEAR, 0, 0, NULL);
								changes.push_back(delEvent);
								if (close(eventFd) < 0) {
									std::cerr << "close error" << std::endl;
								}
							} else {
								// std::cout << "[" << getpid() << "]process [" << newEvents[i].ident << "] data is: " << data << std::endl;

								struct kevent	newEvent;
								EV_SET(&newEvent, eventFd, EVFILT_WRITE, EV_ADD | EV_CLEAR | EV_ONESHOT, 0, 0, NULL);
								changes.push_back(newEvent);
							}					
						}
					} else if (newEvents[i].filter == EVFILT_WRITE) {
							std::string body;
							body = "HTTP/1.1 404 Not Found\r\n";
							body += "Connection: close\r\n";
							body += "Content-Type: text/html\r\n";
							body += "Content-length: 167\r\n\r\n";
							
							body += "<!DOCTYPE html>\r\n";
							body += "<html>\r\n";
							body += "<head>\r\n";
							body += "    <title>Not Found</title>\r\n";
							body += "</head>\r\n";
							body += "<body>\r\n";
							body += "    <h1>404 Not Found</h1>\r\n";
							body += "	<hr>\r\n";
							body += "	<p>Not found requested file</p>\r\n";
							body += "</body>\r\n";
							body += "</html>\r\n";
						// const std::string str = "HTTP/1.1 200 OK\r\nDate: Fri, 10 Jan 2024 12:00:00 GMT\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n";
						// const std::string str = "HTTP/1.1 302 Found\r\nLocation: https://example.com/new-location\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
							int res = send(newEvents[i].ident, body.c_str(), strlen(body.c_str()), 0);
							if (res <= 0) {
								std::cerr << BOLDRED << newEvents[i].ident << " SEND ERROR" << res << "\n";
								clientList.erase(newEvents[i].ident);
								close (newEvents[i].ident);
								continue ;
							}
							// close(newEvents[i].ident);
						// std::string*	writeData = static_cast<std::string*>(newEvents[i].udata);
						// write(newEvents[i].ident, writeData->c_str(), writeData->size());
						// ::delete writeData;
						struct kevent	newEvent;
						EV_SET(&newEvent, newEvents[i].ident, EVFILT_READ, EV_ADD | EV_CLEAR | EV_ONESHOT, 0, 0, NULL);
						changes.push_back(newEvent);
						// clientList.erase(newEvents[i].ident);
						// close(newEvents[i].ident);
					}
				}
			}
#ifdef FORK
		}
	}
#endif
for (int i = 0; i < FORK_NUM; i++) {
	wait(NULL);
}
	sem_close(sem);
	sem_unlink("/testSem");
	sem_close(sem2);
	sem_unlink("/testSem2");
}
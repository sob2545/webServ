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



int	main() {
	sem_t*	sem = sem_open("/testSem", O_CREAT, 0644, 1);
	if (sem == SEM_FAILED) {
		std::cerr << "sem failed\n";
		exit(EXIT_FAILURE);
	}

	std::vector<int>	clientList;

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

	// int	reuse = 1;
	// setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse));
	// setsockopt(fd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse));

	if (bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
		std::cerr << "bind fail" << std::endl;
		return (0);
	}

	if (listen(fd, 10) < 0) {
		std::cerr << "listen fail" << std::endl;
		return (0);
	}

	if (bind(fd2, reinterpret_cast<const sockaddr*>(&addr2), sizeof(addr2)) < 0) {
		std::cerr << "bind fail" << std::endl;
		return (0);
	}

	if (listen(fd2, 10) < 0) {
		std::cerr << "listen fail" << std::endl;
		return (0);
	}


#ifdef FORK
	for (int k(0); k < 4; ++k) {
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

			EV_SET(&changes[0], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
			EV_SET(&changes[1], fd2, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);


			while (1) {
				struct kevent	events[1024];
				std::vector<struct kevent>	newEvents;
				int	nev = kevent(kq, &changes[0], changes.size(), &events[0], 1024, NULL);
				for (int nEvents(0); nEvents < nev; ++nEvents) {
					newEvents.push_back(events[nEvents]);
				}
				changes.clear();
				std::cout << nev << std::endl;
				if (nev < 0) {
					std::cerr << "kevent error" << std::endl;
					return (0);
				}

				for (int i(0); i < nev; ++i) {
					const int	eventFd = newEvents[i].ident;
					if (eventFd == fd) {
						struct sockaddr_in	clientAddr;
						socklen_t	clientLen = sizeof(clientAddr);
						int	clientFd;

						if (sem_trywait(sem) == 0) {
							std::cout << errno << std::endl;
							clientFd = accept(fd, (struct sockaddr*)&clientAddr, &clientLen);
							if (clientFd == -1) {
								std::cerr << "\033[1m\033[31m" << "Accept is -1\n" << "\033[0m";
							}
							printf("accept: client[%d] %d\n", clientFd, getpid());
							clientList.push_back(clientFd);
							sem_post(sem);
						} else {
							if (errno == EAGAIN) {
								continue ;
							}
						}
						if (clientFd < 0) {
							printf("accept fail: %d\n", getpid());
							continue ;
						}

						if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
							std::cerr << "nonblock set fail\n";
						}

						struct kevent newEvent;
						EV_SET(&newEvent, clientFd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
						changes.push_back(newEvent);
					} else if (eventFd == fd2) {
						struct sockaddr_in	clientAddr;
						socklen_t	clientLen = sizeof(clientAddr);
						int	clientFd;

						if (sem_trywait(sem) == 0) {
							std::cout << errno << std::endl;
							clientFd = accept(fd2, (struct sockaddr*)&clientAddr, &clientLen);
							if (clientFd == -1) {
								std::cerr << "\033[1m\033[31m" << "Accept is -1\n" << "\033[0m";
							}
							printf("accept: client[%d] %d\n", clientFd, getpid());
							clientList.push_back(clientFd);
							sem_post(sem);
						} else {
							if (errno == EAGAIN) {
								continue ;
							}
						}

						if (clientFd < 0) {
							printf("accept fail: %d\n", getpid());
							continue ;
						}

						if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
							std::cerr << "nonblock set fail\n";
						}

						struct kevent newEvent;
						EV_SET(&newEvent, clientFd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
						changes.push_back(newEvent);
					}

					if (newEvents[i].filter == EVFILT_READ) {
						char	data[4096];
						bzero(&data, sizeof(data));
						const int	len = recv(eventFd, &data, sizeof(data), 0);


						if (len == 0) {
							// send(eventFd, "", 0, 0);
							close(eventFd);
						} else {
							// std::cout << "[" << getpid() << "]process [" << newEvents[i].ident << "] data is: " << data << std::endl;

							struct kevent	newEvent;
							EV_SET(&newEvent, eventFd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
							changes.push_back(newEvent);
						}					
					} else if (newEvents[i].filter == EVFILT_WRITE) {
						std::string	msg = "HTTP/1.1 200 OK\r\nDate: Mon, 09 Jan 2024 12:00:00 GMT\r\nServer: Apache/2.4.1 (Unix)\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 12345\r\n\r\n<html>\r\n<head>\r\n    <title>Example Page</title>\r\n</head>\r\n<body>\r\n    <h1>Welcome to the Example Page</h1>\r\n    <p>This is a sample page to demonstrate a HTTP 200 OK response.</p>\r\n</body>\r\n</html>\r\n";
						send(eventFd, msg.c_str(), msg.size(), 0);
						struct kevent	newEvent;
						EV_SET(&newEvent, newEvents[i].ident, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
						changes.push_back(newEvent);
					}
				}
			}
#ifdef FORK
		}
	}
#endif
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	sem_close(sem);
	sem_unlink("/testSem");
}
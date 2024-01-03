// #pragma omp ciritical

#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <wait.h>
#include <sstream>
#include <semaphore.h>
#include <pthread.h>



int	main() {
	sem_t*	sem = sem_open("/testSem", O_CREAT, 0644, 1);
	if (sem == SEM_FAILED) {
		std::cerr << "sem failed\n";
		exit(EXIT_FAILURE);
	}

	int	fd = socket(AF_INET, SOCK_STREAM, 0);

	int	fd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0 || fd2 < 0) {
		std::cerr << "socket init fail" << std::endl;
	}

	fcntl(fd, F_SETFL, O_NONBLOCK);
	fcntl(fd2, F_SETFL, O_NONBLOCK);
	struct sockaddr_in	addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(2130706433);
	addr.sin_port = htons(8080);

	struct sockaddr_in	addr2;
	bzero(&addr2, sizeof(addr2));
	addr2.sin_family = AF_INET;
	addr2.sin_addr.s_addr = htonl(2130706433);
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



	for (int k(0); k < 4; ++k) {
		pid_t	childFd = fork();

		if (childFd > 0) {
			std::cout << (k + 1) << "process pid is: " << childFd << std::endl;
		}
		if (childFd == 0) {
			
			int epollFd = epoll_create(1);
			if (epollFd < 0) {
				std::cerr << "epoll fail" << std::endl;
				close(fd);
				return (0);
			}
			struct epoll_event	events;
			events.events = EPOLLIN | EPOLLET;
			events.data.fd = fd;

			struct epoll_event	events2;
			events2.events = EPOLLIN | EPOLLET;
			events2.data.fd = fd2;

			if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &events) < 0) {
				std::cerr << "epoll ctl fail" << std::endl;
				close(fd);
				close(epollFd);
				return (0);
			}

			if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd2, &events2) < 0) {
				std::cerr << "epoll ctl fail" << std::endl;
				close(fd2);
				close(epollFd);
				return (0);
			}

			int	maxEvent = 1024;
			struct epoll_event	epollEvents[maxEvent];
			int		eventCount;
			int		timeout = 0;

			while (1) {
				eventCount = epoll_wait(epollFd, epollEvents, maxEvent, timeout);
				if (eventCount < 0) {
					std::cerr << "epoll wait err" << std::endl;
					return 0;
				}

				for (int i(0); i < eventCount; ++i) {
					if (epollEvents[i].data.fd == fd) {
						struct sockaddr_in	clientAddr;
						int	clientLen = sizeof(clientAddr);
						int	clientFd;

						if (sem_trywait(sem) == 0) {
							clientFd = accept(fd, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddr);
							printf("accept: %d\n", getpid());
							sem_post(sem);
						} else {
							if (errno == EAGAIN) {
								continue ;
							}
						}
							

						if (clientFd < 0) {
							std::stringstream	ss;
							ss << getpid() << ": accept fail" << std::endl;
							std::cerr << ss.str();
							continue;
						}

						if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
							std::cerr << getpid() << ": set nonblock fail" << std::endl;
						}

						struct epoll_event	clientEvent;
						clientEvent.events = EPOLLIN | EPOLLET;
						clientEvent.data.fd = clientFd;

						if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &clientEvent) < 0) {
							std::cerr << "client epoll fail" << std::endl;
							close(clientFd);
							continue;
						}
					} else if (epollEvents[i].data.fd == fd2) {
						struct sockaddr_in	clientAddr;
						int	clientLen = sizeof(clientAddr);
						int	clientFd;

						if (sem_trywait(sem) == 0) {
							clientFd = accept(fd2, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddr);
							printf("accept: %d\n", getpid());
							sem_post(sem);
						} else {
							if (errno == EAGAIN) {
								continue ;
							}
						}

						if (clientFd < 0) {
							printf("accept fail: %d\n", getpid());
							continue;
						}

						if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
							std::cerr << "set nonblock fail" << std::endl;
						}


						struct epoll_event	clientEvent;
						clientEvent.events = EPOLLIN | EPOLLET;
						clientEvent.data.fd = clientFd;

						if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &clientEvent) < 0) {
							std::cerr << "client epoll fail" << std::endl;
							close(clientFd);
							continue;
						}
					}
					else {
						int clientFd = epollEvents[i].data.fd;
						char	data[4096];
						bzero(&data, sizeof(data));
						const int	len = read(clientFd, &data, sizeof(data));


						if (len == 0) {
							std::cout << "client disconnect" << std::endl;
							close(clientFd);
							epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, NULL);
						}
						else {
							std::cout << getpid() << " data is: " << data << std::endl;
						}
					}
				}
			}
		}
		else {
		}
	}
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	sem_close(sem);
	sem_unlink("/testSem");
}
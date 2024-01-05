// #pragma omp ciritical

#include <cstdint>
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
			
			int kq = kqueue();
			if (kq < 0) {
				std::cerr << "epoll fail" << std::endl;
				close(fd);
				return (0);
			}
			struct kevent	changes[2];
			EV_SET(&changes[0], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
			EV_SET(&changes[1], fd2, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

			if (kevent(kq, changes, 2, NULL, 0, NULL) < 0) {
				std::cerr << "kevent fail" << std::endl;
				close(fd);
				close(fd2);
				close(kq);
				return (0);
			}

			while (1) {
				struct kevent	events[1024];
				int	nev = kevent(kq, NULL, 0, events, 1024, NULL);
				if (nev < 0) {
					std::cerr << "kevent error" << std::endl;
					return (0);
				}

				for (int i(0); i < nev; ++i) {
					const int	eventFd = events[i].ident;
					if (events[i].filter == EVFILT_READ) {

						if (eventFd == fd) {
							struct sockaddr_in	clientAddr;
							int	clientLen = sizeof(clientAddr);
							int	clientFd;

							if (sem_trywait(sem) == 0) {
								clientFd = accept(fd, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddr);
								printf("accept: client[%d] %d\n", clientFd, getpid());
								clientList.push_back(clientFd);
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

							struct kevent newEvent;
							EV_SET(&newEvent, clientFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

							if (kevent(kq, &newEvent, 1, NULL, 0, NULL) < 0) {
								std::cerr << "kevent add fail" << std::endl;
								close(clientFd);
								continue;
							}

						} else if (eventFd == fd2) {
							struct sockaddr_in	clientAddr;
							int	clientLen = sizeof(clientAddr);
							int	clientFd;

							if (sem_trywait(sem) == 0) {
								clientFd = accept(fd2, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddr);
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
								continue;
							}

							if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
								std::cerr << "set nonblock fail" << std::endl;
							}

							struct kevent newEvent;
							EV_SET(&newEvent, clientFd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);

							if (kevent(kq, &newEvent, 1, NULL, 0, NULL) < 0) {
								std::cerr << "kevent add fail" << std::endl;
								close(clientFd);
								continue;
							}
							
						} else {
							int clientFd = events[i].ident;
							char	data[4096];
							bzero(&data, sizeof(data));
							const int	len = read(clientFd, &data, sizeof(data));


							if (len == 0) {
								std::cout << "client disconnect" << std::endl;
								close(clientFd);
							} else {
								std::cout << "[" << getpid() << "]process [" << events[i].ident << "] data is: " << data << std::endl;

								for (int k(0); k < clientList.size(); ++k) {
									if (clientFd == clientList[k]) {
										struct kevent	newEvent;
										EV_SET(&newEvent, clientFd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
										if (kevent(kq, &newEvent, 1, NULL, 0, NULL) < 0) {
											std::cerr << "kevent add fail3\n";
											close(clientFd);
										}
										continue ;
									}
									std::string*	writeData = ::new std::string(data);
									struct kevent	newEvent;
									EV_SET(&newEvent, clientList[k], EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, writeData);
									if (kevent(kq, &newEvent, 1, NULL, 0, NULL) < 0) {
										std::cerr << "kevent add fail3\n";
										for (int clientFds(0); clientFds < clientList.size(); ++clientFds) {
											close(clientFds);
										}
										::delete writeData;
										break;
									}
								}
							}
						}					
					} else if (events[i].filter == EVFILT_WRITE) {
						std::string*	writeData = static_cast<std::string*>(events[i].udata);
						write(events[i].ident, writeData->c_str(), writeData->size());
						::delete writeData;
						struct kevent	newEvent;
						EV_SET(&newEvent, events[i].ident, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
						if (kevent(kq, &newEvent, 1, NULL, 0, NULL) < 0) {
							std::cerr << "kevent add fail4\n";
							close(events[i].ident);
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
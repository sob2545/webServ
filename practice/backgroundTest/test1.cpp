#include <unistd.h>
#include <string>
#include <cstdlib>
#include <sys/event.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>

#define PORT 9999

int	backgroundTest(bool tf) {

#ifdef RE
	if (tf) {
		int	pid = fork();
		if (pid != 0) { 
			exit(0);
		}
	}
#endif
			int	fd = socket(AF_INET, SOCK_STREAM, 0);
			if (fd < 0) { 
				std::cerr << "error\n";
				exit(1);
			}
			fcntl(fd, F_SETFL, O_NONBLOCK);
			struct sockaddr_in	addr;
			bzero(&addr, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(0);
			addr.sin_port = htons(PORT);

			if (bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
				std::cerr << "bind fail" << std::endl;
				return (0);
			}

			if (listen(fd, 10) < 0) {
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
							return (0);
						}
						std::vector<struct kevent>	changes(1);
						EV_SET(&changes[0], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					while (1) {
						struct kevent	events[1024];
						std::vector<struct kevent>	newEvents;
						int	nev = kevent(kq, &changes[0], changes.size(), &events[0], 1024, NULL);
						for (int nEvents(0); nEvents < nev; ++nEvents) {
							newEvents.push_back(events[nEvents]);
						}
						changes.clear();

						for (int i(0); i < nev; ++i) {
							const int	eventFd = newEvents[i].ident;
							if (eventFd == fd) {
								struct sockaddr_in	clientAddr;
								socklen_t	clientLen = sizeof(clientAddr);
								int	clientFd;

								// std::cout << "B Accept Error Number: " << errno << std::endl;
								clientFd = accept(fd, (struct sockaddr*)&clientAddr, &clientLen);
								if (clientFd < 0) {
									std::cout << fd << "client accepted\n";
									printf("accept fail: %d\n", getpid());
									continue ;
								}
								// std::cout << "A Accept Error Number: " << errno << std::endl;

								if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
									std::cerr << "nonblock set fail\n";
								}

								struct kevent newEvent;
								EV_SET(&newEvent, clientFd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
								changes.push_back(newEvent);
								continue;
							} else {
								if (newEvents[i].filter == EVFILT_READ) {
									char	data[4096];
									bzero(&data, sizeof(data));
									// std::cout << "Before Error Number: " << errno << std::endl;
									const int	len = read(eventFd, &data, sizeof(data));
									// std::cout << "After Error Number: " << errno << std::endl;


									if (len == 0) {
										std::cout << eventFd << ": Client Close\n";
										close(eventFd);
									} else {

										struct kevent	newEvent;
										EV_SET(&newEvent, eventFd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
										changes.push_back(newEvent);
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
										int res = send(newEvents[i].ident, body.c_str(), strlen(body.c_str()), 0);
										if (res <= 0) {
											std::cerr << "\033[1m\033[31" << "SEND ERROR\n";
											close (newEvents[i].ident);
											continue ;
										}
									struct kevent	newEvent;
									EV_SET(&newEvent, newEvents[i].ident, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
									changes.push_back(newEvent);
									// close(newEvents[i].ident);
								}
							}
						}
					}
	#ifdef FORK
				}
			}
	#endif
	#ifdef FORK
		wait(NULL);
		wait(NULL);
		wait(NULL);
		wait(NULL);
	#endif
}

int	main(int ac, char** av) {
	if (ac != 2) {
		std::cerr << "wrong argument\n";
		return (1);
	}
	std::string	tmp(av[1]);
	if (tmp == "on") {
		backgroundTest(true);
	} else {
		backgroundTest(false);
	}
}
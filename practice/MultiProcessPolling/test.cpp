#include <cstdint>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <vector>

#define PORT 9999
#define MAX_EVENTS 5

int main() {
    int listen_sock, client_sock, kq;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    // struct kevent change_event;
	std::vector<struct kevent>	change_event;
    struct kevent event_list[MAX_EVENTS];
    char buffer[1024];
    int nbytes;

    // 소켓 생성
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock == -1) {
        std::cerr << "소켓 생성 실패" << std::endl;
        return -1;
    }

    fcntl(listen_sock, F_SETFL, O_NONBLOCK);

    // 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

	int	reuse = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR , &reuse, sizeof(reuse));

    // 소켓 바인딩
    if (bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "바인딩 실패" << std::endl;
        return -1;
    }

    // 소켓 리스닝
    if (listen(listen_sock, 100) == -1) {
        std::cerr << "리스닝 실패" << std::endl;
        return -1;
    }

    // kqueue 생성
    kq = kqueue();
    if (kq == -1) {
        std::cerr << "kqueue 생성 실패" << std::endl;
        return -1;
    }

    // 이벤트 등록
	struct kevent	serverEv;
    EV_SET(&serverEv, listen_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &serverEv, 1, NULL, 0, NULL) == -1) {
        std::cerr << "kevent 등록 실패" << std::endl;
        return -1;
    }
	change_event.push_back(serverEv);

    while (true) {
        // system("lsof -i:9999 | wc -l");
		std::vector<struct kevent>	newEv;
        // std::cout << "123\n";
        int nev = kevent(kq, &change_event[0], change_event.size(), event_list, MAX_EVENTS, NULL);
        if (nev < 0) {
            std::cerr << "kevent 오류" << std::endl;
            continue;
        }
		for (int k = 0; k < nev; ++k) {
			newEv.push_back(event_list[k]);
		}
		change_event.clear();

        for (int i = 0; i < nev; i++) {
            int sock = event_list[i].ident;

            // 새로운 연결 요청
            if (sock == listen_sock) {
                client_addr_size = sizeof(client_addr);
                client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_addr_size);
                if (client_sock == -1) {
                    std::cerr << "연결 수락 실패" << std::endl;
                    continue;
                }
                if (fcntl(client_sock, F_SETFL, O_NONBLOCK) < 0) {
                    close(client_sock);
                    continue;
                }
				// std::cout << client_sock << ": Client Accept\n";
				struct kevent	clientEv;
                EV_SET(&clientEv, client_sock, EVFILT_READ, EV_ADD | EV_EOF, 0, 0, NULL);
				change_event.push_back(clientEv);
				EV_SET(&clientEv, client_sock, EVFILT_WRITE, EV_ADD | EV_EOF | EV_DISABLE, 0, 0, NULL);
				change_event.push_back(clientEv);
                // kevent(kq, &change_event, 1, NULL, 0, NULL);
            }
            // 데이터 수신
            else if (event_list[i].filter == EVFILT_READ) {
                bzero(&buffer, sizeof(buffer));
                nbytes = read(sock, buffer, sizeof(buffer));
                if (nbytes <= 0) {
					std::cout << sock << ": Client Close\n";
                    close(sock);
                    continue;
                }
				struct kevent	clientEv;
                EV_SET(&clientEv, sock, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
				change_event.push_back(clientEv);
				EV_SET(&clientEv, sock, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
				change_event.push_back(clientEv);
            } else if (event_list[i].filter == EVFILT_WRITE) {
				std::string body;
				body = "HTTP/1.1 404 Not Found\r\n";
				body += "Date: Tue, 10 Jan 2024 12:00:00 GMT\r\n";
				body += "Content-Type: text/html; charset=UTF-8\r\n";
				body += "Content-length: 167\r\n\r\n";
				
				body += "<!DOCTYPE html>\r\n";
				body += "<html>\r\n";
				body += "<head>\r\n";
				body += "    <title>Not Found</title>\r\n";
				body += "</head>\r\n";
				body += "<body>\r\n";
				body += "    <h1>401 Not Found</h1>\r\n";
				body += "	<hr>\r\n";
				body += "	<p>Not found requested file</p>\r\n";
				body += "</body>\r\n";
				body += "</html>\r\n";
				int res = write(sock, body.c_str(), body.size());
				if (res <= 0) {
					close(sock);
					continue ;
				}
				struct kevent	clientEv;
				EV_SET(&clientEv, sock, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
				change_event.push_back(clientEv);
				struct kevent	clientEv1;
				EV_SET(&clientEv1, sock, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
				change_event.push_back(clientEv1);
			}
        }
        
    }

	std::cout << "server end\n";
    close(listen_sock);
    return 0;
}

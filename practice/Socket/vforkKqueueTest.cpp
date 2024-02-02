#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#define MAX_LINE 256

int main() {
    int listen_fd, conn_fd, kq, nev;
    struct sockaddr_in servaddr;
    struct kevent change_event, event;
    pid_t pid;

    // 서버 소켓 설정
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(12345);

    if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		std::cerr << "bind error\n";
	}
    listen(listen_fd, 10);

    // kqueue 설정
    kq = kqueue();
    EV_SET(&change_event, listen_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &change_event, 1, NULL, 0, NULL);

    while (1) {
        nev = kevent(kq, NULL, 0, &event, 1, NULL);
        if (nev > 0 && event.ident == listen_fd) {
            // 연결 수락
            conn_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);

            // vfork()를 사용하여 자식 프로세스 생성
            pid = vfork();
            if (pid == 0) { // 자식 프로세스
                char buffer[MAX_LINE];
                ssize_t n;

                close(listen_fd); // 자식에서는 서버 소켓을 닫는다

                // 클라이언트로부터 데이터 읽기
                while ((n = read(conn_fd, buffer, MAX_LINE)) > 0) {
                    write(conn_fd, buffer, n); // Echo
                }

                close(conn_fd);
                _exit(0);
            }
            close(conn_fd); // 부모에서는 연결 소켓을 닫는다
        }
    }

    return 0;
}

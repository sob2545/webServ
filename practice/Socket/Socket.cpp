#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <hostname> <port>\n";
        return 1;
    }

    const char *hostname = argv[1];
    const char *port = argv[2];

    struct addrinfo hints, *res;
    int sockfd;

	int status;
    char ipstr[INET6_ADDRSTRLEN];


    // 소켓 주소 구조체 초기화
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4와 IPv6 모두 사용 가능
    hints.ai_socktype = SOCK_STREAM; // TCP 소켓
    hints.ai_flags = AI_PASSIVE; // 내 IP 사용

    // 주소 정보 가져오기
    if (getaddrinfo(hostname, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 2;
    }

	std::cout << "IP addresses for " << hostname << ":" << std::endl;

    for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
        void *addr;
        std::string ipver;

        // 주소체계 확인 및 주소 포인터 설정
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // 주소를 문자열로 변환
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        std::cout << "  " << ipver << ": " << ipstr << std::endl;
    }

    freeaddrinfo(res); // 메모리 해제

    return 0;
}

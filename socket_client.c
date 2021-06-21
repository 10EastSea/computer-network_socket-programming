// 2017029916_양동해

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define PORTNUM 8888

int main(){
    // 클라이언트 소켓 정의
    int client_sockfd;
    struct sockaddr_in client_addr;

    // 클라이언트 소켓 생성 후 addr 값 세팅
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // local host로 접속
    client_addr.sin_port = htons(PORTNUM);

    connect(client_sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)); // 서버와 연결 

    while(1){
        // 버퍼 생성 및 초기화
        char buf[MAXLINE];
        memset(buf, 0x00, sizeof(buf));

        // 입력
        printf(">> ");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';

        write(client_sockfd, buf, sizeof(buf));     // 서버에게 입력 전달
        read(client_sockfd, buf, sizeof(buf));      // 서버로부터 echo 받아옴
        if(strcmp(buf, "exit") == 0){               // 입력한 문자열이 exit이면 클라이언트 연결종료
            // 클라이언트에서 정상적으로 종료가 되었다는 것을 출력
            printf("Successfully terminated!\n");
            close(client_sockfd);
            break;
        }
        printf("Sever(echo): %s \n", buf);          // 받아온 문자열 출력
    }

    return 0;
}
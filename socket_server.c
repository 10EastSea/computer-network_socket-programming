// 2017029916_양동해

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define PORTNUM 8888

int clientIdx = 0;

int main(){
    // 서버 소켓 정의
    int server_sockfd;
    struct sockaddr_in server_addr;

    // 서버 소켓 생성 후 addr 값 세팅
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORTNUM);

    bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)); // bind를 통해 소켓과 addr 묶어줌 
    listen(server_sockfd, 5); // listen으로 대기 

    printf("Accepting...\n");
    while(1){
        // 클라이언트 소켓 정의 
        int client_sockfd;
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        
        // 클라이언트 소켓 accept
        client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, (socklen_t *)&client_len);
        printf("Accept success! And wait for another accept..\n");
        clientIdx++;

        // 멀티 프로세스 (자식 프로세스 생성)
        int pid = fork();
        if(pid == 0){
            // 서버에서 클라이언트가 접속했다는 정보를 것을 출력
            printf("[Client %d] is online.\n", clientIdx);

            // 버퍼 생성 및 초기화
            char buf[MAXLINE];
            memset(buf, 0x00, MAXLINE);

            while(1){
                read(client_sockfd, buf, sizeof(buf));          // 클라이언트로부터 입력 받아옴
                write(client_sockfd, buf, sizeof(buf));         // 클라이언트에게 echo
                if(strcmp(buf, "exit") == 0){                   // 받은 문자열이 exit이면 클라이언트 연결종료
                    // 서버에서 정상적으로 종료가 되었다는 것을 출력
                    printf("[Client %d] is closed\n", clientIdx);
                    close(client_sockfd);
                    goto end;
                }
                printf("[Client %d]: %s\n", clientIdx, buf);    // 받아온 문자열 출력
            }
        }
    }
end:
    close(server_sockfd);

    return 0;
}
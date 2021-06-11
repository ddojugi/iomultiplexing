#include "header.h"
 
 
int main(int argc, char* argv[])
{
    int listenFD, connectFD;
    struct sockaddr_in listenSocket, connectSocket;
    struct timeval timeout;
 
    // reads, cpyReads 생성
    // 이때 cpyReads는 while에서 reads의 복제를 위해 이용
    fd_set reads, cpyReads;
 
    socklen_t addrSz; // address size를 구할 변수
 
    int fdMax, strLen, fdNum, i;
    char buf[BUFFER_SIZE];
 
    //if (argc != 2)
    //{
    //    printf("Usage : %s <port>\n", argv[0]);
    //    exit(1);
    //}
 
    listenFD = socket(PF_INET, SOCK_STREAM, 0);
    memset(&listenSocket, 0, sizeof(listenSocket));
 
    listenSocket.sin_family = AF_INET;
    listenSocket.sin_addr.s_addr = htonl(INADDR_ANY);
    listenSocket.sin_port = htons(PORT);
 
 
    if (bind(listenFD, (struct sockaddr *) &listenSocket, sizeof(listenSocket)) == -1) {
        printf("Can not bind.\n");
        return -1;
    }
 
    if (listen(listenFD, LISTEN_QUEUE_SIZE) == -1) {
        printf("Listen fail.\n");
        return -1;
    }
 
 
    FD_ZERO(&reads); // 소켓 셋 초기화
    FD_SET(listenFD, &reads); //  listenFD 소켓 셋 1로 설정
    fdMax = listenFD; // 소켓 마지막 번호를 받아온다
 
    while (1)
    {
        cpyReads = reads; // 복제
 
        timeout.tv_sec = 5;  // 5초
        timeout.tv_usec = 0;
 
        if ((fdNum = select(fdMax + 1, &cpyReads, 0, 0, &timeout)) == -1) // 타임아웃은 5초로 지정한다.
            break; // select 실패시
 
        if (fdNum == 0) // select의 return값이 0이면 즉, 타임아웃이면 서버 창에 timeout 출력 후 진행
        {
            printf("Time Out!\n");
            continue;
        }
 
        for (i = 0; i < fdMax + 1; i++)
        {
            if (FD_ISSET(i, &cpyReads)) // fd가 켜져있다면
            {
                if (i == listenFD) // 그때 i가 FD와 같다면
                {
                    addrSz = sizeof(connectSocket);
 
                    connectFD = accept(listenFD, (struct sockaddr*)&connectSocket, &addrSz);
 
                    FD_SET(connectFD, &reads); // reads 소켓 셋에 connectFD를 부분을 1로 바꾼다
 
                    if (fdMax < connectFD)
                        fdMax = connectFD;
 
                    printf("connected client : %d\n", connectFD);
                }
 
                else
                {
                    strLen = read(i, buf, BUFFER_SIZE);
 
                    // close request
                    if (strLen == 0)
                    {
                        FD_CLR(i, &reads); // 0으로 지운다
                        close(i);
                        printf("close client : %d\n", i);
                    }
 
                    else
                    {
                        // echo
                        write(i, buf, strLen);
                    }
                }
            }
        }
    }
 
    close(listenFD);
 
    return 0;
}

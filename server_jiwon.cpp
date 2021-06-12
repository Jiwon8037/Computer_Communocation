
#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#include <sstream>
#define BUF_SIZE 1024
#define PORTNUM 8000//포트번호

int main() {
  
    WSADATA wsaData;
    struct sockaddr_in serveraddr;
    SOCKET  listen_sock;
    int count = 0;

    while (true) {

        WSAStartup(MAKEWORD(2, 2), &wsaData);//윈속 초기화

		//소켓생성애러
        if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("Socket Creat Error.\n");
            exit(1);}

        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP
        serveraddr.sin_port = htons(PORTNUM);//포트번호

		//바인드애러
        if (bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
            printf("Socket Bind Error.\n");
            exit(1);}
		//리슨애러
        if (listen(listen_sock, 5) == SOCKET_ERROR) {
            printf("Socket Listen Error.\n");
            exit(1);}

        printf("This server is listening \n");

        struct sockaddr_in clientaddr;
        int len_addr = sizeof(clientaddr);
        SOCKET client_sock;
        int totalbuffnum;
        int buffnum;
        int readbyte;
		long totalreadbyte;
        long file_size;
        char buf[BUF_SIZE];

        FILE* fp;
        std::stringstream ssInt;
        ssInt << count << ".txt";
        fp = fopen(ssInt.str().c_str(), "wb");//바이너리모드 쓰기로 파일열기
        count++;
       client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &len_addr);//accept

        if (client_sock) {
            printf("Connection Request from Client [IP:%s, Port:%d] has been Accepted\n",
                inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
            readbyte = recv(client_sock, buf, BUF_SIZE, 0);//recv함수 데이터 받기
            file_size = atol(buf);
            totalbuffnum = file_size / BUF_SIZE + 1;
            buffnum = 0;
            totalreadbyte = 0;
            
            while (buffnum != totalbuffnum) {
                readbyte = recv(client_sock, buf, BUF_SIZE, 0);//recv함수 데이터 받기
                buffnum++;
                totalreadbyte += readbyte;
                printf("In progress: %d/%dByte(s) [%d%%]\n", totalreadbyte, file_size,
					((buffnum * 100) / totalbuffnum));
                fwrite(buf, sizeof(char), readbyte, fp);

				//파일받기오류
                if (readbyte == SOCKET_ERROR) {
                    printf("File Receive Errpr");
                    exit(1);}
            }
            fclose(fp);
            closesocket(client_sock);//클로즈소켓
        }
        else {
            printf("파일오류");
        }
        WSACleanup();//윈속 종료
        closesocket(listen_sock); //클로즈소켓
    }
    return 0;
}
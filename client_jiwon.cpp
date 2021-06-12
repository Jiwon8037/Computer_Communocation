
#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#define BUF_SIZE 1024
#define PORTNUM 8000//포트번호
#define SERVERIP "127.0.0.1"//IP주소

int main() {
   
    WSADATA wsaData;
    struct sockaddr_in severaddr;
    SOCKET s;

    while (true) {

        WSAStartup(MAKEWORD(2, 2), &wsaData);//윈속초기화

		//소켓생성애러
        if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            printf("Socket Creat Error.\n");
            exit(1);}

        memset(&severaddr, 0, sizeof(severaddr));
        severaddr.sin_family = AF_INET;
        severaddr.sin_addr.s_addr = inet_addr(SERVERIP);//IP
        severaddr.sin_port = htons(PORTNUM);//포트번호

		//커넥트애러
        if (connect(s, (SOCKADDR*)&severaddr, sizeof(severaddr)) == SOCKET_ERROR) {
            printf("Socket Connection Error.\n");
            exit(1);}

        char file_name[BUF_SIZE];
        std::cout << "보내고 싶은 파일명 입력하세요" << std::endl;
        std::cin >> file_name;
        printf("--파일전송시작--\n");

        int totalbuffnum;
        int buffnum;
        int sendbyte;
        long totalsendbyte;
        long file_size;
        char buf[BUF_SIZE];


        FILE* fp;
        fp = fopen(file_name, "rb");//바이너리모드 읽기로 파일열기
		//빈파일일경우
        if (fp == NULL) {
            printf("파일이 없습니다");
            exit(1);}

        fseek(fp, 0, SEEK_END);//파일끝으로이동
        file_size = ftell(fp);//파일크기
        totalbuffnum = file_size / sizeof(buf) + 1;
        fseek(fp, 0, SEEK_SET);//파일처음위치
        buffnum = 0;
        totalsendbyte = 0;

        _snprintf(buf, sizeof(buf), "%d", file_size);
        sendbyte = send(s, buf, sizeof(buf), 0);//send함수 데이터 보내기

        while ((sendbyte = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
            send(s, buf, sendbyte, 0);//send함수 데이터 보내기
            buffnum++;
            totalsendbyte += sendbyte;
            printf("In progress: %d/%dByte(s) [%d%%]\n", totalsendbyte, file_size, ((buffnum * 100) / totalbuffnum));
        }
        WSACleanup();
    }
    closesocket(s);
    return 0;
}

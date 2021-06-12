
#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#define BUF_SIZE 1024
#define PORTNUM 8000//��Ʈ��ȣ
#define SERVERIP "127.0.0.1"//IP�ּ�

int main() {
   
    WSADATA wsaData;
    struct sockaddr_in severaddr;
    SOCKET s;

    while (true) {

        WSAStartup(MAKEWORD(2, 2), &wsaData);//�����ʱ�ȭ

		//���ϻ����ַ�
        if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            printf("Socket Creat Error.\n");
            exit(1);}

        memset(&severaddr, 0, sizeof(severaddr));
        severaddr.sin_family = AF_INET;
        severaddr.sin_addr.s_addr = inet_addr(SERVERIP);//IP
        severaddr.sin_port = htons(PORTNUM);//��Ʈ��ȣ

		//Ŀ��Ʈ�ַ�
        if (connect(s, (SOCKADDR*)&severaddr, sizeof(severaddr)) == SOCKET_ERROR) {
            printf("Socket Connection Error.\n");
            exit(1);}

        char file_name[BUF_SIZE];
        std::cout << "������ ���� ���ϸ� �Է��ϼ���" << std::endl;
        std::cin >> file_name;
        printf("--�������۽���--\n");

        int totalbuffnum;
        int buffnum;
        int sendbyte;
        long totalsendbyte;
        long file_size;
        char buf[BUF_SIZE];


        FILE* fp;
        fp = fopen(file_name, "rb");//���̳ʸ���� �б�� ���Ͽ���
		//�������ϰ��
        if (fp == NULL) {
            printf("������ �����ϴ�");
            exit(1);}

        fseek(fp, 0, SEEK_END);//���ϳ������̵�
        file_size = ftell(fp);//����ũ��
        totalbuffnum = file_size / sizeof(buf) + 1;
        fseek(fp, 0, SEEK_SET);//����ó����ġ
        buffnum = 0;
        totalsendbyte = 0;

        _snprintf(buf, sizeof(buf), "%d", file_size);
        sendbyte = send(s, buf, sizeof(buf), 0);//send�Լ� ������ ������

        while ((sendbyte = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
            send(s, buf, sendbyte, 0);//send�Լ� ������ ������
            buffnum++;
            totalsendbyte += sendbyte;
            printf("In progress: %d/%dByte(s) [%d%%]\n", totalsendbyte, file_size, ((buffnum * 100) / totalbuffnum));
        }
        WSACleanup();
    }
    closesocket(s);
    return 0;
}

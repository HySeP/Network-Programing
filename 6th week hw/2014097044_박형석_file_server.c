#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;

	FILE *fp;
	char buf[BUF_SIZE], file_name[BUF_SIZE];
	int readCnt;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;
	/*
	if (argc != 2) {
	printf("Usage: %s <port>\n", argv[0]);
	exit(1);
	}*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi("123654"));

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);

	clntAdrSz = sizeof(clntAdr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

	recv(hClntSock, (char*)file_name, BUF_SIZE, 0);

	fopen_s(&fp, file_name, "rb");
	if (fp == NULL)
	{
		printf("������ �������� �ʽ��ϴ�.\n");
		return 1;
	}

	while (1)
	{
		//file�� ������ 1byte�� �о�
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);

		if (readCnt<BUF_SIZE)
		{//���� ������ ����
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}

		send(hClntSock, (char*)&buf, BUF_SIZE, 0);
	}

	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, (char*)buf, BUF_SIZE, 0);
	printf("Message from client: %s \n", buf);

	fclose(fp);
	closesocket(hClntSock); closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
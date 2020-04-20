
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
	SOCKET hSocket;

	char buf[BUF_SIZE];
	int readCnt;
	SOCKADDR_IN servAdr;

	/*if (argc != 3) {
	printf("Usage: %s <IP> <port>\n", argv[0]);
	exit(1);
	}*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	FILE *fp;

	

	fopen_s(&fp, "hello.txt", "wb");
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(atoi("123654"));

	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	send(hSocket, "hello.txt", 10, 0);

	while ((readCnt = recv(hSocket, buf, BUF_SIZE, 0)) != 0 || (readCnt = recv(hSocket, buf, BUF_SIZE, 0)) == -1)
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Received file data");
	send(hSocket, "Thank you", 10, 0);
	fclose(fp);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
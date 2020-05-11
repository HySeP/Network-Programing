#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>


#pragma comment(lib, "Ws2_32.lib")
#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;

	//int sock;
	char message[BUF_SIZE];
	int str_len;
	SOCKADDR_IN servAdr;

	/*
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(atoi("123654"));

	//connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));
	
	
	if(connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
	
	while(1) 
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

		send(hSocket, message, strlen(message), 0);
	
		str_len= recv (hSocket, message, BUF_SIZE-1, 0);
		message[str_len]=0;
		printf("Message from server: %s", message);
	}
	
	closesocket(hSocket);
	WSACleanup();
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
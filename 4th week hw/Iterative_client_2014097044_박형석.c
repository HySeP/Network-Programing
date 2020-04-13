#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

//#pragma commnet(lib, "ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30] = { '\0', }, message1[] = "2014097044 박형석\n";
	int str_len = 0, recv_len = 0, recv_cnt = 0;
	int idx=0, readLen=0;

	/*
	if(argc!=3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	*/
	

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");  
	
	hSocket=socket(PF_INET, SOCK_STREAM, 0);
	if(hSocket==INVALID_SOCKET)
		ErrorHandling("hSocket() error");
	
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servAddr.sin_port=htons(32154);
	
	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
		ErrorHandling("connect() error!");

	//send(hSocket, message1, sizeof(message1), 0);
	//recv(hSocket, message, sizeof(message), 0);
 
	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		//q를 입력받으면 클라이언트 종료
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		//보낸 data 길이 저장
		str_len = send(hSocket, message, strlen(message), 0);

		recv_len = 0;
		//클라이언트는 서버로부터 자신이 보낸 데이터의 크기만큼 수신
		//따라서 보낸 길이만큼 recv를 반복
		while (recv_len < str_len)
		{
			recv_cnt = recv(hSocket, &message[recv_len], BUF_SIZE, 0);
			if (recv_cnt == -1)
				ErrorHandling("read() error!");
			recv_len += recv_cnt;
		}
		printf("Message from server: %s", message);
	}



	/*
	while (1)
	{
		if (message[idx - 1] == '\n')
			break;
		readLen = recv(hSocket, &message[idx++], 1, 0);
		if (readLen == -1)
			ErrorHandling("read() error!");
		str_len += readLen;
	}
	*/

	/*
	while(readLen=recv(hSocket, &message[idx++], 1, 0))
	{
		if(readLen==-1)
			ErrorHandling("read() error!");
		
		str_len+=readLen;
	}
	*/

	/*
	printf("Message from server: %s \n", message);  
	printf("Function read call count: %d \n", str_len);
	*/

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#define BUF_SIZE 1024
void ErrorHandling( const char* message );

int main()
{
	WSADATA wasData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	int port = 5001;


	char message[BUF_SIZE];
	int strLen;


	if (WSAStartup(MAKEWORD(2, 2), &wasData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!");
	}
	else
	{
		puts("Connetced......");
	}

	while (1)
	{
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		{
			break;
		}

		send(hSocket, message, strlen(message), 0);
		strLen = recv(hSocket, message, sizeof(message) - 1, 0);
		if (strLen == -1)
		{
			ErrorHandling("read() error!");
		}
		message[strLen] = 0;
		printf("message from server: %s\n", message);
	}

	
	closesocket(hSocket);
	WSACleanup();

	return 0;
}


void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
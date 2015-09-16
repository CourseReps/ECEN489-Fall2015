#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<winsock2.h>
#include<io.h>
#include<iostream>
#include<fstream>
using namespace std;
#pragma comment(lib,"ws2_32") //Winsock Library

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	//char *message;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.");
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");
	/*//Send some data
	message = "GET Image";
	if (send(s, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");*/

	// receive the image
	FILE *picture;
	char *buffer;
	char *sizec;
	char *nc;
	nc = new char[1];
	int a, size=0;
	recv(s, nc, 1, 0);
	int n = nc[0] - '0';
	sizec = new char[n];
	recv(s, sizec, n, 0);
	for (int i = 0; i < n; i++)
	{
		a = sizec[i] - '0';
		size =size*10+a;
	}
	buffer = new char[size];
	picture = fopen("copy1.png", "wb");
	int offset = 0;
	while (size > offset)
	{
		int amount = recv(s, buffer + offset, size - offset, 0);
		if (amount <= 0)
		{
			cout << "Error: " << WSAGetLastError() << endl;
			break;
		}
		else
		{
			offset += amount;
		}
	}
	fwrite(buffer, 1, size, picture);
	fclose(picture);
	puts("Image received\n");
	system("copy1.png");
	getchar();
	closesocket(s);
	WSACleanup();
	return 0;
}

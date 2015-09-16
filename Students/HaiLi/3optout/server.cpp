// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include<stdio.h>
#include<winsock2.h>
#include<string.h>
#include<io.h>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;
#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8888);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");
	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr *)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}

	puts("Connection accepted");
	system("g:/nir/nircmd.exe savescreenshot 1.png");
	char * fileName = "1.png";
	fstream file(fileName, ios::in);
	file.seekg(0, ios::end);
	int sizeInBytes = file.tellg();

	cout << fileName << " is " << sizeInBytes << " bytes long." << endl;
	FILE *picture;
	char *buffer;
	buffer = new char[sizeInBytes];
	picture = fopen("1.png", "rb");
	fread(buffer, 1, sizeInBytes, picture);
	int size = htonl(sizeInBytes);
	int n = sizeInBytes;
	int count = 0;
	do {
		++count;
		n /= 10;
	} while (n);
	char buf[256];
	sprintf(buf, "%d", sizeInBytes);
	const char *si = &buf[0];
	char buf1[256];
	sprintf(buf1, "%d", count);
	const char *si2 = &buf1[0];
	send(new_socket, si2, 1, 0);
	send(new_socket, si,count,0);
	int offset = 0;
	while (sizeInBytes > offset)
	{
		int amount = send(new_socket, buffer + offset, sizeInBytes - offset, 0);
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
	printf("the file was sent");
	getchar();
	free(buffer);
	closesocket(s);
	WSACleanup();
	return 0;
}


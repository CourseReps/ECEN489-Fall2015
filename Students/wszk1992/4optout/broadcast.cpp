#ifndef UNICODE
#define UNICODE
#endif

#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

/*  UDP broadcast */
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int strtosht(char* str)
{
	int len = strlen(str);
	int time = 1;
	int out=0;
	for (int i = 0; i < len; i++)
	{
	
		out += (str[len - i - 1]-'0') * time;
		time = time * 10;
	}
	return out;
}

int main(int argc, char**argv)
{
	WSADATA wsaData;
	struct sockaddr_in servaddr;
	SOCKET sockfd = INVALID_SOCKET;
	struct hostent *host_out;
	FILE*pfile;
	char mesg[500]="0";
	char* json_saveaddr = "ipjson.json";
	int result=0;
	
	if (argc != 2)
	{
		printf("usage: broadcast <PORT> \n");
		exit(1);
	}
	//initialate WSA
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if ( result!= NO_ERROR)
	{
		printf("Initial Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	//socket
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if ( sockfd == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//setsock
	int optval=1;
	result = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));


	//get server IP Address
	host_out = gethostbyname("255.255.255.255");
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = *(struct in_addr *)*host_out->h_addr_list;
	servaddr.sin_port = htons(strtosht(argv[1]));

	//open JSON 
	printf("Opening the JSON.... \n");
	fopen_s(&pfile, json_saveaddr, "rb");
	if (pfile == NULL)
	{
		printf("error opening file!");
		return 1;
	}
	fseek(pfile, 0, SEEK_END);
	int flength = ftell(pfile);
	rewind(pfile);
	fread(mesg, 1, flength, pfile);
	fclose(pfile);
	printf("INFO:\n%s\n", mesg);
	//sendto
	result = sendto(sockfd, mesg, flength, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (result == SOCKET_ERROR)
	{
		printf("error:%d\n", WSAGetLastError());
		return 1;
	}
	printf("sending JSON ( %d bytes ) to the server\n", flength);
	closesocket(sockfd);
	return 0;
}
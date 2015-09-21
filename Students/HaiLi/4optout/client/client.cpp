#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include<stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main() {
	string fn = "Hai";
	string ln = "Li";
	string gn = "lihai1991";
	string ip = "127.0.0.1";
	ofstream fileout("file.json");
	fileout << "{" << endl;
	fileout << "  " << "\"firstName\": " << "\"" << fn << "\"," << endl;
	fileout << "  " << "\"lastName\": " << "\"" << ln << "\"," << endl;
	fileout << "  " << "\"gitUserName\": " << "\"" << gn << "\"," << endl;
	fileout << "  " << "\"currentIP\": " << "\"" << ip << "\"" <<endl;
	fileout << "}" << endl;
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
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	char * fileName = "file.json";
	fstream file(fileName, ios::in);
	file.seekg(0, ios::end);
	int sizeInBytes = file.tellg();

	cout << fileName << " is " << sizeInBytes << " bytes long." << endl;
	FILE *json;
	char *buffer;
	buffer = new char[sizeInBytes];
	json = fopen("file.json", "rb");
	fread(buffer, 1, sizeInBytes, json);
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
	sendto(s, si2, 1, 0, (struct sockaddr *) &server, sizeof(server));
	sendto(s, si, count, 0, (struct sockaddr *) &server, sizeof(server));
	int offset = 0;
	while (sizeInBytes > offset)
	{
		int amount = sendto(s, buffer + offset, sizeInBytes - offset, 0, (struct sockaddr *) &server, sizeof(server));
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
	getchar();
	return 0;
}


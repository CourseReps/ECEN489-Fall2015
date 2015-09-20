// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<winsock2.h>
#include<io.h>
#include<iostream>
#include<fstream>
#include <iostream>
#include <sstream>
#include <sqlite3.h>
using namespace std;
#pragma comment(lib,"ws2_32") //Winsock Library

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen = sizeof(si_other);
	//char *message;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.");
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");
	server.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	
	/*//Send some data
	message = "GET Image";
	if (send(s, message, strlen(message), 0) < 0)
	{
	puts("Send failed");
	return 1;
	}
	puts("Data Send\n");*/

	// receive the image
	FILE *json;
	char *buffer;
	char *sizec;
	char *nc;
	nc = new char[1];
	int a, size = 0;
	recv(s, nc, 1, 0);
	int n = nc[0] - '0';
	sizec = new char[n];
	recv(s, sizec, n, 0);
	for (int i = 0; i < n; i++)
	{
		a = sizec[i] - '0';
		size = size * 10 + a;
	}
	buffer = new char[size];
	json = fopen("copy.json", "wb");
	int offset = 0;
	while (size > offset)
	{
		int amount = recvfrom(s, buffer + offset, size - offset, 0, (struct sockaddr *) &si_other, &slen);
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
	fwrite(buffer, 1, size, json);
	fclose(json);
	puts("file received\n");
	//system("copy.json");
	ifstream jsonfile("copy.json");
	stringstream input;
	input << jsonfile.rdbuf();
	string fileString = input.str();
	int pos1 = fileString.find("firstName");
	int pos2 = fileString.find("lastName");
	int pos3 = fileString.find("gitUserName");
	int pos4 = fileString.find("currentIP");
	int pos5 = fileString.find("}");
	string fn = fileString.substr(pos1 + 13, pos2-24);
	string ln = fileString.substr(pos2 + 12, pos3 - pos2-18);
	string gun = fileString.substr(pos3 + 15, pos4 -pos3- 21);
	string ip = fileString.substr(pos4 + 13, pos5 - pos4-15);
	const char *fn1 = fn.c_str();
	const char *ln1 = ln.c_str();
	const char *gun1 = gun.c_str();
	const char *ip1 = ip.c_str();
	cout << fn << endl << ln << endl << gun << endl << ip << endl;
	//cout << pos1 << " " << pos2;
	//cout << pos3 << " " << pos4 << " " << pos5;
	sqlite3 *db;
	char *err_msg = 0;
	int rc = sqlite3_open("json.db", &db);
	char *sql = "DROP TABLE IF EXISTS Info;"
		"CREATE TABLE Info(firstName text, lastName text, gitUserName text,currentIP text);";
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	string sql1 = "INSERT INTO Info VALUES(";
	sql1 += "\"";
	sql1 += fn;
	sql1 += "\",";
	sql1 += "\"";
	sql1 += ln;
	sql1 += "\",";
	sql1 += "\"";
	sql1 += gun;
	sql1 += "\",";
	sql1 += "\"";
	sql1 += ip;
	sql1 += "\");";
	const char *sqll = sql1.c_str();
	cout << sqll;
	rc = sqlite3_exec(db, sqll, 0, 0, &err_msg);
	getchar();
	closesocket(s);
	WSACleanup();
	return 0;
}

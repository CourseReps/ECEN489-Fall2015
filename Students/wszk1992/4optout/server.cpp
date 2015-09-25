
/*  UDP server */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sqlite3.h>

#define MESGLEN 200
#pragma comment (lib, "Ws2_32.lib")
using namespace std;

struct JSON
{
	char firstname[20];
	char lastname[20];
	char gitUserName[20];
	char IPAddr[20];
}json;

int strtosht(char* str)
{
	int len = strlen(str);
	int time = 1;
	int out = 0;
	for (int i = 0; i < len; i++)
	{

		out += (str[len - i - 1] - '0') * time;
		time = time * 10;
	}
	return out;
}
JSON parsejson(ifstream* pstream)
{
	stringstream strm;
	string str, sub;
	string name[4], value[4];
	int pos1, pos2, posend;
	char tab2[4][20];

	strm << pstream->rdbuf();
	str = strm.str();
	posend = str.find("}");
	for (int i = 0; i < 4; i++)
	{
		pos1 = str.find("\"");
		pos2 = str.find(":");
		str = str.substr(pos2 + 1, posend - pos1 - 1);
		pos1 = str.find("\"");
		if (i < 3)
			pos2 = str.find(",");
		else
			pos2 = str.find("}");
		value[i] = str.substr(pos1 + 1, pos2 - pos1 - 2);
		str = str.substr(pos2 + 1, posend - pos1 - 1);
	}
	for (int i = 0; i < 4; i++)
	{
		strncpy(tab2[i], value[i].c_str(), sizeof(tab2[i]));
		tab2[i][sizeof(tab2[i]) - 1] = 0;
	}
	JSON json;
	strcpy(json.firstname, tab2[0]);
	strcpy(json.lastname, tab2[1]);
	strcpy(json.gitUserName, tab2[2]);
	strcpy(json.IPAddr, tab2[3]);
	return json;
}

int main(int argc, char**argv)
{
	WSADATA wsaData;
	SOCKET sockfd;
	struct hostent *host_entry;
	struct sockaddr_in servaddr,otheraddr;
	char mesg[MESGLEN];
	char szHostName[255];
	char addr[100];
	int len;
	int result;
	FILE* pfile;
	char* json_saveaddr = "ipjson_sever.json";
	char *dbaddr = "jsondata.db";
	if (argc != 2)
	{
		printf("usage: server <Port>\n");
		exit(1);
	}

	//initialate WSA
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		printf("Initial Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	//call socket
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd== INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//set the serveradd
	gethostname(szHostName, sizeof(szHostName));
	host_entry = gethostbyname(szHostName);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = *(struct in_addr *)*host_entry->h_addr_list;
	servaddr.sin_port = htons(strtosht(argv[1]));
	//print info
	inet_ntop(servaddr.sin_family, &(servaddr.sin_addr), addr, sizeof(addr));
	printf("info:\nIP:%s\nPORT:%d\n",addr,ntohs(servaddr.sin_port));
	//bind
	result=bind(sockfd, ( sockaddr *)&servaddr, sizeof(servaddr));
	if (result == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}
	printf("initial successful!\nlistening....\n");
	//receiving data
	len=sizeof(otheraddr);
	result = recvfrom(sockfd, mesg, sizeof(mesg)-1, 0, (struct sockaddr *)&otheraddr, &len);
	if (result == SOCKET_ERROR)
	{
		printf("receiving with error: %ld\n", WSAGetLastError());
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}
	else if (result > 0) 
	{
		inet_ntop(otheraddr.sin_family, &(otheraddr.sin_addr), addr, strlen(addr));
		for (int i = 0; i < sizeof(mesg) - 1; i++)
		{
			if (mesg[i] == '}')
			{
				mesg[i + 1] = '\0';
				break;
			}
		}
		printf("receive %d bytes message from %s\nData:\n%s\n\n", strlen(mesg), addr, mesg);
	}

	//create and write json
	fopen_s(&pfile, json_saveaddr, "wb");
	if (pfile == NULL)
	{
		printf("error opening file!");
		return 1;
	}
	fwrite(mesg, 1, strlen(mesg)+1, pfile);

	fclose(pfile);

	//parse json
	ifstream jsonfile(json_saveaddr);
	json=parsejson(&jsonfile);

	//SQlite
	printf("opening sqlite...\n");
	sqlite3 *db;
	char *err_msg = 0;
	int rc = sqlite3_open(dbaddr, &db);
	if (rc != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);

		return 1;
	}
	char temp[100];
	char sql[1000] = "DROP TABLE IF EXISTS std;"
		"CREATE TABLE std(Name TEXT, Value TEXT);";

	sprintf(temp, "INSERT INTO std VALUES('firstname','%s');", json.firstname);
	strcat(sql, temp);
	sprintf(temp, "INSERT INTO std VALUES('lastname','%s');", json.lastname);
	strcat(sql, temp);
	sprintf(temp, "INSERT INTO std VALUES('gitUserName','%s');", json.gitUserName);
	strcat(sql, temp);
	sprintf(temp, "INSERT INTO std VALUES('IPAddr','%s');", json.IPAddr);
	strcat(sql, temp);

	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK) {

		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		sqlite3_close(db);

		return 1;
	}

	sqlite3_close(db);
	printf("save database successful in %s\n",dbaddr);
	return 0;
}
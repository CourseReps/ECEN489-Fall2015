#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <arpa/inet.h>
#include "sqlite3.h"

using namespace std;

#define MYPORT 1111
#define REMOTEPORT 2015

int main()
{
	//setup database
	sqlite3 *db;
	sqlite3_open("db1.db", &db);
	//sqlite3_exec(db, "DROP TABLE IF EXISTS TBL1;", 0, 0, 0);
	sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS TBL1(deviceID INT PRIMARY KEY UNIQUE, deviceType TEXT, Data INT, Timestamp INT);", 0, 0, 0);
	
	//get IPs
	vector<unsigned long> IPtable;
	char IP1[15] = "10.201.72.73";
	unsigned long ip1;
	inet_pton(AF_INET, IP1, &ip1);
	IPtable.push_back(ip1); 
	
	//set up UDP socket
	int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketfd < 0)
	{
		cerr << "Error creating socket!\n";
		return -1;
	}
	struct sockaddr_in serv_addr;
	memset(&serv_addr, sizeof(serv_addr), 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(MYPORT);
	if(bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		cerr << "Error binding socket!\n";
		return -2;
	}
	
	//iterate through IPs and take action
	struct sockaddr_in rem_addr;
	memset(&rem_addr, sizeof(rem_addr), 0);
	rem_addr.sin_family = AF_INET;
	rem_addr.sin_port = htons(REMOTEPORT);
	char cmd[9] = "get_data";
	string JSON = "";
	int idpos, typepos, datapos, timepos, ippos, i;
	int id, data, timestamp;
	string type, ip;
	char tmp;
	while(1)
	{
		for(vector<unsigned long>::iterator x = IPtable.begin(); x != IPtable.end(); ++x)
		{
			rem_addr.sin_addr.s_addr = *x;
			if(sendto(socketfd, cmd, sizeof(cmd), 0, (struct sockaddr*)&rem_addr, sizeof(rem_addr)) < 0) //send request
			{
				cerr << "Sending failed for IP #" << distance(IPtable.begin(),x) << endl;
				continue;
			}
			char buffer[256] = {0};
			if(recvfrom(socketfd, buffer, sizeof(buffer), 0, 0, 0) < 0) //receive JSON object
			{
				cerr << "Socket read failed for IP #" << distance(IPtable.begin(),x) << endl;
				continue;
			}
		
			cout << "Packet received!\n";
			if(buffer[0] != '{')
			{ 
				JSON = buffer;
				cout << JSON;
				continue;
			}
			cout << "Valid packet\n";
			JSON = buffer;
			cout << JSON << endl;
			idpos = JSON.find("DeviceID");
			typepos = JSON.find("DeviceType");
			datapos = JSON.find("Data");
			timepos = JSON.find("Timestamp");
		
			i = 0;
			string in1 = "";
			while(1)
			{
				tmp = JSON.at(idpos+12+i);
				if(tmp == '\"')
					break;
				in1.push_back(tmp);
				++i;
			}
			id = atoi(in1.c_str());
		
			type.clear();
			i = 0;
			while(1)
			{
				tmp = JSON.at(typepos+14+i);
				if(tmp == '\"')
					break;
				type.push_back(tmp);
				++i;
			}
		
			i = 0;
			string in3 = "";
			while(1)
			{
				tmp = JSON.at(datapos+8+i);
				if(tmp == '\"')
					break;
				in3.push_back(tmp);
				++i;
			}
			data = atoi(in3.c_str());
		
			i = 0;
			string in4 = "";
			while(1)
			{
				tmp = JSON.at(timepos+13+i);
				if(tmp == '\"')
					break;
				in4.push_back(tmp);
				++i;
			}
			timestamp = atoi(in4.c_str());
		
			char tablecmd[256] = {0};
			sprintf(tablecmd, "INSERT OR REPLACE INTO TBL1 VALUES(%d, \"%s\", %d, %d);", id, type.c_str(), data, timestamp);
			cout << tablecmd << endl;
			sqlite3_exec(db, tablecmd, 0, 0, 0);	
		}
		usleep(1000000);
	}
}

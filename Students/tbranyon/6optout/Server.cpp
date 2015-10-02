#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <unistd.h>
#include "sqlite3.h"

using namespace std;

#define PORT_NUMBER 1111

int main(int argc, char* argv[]) //only arg is db name
{
	if(argc < 2)
	{
		cerr << "Usage: server <database name>\n";
		return -1;
	}
	sqlite3 *db;
	sqlite3_open(argv[1],&db);
	char* err_msg = 0;
	string sql = "DROP TABLE IF EXISTS TBL1;";
	sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);
	string createcmd = "CREATE TABLE TBL1(deviceID INT PRIMARY KEY UNIQUE, deviceType TEXT, Data INT, Timestamp INT, currentIP TEXT);";
	sqlite3_exec(db, createcmd.c_str(), 0, 0, &err_msg);

	int socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socketfd < 0)
	{
		cerr << "Error opening socket, exiting\n";
		close(socketfd);
		return -2;
	}
	int brdcstena = 1;
	setsockopt(socketfd, IPPROTO_UDP, SO_BROADCAST, &brdcstena, sizeof(brdcstena)); //enable broadcasting on socket 
	
	struct sockaddr_in sa;
	memset((char*)&sa,sizeof(sa),0);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(PORT_NUMBER);
	
	if(bind(socketfd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
	{
		cerr << "Error binding socket, exiting\n";
		close(socketfd);
		return -3;
	}

	char buffer[512];
	string JSON = "";
	int idpos, typepos, datapos, timepos, ippos, i;
	int id, data, timestamp;
	string type, ip;
	char tmp;
	
	//read JSON object from UDP socket and insert into table, overwriting if a matching entry exists (same deviceID)
	while(1)
	{
		recvfrom(socketfd, &buffer, sizeof(buffer), 0, 0, 0);
		system("clear");
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
		ippos = JSON.find("currentIP");
		
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
		
		ip.clear();
		i = 0;
		while(1)
		{
			tmp = JSON.at(ippos+13+i);
			if(tmp == '\"')
				break;
			ip.push_back(tmp);
			++i;
		}
		
		char tablecmd[256] = {0};
		sprintf(tablecmd, "INSERT OR REPLACE INTO TBL1 VALUES(%d, \"%s\", %d, %d, \"%s\");", id, type.c_str(), data, timestamp, ip.c_str());
		cout << tablecmd << endl;
		sqlite3_exec(db, tablecmd, 0, 0, &err_msg); 
	}
	close(socketfd);
	return -255;
}

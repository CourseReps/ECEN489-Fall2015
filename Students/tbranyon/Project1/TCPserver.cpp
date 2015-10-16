#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include "sqlite3.h"

#define PORT_NUMBER 2015

using namespace std;

static int readdb_cb(void* strJSON, int argc, char** argv, char** colname)
{
	string retval = "";
	retval += "{\n";
	retval += "\"ID\":\""; retval += argv[0]; retval += "\"\n";
	retval += "\"IRRange\":\""; retval += argv[1]; retval += "\"\n"; 
	retval += "\"PumpRate\":\""; retval += argv[2]; retval += "\"\n"; 
	retval += "\"Flowrate\":\""; retval += argv[3]; retval += "\"\n"; 
	retval += "\"SolenoidState\":\""; retval += argv[4]; retval += "\"\n";
	retval += "\"Timestamp\":\""; retval += argv[5]; retval += "\"\n";
	retval += "\"CurrentIP\":\""; retval += argv[6]; retval += "\"\n";
	retval += "}";
	char *rtn_ptr = static_cast<char*>(strJSON);
	sprintf(rtn_ptr, "%s", retval.c_str());
	return 0;
}

string get_data_as_JSON()
{
	sqlite3 *db;
	sqlite3_open("../teensydata.db", &db);
	if(db < 0 || db == NULL)
		cerr < "Err opening db\n";
	char strJSONbuf[512];
	sqlite3_exec(db, "SELECT * FROM TBL1;", readdb_cb, &strJSONbuf, 0);
	sqlite3_close(db);
	string strJSON = "";
	strJSON += strJSONbuf;
	return strJSON;
}

int main()
{
	struct sockaddr_in myaddr, remaddr;
	int socketfd, newsocketfd;
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		cerr << "Error creating socket!\n";
		return -1;
	}
	
	bzero((char*)&myaddr,sizeof(myaddr)); //zero out socket structure before setup
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(PORT_NUMBER);
	
	//bind socket
	if(bind(socketfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0)
	{
		cerr << "Error binding socket!\n";
		return -2;
	}
	
	//monitor socket for connect requests
	listen(socketfd,5);
	
	//may need to fork this part in new process
	int n, bufsize = 0;
	char data[512] = {0};
	while(1)
	{
		socklen_t length = sizeof(remaddr);
		newsocketfd = accept(socketfd, (struct sockaddr*)&remaddr, &length);
		if(newsocketfd < 0)
		{
			cerr << "Failed to accept connection!\n";
			continue;
		}
		
		bzero(data, 512); //zero out buffer
		string funcreturn = get_data_as_JSON();
		bufsize = funcreturn.length();
		sprintf(data, "%s", funcreturn.c_str()); //read info from db
		cout << "Sending:\n" << data << endl;
		n = write(newsocketfd, data, bufsize);
		if(n < 0)
			cerr << "Error writing to socket\n"; 
		close(newsocketfd);	
	}
	close(socketfd);
	return 1;	
}

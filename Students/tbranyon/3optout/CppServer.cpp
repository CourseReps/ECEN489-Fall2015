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

using namespace std;

#define PORT_NUMBER 2015

int main()
{
	//create socket
	struct sockaddr_in serv_addr, cli_addr;
	int socketfd, newsocketfd, portnum;
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		cerr << "Error opening socket, exiting...\n";
		return 1;
	}
	
	bzero((char*) &serv_addr, sizeof(serv_addr)); //initialize "buffer" serv_addr to all zeroes
	portnum = PORT_NUMBER;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; //address is our address since we're the host
	serv_addr.sin_port = htons(portnum); //set port number in network byte order
	
	//bind socket
	if(bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		cerr << "Error binding socket, exiting...\n";
		return 2;
	}
	
	//monitor for connections
	listen(socketfd,5);
	
	while(1) //stay alive after we finish this request and wait for other requests
	{
		//accept incoming connect request
		socklen_t clilen = sizeof(cli_addr);
		newsocketfd = accept(socketfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsocketfd < 0)
		{
			cerr << "Error accepting socket connection, exiting...\n";
			return 3;
		}
	
		//listen for "get_photo" command, ignoring all others
		char sockdata[10]; //buffer for received data
		bzero(sockdata, sizeof(sockdata));
		while(memcmp(sockdata,"get_photo",9) != 0) //block until we receive the command
		{
			int n = read(newsocketfd, sockdata, 9); //read from socket
			if(n < 0)
				cerr << "Error reading from socket\n";
		}
		cout << "Command received!\n";
	
		//take screenshot and read the file into a buffer
		system("import -window root screenshot.jpeg"); //take the screenshot and store to jpeg
		FILE* image = fopen("screenshot.jpeg", "rb"); //open file
		if(!image)
		{
			cerr << "Error opening screenshot file\n";
			continue; //exits this iteration of the socket connection loop
		}
		fseek(image, 0, SEEK_END);
		int size = ftell(image);
		fseek(image, 0, SEEK_SET);
		cout << sizeof(int); //DEBUG
		if(write(newsocketfd, (void*)&size, sizeof(int)) < 0)
			cerr << "Error writing length to socket\n";
		char buffer[10240];
		int readsize = 0;
		while(!feof(image))
		{
			readsize = fread(buffer, 1, sizeof(buffer)-1, image);
			if(write(newsocketfd, buffer, readsize) < 0)
				cerr << "Error writing file to socket\n";
			bzero(buffer,sizeof(buffer));
		}
		close(newsocketfd); //close the opened connection
	}

	return 0;
}

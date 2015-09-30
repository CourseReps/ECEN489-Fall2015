#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <ctime>
#include <arpa/inet.h>

#define MY_PORT_NUMBER 2015
#define REM_PORT_NUMBER 1111

using namespace std;

const char* cmd1 = "cat /dev/ttyACM0 > data.txt 2>/dev/null";
const char* cmd2 = "cat /dev/ttyACM1 > data.txt 2>/dev/null";
const char* cmd3 = "cat /dev/ttyACM3 > data.txt 2>/dev/null";

int main(int argc, char* argv[])
{	
	if(argc < 2)
	{
		cerr << "Not enough arguments\n";
		return -5;
	}
	system("clear");
	//read serial data
	cout << "\nRetrieving serial reading...\n";
	int data = -1;
	if(system(cmd1))
		if(system(cmd2))
			if(system(cmd3))
				cerr << "Error finding serial device!!!! Check /dev/ttyACM#...\n";
	FILE* fp = fopen("data.txt", "r");
	fscanf(fp, "%d", &data);
	fclose(fp);
	system("rm data.txt");
	cout << "Teensy ADC reading is: " << data << "\n\n";
	char chardata[10];
	sprintf(chardata, "%d", data);
	string strData = chardata;
	
	//find IP
	cout << "Retrieving IP...\n";
	system("ifconfig wlan0 | grep \"inet addr\" > ip.txt");
	fp = fopen("ip.txt", "r");
	while(fgetc(fp) != ':'){} //seek to IP start
	char tmp = '\0';
	string IP = "";
	while(1)
	{
		tmp = fgetc(fp);
		if(tmp == ' ')
			break;
		IP.push_back(tmp);
	}
	fclose(fp);
	cout << "Current IP is: " << IP << "\n\n";
	
	//get timestamp
	cout << "Getting timestamp...\n";
	time_t timer;
	time(&timer);
	char strdate[100];
	sprintf(strdate, "%s", ctime(&timer));
	char timetmp[100];
	sprintf(timetmp, "%li", timer);
	string timestamp = timetmp;
	cout << "Current timestamp is: " << timestamp << " -- " << strdate << "\n";
	
	//get device ID
	//for now, we'll use the ASCII values of the first 4 characters of the hostname to form an ID
	cout << "Getting Device ID...\n";
	unsigned int devID = 0;
	char hostname[4];
	gethostname(hostname, sizeof(hostname));
	devID = hostname[0] + hostname[1] + hostname[2] + hostname[3];
	char charID[100];
	sprintf(charID, "%u", devID);
	string strID = charID;
	cout << "Device ID is: " << strID << endl << endl;
	//create JSON string
	cout << "JSON object is: \n";
	string JSON = "";
	JSON.reserve(256);
		JSON += "{\n";
		JSON += "\"DeviceID\": \"" + strID; JSON += "\",\n";
		JSON += "\"DeviceType\": \"Photoresistor\",\n";
		JSON += "\"Data\": \"" + strData; JSON += "\",\n";
		JSON += "\"Timestamp\": \"" + timestamp; JSON += "\",\n";
		JSON += "\"currentIP\": \"" + IP + "\",\n";
		JSON += "}";
	cout << JSON << endl;

	cout << "\nTransmitting data...";
	//create UDP socket
	int socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socketfd < 0)
	{
		perror("Error creating socket, exiting");
		return -2;
	}
	int brdcstena = 1;
	setsockopt(socketfd, IPPROTO_UDP, SO_BROADCAST, &brdcstena, sizeof(brdcstena)); //enable broadcasting on socket 

	/*struct sockaddr_in bcast;
	inet_pton(AF_INET, "192.168.1.255", &(bcast.sin_addr)); */

	struct sockaddr_in myaddr;
	memset((char*)&myaddr, 0, sizeof(myaddr)); //zero out struct
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY); //almost certainly wrong for broadcast
	myaddr.sin_port = htons(MY_PORT_NUMBER);
	
	//bind socket
	if(bind(socketfd, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0)
	{
		perror("Error binding socket, exiting");
		return -3;
	}
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	struct sockaddr_in bcast;
	inet_pton(AF_INET, argv[1], &(bcast.sin_addr));
	serv_addr.sin_addr.s_addr = bcast.sin_addr.s_addr; //@TODO fix
	serv_addr.sin_port = htons(REM_PORT_NUMBER);
	
	//send data over broadcast socket
	int ret = sendto(socketfd, JSON.c_str(), JSON.length(), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 
	if(ret < 0)
		perror("Error sending data");
	cout << "Done.\n";
	
	close(socketfd);
	
	return 0;
}

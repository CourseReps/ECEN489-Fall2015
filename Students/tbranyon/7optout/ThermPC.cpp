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
#include <math.h>
#include <stdlib.h>

//circuit parameters
#define R1 32500
#define VCC 3.3f

//Steinhart-Hart Parameteters
//Thermistor model: Vishay 01C1002F
#define A 1.2E-3
#define B 1.7909E-4
#define C 2.9189E-7

#define MY_PORT_NUMBER 2015
#define REM_PORT_NUMBER 1111

using namespace std;

int get_ADC()
{
	int data = 0;
	cout << "\nRetrieving serial reading...\n";
	system("echo 1 > /dev/ttyACM0");
	usleep(50000);
	system("cat /dev/ttyACM0 > data.txt");
	FILE* fp = fopen("data.txt", "r");
	fscanf(fp, "%d", &data);
	fclose(fp);
	//system("echo 0 > /dev/ttyACM0");
	system("rm data.txt");
	return data;
}

string get_IP_str()
{
	FILE* fp;
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
	return IP;
}

string get_timestamp_str()
{
	time_t timer;
	time(&timer);
	char strdate[100];
	sprintf(strdate, "%s", ctime(&timer));
	char timetmp[100];
	sprintf(timetmp, "%li", timer);
	string timestamp = timetmp;
	return timestamp;
}

string get_devID()
{
	//get device ID
	//for now, we'll use the ASCII values of the first 4 characters of the hostname to form an ID
	unsigned int devID = 0;
	char hostname[4];
	gethostname(hostname, sizeof(hostname));
	devID = hostname[0] + hostname[1] + hostname[2] + hostname[3];
	char charID[100];
	sprintf(charID, "%u", devID);
	string retval = charID;
	return retval;
}

double calc_temp(int adc)
{
	double adc_volts = 3.3 * adc / 1024;
	double rt = adc_volts * R1 / (VCC + adc_volts);
	return 1/(A+B*log(rt)+C*(log(rt)*log(rt)*log(rt))); //Steinhart-Hart Equation
}

int main(int argc, char* argv[])
{		
	//setup UDP socket
	int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketfd < 0)
	{
		perror("Error creating socket, exiting");
		return -2;
	}
	int brdcstena = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_BROADCAST, &brdcstena, sizeof(brdcstena)); //enable broadcasting on socket 

	struct sockaddr_in myaddr;
	memset((char*)&myaddr, 0, sizeof(myaddr)); //zero out struct
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(MY_PORT_NUMBER);
	
	//bind socket
	if(bind(socketfd, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0)
	{
		perror("Error binding socket, exiting");
		return -3;
	}
	
	
	while(1)
	{
		struct sockaddr remote;
		socklen_t slen = sizeof(remote);
		char sockdata[9]; //buffer for received data
		bzero(sockdata, sizeof(sockdata));
	
		while(memcmp(sockdata,"get_data",8) != 0) //block until we receive the command
		{
			cout << "Entering read state\n";
			int n = recvfrom(socketfd, &sockdata, 8, 0, &remote, &slen); //read from socket
			if(n < 0)
				cerr << "Error reading from socket\n";
		}
		cout << "Command received!\n";
	
	
		//get values and wrap into JSON object
		char chData[10] = {0};
		double temp = calc_temp((double)get_ADC()) - 273.15;
		sprintf(chData, "%.1f", temp);
		string strData = chData;
		string timestamp = get_timestamp_str();
		//string IP = get_IP_str();
		string strID = "1000";
		//create JSON string
		cout << "JSON object is: \n";
		string JSON = "";
		JSON.reserve(256);
			JSON += "{\n";
			JSON += "\"DeviceID\": \"" + strID; JSON += "\",\n";
			JSON += "\"DeviceType\": \"Thermistor\",\n";
			JSON += "\"Data\": \"" + strData; JSON += "\",\n";
			JSON += "\"Timestamp\": \"" + timestamp; JSON += "\",\n";
			//JSON += "\"currentIP\": \"" + IP + "\",\n";
			JSON += "}";
		cout << JSON << endl;
	
		struct sockaddr_in serv_addr;
		struct sockaddr_in *rem_addr = (struct sockaddr_in*)&remote;
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = rem_addr->sin_addr.s_addr;
		serv_addr.sin_port = htons(REM_PORT_NUMBER);
	
		char ipstr[INET_ADDRSTRLEN] = {0};
		cout << "Request came from: " << inet_ntop(AF_INET, &(serv_addr.sin_addr), ipstr, INET_ADDRSTRLEN) << endl;
	
		//send data over broadcast socket
		int ret = sendto(socketfd, JSON.c_str(), JSON.length(), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 
		if(ret < 0)
		{
			perror("Error sending data");
			cerr << ret;
		}
		cout << "Done.\n";
	}
	
	close(socketfd);
	
	return 0;
}

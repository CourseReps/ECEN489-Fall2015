#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <time.h>
#include "sqlite3.h"

using namespace std;

string get_IP_str()
{
	FILE* fp;
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

int main()
{
	sqlite3 *db;
	sqlite3_open("../teensydata.db", &db);
	sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS TBL1(deviceID INT PRIMARY KEY UNIQUE, IR_range0 DOUBLE, IR_range1 DOUBLE, PumpRate0 DOUBLE, PumpRate1 DOUBLE, FlowRate DOUBLE, Timestamp INT, IP_addr TEXT);", 0, 0, 0);
	FILE* bt;
	while(1)	
	{
		bt = fopen("/dev/rfcomm0","w");
		fprintf(bt, "get_data");
		fclose(bt);
		bt = fopen("/dev/rfcomm0","r");
		//FILE* fp = fopen("data.txt", "r");
		/*
			Assume data structure from Teensy will be:
			IR0,IR1,PumpRate0,PumpRate1,FlowRate
			00.00,00.00,00.00,00.00
		*/
		double ir0 = 0;			
		fscanf(bt,"%lf", &ir0);		//IR0
		fseek(bt,1,SEEK_CUR); 		//skip comma
		double ir1 = 0;				
		fscanf(bt,"%lf", &ir1);		//IR1
		fseek(bt,1,SEEK_CUR); 		//skip comma
		double pmprt0 = 0;
		fscanf(bt,"%lf",&pmprt0);	//Pump0 rate
		fseek(bt,1,SEEK_CUR); 		//skip comma
		double pmprt1 = 0;
		fscanf(bt,"%lf",&pmprt1);	//Pump1 rate
		fseek(bt,1,SEEK_CUR); 		//skip comma
		double flowrt = 0;
		fscanf(bt,"%lf",&flowrt);	//Flow rate
		fseek(bt,1,SEEK_CUR); 		//skip comma
		
		int deviceID = 1;			//Set devID to 1
		time_t timestamp;
		time(&timestamp);			//retrieve current UNIX timestamp
		string IP = get_IP_str();	//get current IP address as string
		char tablecmd[256] = {0};
		sprintf(tablecmd, "INSERT OR REPLACE INTO TBL1 VALUES(%d, %lf, %lf, %lf, %d, %ld, \"%s\");", deviceID, ir, pmprt, flowrt, solenoid, timestamp, IP.c_str());
		sqlite3_exec(db, tablecmd, 0, 0, 0);
		cout << tablecmd << endl << endl;
		fclose(bt);
		system("killall cat");
		usleep(100000); //wait 100ms before retrying
	}
	return 0;
}

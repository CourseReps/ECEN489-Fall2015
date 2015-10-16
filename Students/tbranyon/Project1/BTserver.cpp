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
	sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS TBL1(deviceID INT PRIMARY KEY UNIQUE, IR_range DOUBLE, PumpRate DOUBLE, FlowRate DOUBLE, SolenoidState BOOLEAN, Timestamp INT, IP_addr TEXT);", 0, 0, 0);
	FILE* bt;
	while(1)	
	{
		bt = fopen("/dev/rfcomm0","w");
		fprintf(bt, "%d", 1);
		fclose(bt);
		usleep(1000); //wait 1ms for data
		fopen("/dev/rfcomm0","r");
		//FILE* fp = fopen("data.txt", "r");
		/*
			Assume data structure from Teensy will be:
			IR,PumpRate,FlowRate,Solenoid
			00.00,00.00,00.00,0
		*/
		double ir = 0;
		fscanf(bt,"%lf", &ir);
		double pmprt = 0;
		fseek(bt,1,SEEK_CUR); //skip comma
		fscanf(bt,"%lf",&pmprt);
		double flowrt = 0;
		fseek(bt,1,SEEK_CUR); //skip comma
		fscanf(bt,"%lf",&flowrt);
		int solenoid = 0;
		fseek(bt,1,SEEK_CUR); //skip comma
		fscanf(bt,"%d",&solenoid);
		int deviceID = 3;
		time_t timestamp;
		time(&timestamp);
		string IP = get_IP_str();
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

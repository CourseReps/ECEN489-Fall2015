#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>

using namespace std;

string btDev;
sqlite3 *db;

void setUpDb()
{
  int rc;
  rc = sqlite3_open("Project1.db", &db);
  char *zErrMsg = 0;
  string sql;
  sql = "CREATE TABLE IF NOT EXISTS SYSTEM_DATA("  \
         "DeviceID           TEXT    NOT NULL," \
         "IRRange            TEXT     NOT NULL," \
         "PumpRate           TEXT    NOT NULL," \
         "FlowRate           TEXT    NOT NULL," \
         "SolenoidState           TEXT    NOT NULL," \
         "Timestamp           TEXT    NOT NULL," \
         "currentIP            TEXT     NOT NULL);";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
}

void writeValuesToDb(string values)
{
	int rc;


	vector<string> vals;
  vals = split(vals,values, boost::is_any_of(","));
	
	char *zErrMsg = 0;
	string sql;
	sql = "INSERT INTO SYSTEM_DATA (DeviceID,IRRange,PumpRate,FlowRate,SolenoidState,Timestamp,currentIP) "  \
         "VALUES ('" + vals[0] + "','"+ vals[1] + "','"+ vals[2] + "','"+ vals[3] + "','"+ vals[4] + "','" +  to_string(time(0)) +"','127.0.0.1');";
    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Records created successfully\n");
    }
}
void sendBtCommand()
{
  cout<<"Sending command...\n";
  string command = "echo \"get_data\" > " + btDev;
  system(command.c_str());
}

string readFromBt()
{
  char buff[128];
  string command = "head -n 2 " + btDev;
  FILE *fp = popen(command.c_str(),"r");

  while ( fgets( buff, 128, fp ) != NULL ) {

    //printf("%s", buff );

  }
  string ret(buff);
  ret = ret.substr(0, ret.length());

  cout<<ret<<"\n";

  return ret;

}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		return 0;
	}

	btDev = argv[1];

	setUpDb();

	while(true)
	{
		//sendBtCommand();
		string values = readFromBt();
		writeValuesToDb(values);

	}
}
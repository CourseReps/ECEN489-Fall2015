#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sqlite3.h>
#include <vector>
#include <unistd.h>

using namespace std;

string getValue(string devi){
  string ret="NULL";
//  string comm = "sudo head -1 /dev/rfcomm0 > read.txt";
//  system(comm.c_str());
  ifstream myinput("read.txt");
  if(myinput.is_open()){
    string line;
    if (getline(myinput, line))
      ret = line;
    return ret;
  }
  return ret;
}

void writeValue(int x){
  //string comm = "echo " + to_string(x) + " | sudo tee /dev/rfcomm0";
  //system(comm.c_str());
}

std::string getCurrentIP(){
  return "192.168.1.108";
  system("ifconfig en0 | grep \"inet \" > currentip.txt");
  ifstream myinput("currentip.txt");
  std::string line;
  if(myinput.is_open()){
      getline(myinput, line);
      line = line.substr(line.find_first_of("inet")+5);
      line = line.substr(0, line.find_first_of(" "));
      myinput.close();
      if(line.length()<8)
        return "0.0.0.0";
      return line;
  }
  else
    return "0.0.0.0";
}

void addDeviceInfo(std::vector<std::string> inf, sqlite3 *db){
  std::string comm = "INSERT INTO SensorData (ID, IRSensor, Pumprate, Flowrate, Solenoid, Timestamp, CurrentIP)";
  comm+=" SELECT '"+inf[0]+"', '"+inf[1]+"', '"+inf[2]+"', '"+inf[3]+"', '"+inf[4]+"', '"+inf[5]+"', '"+inf[6]+"'";
  comm+=" WHERE NOT EXISTS (SELECT ID FROM SensorData WHERE ID = '"+inf[0]+"' AND IRSensor='"+inf[1]+"' AND Pumprate='"+inf[2]+"' AND FlowRate='"+inf[3]+"' AND Solenoid='"+inf[4]+"' AND Timestamp='"+inf[5]+"' AND CurrentIP='"+inf[6]+"')";
  char *err_msg = 0;

  int rc = sqlite3_open("sensordata.db", &db);

  if (rc != SQLITE_OK) {
    std::cout<<"Cannot open database: "<<sqlite3_errmsg(db);
    sqlite3_close(db);
  }

  rc = sqlite3_exec(db, comm.c_str(), 0, 0, &err_msg);
  if (rc != SQLITE_OK) {

    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
  }
  sqlite3_close(db);
}

std::vector<double> getDataFromString(std::string s){
  int found=0;
  std::vector<double> ret;
  do{
    found=s.find(",");
    if(found!=std::string::npos){
      string temp = s.substr(0, found);
//      cout<<"temp: "<<temp<<"\n";
      s=s.substr(found+1);
//      cout<<"new s: "<<s<<"\n";
      ret.push_back(stod(temp));
    }
    else
      ret.push_back(stod(s));
  }
  while(found!=std::string::npos);

  return ret;
}

std::string getJSON(std::vector<std::string> stuff){
  std::string ret;

  ret+="{ ";
  ret+="\"ID\": \""+stuff[0]+"\", ";
  ret+="\"IRRange\": \""+stuff[1]+"\", ";
  ret+="\"PumpRate\": \""+stuff[2]+"\", ";
  ret+="\"FlowRate\": \""+stuff[3]+"\", ";
  ret+="\"SolenoidState\": \""+stuff[4]+"\", ";
  ret+="\"Timestamp\": \""+stuff[5]+"\", ";
  ret+="\"CurrentIP\": \""+stuff[6]+"\" ";
  ret+="}";

  return ret;
}


int main(){
  sqlite3 *db;
  char *err_msg = 0;
  sqlite3_stmt *stmt;
  int rc = sqlite3_open("sensordata.db", &db);

  if (rc != SQLITE_OK) {
    std::cout<<"Cannot open database: "<<sqlite3_errmsg(db);
    sqlite3_close(db);
    return 1;
  }

  std::string com = "CREATE TABLE IF NOT EXISTS SensorData(ID TEXT, IRSensor TEXT, Pumprate TEXT, FlowRate TEXT, Solenoid TEXT, Timestamp TEXT, CurrentIP TEXT)";
  rc = sqlite3_exec(db, com.c_str(), 0, 0, &err_msg);
  if (rc != SQLITE_OK) {

    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
  }
  sqlite3_close(db);

  while(true){
    getValue("rfcomm0");
    string vals = getValue("rfcomm0");
    vector<double> currentData = getDataFromString(vals);
    std::vector<string> v;
    v.push_back("Team4");
    v.push_back(to_string(currentData[0]));
    v.push_back(to_string(currentData[1]));
    v.push_back(to_string(currentData[2]));
    v.push_back(to_string(currentData[3]));
    v.push_back(to_string(time(0)));
    v.push_back(getCurrentIP());
    addDeviceInfo(v, db);
    usleep(1000000);
  }


}

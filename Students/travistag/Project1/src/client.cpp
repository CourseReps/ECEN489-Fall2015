//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <sqlite3.h>


using boost::asio::ip::udp;
using namespace std;

string getValue(string devi){
  string ret="NULL";
  string comm = "head -1 /dev/"+devi+" > read.txt";
  system(comm.c_str());
  ifstream myinput("read.txt");
  if(myinput.is_open()){
    string line;
    if (getline(myinput, line))
      ret = line;
    return ret;
  }
  return ret;
}

std::string getCurrentIP(){
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

  ret+="{\n";
  ret+="\"DeviceID\": \""+stuff[0]+"\",\n";
  ret+="\"DeviceType\": \""+stuff[1]+"\",\n";
  ret+="\"Data\": \""+stuff[2]+"\",\n";
  ret+="\"Timestamp\": \""+stuff[3]+"\",\n";
  ret+="\"currentIP\": \""+stuff[4]+"\"\n";
  ret+="}";

  return ret;
}


void addDeviceInfo(std::vector<std::string> inf, sqlite3 *db){
  std::string comm = "INSERT INTO SensorData (Flowrate, Pumprate, IRsensor, Solenoid, Time)";
  comm+=" SELECT "+inf[0]+", '"+inf[1]+"', "+inf[2]+", '"+inf[3]+"', '"+inf[4]+"'";
  comm+=" WHERE NOT EXISTS (SELECT Pumprate FROM SensorData WHERE Flowrate='"+inf[0]+"' AND Pumprate='"+inf[1]+"' AND IRSensor='"+inf[2]+"' AND Solenoid='"+inf[3]+"' AND Time='"+inf[4]+"')";
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

int main(int argc, char* argv[])
{

  if(argc<2){
    std::cout<<"\nPlease enter a port # to broadcast on, adn device to read!\n\n";
    return 1;
  }
  sqlite3 *db;
  char *err_msg = 0;
  sqlite3_stmt *stmt;
  int rc = sqlite3_open("sensordata.db", &db);

  if (rc != SQLITE_OK) {
    std::cout<<"Cannot open database: "<<sqlite3_errmsg(db);
    sqlite3_close(db);
    return 1;
  }

  std::string com = "CREATE TABLE IF NOT EXISTS SensorData(Flowrate TEXT, Pumprate TEXT, IRSensor TEXT, Solenoid TEXT, Time TEXT)";
  rc = sqlite3_exec(db, com.c_str(), 0, 0, &err_msg);
  if (rc != SQLITE_OK) {

    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
  }
  sqlite3_close(db);

  while(true){

  string rawdat = getValue(argv[1]);

  vector<double> currentData = getDataFromString(rawdat);
  std::vector<string> v;
  for(int i=0; i<currentData.size(); i++){
    cout<<currentData[i]<<"\n";
    v.push_back(to_string(currentData[i]));
  }
  v.push_back(to_string(time(0)));
  addDeviceInfo(v, db);
  cout<<"\n\n";
  sleep(3);
}
  /*string myIP=getCurrentIP();

  try
  {

    boost::asio::io_service io_service;

    udp::resolver resolver(io_service);
    
    boost::system::error_code error;
    boost::asio::ip::udp::socket socket(io_service);
    while(true){

    socket.open(boost::asio::ip::udp::v4(), error);
    if (!error)
    {
        socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::socket_base::broadcast(true));
        int val;
        getValue(argv[2]);   //clear out the reading buffer
        do{
          val = getValue(argv[2]);
        }while(val==-999);
        std::vector<std::string> mess;
        string devID = "123";
        mess.push_back(devID);
        mess.push_back("Photosensor");
        mess.push_back(to_string(val));
        mess.push_back(to_string(time(0)));
        mess.push_back(myIP);
        string message = getJSON(mess);

        boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), atoi(argv[1])); 
        socket.send_to(boost::asio::buffer(message), senderEndpoint);          
        
        socket.close(error);
    }
	sleep(1);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }*/

  return 0;
}

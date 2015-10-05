//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <sqlite3.h>

using boost::asio::ip::udp;
using namespace std;





void addDeviceInfo(std::vector<std::string> inf, sqlite3 *db){
  std::string comm = "INSERT INTO PhotoData (ID, Type, Data, Time, IPAddress)";
  comm+=" SELECT "+inf[0]+", '"+inf[1]+"', "+inf[2]+", '"+inf[3]+"', '"+inf[4]+"'";
  comm+=" WHERE NOT EXISTS (SELECT ID FROM PhotoData WHERE ID="+inf[0]+" AND Type='"+inf[1]+"' AND Data="+inf[2]+" AND Time='"+inf[3]+"' AND IPAddress='"+inf[4]+"')";
  char *err_msg = 0;

  int rc = sqlite3_open("photodata.db", &db);

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

std::string getJSON(std::vector<std::string> stuff){
  std::string ret="";

  ret+="{\n";
  ret+="\"DeviceID\": \""+stuff[0]+"\",\n";
  ret+="\"DeviceType\": \""+stuff[1]+"\",\n";
  ret+="\"Data\": \""+stuff[2]+"\",\n";
  ret+="\"Timestamp\": \""+stuff[3]+"\"\n";
  ret+="}";

  return ret;
}

string getValue(string devi){
  return "12.5";
  string ret = "-999";
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

int main(int argc, char* argv[])
{
  if(argc<2){
    cout<<"Please enter device name!";
  }

  try
  {
    boost::asio::io_service io_service;

    udp::socket socket(io_service, udp::endpoint(udp::v4(), 1111));

    for (;;)
    {
      vector<char> recv_buf(1000);
      udp::endpoint remote_endpoint;
      boost::system::error_code error;
      string comm="";
      int x = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);
      if(x>0){
        for(int i=0; i<x; i++){
          cout<<recv_buf[i];
          comm+=recv_buf[i];
        }
        cout<<"\n";
      }

      if (error && error != boost::asio::error::message_size)
        throw boost::system::system_error(error);

      if(comm.substr(0,8).compare("get_data")==0){
        getValue(argv[1]);    //clear out buffer
        std::vector<string> dat;
        dat.push_back("123");
        dat.push_back("Thermistor");
        dat.push_back(getValue(argv[1]));
        dat.push_back(to_string(time(0)));
        string message=getJSON(dat);
        cout<<message<<"\n";

        boost::system::error_code ignored_error;
        socket.send_to(boost::asio::buffer(message),
            remote_endpoint, 0, ignored_error);
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
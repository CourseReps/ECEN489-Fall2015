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
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using boost::asio::ip::tcp;
using namespace std;

string deviceName;

string get_json(vector< pair<string,string> > json_values)
{
  string ret = "{\n";
  for(int i=0; i<json_values.size()-1; i++)
  {
    ret += '"' + json_values[i].first + "\": " + '"' + json_values[i].second + "\",\n";
  }
  ret += '"' + json_values[json_values.size()-1].first + "\": " + '"' + json_values[json_values.size()-1].second + "\"\n}";
  return ret;
}

string get_ip()
{
  char buff[128];

  FILE *fp = popen("ipconfig getifaddr en0","r");

  while ( fgets( buff, 128, fp ) != NULL ) {

    //printf("%s", buff );

  }
  string ret(buff);
  ret = ret.substr(0, ret.length()-1);

  return ret;

}

string readFromBt()
{
  
  char buff[128];
  string command = "head -n 1 " + deviceName;
  FILE *fp = popen(command.c_str(),"r");

  while ( fgets( buff, 128, fp ) != NULL ) {

    //printf("%s", buff );

  }
  string ret(buff);
  ret = ret.substr(0, ret.length());

  cout<<ret<<"\n";

  return ret;

}

string readFromBt2()
{
  
  char buff[128];
  string command = "head -n 2 " + deviceName;
  FILE *fp = popen(command.c_str(),"r");

  while ( fgets( buff, 128, fp ) != NULL ) {

    //printf("%s", buff );

  }
  string ret(buff);
  //ret = ret.substr(ret.find("\n")+1);

  cout<<ret<<"\n";

  return ret;

}

int main(int argc, char* argv[])
{

  if(argc != 3)
  {
    return 0;
  }
  unsigned short port = (unsigned short) strtoul(argv[1], NULL, 0);
  deviceName = argv[2];

  try
  {

    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      string command(buf.data(), len);

      cout<<"Got command: "<<command<<"\n";


      if (error && error != boost::asio::error::message_size)
        throw boost::system::system_error(error);

      vector<pair<string, string> > json_values;
      vector<string> values;
      string vals = readFromBt2();
      //vals.erase(std::remove(vals.begin(), vals.end(), '\n'), vals.end());

      values = split(values,vals, boost::is_any_of(","));
      if(values.size() == 5)
      {
        json_values.push_back(pair<string, string>("ID", values[0]));
        json_values.push_back(pair<string, string>("IRRange", values[1]));
        json_values.push_back(pair<string, string>("PumpRate", values[2]));
        json_values.push_back(pair<string, string>("FlowRate", values[3]));
        json_values.push_back(pair<string, string>("SolenoidState", values[4]));
        json_values.push_back(pair<string, string>("Timestamp", to_string(time(0))));
        json_values.push_back(pair<string, string>("CurrentIP", get_ip()));
      }

      boost::system::error_code ignored_error;

      cout<<"Sending..."<<"\n";
      cout<<get_json(json_values)<<"\n";

      if(command.find("get_data") != string::npos)
      {
        boost::asio::write(socket, boost::asio::buffer(get_json(json_values)), boost::asio::transfer_all(), ignored_error);
      }
      else
      {
        cout<<"Sending command to Arduino!\n";
        int voltage = stoi(command);
        if(voltage >-1 && voltage <256)
        {
          string cmd = "echo " + command + " | sudo tee " + deviceName;
          system(cmd.c_str());
        }

      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
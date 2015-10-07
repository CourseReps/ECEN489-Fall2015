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
#include <fstream>


using boost::asio::ip::udp;
using namespace std;

std::string findString(std::string s){
  int openq = s.find_first_of("\"");
  std::string s2 = s.substr(openq+1);
  s2 = s2.substr(0, s2.find_first_of("\""));
  return s2;
}

std::string expect(std::string whole, std::string search){
  if(whole.find(search)!=std::string::npos){
    whole = whole.substr(whole.find_first_of(search)+search.length());
    //cout<<"Got "<<search<<"\n";
  }
  else{
    std::cout<<"\n\nImproperly formatted JSON! Did not find "<<search<<"\n\n";
    return "";
  }
  return whole;
}

void writeValue(int on){
  if(on==1){
    system("echo \"b\" > /dev/cu.usbmodem1062571");
  }
  else
    system("echo \"a\" > /dev/cu.usbmodem1062571");
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

std::vector<std::string> getDeviceInfo(std::string json){
  std::vector<std::string> ret;
  json = expect(json, "{");
  json = expect(json, "\"DeviceID\"");
  json = expect(json, ":");
  std::string fn = findString(json);
  ret.push_back(fn);
  json = expect(json, ",");
  json = expect(json, "\"DeviceType\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, ",");
  json = expect(json, "\"Data\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, ",");
  json = expect(json, "\"Timestamp\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, "}");
  return ret;
}




int main(int argc, char* argv[])
{
  try
  {

    std::vector<string> IPs;
    std::vector<double> temps;
    IPs.push_back("10.202.104.83");
   //IPs.push_back("10.202.115.200");

    for(int i=0; i<IPs.size(); i++){

      cout<<"Connecting to server at "<<IPs[i]<<"\n";
      boost::asio::io_service io_service;

      udp::resolver resolver(io_service);
      udp::resolver::query query(udp::v4(), IPs[i], "1111");
      udp::endpoint receiver_endpoint = *resolver.resolve(query);

      udp::socket socket(io_service);
      socket.open(udp::v4());

      string send_buf = "get_data";
      socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
      cout<<"Command sent\n";
      //boost::array<char, 128> recv_buf;
      std::vector<char> recv_buf(1000);
      udp::endpoint sender_endpoint;
      string jsonrec="";
      int len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
      for(int j=0; j<len; j++){
        jsonrec+=recv_buf[j];
      }
      //std::cout<<jsonrec<<"\n";
      std::vector<string> jsoninfo = getDeviceInfo(jsonrec);
      //cout<<jsoninfo[2]<<"\n";
      temps.push_back(stod(jsoninfo[2]));

    }
    for(int i=0; i<IPs.size(); i++){
      cout<<IPs[i]<<":\t"<<to_string(temps[i]);
      if(temps[i]>100)
        cout<<"\tThis guys HOT!!!";
      cout<<"\n";
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
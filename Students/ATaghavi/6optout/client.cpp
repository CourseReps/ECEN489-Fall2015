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
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <ctime>

using boost::asio::ip::tcp;
using namespace std;

int get_photo_val()
{
  char buff[128];

  FILE *fp = popen("cat < /dev/cu.usbmodem1062851","r");
while(fgets( buff, 128, fp ) != NULL ) {
    break;
  }
  string ret(buff);
  ret = ret.substr(0, ret.length()-1);

  return atoi(ret.c_str());

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

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: client port" << std::endl;
      return 1;
    }

    

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(argv[1], argv[2]);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);

    //for (;;)
    {
      int val = get_photo_val();
      vector< pair<string,string> > json_values;
      json_values.push_back(pair<string,string>("photoValue",to_string(val)));
      time_t result = time(nullptr);
      string timestamp = asctime(localtime(&result));
      json_values.push_back(pair<string,string>("timestamp",timestamp));
      json_values.push_back(pair<string,string>("ip",get_ip()));

      //get ip address
      boost::array<char, 128> buf;
      boost::system::error_code error;
      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(get_json(json_values) ),
          boost::asio::transfer_all(), ignored_error);

      string resp;
      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      string response(buf.data(), len);

      if(response[0] == 'O')
      {
        system("echo 'X' > /dev/cu.usbmodem1062851");
      }
      else
      {
        system("echo 'P' > /dev/cu.usbmodem1062851");
      }

      
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }


  return 0;
}
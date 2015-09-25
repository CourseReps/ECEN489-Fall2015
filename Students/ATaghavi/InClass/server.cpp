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

using boost::asio::ip::tcp;
using namespace std;

void take_screenshot(string photo_name)
{
  cout<<"taking photo...\n";
  photo_name = "screencapture " + photo_name;
  system(photo_name.c_str());
}

void take_webcam_shot()
{
  system("imagesnap -w 1.0");
}

string get_val_from_json(string val,string data)
{
  int pos = 0;
  pos = data.find(val,pos);
  pos = data.find(":",pos);
  pos = data.find("\"",pos);
  string valuee = data.substr( pos+1,data.find("\"",pos+1) - pos - 1  );
  return valuee;
}

int main()
{
  
  try
  {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 14));

    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      string command(buf.data(), len);

      cout<<"Message: "<<command<<"\n";
      boost::system::error_code ignored_error;

      int value = atoi(command.c_str());
      if(value > 500)
      {
        boost::asio::write(socket, boost::asio::buffer("ON"), boost::asio::transfer_all(), ignored_error);
      }
      else
      {
        boost::asio::write(socket, boost::asio::buffer("NOT"), boost::asio::transfer_all(), ignored_error);
      }

      
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }


  return 0;
}
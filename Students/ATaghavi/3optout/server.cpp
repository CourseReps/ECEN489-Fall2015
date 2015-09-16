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
  photo_name = "screencapture " + photo_name;
  system(photo_name.c_str());
}

void take_webcam_shot()
{
  system("imagesnap -w 1.0");
}

int main()
{
  try
  {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      string command(buf.data(), len);

      if(command == "get_photo")
      {
        //take the photo
        take_screenshot("screenshot.jpg");

        //read file into buffer
        ifstream fileIn("screenshot.jpg");
        stringstream s;
        s << fileIn.rdbuf();
        string fileString = s.str();

        //write buffer to socket
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(fileString), boost::asio::transfer_all(), ignored_error);

      }

      else if(command == "get_webcam")
      {
        take_webcam_shot();

        //read file into buffer
        ifstream fileIn("snapshot.jpg");
        stringstream s;
        s << fileIn.rdbuf();
        string fileString = s.str();

        //write buffer to socket
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(fileString), boost::asio::transfer_all(), ignored_error);
      }

      cout<<"From client: "<<command<<"\n";

      
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
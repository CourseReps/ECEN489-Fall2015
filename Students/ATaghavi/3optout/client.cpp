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

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      cout<<"Enter command: ";
      string command = "";
      cin>>command;

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(command),
          boost::asio::transfer_all(), ignored_error);

      boost::asio::streambuf response;
      time_t now = time(0);
      string s = ctime(&now);
      string fileName = "photos/" + s + ".jpg";

      ofstream fileOut(fileName.c_str());
      while (boost::asio::read(socket, response,
          boost::asio::transfer_at_least(1), error))
        {
            fileOut<<&response;
        }
        fileOut.close();

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
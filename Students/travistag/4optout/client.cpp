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


using boost::asio::ip::udp;

std::string getJSON(std::vector<std::string> stuff){
  std::string ret;

  ret+="{\n";
  ret+="\"firstName\": \""+stuff[0]+"\",\n";
  ret+="\"lastName\": \""+stuff[1]+"\",\n";
  ret+="\"gitUserName\": \""+stuff[2]+"\",\n";
  ret+="\"currentIP\": \""+stuff[3]+"\"\n";
  ret+="}";

  return ret;
}

int main(int argc, char* argv[])
{

  std::vector<std::string> inf;
  inf.push_back("Travis");
  inf.push_back("Taghavire");
  inf.push_back("travistag");
  inf.push_back("127.0.0.1");

  std::string infojson = getJSON(inf);

  try
  {

    boost::asio::io_service io_service;

    udp::resolver resolver(io_service);
    
    boost::system::error_code error;
    boost::asio::ip::udp::socket socket(io_service);

    socket.open(boost::asio::ip::udp::v4(), error);
    if (!error)
    {
        socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::socket_base::broadcast(true));

        boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), 2015); 

     //   while(true){
          socket.send_to(boost::asio::buffer(infojson), senderEndpoint);
     //   }           

        
        socket.close(error);
    }

  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
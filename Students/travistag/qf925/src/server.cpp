#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;


int main(){
  boost::asio::io_service io_service;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));
  while(true)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      std::cout<<"Received connection, waiting for command\n";

      std::vector<char> buf (1000);
      boost::system::error_code error;
      int len = socket.read_some(boost::asio::buffer(buf), error);

      std::string comm(buf.begin(), buf.end());
      std::cout<<"Received command: "<<comm<<"\n";
      int x = atoi(comm.c_str());
      string message;
      if(x > 350)
        message="Y";
      else
        message="N";

        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
      }

}

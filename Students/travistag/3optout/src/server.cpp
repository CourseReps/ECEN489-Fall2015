#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string getScreenshotFile(){
    system("screencapture -t jpg screen.jpg\n");
    std::string ret;
    std::ifstream myinput("screen.jpg");
    if(myinput.is_open()){
      std::string line;
      bool firstone=true;
      while(getline(myinput, line)){
        if(!firstone)
          ret+="\n";
        ret+=line;
        firstone=false;
      }
    myinput.close();
  }
  return ret;
}

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
      if(comm.substr(0,9).compare("get_photo")==0){
        std::string message = getScreenshotFile();
        std::cout<<"Captured screenshot\n";

        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
      }
    }

}

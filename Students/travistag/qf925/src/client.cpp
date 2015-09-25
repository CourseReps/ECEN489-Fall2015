#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fstream>
#include <stdlib.h>

using boost::asio::ip::tcp;
using namespace std;

int getValue(){
  int ret = -999;
  system("head -1 /dev/cu.usbmodem1062571 > read.txt");
  ifstream myinput("read.txt");
  if(myinput.is_open()){
    string line;
    if (getline(myinput, line))
      ret = atoi(line.c_str());
    return ret;
  }
  return ret;
}

void writeValue(int on){
  if(on==1){
    system("echo \"b\" > /dev/cu.usbmodem1062571");
  }
  else
    system("echo \"a\" > /dev/cu.usbmodem1062571");
}

int main(int argc, char* argv[]){

	if(argc<3){
		std::cout<<"Must enter a server host and port!\n\n";
		return 1;
	}

  cout<<getValue()<<"\n";

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

  	while(true)
    {
      std::vector<char> buf (1000);
      boost::system::error_code error;

        std::string message;
        int val;
        do{
          val = getValue();
        }while(val==-999);
        message = to_string(val);
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);


        int len;
        do{
              len = socket.read_some(boost::asio::buffer(buf), error);
        }while(len>0);
        if(buf[0]=='Y'){
          writeValue(0);
          cout<<"Value was over threshold\n";
        }
        if(buf[0]=='N'){
          writeValue(1);
          cout<<"value was under threshold\n";
        }

        if (error == boost::asio::error::eof)
          break; // Connection closed cleanly by peer.
        else if (error)
          throw boost::system::system_error(error); // Some other error.
      
      
    }

  	return 0;
}

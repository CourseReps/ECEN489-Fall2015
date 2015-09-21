#include <iostream>
#include <boost/array.hpp>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <ctime>

using boost::asio::ip::udp;
using namespace std;

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

int main(int argc, char* argv[])
{

  //while(true)
  {	
	  try
	  {
	    if (argc != 2)
	    {
	      std::cerr << "Usage: client port" << std::endl;
	      return 1;
	    }
	    get_ip();
	    unsigned short port = (unsigned short) strtoul(argv[1], NULL, 0);

	    boost::asio::io_service io_service;
	    udp::resolver resolver(io_service);  
	    udp::socket socket(io_service);

	    string command = "{\n\"firstName\": \"Austin\",\n\"lastName\": \"Taghavi\",\n\"gitUserName\": \"ATaghavi\",\n\"currentIP\": \""+get_ip()+"\"\n}";
	    boost::system::error_code error;

		socket.open(udp::v4(), error);
	    if (!error)
	    {
	        socket.set_option(udp::socket::reuse_address(true));
	        socket.set_option(boost::asio::socket_base::broadcast(true));

	        udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), port);            

	        socket.send_to(boost::asio::buffer(command), senderEndpoint);
	        socket.close(error);
	    }

	  }

	  catch(exception e)
	  {
	  	return -1;
	  }
  }
  return 0;
}
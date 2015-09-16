#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fstream>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]){

	if(argc<2){
		std::cout<<"Must enter a server host and port!\n\n";
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

  	while(true)
    {
     // boost::array<char, 1000> buf;
      std::vector<char> buf (1000);
      boost::system::error_code error;

      std::string message;
      std::cin>>message;
      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);


      int len;
     std::ofstream myoutput("out.jpg");
      do{
            len = socket.read_some(boost::asio::buffer(buf), error);
            for(int i=0; i<len; i++)
                myoutput<<buf[i];
      }while(len>0);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      
      
      myoutput.close();
    }

  	return 0;
}

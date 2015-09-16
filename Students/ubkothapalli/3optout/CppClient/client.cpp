#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {

        //const char *port = "1036"; //the port we connect
        const unsigned int buff_size = 65536; //16384;//65536; //the size of the read buffer

        if (argc != 4)
        {
            std::cerr << "Usage: file_client <host> <port> <filename>" << std::endl;
            return 1;
        }
        boost::asio::io_service io_service; //asio main object
        tcp::resolver resolver(io_service); //a resolver for name to @
        tcp::resolver::query query(argv[1], argv[2]); //ask the dns for this resolver
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query); //iterator if multiple answers for a given name
        tcp::resolver::iterator end;

        tcp::socket socket(io_service); //attach a socket to the main asio object
        socket.connect(*endpoint_iterator); //connect to the first returned object

        std::fstream f(argv[3],std::fstream::out); //open in write mode the output file
        unsigned int count = 0; //a counter
        std::string mess;
        std::cin>>mess;
        boost::system::error_code ignored_error;
        boost::asio::write(socket,boost::asio::buffer(mess),boost::asio::transfer_all(),ignored_error);

        while(1) { //loop until (see break after)
            boost::array<char, buff_size> buf; //create read buffer
            boost::system::error_code error; //in case of error



            size_t len = socket.read_some(boost::asio::buffer(buf), error); //read data
            std::cout << "Read " << len <<  std::endl;
            count += len;
            std::cout << "Read a total of " << count << " bytes " << std::endl;
            if (error == boost::asio::error::eof ) { //if end of file reached
                f.write(buf.data(),len); //finish write data
                f.close();   break; // Connection closed cleanly by peer.
            }
            else if (error) {
                throw boost::system::system_error(error); // Some other error.
            }
            else {
                f.write(buf.data(),len); //write some data
            }
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    system("display *.jpg");

    return 0;
}

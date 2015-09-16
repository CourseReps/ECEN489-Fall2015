#include <iostream>
#include <string>
#include <fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
    try
    {

        //const char *port = "1036"; //the port we connect


        if (argc != 3)
        {
            std::cerr << "Usage: ./cppclient <host> <port> " << std::endl;
            return 1;
        }
        boost::asio::io_service io_service; //asio main object
        tcp::resolver resolver(io_service); //a resolver for name to @
        tcp::resolver::query query(argv[1], argv[2]); //ask the dns for this resolver
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query); //iterator if multiple answers for a given name


        tcp::socket socket(io_service); //attach a socket to the main asio object
        boost::asio::connect(socket, endpoint_iterator);
        std::cout << "connected to server" <<  std::endl;

       // std::fstream op_file(argv[2],std::fstream::out); //open in write mode the output file
        std::fstream op_file("Client.jpg",std::fstream::out); //open in write mode the output file
        std::system("chmod 755 Client.jpg \n");

        std::string client_request = "get_photo";
        boost::system::error_code error; //in case of error
        boost::asio::write(socket, boost::asio::buffer(client_request), boost::asio::transfer_all(),error);
        std::cout << "sent the screenshot request:" <<client_request<<  std::endl;

        // read the image from the server
        unsigned int count = 0; //a counter
        const unsigned int buff_size = 60000; //the size of the read buffer

        while(1) { //loop until (see break after)

            boost::array<char,  buff_size> buf; //create read buffer

            size_t len = socket.read_some(boost::asio::buffer(buf), error); //read data
            count += len;

            if (error == boost::asio::error::eof ) { //if end of file reached
                op_file.write(buf.data(),len); //finish write data
                op_file.close();   break; // Connection closed cleanly by peer.
            }
            else if (error) {
                throw boost::system::system_error(error); // Some other error.
            }
            else {
                op_file.write(buf.data(),len); //write some data
            }
        }



       std::system("display Client.jpg \n");
        std::cout << "Received screenshot of size " << count << " bytes " << std::endl;
    }

    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}


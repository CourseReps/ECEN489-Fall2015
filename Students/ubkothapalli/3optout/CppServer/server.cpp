#include <iostream>
#include <string>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <error.h>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        const unsigned int port = 13; //port the server listen
        const unsigned int buff_size = 65536;//16384; //size of the send buffer

        boost::asio::io_service io_service; //main asio object
        tcp::endpoint endpoint(tcp::v4(), port); //endpoint
        tcp::acceptor acceptor(io_service, endpoint);  //we accept connection there

        //server loop
        while(1) {
            tcp::socket socket(io_service);  //create a socket
            acceptor.accept(socket); //attach to the acceptor
            //we have got a new connection !
            std::cout << " Received connection from  " << socket.remote_endpoint().address() << " : "
            << socket.remote_endpoint().port() << std::endl;




            char* buff = new char[buff_size]; //creating the buffer
            unsigned int count = 0; //counter


            std::vector<char> mess(1000);
            int siz = socket.read_some(boost::asio::buffer(mess));
            std::string command(mess.begin(),mess.end());
            std::cout<<"the recieved command" << command<<"\n";
            system("import -window root screen.jpg\n");
            std::fstream file("screen.jpg");

            if(command.substr(0,9).compare("get_photo")==0) {


                 //we open this file
                std::cout << "Sending" << std::endl;
                while (!file.eof()) { //loop until there is no more data to send
                    memset(buff, 0, buff_size); //cleanup the buffer
                    file.read(buff, buff_size); //read some data
                    boost::system::error_code ignored_error;
                    unsigned int len = file.gcount(); //get the effective number of bytes read
                    boost::asio::write(socket, boost::asio::buffer(buff, len),
                                       boost::asio::transfer_all(), ignored_error); //send
                    count += len; //increment counter
                }
            }
            file.close(); //close file
            delete(buff);  //delete buffer
            std::cout << "Finished" << std::endl;
            std::cout << "Sent "  << count << " bytes" << std::endl;
        }

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

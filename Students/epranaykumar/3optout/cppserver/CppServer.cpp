#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;



int main()
{
    try
    {
        const unsigned int port = 1036; //port the server listen

        boost::asio::io_service io_service; //main asio object

       //get local host addresses
        boost::asio::ip::tcp::resolver resolver(io_service);
        std::string h = boost::asio::ip::host_name();

        std::cout << "Port Number: " << port << '\n';
        std::cout << "Host Name: " << h << '\n';
        std::cout << "Associated IP's: \n";
        std::system("ifconfig | grep \"inet addr\" \n"); // not portable

       /* tcp::resolver::query query(h, "");
        tcp::resolver::iterator iter = resolver.resolve(query);
        tcp::resolver::iterator end;
        while(iter != end)
        {
            std::cout << iter->endpoint().address().to_string() << std::endl;
            ++iter;
        }
        */

        // set reusable address
        boost:: asio::ip::tcp::acceptor::reuse_address(true);
        tcp::endpoint endpoint(tcp::v4(), port); //endpoint
        tcp::acceptor acceptor(io_service, endpoint);  //Accept connection here


        // create a socket and accept connection

        tcp::socket socket(io_service);  //create a socket
        acceptor.accept(socket);         //listen and accept the connection

        // display the local and remote addresses
        boost::asio::ip::tcp::endpoint hostadd = socket.local_endpoint();


        std::cout << "\nConnection established with   " << hostadd << std::endl;
        std::cout << " \t Server Address:Port  " << hostadd << std::endl;
        std::cout << " \t Client Address:Port  " << socket.remote_endpoint().address() << " : "
          << socket.remote_endpoint().port() << std::endl;

        // get the client message
        char bufer[20];
        boost::system::error_code error; //in case of error
        std::cout << "\nWaiting for client request:  "<< std::endl;
        size_t fg = socket.read_some(boost::asio::buffer(bufer), error);
        std::string client_request(bufer);
        std::cout << "\nReceived client request:" << client_request<<std::endl;


        //process the client request
        if(client_request.compare("get_photo")==0) {

            //get screen shot
            std::cout << "\tcapturing screen shot  " << std::endl;
            std::system("import -window root server_image.jpg \n");   // not portable
            std::fstream file("server_image.jpg"); //open the captured image


            //send the captured screen shot
            const unsigned int buff_size = 60000; //size of the send buffer
            char *buff = new char[buff_size]; //creating the buffer
            unsigned int count = 0; //counter
            std::cout << "\tsending the screensot" << std::endl;

            while (!file.eof()) { //loop until there is no more data to send

                memset(buff, 0, buff_size); //cleanup the buffer
                file.read(buff, buff_size); //read some data
                boost::system::error_code ignored_error;
                unsigned long int len = file.gcount(); //get the effective number of bytes read

                boost::asio::write(socket, boost::asio::buffer(buff, len), boost::asio::transfer_all(),
                                   ignored_error); //send */

                count += len; //increment counter
            }

            file.close(); //close file
            delete (buff);  //delete buffer

            std::cout << "\nSent the screenshot of size : " << count << " bytes" << std::endl;
        }
        else{
            std::cout << " incorrect request  " << std::endl;

        }

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


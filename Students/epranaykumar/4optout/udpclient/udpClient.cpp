#include <iostream>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include "json/json.h"


using boost::asio::ip::udp;

int main(int argc, char *argv[]) {


    std::system("ifconfig | grep 'inet addr:'|grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1}'  -> ./buff.txt \n"); // not portable

    std::ifstream input("buff.txt");
    std::string ip;
    if(input.is_open())   getline(input,ip);

    std::cout<<ip<<std::endl;

    Json::Value value(Json::objectValue);
    value["firstName"] = "PRANAY KUMAR";
    value["lastName"] = "EEDARA";
    value["gitUserName"] = "epranaykumar";
    value["currentIP"] = ip;

    Json::FastWriter fastWriter;
    std::string jsonMessage = fastWriter.write(value);
    //Json::StyledWriter styledWriter;
    //std::string jsonMessage = styledWriter.write(value);;
    std::cout<<jsonMessage;

    // setup udp socket
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service);

    boost::system::error_code error;
    socket.open(boost::asio::ip::udp::v4(), error);
    if (!error){
        // TO reuse address and enable broadcast
        socket.set_option(boost:: asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::socket_base::broadcast(true));
    }

    boost::asio::ip::udp::endpoint remote_endpoint;
    remote_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), 1050);



// broadcast the client info
    std::cout << "Send to " << remote_endpoint << std::endl;
    boost::system::error_code ignored_error;
    socket.send_to(boost::asio::buffer(jsonMessage), remote_endpoint, 0, ignored_error);

    socket.close(error);


    return 0;
}
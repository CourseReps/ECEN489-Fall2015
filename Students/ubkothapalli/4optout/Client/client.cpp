#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>


using boost::asio::ip::udp;




int main(int argc, char *argv[]) {


    // My details section



    std::system("ifconfig | grep \"inet addr:\"|grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1}'  -> /home/uday/ClionProjects/4optout/Client/build/myip.txt \n"); // not portable
    std::ifstream ipinput("myip.txt");
    std::string temp, CurIP;
    if(ipinput.is_open()){
        while(getline(ipinput,temp)){
            //std::cout << CurIP ;
            CurIP = temp;
        }


    }
    std::system("rm /home/uday/ClionProjects/4optout/Client/build/myip.txt");




    std::vector<std::string> myInfo;
    myInfo.push_back("Uday Bhaskar");
    myInfo.push_back("Kothapalli");
    myInfo.push_back("ubkothapalli");
    myInfo.push_back(CurIP);

    std::cout <<"my ip rightnow" << CurIP << "\n";

    // JSON object creation

    std::string details_JSON;
    details_JSON += "{\n" ;
    details_JSON+= "\"firstName\": \"" + myInfo[0]+ "\", \n" ;
    details_JSON+= "\"lastName\": \"" + myInfo[1]+ "\", \n" ;
    details_JSON+= "\"gitUserName\": \"" + myInfo[2]+ "\", \n" ;
    details_JSON+= "\"myCurrentIP\": \"" +CurIP +"\", \n" ;
    details_JSON+= "} \n" ;

    std::ofstream Mydetails("MyDetails.json");
    if(Mydetails.is_open()){
        Mydetails <<details_JSON  ;


    }


    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service);

    boost::system::error_code error;
    socket.open(boost::asio::ip::udp::v4(), error);
    if (!error){
        socket.set_option(boost:: asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::socket_base::broadcast(true));
    }

    boost::asio::ip::udp::endpoint remote_end;

    remote_end = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), 1992);

    boost::system::error_code ignored_error;


    socket.send_to(boost::asio::buffer(details_JSON), remote_end, 0, ignored_error);

    socket.close(error);


    return 0;
}
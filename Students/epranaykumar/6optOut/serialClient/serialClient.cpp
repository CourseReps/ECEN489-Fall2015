#include <iostream>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include "json/json.h"


using boost::asio::ip::udp;

int main(int argc, char *argv[]) {


    // setup udp socket
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service);

    boost::system::error_code error;
    socket.open(boost::asio::ip::udp::v4(), error);
    std::cout << error << std::endl;
    if (!error) {
        // TO reuse address and enable broadcast
        socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::socket_base::broadcast(true));
    }

    boost::asio::ip::udp::endpoint remote_endpoint;
    remote_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), 1111);


    // get IP
    std::system("ifconfig | grep 'inet addr:'|grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1}'  -> ./buff.txt \n"); // not portable

    std::ifstream input("buff.txt");
    std::string ip;
    if(input.is_open())   getline(input,ip);

    std::cout<<ip<<std::endl;

    // initialize JSON object
    Json::Value value(Json::objectValue);
    value["DeviceID"] = "Pranay_Kumar";
    value["DeviceType"] = "Photosensor";
    value["Timestamp"] = "epranaykumar";
    value["currentIP"] = ip;


    // setup serial communication for teensy
    system("stty -F /dev/ttyACM0 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
    std::ifstream Arduino_Input("/dev/ttyACM0"); //Opens the tty connection as an ifstream

    int Voltage;	//The Arduino is reading the voltage from A0
    long int Time = time(NULL);


    while(time(NULL)-Time < 2){ }// Wait five seconds for the Arduino to start up

    try {

        int i=0;

        for (int j=0;j < 5;j++){

            // start the serial stream
            std:: ofstream Arduino_Output("/dev/ttyACM0");//Opens the tty connection as an ofstream,
            Arduino_Output<<'1';
            Arduino_Output.close();

            std::cout << "\nSent the request for analog readings on pin 13 "<<std:: endl;

            Time = time(NULL);
            while(time(NULL)-Time < 1){ }	//wait one second to get good numbers into the Arduino stream


            while(!Arduino_Input.eof())	//while the eof flag isn't set
            {
                // read the serial stream input from teensy
                Arduino_Input >> Voltage;	//will set the error flag if not ready, will get a number from the Arduino stream if ready
                if(Voltage==2000){break;}   // sometimes end of file is not reaching it is just waiting for another reading (so to break the loop)
                std:: cout <<"\nAnalog reading: "<< Voltage <<"\n"<<std:: endl;	//Output it to the cout stream


                value["Data"] = std::to_string(Voltage);
                value["Timestamp"] = std::to_string(time(NULL));

                // convert the json message to string
                Json::FastWriter fastWriter;
                std::string jsonMessage = fastWriter.write(value);
                std::cout<<jsonMessage<<std:: endl;

                // broadcast the JSON message
                std::cout << "Send to " << remote_endpoint << std::endl;
                boost::system::error_code ignored_error;
                socket.send_to(boost::asio::buffer(jsonMessage), remote_endpoint, 0, ignored_error);

                std::cout<< i <<std:: endl;

                sleep(1);
                i++;
            }

            std::cout << "Total number of analog readings sent: "<< i <<std:: endl;
            Arduino_Input.clear();	//eof flag won't clear itself
        }

        Arduino_Input.close();	//Close the ifstream
        socket.close(error);
        return 0;
    }

    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

}

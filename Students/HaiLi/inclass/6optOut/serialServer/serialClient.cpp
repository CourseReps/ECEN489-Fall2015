#include <iostream>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include "json/json.h"
#include <math.h>

using boost::asio::ip::udp;

int main(int argc, char *argv[]) {


    // setup udp socket
    double A=0.0014323;
    double B=0.0001537;
    double C=0.00000031033;
//std::cout<<C;
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service);

    boost::system::error_code error;
    socket.open(boost::asio::ip::udp::v4(), error);
    //std::cout << error << std::endl;
    if (!error) {
        // TO reuse address and enable broadcast
        socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::socket_base::broadcast(true));
    }

    boost::asio::ip::udp::endpoint remote_endpoint;
    remote_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), 1111);


    /*// get IP
    std::system("ifconfig | grep 'inet addr:'|grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1}'  -> ./buff.txt \n"); // not portable

    std::ifstream input("buff.txt");
    std::string ip;
    if(input.is_open())   getline(input,ip);

    std::cout<<ip<<std::endl;*/

    // initialize JSON object
int fd;
fd=open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
   if (fd<0){
 	std::cout<<"Cannot open";}
while(1){    
Json::Value value(Json::objectValue);
    value["DeviceID"] = "Hai Li";
    value["DeviceType"] = "Thermistor";
    value["Timestamp"] = "TS";
    //value["currentIP"] = ip;
    char buff[1024];
long int Time = time(NULL);
        //system("head -1 /dev/ttyACM0 > read.txt");
        //FILE *fp = popen("cat < /dev/cu.usbmodem1062851","r");
        //FILE *fp=fopen("/dev/ttyACM0","wr");
        //sleep(1);
        //while(fgets( buff, 1024, fp ) != NULL ) {
        //break;}
        //while(fgets(buff,128,stdin)!=NULL){break;}
        
	//system(" arduino-serial -b 9600 -p /dev/ttyACM > read.txt");

	//fcntl(fd, F_SETFL,0);	
	int n = read(fd, buff, 10);
buff[n]='\0';
printf("%s",buff);
if(n==0) continue;
	//fscanf(fp,"%s",buff);
/*int i=0;        
for(i=0;i<100;i++){
if(buff[i]=='\n'){
buff[i]='\0';
break;}}

printf("%s %d",buff,i);*/
        double val=atof(buff);

printf("%f",val);
        /*while(val>5||val==0){
	memset(&buff,'0',128); 	
	n = read(fd, buff, 100);
	//fscanf(fp,"%s",buff);
        val=atoi(buff);}*/
//printf("%d",n);
        //std::cout<<val;

        val=(32500.0*val)/(3.3-val);
//std::cout<<val;

        val=A+B*log(val)+C*pow(log(val),3.0);
//std::cout<<val;

   	val=1.0/val;
//std::cout<<val;

	val=val-273.0;
 value["Data"] = std::to_string(val);
                value["Timestamp"] = std::to_string(time(NULL));
// convert the json message to string
                Json::FastWriter fastWriter;
                std::string jsonMessage = fastWriter.write(value);
                std::cout<<jsonMessage<<std::endl;

        char buffr[200];
	boost::asio:: ip::udp::socket socket(io_service,
                                         boost::asio:: ip::udp::endpoint( boost::asio::ip::udp::v4(), 1111 ));

    boost::asio:: ip::udp::endpoint sender_endpoint;        
	//boost::system::error_code recv_error;
        std::size_t bytes_received = socket.receive_from(boost::asio::buffer(buffr), sender_endpoint);
        std::string Message(buffr);
        Message = Message.substr(0,bytes_received);
std::cout<<Message;
if (Message.compare("get_data")==0){
                // broadcast the JSON message
                std::cout << "Send to " << remote_endpoint << std::endl;
                boost::system::error_code ignored_error;
                socket.send_to(boost::asio::buffer(jsonMessage), remote_endpoint, 0, ignored_error);}}


close(fd);
return 0;


  

}

#include <ctime>
#include <iostream>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include "json/json.h"
#include <time.h>
#include <unistd.h>
#include <fstream>

using boost::asio::ip::udp;


std::string getTimestamp(){
  time_t ltime;
  time(&ltime);
  // std::cout << ltime <<"\n";
  std::string unixTime = std::to_string(ltime);
  return unixTime;

}


// gathering and arranging data for JSON
std::vector<std::string> getInfo(  std::string dataSensor){
  std::vector<std::string> myInfo;

  //json elements
  //Fetching the current time in Unix timestamp format
  std::string Ctime = getTimestamp();

  // Fetching data from the serial input from the device.
  std::string data = dataSensor;
  //std::cout<< data << "\n";

  myInfo.push_back("007");            // Pushing the device id
  myInfo.push_back("Thermistor");    // Pushing the device type
  myInfo.push_back(data);             // obtain data from the serial port
  myInfo.push_back(Ctime);            // pushing in the Uniix Timestamp
  //myInfo.push_back(CurIP);            // pushing in the current IP of the client


  return myInfo;
}

// creating JSON
std::string createJSON(std::vector<std::string> myInfo){


  Json::Value Sensordetails(Json::objectValue);
  Sensordetails["DeviceID"]=myInfo[0];
  Sensordetails["DeviceType"]=myInfo[1];
  Sensordetails["Data"]=myInfo[2];
  Sensordetails["Timestamp"]=myInfo[3];
  //Sensordetails["currentIP"]=myInfo[4];

  // writing sensor data in JSON format
  Json::FastWriter fastWriter;
  std::string sensorJson = fastWriter.write(Sensordetails);

  std::ofstream Mydetails("sensorDetails.json");
  if(Mydetails.is_open()){
    Mydetails <<sensorJson ;
  }
  Mydetails.clear();
  Mydetails.close();


  return sensorJson;

}

std::string sensorDataAcquisition() {

  std::string tempArduino;

  system("stty -F /dev/ttyACM0 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts");    //Activates the tty connection with the Arduino


  std::ofstream Arduino_output("/dev/ttyACM0");
  Arduino_output << 'Y';
  Arduino_output.clear();
  Arduino_output.close();


  std::ifstream Arduino_Input("/dev/ttyACM0");
//Opens the tty connection as an ifstream

  double Voltage;    //The Arduino is reading the voltage from A0

  int i;


  //Wait five seconds for the Arduinoto start up

  long int Time = time(NULL);
  Time = time(NULL);
    while(time(NULL)-Time < 3){}	//wait one second to getgood numbers into the Arduino stream
   // while (!Arduino_Input.eof())    //while the eof flage isn't set
    //{
      Arduino_Input >> Voltage;                             //will set theerror flag if not ready, will get a number from the Arduino stream if ready
     // std::cout << Voltage << std::endl;    //Output it to the coutstream

  double SERIESRESISTOR = 15000;
  Voltage = 1023 / Voltage - 1;
  Voltage = SERIESRESISTOR / Voltage;
  double TEMPERATURENOMINAL= 30;
  double BCOEFFICIENT=6899;
  double THERMISTORNOMINAL =36000;
  double steinhart = Voltage / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C


  tempArduino = boost::lexical_cast<std::string>(steinhart);         //Output vector

     // std::cout<<tempArduino << " this is a value \n";


    Arduino_Input.clear();    //eof flag won't clear itself


  Arduino_Input.close();    //Close the ifstream

  return tempArduino;


}


int main()
{
  try
  {
    boost::asio::io_service io_service;

    udp::socket socket(io_service, udp::endpoint(udp::v4(), 13));

    for (;;)
    {
      boost::array<char, 1> recv_buf;
      std::string authenticate(recv_buf.begin(),recv_buf.end());
      std::string me = "get_data";

      char aut;
      if (authenticate.compare(me) ==0) {
        aut = '0';
      }
      udp::endpoint remote_endpoint;
      boost::system::error_code error;

      socket.receive_from(boost::asio::buffer(recv_buf),
                          remote_endpoint, 0, error);

      if (error && error != boost::asio::error::message_size)
        throw boost::system::system_error(error);


      // Getting the arduino data
      std::string datathermo=  sensorDataAcquisition();
      std::vector<std::string> myInfo = getInfo( datathermo);
      std::string data_JSON = createJSON(myInfo);


      std::cout << "\n" << data_JSON << "n";
      boost::system::error_code ignored_error;
      socket.send_to(boost::asio::buffer(data_JSON),
                     remote_endpoint, 0, ignored_error);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
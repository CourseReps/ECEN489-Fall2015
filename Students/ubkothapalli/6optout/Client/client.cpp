#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include "json/json.h"
#include <time.h>
#include <unistd.h>


using boost::asio::ip::udp;


std::string getTimestamp(){
    time_t ltime;
    time(&ltime);
    // std::cout << ltime <<"\n";
    std::string unixTime = std::to_string(ltime);
    return unixTime;

}




// gathering and arranging data for JSON
std::vector<std::string> getInfo( std::string CurIP, std::string dataSensor){
    std::vector<std::string> myInfo;

    //json elements
    //Fetching the current time in Unix timestamp format
    std::string Ctime = getTimestamp();

    // Fetching data from the serial input from the device.
    std::string data = dataSensor;
    std::cout<< data << "\n";

    myInfo.push_back("007");            // Pushing the device id
    myInfo.push_back("Photosensor");    // Pushing the device type
    myInfo.push_back(data);             // obtain data from the serial port
    myInfo.push_back(Ctime);            // pushing in the Uniix Timestamp
    myInfo.push_back(CurIP);            // pushing in the current IP of the client


    return myInfo;
}

// creating JSON
std::string createJSON(std::vector<std::string> myInfo){


    Json::Value Sensordetails(Json::objectValue);
    Sensordetails["DeviceID"]=myInfo[0];
    Sensordetails["DeviceType"]=myInfo[1];
    Sensordetails["Data"]=myInfo[2];
    Sensordetails["Timestamp"]=myInfo[3];
    Sensordetails["currentIP"]=myInfo[4];

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

    /*


        std::string details_JSON;
        details_JSON += "{" ;
        details_JSON+= "\"DeviceID\":\"" + myInfo[0]+ "\",";
        details_JSON+= "\"DeviceType\":\"" + myInfo[1]+ "\"," ;
        details_JSON+= "\"Data\":\"" + myInfo[2]+ "\"," ;
        details_JSON+= "\"Timestamp\":\"" +myInfo[3] +"\"," ;
        details_JSON+= "\"currentIP\":\"" +myInfo[4] +"\"" ;
        details_JSON+= "}" ;

        std::ofstream Mydetails("sensorDetails.json");
        if(Mydetails.is_open()){
            Mydetails <<details_JSON ;
        }
    Mydetails.clear();
        Mydetails.close();


        return details_JSON;
        */
}



// braodcasting fucntion
void broadcastData(std::string data_JSON){

    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service);

    boost::system::error_code error;
    socket.open(boost::asio::ip::udp::v4(), error);
    if (!error){
        socket.set_option(boost:: asio::ip::udp::socket::reuse_address(true));
        socket.set_option(boost::asio::socket_base::broadcast(true));
    }

    boost::asio::ip::udp::endpoint remote_end;

    remote_end = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), 1111);

    boost::system::error_code ignored_error;


    socket.send_to(boost::asio::buffer(data_JSON), remote_end, 0, ignored_error);

    socket.close(error);


}


//sensor DATA Acquisition
std::string sensorDataAcquisition(std::string CurIP) {

    std::string dataArduino;
    std::ifstream Arduino_Input("/dev/ttyACM2");
//Opens the tty connection as an ifstream






//Opens the tty connection as an ofstream, not used in this example


    int Voltage;    //The Arduino is reading the voltage from A0

    int i;

       //Wait five seconds for the Arduinoto start up
    for (i = 0; i < 8;) {

        //while(time(NULL)-Time < 3){}	//wait one second to getgood numbers into the Arduino stream
        while (!Arduino_Input.eof())    //while the eof flage isn't set
        {
            Arduino_Input >> Voltage;                             //will set theerror flag if not ready, will get a number from the Arduino stream if ready
            std::cout << Voltage << std::endl;    //Output it to the coutstream
            dataArduino = std::to_string(Voltage);         //Output vector

            std::cout<<dataArduino << " this is a value \n";

            // JSON data grathering and myInfo vector creation
            std::vector<std::string> myInfo = getInfo(CurIP, dataArduino);
            //std::cout<< myInfo[1] << "this is the data \n";

            // JSON object creation

            std::string data_JSON = createJSON(myInfo);
            std::cout << data_JSON << "\n";


            // broadcasting the JSON at port 1111
            broadcastData(data_JSON);
            sleep(4);

            i++;    //Increament i, it is not known how many numbersI'll get at a time
        }
        Arduino_Input.clear();    //eof flag won't clear itself
    }

    Arduino_Input.close();    //Close the ifstream

    return dataArduino;


}



int main(int argc, char *argv[]) {


    // My details section



    std::system("ifconfig | grep \"inet addr:\"|grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1}'  -> ./myip.txt \n"); 
    std::ifstream ipinput("myip.txt");
    std::string temp, CurIP;
    if(ipinput.is_open()){
        while(getline(ipinput,temp)){
            //std::cout << CurIP ;
            CurIP = temp;
        }
    }
    std::system("rm ./myip.txt");



    /*
     * // using JSON parsing library
    Json::Value Sensordetails(Json::objectValue);
    Sensordetails["DeviceID"]="xxx";
    Sensordetails["DeviceType"]="Photosensor";
    Sensordetails["Data"]="xxx";
    Sensordetails["Timestamp"]="datemonthyear";
    Sensordetails["currentIP"]=CurIP;

    // writing sensor data in JSON format
    Json::FastWriter fastWriter;
    std::string sensorJson = fastWriter.write(Sensordetails);


*/



    while(1) {
        char userInput;
        std::cout << "Do you want to read sensor value? Y/N \t";
        std::cin >> userInput;
        int szd;
        //std::vector<std::string> dataArduino;
        if (userInput == 'Y') {

            std:: cout << "getting to the acquisition";
            system("stty -F /dev/ttyACM2 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts");    //Activates the tty connection with the Arduino

            std::ofstream Arduino_Output("/dev/ttyACM2");
            Arduino_Output << userInput;

            //Arduino_Output.clear();
            Arduino_Output.close();
            long int Time = time(NULL);
            Time = time(NULL);

            std:: cout <<"getting to the pernultimate step";
            while (time(NULL) - Time < 2) { } //watiting for 2sec for proper readings

            std:: cout<< "crossed the waiting";
            for (int i = 0; i <= 1; i++){
                // Getting Sensor readings
                std:: cout << "started";
                sensorDataAcquisition(CurIP);
                std:: cout << "looping";
            }
            //szd = dataArduino.size();
        }
        else if (userInput=='N'){
            std::cout << "Answer is No";
            break;
        }

        /*
        int count;
        for (count = 0; count < szd; count++) {


            // JSON data grathering and myInfo vector creation
            std::vector<std::string> myInfo = getInfo(CurIP, dataArduino);


            // JSON object creation

            std::string data_JSON = createJSON(myInfo);
            std::cout << data_JSON << "\n";


            // broadcasting the JSON at port 1111
            broadcastData(data_JSON);

            sleep(5);
        }
         */
        std:: cout <<"while looping";
    }
    return 0;
}

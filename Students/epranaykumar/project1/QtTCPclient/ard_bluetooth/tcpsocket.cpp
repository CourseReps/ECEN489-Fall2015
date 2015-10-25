#include "tcpsocket.h"


#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include "json/json.h"


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileInfo>
#include <QtCore>


#include  <unistd.h>
#include <time.h>


int prev_last_row=0;

TCPSocket::TCPSocket()
{
 //  ConnecttoTCPserver();

}


/*TCPSocket::~TCPSocket(){

    stopTCPSocket();

}
*/


void TCPSocket::ConnecttoTCPserver(){

    for(;;){

         qDebug( "listening for pull Requests" );

         const unsigned int port = 1036; //port the server listen


         boost::asio::io_service io_service; //asio main object
         boost::asio::ip::tcp::resolver resolver(io_service);

         std::cout << "Associated IP's: \n";
         std::system("ifconfig | grep \"inet addr\" \n"); // not portable

         // set reusable address
         boost:: asio::ip::tcp::acceptor::reuse_address(true);
         boost:: asio::ip::tcp::endpoint endpoint( boost:: asio::ip::tcp::v4(), port); //endpoint
         boost:: asio::ip::tcp::acceptor acceptor(io_service, endpoint);  //Accept connection here

         // create a socket and accept connection

         boost:: asio::ip::tcp::socket socket(io_service);  //create a socket
         acceptor.accept(socket);         //listen and accept the connection

         // display the local and remote addresses
         boost::asio::ip::tcp::endpoint hostadd = socket.local_endpoint();

         std::cout << "\nConnection established with   " << hostadd << std::endl;
         std::cout << " \t Server Address:Port  " << hostadd << std::endl;
         std::cout << " \t Client Address:Port  " << socket.remote_endpoint().address() << " : "
           << socket.remote_endpoint().port() << std::endl;

         boost::system::error_code error;
         const unsigned int buff_size = 200; //size of the send buffer
         char buff[buff_size]; //creating the buffer

         std::size_t bytes_received  = socket.read_some(boost::asio::buffer(buff), error); //read data

         std::string jsonMes(buff);
         jsonMes = jsonMes.substr(0, bytes_received);
         std::cout << "recieved message:  \n" << jsonMes << std::endl;

         if (jsonMes.compare("get_data") == 0) {
             sendDatabase(socket);
              //emit gotpollingReq();

         }
    }



    emit  finished();

}

 void TCPSocket::stopTCPSocket(){


 }

 void TCPSocket::sendDatabase(boost:: asio::ip::tcp::socket& socket){


      // assign the appropriate driver to the database
       QSqlDatabase db1 = QSqlDatabase::addDatabase("QSQLITE");

       // open the specified database
        db1.setDatabaseName(  "/home/pranaykumar/data.db");

        if(!db1.open()){
             qDebug("Failed to open the database");
        }
        else{
             qDebug("Connected to the database");
             QString stmt =QString("SELECT  Flowrate, Voltage, IR_sensor, RelayState, DeviceID,Timestamp FROM DATA ORDER BY Timestamp;");
             qDebug() <<"Query:"<< stmt;
             QSqlQuery query;
             query.prepare(stmt);

             if( !query.exec() )
                 qDebug() << query.lastError();
             else
                 qDebug( "Qeury: Executed successfully" );
             //count the current number of rows in database ordered by timestamp
             int  cur_last_row;
             if(query.last())   cur_last_row = query.at();

             qDebug()<<"current last row: "<<cur_last_row;

             if(cur_last_row >= prev_last_row) {

                 for(int i=prev_last_row; i<cur_last_row+1;i++){

                      query.seek(i);

                     // initialize JSON object                     
                     Json::Value value(Json::objectValue);
                     value["ID"] = "team2";
                     value["IRRange"] = query.value(2).toDouble();
                     value["PumpRate"] = query.value(1).toDouble();
                     value["FlowrRate"] = query.value(0).toDouble();//flowrate,voltage,irvalue,timestamp,relaystate
                     value["SolenoidState"] = query.value(3).toDouble();
                     value["Timestamp"] =  query.value(5).toDouble();
                     value["CurrentIP"] = "Pranay_Kumar";


                     //convert the json message to string
                     Json::FastWriter fastWriter;
                     std::string jsonMessage = fastWriter.write(value);
                     std::cout<<jsonMessage<<std:: endl;

                     boost::system::error_code ignored_error;
                    // socket.(boost::asio::buffer(jsonMessage),remote_endpoint, 0, ignored_error);
                     boost::asio::write(socket, boost::asio::buffer(jsonMessage), boost::asio::transfer_all(),
                                                        ignored_error); //send */



                 }
             }
}



 }

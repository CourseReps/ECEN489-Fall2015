#include <iostream>
#include <fstream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "json/json.h"

#include "sqlite3/sqlite3.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {


     if (argc != 3)
        {
            std::cerr << "Usage: ./serialServer <host> <port> " << std::endl;
            return 1;
        }
        boost::asio::io_service io_service; //asio main object
        tcp::resolver resolver(io_service); //a resolver for name to @
        tcp::resolver::query query(argv[1], argv[2]); //ask the dns for this resolver
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query); //iterator if multiple answers for a given name


        tcp::socket socket(io_service); //attach a socket to the main asio object
        boost::asio::connect(socket, endpoint_iterator);
        std::cout << "connected to server" <<  std::endl;


    // setup the sqlite database
    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *stmt;

    int rc = sqlite3_open("data.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // create table
    char sql[200] = "CREATE TABLE TBL( DeviceID TEXT, Distance TEXT, PumpRate TEXT, FlowRate TEXT, SolenoidState TEXT, Timestamp TEXT, IP_addr TEXT);";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    sqlite3_exec(db,"PRAGMA journal_mode = WAL",0,0,&err_msg);
    sqlite3_close(db);

     std::string client_request = "get_data";
        boost::system::error_code error; //in case of error
        boost::asio::write(socket, boost::asio::buffer(client_request), boost::asio::transfer_all(),error);

       

    for(;;) {

        // Receive data.
        const unsigned int buff_size = 200; 
        char buff[buff_size];
	boost::system::error_code error;
        std::size_t bytes_received = socket.read_some(boost::asio::buffer(buff), error);      
	std::string jsonMessage(buff);
        jsonMessage = jsonMessage.substr(0,bytes_received);
	if(bytes_received!=0){
        std::cout << "recieved message:  \n" << jsonMessage << std::endl;


        // Parse data.
        Json::Value client_msg;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(jsonMessage, client_msg);

            // store to Sqlite database
            if (parsingSuccessful) {

                std::string deviceid = client_msg["DeviceID"].asString();
                std::string distance = client_msg["Distance"].asString();
                std::string PumpRate = client_msg["PumpRate"].asString();
                std::string FlowRate = client_msg["FlowRate"].asString();
                std::string SolenoidState = client_msg["SolenoidState"].asString();
		std::string Timestamp = client_msg["Timestamp"].asString();
		std::string IP_addr = client_msg["IP_addr"].asString();
                char s1[30],s2[30],s3[30],s4[30],s5[30],s6[30],s7[30];
 		strcpy(s1,deviceid.c_str());
		strcpy(s2,distance.c_str());
		strcpy(s3,PumpRate.c_str());
		strcpy(s4,FlowRate.c_str());
		strcpy(s5,SolenoidState.c_str());
		strcpy(s6,Timestamp.c_str());
		strcpy(s7,IP_addr.c_str());
                rc = sqlite3_open("data.db", &db);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
                    sqlite3_free(err_msg);
                    sqlite3_close(db);
                    return 1;
                }
                //insert data
                char sql4[200];
           	sprintf(sql4, "INSERT INTO TBL VALUES('%s','%s','%s','%s','%s','%s','%s');", s1, s2, s3, s4, s5, s6, s7);
      
                rc = sqlite3_exec(db, sql4, 0, 0, &err_msg);

                if (rc != SQLITE_OK) {
                    fprintf(stderr, "\n SQL error: %s   %s\n", err_msg, sql4);
                    sqlite3_free(err_msg);
                }

                sqlite3_close(db);}

            }// if parsing successful
    }//for loop

    return 0;
}

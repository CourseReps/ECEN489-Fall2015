#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include "json/json.h"

#include "sqlite3/sqlite3.h"
using boost::asio::ip::udp;

int main(int argc, char* argv[])
{


  // for stroing all the temop data
  //std::vector<std::vector<std::string>> TempData;
  //int k = 0;
  // setup the sqlite database
  sqlite3 *db;
  char *err_msg = 0;
  sqlite3_stmt *stmt;

  int rc = sqlite3_open("sensorDatabase.db", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return 1;
  }

  // create table
  char sql[200] = "CREATE TABLE sensorData( DeviceID TEXT,DeviceType TEXT, Data TEXT,Timestamp TEXT );";
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  sqlite3_close(db);
  // printf("%s\n %s \n", sql,err_msg);

 // while(true)
 {
    try {
      if (argc != 2) {
        std::cerr << "Usage: client <host>" << std::endl;
        return 1;
      }

      boost::asio::io_service io_service;

      udp::resolver resolver(io_service);
      udp::resolver::query query(udp::v4(), argv[1], "daytime");
      udp::endpoint receiver_endpoint = *resolver.resolve(query);

      udp::socket socket(io_service);
      socket.open(udp::v4());

      boost::array<char, 20> send_buf = {"get_data"};
      std::string authenticate(send_buf.begin(), send_buf.end());
      socket.send_to(boost::asio::buffer(authenticate), receiver_endpoint);

      boost::array<char, 150> recv_buf;
      udp::endpoint sender_endpoint;
      size_t len = socket.receive_from(
              boost::asio::buffer(recv_buf), sender_endpoint);

      std::cout.write(recv_buf.data(), len);


      std::string jsonMessage(recv_buf.begin(), recv_buf.end());;

      // storing
      // Parse data.
      Json::Value client_msg;
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(jsonMessage, client_msg);

      // store to Sqlite database
      if (parsingSuccessful) {

        std::string deviceid = client_msg["DeviceID"].asString();
        std::string devicetype = client_msg["DeviceType"].asString();
        std::string data = client_msg["Data"].asString();
        std::string timestamp = client_msg["Timestamp"].asString();
        //std::string currentIP = client_msg["currentIP"].asString();


        double temperature = std::stod(data);

        double threshold = 70.00;

        // calibrating the temperature recieved

        if (temperature >= threshold)
        {
          std::cout << "the sensor is at the hot end";
        }
        else if(temperature < threshold)
        {
          std:: cout << "the sensor is at the cold end" "\n";
        }

        // Decision making

        //###################################################################################


        rc = sqlite3_open("sensorDatabase.db", &db);
        if (rc != SQLITE_OK) {
          fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
          sqlite3_free(err_msg);
          sqlite3_close(db);
          return 1;
        }
        //insert data
        char sql4[200];

        std::string statement =
                " INSERT INTO sensorData VALUES(' " + deviceid + " ',' " + devicetype + " ',' " + data + " ',' " +
                timestamp + " ') ";
        strcpy(sql4, statement.c_str());


        //  std::cout << sql4 << std::endl;
        rc = sqlite3_exec(db, sql4, 0, 0, &err_msg);

        if (rc != SQLITE_OK) {
          fprintf(stderr, "\n SQL error: %s   %s\n", err_msg, sql4);
          sqlite3_free(err_msg);
        }

        sqlite3_close(db);


      }
    }
    catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

  }
  return 0;
}
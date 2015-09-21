#include <iostream>
#include <fstream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "json/json.h"

#include "sqlite3/sqlite3.h"

using boost::asio::ip::udp;

int main(int argc, char *argv[]) {


    boost::asio::io_service io_service;

    // Client binds to any address on port **** (the same port on which
    // broadcast data is sent from server).
    boost::asio:: ip::udp::socket socket(io_service,
                                         boost::asio:: ip::udp::endpoint( boost::asio::ip::udp::v4(), 1050 ));

    boost::asio:: ip::udp::endpoint sender_endpoint;

    // Receive data.
    const unsigned int buff_size = 200; //size of the send buffer
    char buff[buff_size]; //creating the buffer
    std::size_t bytes_received = socket.receive_from(boost::asio::buffer(buff), sender_endpoint);
    std::string jsonMessage(buff);
    std::cout << "recieved message:  \n" <<jsonMessage << std::endl;

    socket.close();

    // Parse data.
    Json::Value client_msg;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonMessage, client_msg);

    // store to Sqlite database
    if (parsingSuccessful)
    {
        std::string firstName = client_msg["firstName"].asString();
        std::string lastName = client_msg["lastName"].asString();
        std::string gitUserName = client_msg["gitUserName"].asString();
        std::string currentIP = client_msg["currentIP"].asString();

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
        char sql[200] ="CREATE TABLE Client_IP( firstName TEXT, lastName TEXT, gitUserName TEXT,currentIP TEXT);";
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
       // printf("%s\n %s \n", sql,err_msg);

        //insert data
        char sql4[200];

        std::string statement = " INSERT INTO Client_IP VALUES(' "+firstName+" ',' " + lastName + " ',' "+ gitUserName + " ',' " + currentIP + " ') ";
        strcpy(sql4, statement.c_str());
       // sprintf(sql4,"INSERT INTO DATA VALUES('%s','%s','%s','%s')",firstName,lastName,gitUserName,CurrentIP);

      //  std::cout << sql4 << std::endl;
        rc=sqlite3_exec(db,sql4,0,0,&err_msg);

        if( rc!=SQLITE_OK ){
            fprintf(stderr, "\n SQL error: %s   %s\n", err_msg,sql4);
            sqlite3_free(err_msg);
        }

        sqlite3_close(db);
    }

    return 0;
}
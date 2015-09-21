#include <iostream>
#include <fstream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "sqlite3/sqlite3.h"

using boost::asio::ip::udp;

int main(int argc, char *argv[]) {


    boost::asio::io_service io_service;

    boost::asio:: ip::udp::socket socket(io_service,
                                         boost::asio:: ip::udp::endpoint( boost::asio::ip::udp::v4(), 1992 ));

    boost::asio:: ip::udp::endpoint sender_endpoint;

    // Receive data.

    char receivedMsg[150];
    std::size_t bytes_transferred =
            socket.receive_from(boost::asio::buffer(receivedMsg,150), sender_endpoint);

    std::cout << "\n" << receivedMsg << "\n";


    socket.close();

    // Printing the broadcast into a json file
    std::string detailsmsg(receivedMsg);

    std::ofstream Mydetails("myDetails.json");

    Mydetails << detailsmsg;

    std::vector<std::string> myInfo;

    Mydetails.close();
    //Breaking down the JSON file into useful entities.
    std::ifstream Json("myDetails.json");

    std::string line;
     if (Json.is_open()) {
        while (getline(Json, line)) {

                // extracting the first name
                 if (line.find("\"firstName\"") != std::string::npos){
                    std::string temp = line.substr(line.find(": \"")+3,line.find(",")-line.find(": \"")-4);

                myInfo.push_back(temp);
                 }

            // extractinf the last name
                if (line.find("\"lastName\"") != std::string::npos){
                    std::string temp = line.substr(line.find(": \"")+3,line.find(",")-line.find(": \"")-4);

                myInfo.push_back(temp);
                }
            //extracting git user name
                if (line.find("\"gitUserName\"") != std::string::npos){
                    std::string temp = line.substr(line.find(": \"")+3,line.find(",")-line.find(": \"")-4);

                myInfo.push_back(temp);
                }
            // extracting my current ip address
                if (line.find("\"myCurrentIP\"") != std::string::npos){
                    std::string temp = line.substr(line.find(": \"")+3,line.find(",")-line.find(": \"")-4);

                myInfo.push_back(temp);
                }
        }

    }

    //Converting the braodcast into a sqlite database


    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("myDetails.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char *sql = "DROP TABLE IF EXISTS Info;"
            "CREATE TABLE Info(firstName text, lastName text, gitUserName text, currentIP text);";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    std::string sql1 = "INSERT INTO Info VALUES(";
    sql1 += "\"";
    sql1 += myInfo[0];
    sql1 += "\",";
    sql1 += "\"";
    sql1 += myInfo[1];
    sql1 += "\",";
    sql1 += "\"";
    sql1 += myInfo[2];
    sql1 += "\",";
    sql1 += "\"";
    sql1 += myInfo[3];
    sql1 += "\");";
    const char *sqll = sql1.c_str();
    std::cout << sqll;

    rc = sqlite3_exec(db, sqll, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {


        fprintf(stderr, "SQL error occured: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }

    sqlite3_close(db);

    return 0;
}
//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

using boost::asio::ip::udp;
using namespace std;

sqlite3 *db;

string get_val_from_json(string val,string data)
{
  int pos = 0;
  pos = data.find(val,pos);
  pos = data.find(":",pos);
  pos = data.find("\"",pos);
  string valuee = data.substr( pos+1,data.find("\"",pos+1) - pos - 1  );
  return valuee;
}

vector< pair<string, string> > parse_json(string data)
{
  vector< pair<string, string> > vals;
  vals.push_back( pair<string,string>("DeviceID", get_val_from_json("DeviceID", data)) );
  vals.push_back( pair<string,string>("DeviceType", get_val_from_json("DeviceType", data)) );
  vals.push_back( pair<string,string>("Data", get_val_from_json("Data", data)) );
  vals.push_back( pair<string,string>("Timestamp", get_val_from_json("Timestamp", data)) );
  vals.push_back( pair<string,string>("currentIP", get_val_from_json("currentIP", data)) );
  return vals;
}

void set_up_db()
{
  int rc;
  char *zErrMsg = 0;
  string sql;
  sql = "CREATE TABLE IF NOT EXISTS IP_DATA("  \
         "DeviceID           TEXT    NOT NULL," \
         "DeviceType            TEXT     NOT NULL," \
         "Data           TEXT    NOT NULL," \
         "Timestamp           TEXT    NOT NULL," \
         "currentIP            TEXT     NOT NULL);";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
}

void add_to_database(vector<pair<string,string> > vec)
{
  int rc;
  char *zErrMsg = 0;
  string sql;
  sql = "INSERT INTO IP_DATA (DeviceID,DeviceType,Data,Timestamp,currentIP) "  \
        "SELECT '" + vec[0].second + "' , '"+ vec[1].second + "' , '"+ vec[2].second + "' , '"+ vec[3].second + "' , '"+ vec[4].second + "' " \
        "WHERE NOT EXISTS (SELECT DeviceID FROM IP_DATA WHERE DeviceID='"+vec[0].second+"' AND Timestamp='" + vec[3].second + "' AND currentIP='" + vec[4].second + "');";

  rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
     if( rc != SQLITE_OK ){
     fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
     }else{
        fprintf(stdout, "Table created successfully\n");
     }
}

int main(int argc, char* argv[])
{
  try
  {
    int rc;
    rc = sqlite3_open("6optout.db", &db);
    set_up_db();

    if (argc != 2)
    {
        std::cerr << "Usage: client port" << std::endl;
        return 1;
    }
    unsigned short port = (unsigned short) strtoul(argv[1], NULL, 0);
    boost::asio::io_service io_service;
    udp::socket sock(io_service, udp::endpoint(udp::v4(), port));

    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
    }else{
      fprintf(stderr, "Opened database successfully\n");
    }
    int max_length = 128;
    for (;;)
    {
      char data[max_length];
        udp::endpoint sender_endpoint;
        size_t length = sock.receive_from(
            boost::asio::buffer(data, max_length), sender_endpoint);
        
        string json(data);

        std::string::iterator end_pos = std::remove(json.begin(), json.end(), ' ');
        json.erase(end_pos, json.end());

        vector< pair< string, string> > values = parse_json(data);
        
        add_to_database(values);
      /*
      cout<<"Message: "<<command<<"\n";
      boost::system::error_code ignored_error;

      int value = atoi(command.c_str());
      if(value > 500)
      {
        boost::asio::write(socket, boost::asio::buffer("ON"), boost::asio::transfer_all(), ignored_error);
      }
      else
      {
        boost::asio::write(socket, boost::asio::buffer("NOT"), boost::asio::transfer_all(), ignored_error);
      }
      */ 
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }


  return 0;
}
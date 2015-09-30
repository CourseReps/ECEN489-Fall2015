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

using boost::asio::ip::tcp;
using namespace std;

sqlite3 *db;

void take_screenshot(string photo_name)
{
  cout<<"taking photo...\n";
  photo_name = "screencapture " + photo_name;
  system(photo_name.c_str());
}

void take_webcam_shot()
{
  system("imagesnap -w 1.0");
}

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
  vals.push_back( pair<string,string>("photoVal", get_val_from_json("photoVal", data)) );
  vals.push_back( pair<string,string>("ip", get_val_from_json("ip", data)) );
  vals.push_back( pair<string,string>("timestamp", get_val_from_json("timestamp", data)) );
  return vals;
}

void set_up_db()
{
  int rc;
  char *zErrMsg = 0;
  string sql;
  sql = "CREATE TABLE IF NOT EXISTS IP_DATA("  \
         "firstName           TEXT    NOT NULL," \
         "lastName            TEXT     NOT NULL," \
         "gitUserName           TEXT    NOT NULL," \
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
  sql = "INSERT INTO IP_DATA (firstName,lastName,gitUserName,currentIP) "  \
        "SELECT '" + vec[0].second + "' , '"+ vec[1].second + "' , '"+ vec[2].second + "' , '"+ vec[3].second + "' " \
        "WHERE NOT EXISTS (SELECT firstName FROM IP_DATA WHERE firstName='"+vec[0].second+"' AND lastName='" + vec[1].second + "' AND currentIP='" + vec[3].second + "');";

  rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
     if( rc != SQLITE_OK ){
     fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
     }else{
        fprintf(stdout, "Table created successfully\n");
     }
}

int main()
{
  
  try
  {
    boost::asio::io_service io_service;

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 14));

    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      string command(buf.data(), len);

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

      
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }


  return 0;
}
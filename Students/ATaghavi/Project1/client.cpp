//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <ctime>
#include <sqlite3.h>

using boost::asio::ip::tcp;
using namespace std;

sqlite3 *db;

string get_val_from_json(string val,string data)
{
  int pos = 0;
  pos = data.find(val,pos);
  pos = data.find(":",pos);
  pos = data.find("\"",pos);
  string valuee = data.substr( pos+1,data.find("\"",pos+1) - pos - 1  );

  cout<<"Finding "<<val<<" = "<<valuee<<"\n";

  return valuee;
}

void writeValuesToDb(vector<string> vals)
{
  int rc;
  
  char *zErrMsg = 0;
  string sql;
  sql = "INSERT INTO SYSTEM_DATA (ID,IRRange,PumpRate,FlowRate,SolenoidState,Timestamp,CurrentIP) "  \
         "VALUES ('" + vals[0] + "','"+ vals[1] + "','"+ vals[2] + "','"+ vals[3] + "','"+ vals[4] + "','" + vals[5] +"','"+vals[6]+"');";
    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "Records created successfully\n");
    }
}

vector< string > parse_json(string data)
{
  vector< string > vals;
  vals.push_back( get_val_from_json("ID", data));
  vals.push_back( get_val_from_json("IRRange", data));
  vals.push_back( get_val_from_json("PumpRate", data));
  vals.push_back( get_val_from_json("FlowRate", data));
  vals.push_back( get_val_from_json("SolenoidState", data));
  vals.push_back( get_val_from_json("Timestamp", data));
  vals.push_back( get_val_from_json("CurrentIP", data));
  
  return vals;
}

void setUpDb()
{
  int rc;
  rc = sqlite3_open("Project1.db", &db);
  char *zErrMsg = 0;
  string sql;
  sql = "CREATE TABLE IF NOT EXISTS SYSTEM_DATA("  \
         "ID           TEXT    NOT NULL," \
         "IRRange            TEXT     NOT NULL," \
         "PumpRate           TEXT    NOT NULL," \
         "FlowRate           TEXT    NOT NULL," \
         "SolenoidState           TEXT    NOT NULL," \
         "Timestamp           TEXT    NOT NULL," \
         "CurrentIP            TEXT     NOT NULL);";

   /* Execute SQL statement */
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
    if (argc != 2)
    {
      std::cerr << "Usage: client port" << std::endl;
      return 1;
    }
    vector<string> ip_addrs;
    ip_addrs.push_back("192.168.1.100");
    ip_addrs.push_back("192.168.1.108");
    unsigned short port = (unsigned short) strtoul(argv[1], NULL, 0);

    int THRESHOLD = 100;

    setUpDb();

    while(true)
    {

    for(int i=0; i<ip_addrs.size(); i++)
    {
      boost::asio::io_service io_service;
    string hostName = ip_addrs[i];
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(hostName, to_string(port));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);
    string command = "get_data";
    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(command),
    boost::asio::transfer_all(), ignored_error);

    boost::asio::streambuf response;
    stringstream ss;
    boost::array<char, 256> buf;
      boost::system::error_code error2;
     size_t len = socket.read_some(boost::asio::buffer(buf), error2);

     string json(buf.data(), len);

        cout<<"Received : "<<json<<"\n";

        vector<string> jsonValues = parse_json(json);
        writeValuesToDb(jsonValues);

    }
  }
  
}
catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
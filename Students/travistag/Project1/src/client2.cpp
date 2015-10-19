#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <sqlite3.h>
#include <stdlib.h>

using boost::asio::ip::tcp;
using namespace std;

std::string findString(std::string s){
  int openq = s.find_first_of("\"");
  std::string s2 = s.substr(openq+1);
  s2 = s2.substr(0, s2.find_first_of("\""));
  return s2;
}

std::string expect(std::string whole, std::string search){
 // cout<<"Whole: "<<whole<<"\n";
 // cout<<"Looking for: "<<search<<"\n\n";
  if(whole.find(search)!=std::string::npos){
    whole = whole.substr(whole.find_first_of(search)+search.length());
    //cout<<"Got "<<search<<"\n";
  }
  else{
    std::cout<<"\n\nImproperly formatted JSON! Did not find "<<search<<"\n\n";
    cout<<whole<<"\n";
    return "";
  }
  return whole;
}

std::vector<std::string> getDeviceInfo(std::string json){
  std::vector<std::string> ret;
  json = expect(json, "{");
  json = expect(json, "\"ID\"");
  json = expect(json, ":");
  std::string fn = findString(json);
  ret.push_back(fn);
  json = expect(json, ",");
  json = expect(json, "\"IRRange\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, ",");
  json = expect(json, "\"PumpRate\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, ",");
  json = expect(json, "\"FlowRate\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, ",");
  json = expect(json, "\"SolenoidState\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, ",");
  json = expect(json, "\"Timestamp\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, ",");
  json = expect(json, "\"CurrentIP\"");
  json = expect(json, ":");
  ret.push_back(findString(json));
  json = expect(json, "}");
  return ret;
}


void addDeviceInfo(std::vector<std::string> inf, sqlite3 *db){
  std::string comm = "INSERT INTO SensorData (ID, IRSensor, Pumprate, Flowrate, Solenoid, Timestamp, CurrentIP)";
  comm+=" SELECT '"+inf[0]+"', '"+inf[1]+"', '"+inf[2]+"', '"+inf[3]+"', '"+inf[4]+"', '"+inf[5]+"', '"+inf[6]+"'";
  comm+=" WHERE NOT EXISTS (SELECT ID FROM SensorData WHERE ID = '"+inf[0]+"' AND IRSensor='"+inf[1]+"' AND Pumprate='"+inf[2]+"' AND FlowRate='"+inf[3]+"' AND Solenoid='"+inf[4]+"' AND Timestamp='"+inf[5]+"' AND CurrentIP='"+inf[6]+"')";
  char *err_msg = 0;

  int rc = sqlite3_open("sensordata.db", &db);

  if (rc != SQLITE_OK) {
    std::cout<<"Cannot open database: "<<sqlite3_errmsg(db);
    sqlite3_close(db);
  }

  rc = sqlite3_exec(db, comm.c_str(), 0, 0, &err_msg);
  if (rc != SQLITE_OK) {

    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
  }
  sqlite3_close(db);
}


int main(int argc, char* argv[])
{
  sqlite3 *db;
  char *err_msg = 0;
  sqlite3_stmt *stmt;
  int rc = sqlite3_open("sensordata.db", &db);

  if (rc != SQLITE_OK) {
    std::cout<<"Cannot open database: "<<sqlite3_errmsg(db);
    sqlite3_close(db);
    return 1;
  }

  std::string com = "CREATE TABLE IF NOT EXISTS SensorData(ID TEXT, IRSensor TEXT, Pumprate TEXT, FlowRate TEXT, Solenoid TEXT, Timestamp TEXT, CurrentIP TEXT)";
  rc = sqlite3_exec(db, com.c_str(), 0, 0, &err_msg);
  if (rc != SQLITE_OK) {

    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
  }
  sqlite3_close(db);


  vector<string> IPs;
  IPs.push_back("192.168.1.108");
  //IPs.push_back("192.168.1.100");

  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    while(true){

      for(int i=0; i<IPs.size(); i++){

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(IPs[i], "2222");
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
          cout<<"Could not connect to "<<IPs[i]<<"\n";

          std::vector<char> buf(10000);
          string mess="get_data";
          boost::system::error_code ignored_error;
          boost::asio::write(socket, boost::asio::buffer(mess), boost::asio::transfer_all(), ignored_error);

          size_t len = socket.read_some(boost::asio::buffer(buf), error);


          if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
          else if (error)
            throw boost::system::system_error(error); // Some other error.

          string jsmessage = "";
          for(int i=0; i<len; i++)
            jsmessage+=buf[i];
          cout<<"Message:\n\n"<<jsmessage<<"\n\n";
          vector<string> cd = getDeviceInfo(jsmessage);
          addDeviceInfo(cd, db);
      }
      sleep(1);
    }
    
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
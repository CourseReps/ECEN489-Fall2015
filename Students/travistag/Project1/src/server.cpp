#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

string getValue(string devi){
  string ret="NULL";
//  string comm = "sudo head -1 /dev/rfcomm0 > read.txt";
//  system(comm.c_str());
  ifstream myinput("read.txt");
  if(myinput.is_open()){
    string line;
    if (getline(myinput, line))
      ret = line;
    return ret;
  }
  return ret;
}

void writeValue(int x){
  //string comm = "echo " + to_string(x) + " | sudo tee /dev/rfcomm0";
  //system(comm.c_str());
}

std::string getCurrentIP(){
  return "192.168.1.108";
  system("ifconfig en0 | grep \"inet \" > currentip.txt");
  ifstream myinput("currentip.txt");
  std::string line;
  if(myinput.is_open()){
      getline(myinput, line);
      line = line.substr(line.find_first_of("inet")+5);
      line = line.substr(0, line.find_first_of(" "));
      myinput.close();
      if(line.length()<8)
        return "0.0.0.0";
      return line;
  }
  else
    return "0.0.0.0";
}

std::vector<double> getDataFromString(std::string s){
  int found=0;
  std::vector<double> ret;
  do{
    found=s.find(",");
    if(found!=std::string::npos){
      string temp = s.substr(0, found);
//      cout<<"temp: "<<temp<<"\n";
      s=s.substr(found+1);
//      cout<<"new s: "<<s<<"\n";
      ret.push_back(stod(temp));
    }
    else
      ret.push_back(stod(s));
  }
  while(found!=std::string::npos);

  return ret;
}

std::string getJSON(std::vector<std::string> stuff){
  std::string ret;

  ret+="{ ";
  ret+="\"ID\": \""+stuff[0]+"\", ";
  ret+="\"IRRange\": \""+stuff[1]+"\", ";
  ret+="\"PumpRate\": \""+stuff[2]+"\", ";
  ret+="\"FlowRate\": \""+stuff[3]+"\", ";
  ret+="\"SolenoidState\": \""+stuff[4]+"\", ";
  ret+="\"Timestamp\": \""+stuff[5]+"\", ";
  ret+="\"CurrentIP\": \""+stuff[6]+"\" ";
  ret+="}";

  return ret;
}


int main(){
  boost::asio::io_service io_service;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 2222));
  while(true)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      std::cout<<"Received connection, waiting for command\n";

      std::vector<char> buf (1000);
      boost::system::error_code error;
      int len = socket.read_some(boost::asio::buffer(buf), error);

      std::string comm(buf.begin(), buf.end());
      string message;
      std::cout<<"Received command: "<<comm<<"\n";
      if(comm.substr(0,8).compare("get_data")==0){
        cout<<"Reading from head, packing into JSON and sending!\n\n";
        getValue("rfcomm0");
        string vals = getValue("rfcomm0");
        vector<double> currentData = getDataFromString(vals);
        std::vector<string> v;
        v.push_back("Team4");
        v.push_back(to_string(currentData[0]));
        v.push_back(to_string(currentData[1]));
        v.push_back(to_string(currentData[2]));
        v.push_back(to_string(currentData[3]));
        v.push_back(to_string(time(0)));
        v.push_back(getCurrentIP());
        message = getJSON(v);
      }
      else{
        try{
        int x = atoi(comm.c_str());
        cout<<"Setting pump to be "<<x<<"\n\n";
        message = "OK";
        writeValue(x);
      }
      catch(exception e){
        cout<<"Voltage value must be an int!\n\n";
        message = "FAIL";
      }
      }
      

        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);
      }

}

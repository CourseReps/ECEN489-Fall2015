//This is a Client that takes takes in a JSON Object and broadcasts it to the Network (255.255.255.255) through UDP Datagrams

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fstream>
#include <ctime>
#include "json/json.h"
#define MYPORT 1111 // the port users will be connecting to


using namespace std;

void sigchld_handler(int s)
{
    while(wait(NULL) > 0);
}


int main (int argc, char* argv[])
{
    int sockfd, bytes_sent, bytes_received, broadcast=1, size;
    char *chptr, *hostIPAddr = new char, *cmd = new char[100];
    socklen_t sin_size = (socklen_t)sizeof(sockaddr_in);;
    struct sigaction sa;


    sockaddr_in server_addr; // connector’s address information
    sockaddr_in clientAddr;
    sockaddr_in hostIP;
    socklen_t hostIPlen = sizeof(hostIP);


    //Fetching the IP Address
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    const char* kGoogleDnsIp = "8.8.8.8";
    uint16_t kDnsPort = 53;
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
    serv.sin_port = htons(kDnsPort);

    int err = connect(sock, (const sockaddr*) &serv, sizeof(serv));


    if(getsockname(sock, (sockaddr*) &hostIP, &hostIPlen) == -1){
        perror("getsockname");
    }

    hostIPAddr = inet_ntoa (hostIP.sin_addr);


    // get the host info
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }





    int broadcastsock;
    // this call is what allows broadcast packets to be sent:
    if ((broadcastsock = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &broadcast,sizeof broadcast)) == -1) {
        perror("setsockopt (SO_REUSEADDR)");
        exit(1);
    }


    server_addr.sin_family = AF_INET;// host byte order
    server_addr.sin_port = htons(MYPORT); // short, network byte order
    server_addr.sin_addr.s_addr = inet_addr(hostIPAddr);
    memset(&(server_addr.sin_zero), '\0', 8); // zero the rest of the struct


    if(bind(sockfd, (sockaddr *)&server_addr, sizeof(sockaddr)) == -1)
    {
        perror("bind");

        exit(1);

    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    //

    /* Create JSON object with required values */
    cout << "\nCreating JSON object..."<<endl;
    Json::Value JSONObject;
    string hostIPAddress(hostIPAddr);

    time_t result = time(nullptr);
    string localTimeStamp =  asctime(localtime(&result));




    string DeviceID = "DeviceID";
    string DeviceType = "DeviceType";
    string Data = "Data";
    string Timestamp = "Timestamp";
    string currentIP = "currentIP";

    JSONObject[DeviceID] = "101";
    JSONObject[DeviceType] = "Thermistor";
    JSONObject[Timestamp] = localTimeStamp;
    JSONObject[currentIP] = hostIPAddress;


    // setup serial communication for teensy
    system("stty -F /dev/ttyACM9 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
    std::ifstream TeensyInput("/dev/ttyACM9"); //Opens the tty connection as an ifstream

    double Voltage;
    int Time = time(NULL);

        int i=0;

        while(time(NULL)-Time < 1){ }


            while(i==0)
            {

                TeensyInput>>Voltage;
                cout <<"\nAnalog reading: "<< Voltage <<"\n"<<endl;


                JSONObject[Data] = Voltage;

                // convert the json message to string
                Json::FastWriter fastWriter;
                string jsonMessage = fastWriter.write(JSONObject);
                cout<<jsonMessage<<endl;

                sleep(1);
                i++;
            }

            cout << "Total number of analog readings sent: "<< i <<endl;
            TeensyInput.clear();


        TeensyInput.close();





    cout << endl << JSONObject << endl;

    /* Write JSON object to file */
    ofstream jsonFileWrite;

    jsonFileWrite.open("/home/karticbhargav/SendThermistorData.json", ios_base::trunc);
    jsonFileWrite << JSONObject << endl;
    jsonFileWrite.close();


    if ((bytes_received = recvfrom(sockfd,cmd, 1000000, 0,(sockaddr *)&clientAddr, &sin_size)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }

/*    clientAddr.sin_family = AF_INET;// host byte order
    clientAddr.sin_port = htons(MYPORT); // short, network byte order
    clientAddr.sin_addr.s_addr = inet_addr("10.202.49.123");
    memset(&(clientAddr.sin_zero), '\0', 8); // zero the rest of the struct*/

    ifstream fin ("/home/karticbhargav/SendThermistorData.json");

    if (fin.is_open())
    {
        fin.seekg(0,ios::end);		//get file size
        size = fin.tellg();
        chptr = new char[size];

        fin.seekg(0,ios::beg);
        fin.read(chptr, size);
        fin.close();
    }

    chptr[size] = '\0';

    int len = strlen(chptr);

    if(strcmp(cmd, "get_data") == 0) {

        cout<< "Command Received: "<<cmd<<endl;

        if ((bytes_sent = sendto(sockfd, chptr, len, 0, (struct sockaddr *) &clientAddr, sizeof(sockaddr))) == -1) {
            perror("sendto");
            exit(1);
        }


        cout << endl << "Thermistor Values Sent to the Client" << endl;
    }

    close(sockfd);

    return 0;

}

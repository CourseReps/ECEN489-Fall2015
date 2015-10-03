
/*6optout client */
#include <json/json.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

#define DEVICEID "Kan Zheng"
#define DEVICETYPE "Photosensor"

using namespace std;

void get_time(char* buf,int buflen);
void remove_enter(char* buf,int buflen);
void save_json(char* data,char* IPAddr,char* strjson);
void init_udp_client_socket(char* IPAddr,char* port,struct addrinfo** paddr,int* psockfd);
int sendtoall(int sockfd, char *buf, int *len,struct addrinfo* Addr);
void broadcast_json(char* strjson,int strlenjson,struct addrinfo* Addr, int sockfd);

int main(int argc, char**argv)
{
	char szHostName[255];
	struct hostent *host_entry;
	struct sockaddr_in myaddr;
	int result;
	char *portname = "/dev/ttyACM0";
	char IPAddr[16];
	char buffer[15];
	char strjson[100];
	struct addrinfo* Addr;
	int sockfd;
	
	//check your input
	if(argc!=2)
	{
	  printf("Usage: %s <PORT>\n",argv[0]);
	  exit(1);
	}
	
	//init_socket funtion
	init_udp_client_socket("255.255.255.255",argv[1],&Addr,&sockfd);
	
	//get my IP Address
	gethostname(szHostName, 255);
	host_entry = gethostbyname(szHostName);
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr = *(struct in_addr *)*host_entry->h_addr_list;
	inet_ntop(myaddr.sin_family, &(myaddr.sin_addr),IPAddr, sizeof(IPAddr));
	
	//open serial file: ttyACM0
	int fd = open (portname, O_RDWR | O_NOCTTY | O_NDELAY);
	if ( fd < 0 )
	{
	    cout << "Error " << errno << " opening " << portname << ": " << strerror (errno) << endl;
	    exit(0);
	}
	
	while (1)
	{
	    result=read(fd,&buffer,sizeof buffer);
	    remove_enter(buffer,result);
	    if(result > 0 )
	    {
	      save_json(buffer,IPAddr,strjson);
	      broadcast_json(strjson,strlen(strjson),Addr,sockfd);
	      memset (&buffer, '\0', sizeof buffer);
//	      printf("Message sent\n");
	    }
	    else if (result <0)
	    {
	      cout << "Error " << errno << " reading " << ": " << strerror (errno) << endl;
	      exit(0);
	    }
	}

}

void init_udp_client_socket(char* IPAddr,char* port,struct addrinfo** paddr,int* psockfd)
{
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rv = getaddrinfo(IPAddr, port, &hints, &servinfo)) != 0) 
  {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      exit(1);
  }
  // loop through all the results and make a socket
  for(p = servinfo; p != NULL; p = p->ai_next) 
  {
      if (((*psockfd) = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
      {
	  perror("talker: socket");
	  continue;
      }
      break;
  }
  
  if(p==NULL)
  {
      fprintf(stderr, "talker: failed to create socket\n");
	  exit(1);
  }
    // this call is what allows broadcast packets to be sent:
    socklen_t broadcast;
    if (setsockopt(*psockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }
  
  *paddr=p;
}

// void init_read(char* buffer,int fd)
// {
//   int result;
//   while(1)
//   {
//     result=read(fd,&buffer,sizeof buffer);
//     if(result>0)
//       break;
//   }
// }

void broadcast_json(char* strjson,int strlenjson,struct addrinfo* Addr, int sockfd)
{
  if (sendtoall(sockfd, strjson, &strlenjson,Addr) == -1) 
  {
      perror("talker: sendtoall");
      exit(1);
  }
}

int sendtoall(int sockfd, char *buf, int *len,struct addrinfo* Addr)
{
    int total = 0;
    // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;
    while(total < *len) 
    {
	n = sendto(sockfd, buf+total, bytesleft, 0, Addr->ai_addr, Addr->ai_addrlen);
	if (n == -1) {perror("talker: failed to send\n"); break; }
	total += n;
	bytesleft -= n;
    }
    *len = total; // return number actually sent here
    return n==-1?-1:0; // return -1 on failure, 0 on success
}

void remove_enter(char* buf,int buflen)
{
  buf[buflen]='\0';
  for(int i=0;i<buflen;i++)
  {
    if(buf[i]=='\r'||buf[i]=='\n')
    {
      buf[i]='\0';
      break;
    }
  }
}
void get_time(char* buf,int buflen)
{
    time_t     now;
    struct tm  ts;
    // Get current time
    time(&now);
    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&now);
    strftime(buf, buflen, "%a %Y-%m-%d %H:%M:%S %Z", &ts);
}

void save_json(char* data,char* IPAddr,char* strjson)
{
    char Times[80]="";
    get_time(Times,sizeof Times);
    json_object * jobj = json_object_new_object();
    json_object *DeviceID = json_object_new_string(DEVICEID);
    json_object *DeviceType = json_object_new_string(DEVICETYPE);
    json_object *Data = json_object_new_string(data);
    json_object *Timestamp = json_object_new_string(Times);
   json_object *currentIP = json_object_new_string(IPAddr);

    json_object_object_add(jobj,"DeviceID", DeviceID);
    json_object_object_add(jobj,"DeviceType", DeviceType);
    json_object_object_add(jobj,"Data", Data);
    json_object_object_add(jobj,"Timestamp", Timestamp);
    json_object_object_add(jobj,"currentIP", currentIP);
    strcpy(strjson,json_object_to_json_string(jobj));
}





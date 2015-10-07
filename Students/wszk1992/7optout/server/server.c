
/*6optout server */
#include <json/json.h>
#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void init_udp_server_socket(int* psockfd, char* port);
void init_sqlite(char* sql,char* filename,sqlite3** db);
void json_to_sqlite(char* strjson,char* sql);

void debugtool()
{
  printf("DEBUG\n");
}

int main(int argc, char**argv)
{
	int sockfd;
	int rc;
	sqlite3* db;
	struct sockaddr_in otheraddr;
	int result;
	FILE* pfile; 
	char* json_saveaddr = "ipjson_sever.json";
	char *dbaddr = "jsondata.db";
	char* err_msg;
	char mesg[200];
	char bufmesg[1000]="";
	char tempmesg[200]="";
	char sql[1000]="";
	int i,j;
	
	//check input
	if (argc != 2)
	{
		printf("usage: server <Port>\n");
		exit(1);
	}
	//init socket
	init_udp_server_socket(&sockfd,argv[1]);
	//init sqlite

	init_sqlite(sql,dbaddr,&db);
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if (rc != SQLITE_OK) 
	{
	    fprintf(stderr, "SQL init error: %s\n", err_msg);
	    sqlite3_free(err_msg);
	    sqlite3_close(db);
	    exit(1);
	}
	//receiving data
	socklen_t len=sizeof(otheraddr);
	while(1)
	{
	  result = recvfrom(sockfd, mesg, sizeof(mesg), 0, (struct sockaddr *)&otheraddr, &len);

	  if(result < 0)
	  {
	      perror("talker: recvfrom");
	      exit(1);
	  }
 	  else if (result > 0)
	  {

 	      mesg[result]='\0';
	      json_to_sqlite(mesg,sql);
	      rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	      if (rc != SQLITE_OK) 
	      {
		  fprintf(stderr, "SQL error: %s\n", err_msg);
		  sqlite3_free(err_msg);
		  sqlite3_close(db);
		  exit(1);
	      }
//	      printf("Message received\n");
	  }

  
	}
	sqlite3_close(db);
	return 0;
}

void init_udp_server_socket(int* psockfd, char* port)
{
  int rv;
  struct addrinfo hints, *servinfo, *p;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
  
  //getaddrinfo
  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }
  
  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((*psockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }
    if (bind(*psockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(*psockfd);
      perror("listener: bind");
      continue;
    }
    break;
  }
  if (p == NULL) {
    fprintf(stderr, "listener: failed to bind socket\n");
    exit(1);
  }
  freeaddrinfo(servinfo);
  printf("listener: waiting to recvfrom...\n");
}

void init_sqlite(char* sql,char* filename,sqlite3** db)
{

    int rc = sqlite3_open(filename, db);
    if (rc != SQLITE_OK) 
    {
	fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
	sqlite3_close(*db);
	exit(1);
    }
    strcat(sql, "DROP TABLE IF EXISTS Info;");
     strcat(sql, "create table IF NOT EXISTS  photosensor(DeviceID TEXT,DeviceType TEXT,Data TEXT,Timestamp TEXT,currentIP TEXT);");
}

void json_to_sqlite(char* strjson,char* sql)
{
    
    int i=0;
    char *err_msg = 0;
    char tempval[5][100];
 
    json_object * jobj= json_tokener_parse(strjson);
    json_object_object_foreach(jobj, key, val)
    {
      strcpy(tempval[i],json_object_get_string(val));
      i++;
    }
    
    char temp[1000];

    sprintf(temp, "INSERT INTO photosensor VALUES('%s','%s','%s','%s','%s');", tempval[0], tempval[1], tempval[2], tempval[3], tempval[4]);
    printf("%s,%s,%s,%s,%s\n", tempval[0], tempval[1], tempval[2], tempval[3], tempval[4]);
    strcpy(sql, temp);


}


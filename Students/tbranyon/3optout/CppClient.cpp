#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>

using namespace std;

int get_serial_data()
{
  char buff[128];
	cout << "opening\n";
  FILE *fp = popen("cat < /dev/ttyACM0","r");
  while(fgets( buff, 128, fp ) != NULL ) {
    break;
  }
  string ret(buff);
  ret = ret.substr(0, ret.length()-1);
  pclose(fp);
  return atoi(ret.c_str());
}

int main(int argc, char *argv[]) //arguments: hostname(1) portnumber(2) 
{
	if(argc < 3)
	{
		cerr << "Error, missing arguments!\n";
		return 1;
	}
	//create socket
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		cerr << "Error creating socket, exiting...\n";
		return 2;
	}
	int portno = atoi(argv[2]);
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		cerr << "Error, no such host, exiting...\n";
		return 3;
	}
	
	//connect socket to server
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	int retval = connect(socketfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(retval < 0)
	{
		cerr << "Error connecting to server\n";
		cerr << "connect() returned " << retval << endl;
		return 4;
	}
	
	//data request/transfer
	//char cmd[10] = "get_photo
	int data;
	system("cat /dev/ttyACM0 > data.txt");
	FILE* fp = fopen("data.txt", "r");
	fscanf(fp, "%d", &data);
	fclose(fp);
	system("rm data.txt");
	int n = write(socketfd, &data, sizeof(int));
	if(n < 0)
	{
		cerr << "error writing to socket, exiting!\n";
		return 2;
	}
	int incoming;
	n = read(socketfd, &incoming, sizeof(int)); //blocks until data is there
	if(n < 0)
	{
		cerr << "Error reading, exiting!\n";
		return 1;
	}
	cout << incoming << endl;
	char cmd[78] = {0};
	sprintf(cmd, "echo %d > /dev/ttyACM0", incoming);
	system(cmd);

	/*int n = write(socketfd, cmd, sizeof(cmd));
	if(n < 0)
		cerr << "Error writing command\n";*/
	
	/*unsigned int filesize = 0;
	n = read(socketfd, &filesize, sizeof(int));
	if(n < 0)
		cerr << "Error reading size\n";
	cout << "Size: " << n << endl;*/
	
	/*char buffer[10240];
	FILE* image = fopen("remotescreenshot.jpeg", "wb");
	struct timeval timeout = {5, 0}; //5.0000000 second max timeout
	fd_set fds;
	int buffer_fd, recv_size, totalbytes = 0;*/
	/*while(1)//totalbytes < filesize)
	{
		FD_ZERO(&fds);
		FD_SET(socketfd, &fds);
		buffer_fd = select(FD_SETSIZE,&fds,NULL,NULL,&timeout); //check that data is available
		if(buffer_fd < 0)
		{
			cerr << "Error with file descriptor\n";
			continue;
		}
		else if(buffer_fd == 0)
		{
			cerr << "Timeout reached\n";
			continue;
		}
		else
		{
			cout << "Bytes received!\n";
			do{
				recv_size = read(socketfd, buffer, sizeof(buffer));
			}while(recv_size < 0);
		}
		if(recv_size == 0)
			break;
		n = fwrite(buffer, 1, recv_size, image);
		if(n < recv_size)
			cerr << "Error writing to file!\n";
		totalbytes += recv_size;
		//cout << "Total: " << filesize << "\t Received: " << totalbytes << endl;
	}
	fclose(image);*/
	
	
	//close connection and display screenshot
	close(socketfd);
	//system("gpicview remotescreenshot.jpeg &");
	return 0;
}

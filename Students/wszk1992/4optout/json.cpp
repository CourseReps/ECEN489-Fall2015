#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <Ws2tcpip.h>
#include <sstream>
#include <winsock2.h>

#pragma comment (lib, "Ws2_32.lib")
using namespace std;


struct JSON
{
	char firstname[20];
	char lastname[20];
	char gitUserName[20];
	char IPAddr[20];
}json;



int getjson(ofstream* pst, struct JSON json)
{
	if (pst->is_open())
	{

		*pst << "{";
		*pst << "\"firstName\": " << "\"" << json.firstname << "\",";
		*pst << "\"lastName\": " << "\"" << json.lastname << "\",";
		*pst << "\"gitUserName\": " << "\"" << json.gitUserName << "\",";
		*pst << "\"currentIP\": " << "\"" << json.IPAddr << "\"";
		*pst << "}";
		return 1;
	}
	else
		return 0;
}


int main(int argc, char**argv)
{
	WSADATA wsaData;
	char szHostName[255];
	struct hostent *host_entry;
	struct sockaddr_in myaddr;
	int result;
	char select='2';
	char* json_saveaddr = "ipjson.json";

	//initialate WSA
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		printf("Initial Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	//get my IP Address
	gethostname(szHostName, 255);
	host_entry = gethostbyname(szHostName);
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr = *(struct in_addr *)*host_entry->h_addr_list;

	while (select == '2')
	{
		//input info into json struct
		inet_ntop(myaddr.sin_family, &(myaddr.sin_addr), json.IPAddr, sizeof(json.IPAddr));
		cout << "firstName: " << endl;
		cin >> json.firstname;
		cout << "lastName: " << endl;  
		cin >> json.lastname;
		cout << "gitUserName: " << endl;
		cin >> json.gitUserName;
		printf("\nConfirm your info:\nfirstname:%s\nlastname:%s\ngitUserName:%s\nIPAddr:%s\n",
			json.firstname, json.lastname, json.gitUserName, json.IPAddr);
		printf("\nContinue:1, Input again:2, Exit: Other keys\n");
		cin >> select;
		cin.clear();
		fflush(stdin);
		if (select == '1')
			break;
		else if (select == '2')
			continue;
		else
			return 1;
	}

	//json
	ofstream myoutput(json_saveaddr);
	result = getjson(&myoutput, json);
	myoutput.close();
	if (result == 0)
	{
		printf("error in json\n");
		return 1;
	}

	printf("Save json successful in %s\n",json_saveaddr);

}
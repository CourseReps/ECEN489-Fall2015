// UDP_Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

SOCKET sock;
struct sockaddr_in Recv_addr;
int len = sizeof(struct sockaddr_in);
int port;
Json::Value objJSONin;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int databaseCreate()
{
    sqlite3 *db;
    int open_retval, exec_retval;
    char *zErrMsg;
    string insertStr;

    cout << "\nOpening Database...";
    open_retval = sqlite3_open("Broadcast.db", &db);
    if (open_retval)
    {
        cout << "\nError opening database...\nExiting...";
        sqlite3_close(db);
        _getch();
        return 1;
    }

    cout << "\nParsing JSON data for database write...";

    Json::StreamWriterBuilder builder;

    string firstName = Json::writeString(builder, objJSONin["firstName"]);
    firstName.erase(firstName.begin());
    firstName.erase(firstName.end() - 1, firstName.end());

    string lastName = Json::writeString(builder, objJSONin["lastName"]);
    lastName.erase(lastName.begin());
    lastName.erase(lastName.end() - 1, lastName.end());

    string gitUserName = Json::writeString(builder, objJSONin["gitUserName"]);
    gitUserName.erase(gitUserName.begin());
    gitUserName.erase(gitUserName.end() - 1, gitUserName.end());

    string currentIP = Json::writeString(builder, objJSONin["currentIP"]);
    currentIP.erase(currentIP.begin());
    currentIP.erase(currentIP.end() - 1, currentIP.end());

    insertStr = "INSERT INTO BroadcastData (FirstName, LastName, GitHubID, CurrentIP) VALUES ('"
        + firstName + "', '" + lastName + "', '" + gitUserName + "', '" + currentIP + "' );";

    cout << "\nWriting data into database...";

    exec_retval = sqlite3_exec(db, insertStr.c_str(), callback, 0, &zErrMsg);
    if (exec_retval != SQLITE_OK)
    {
        cerr << "\nSQL error: " << zErrMsg << "\nExiting...";
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        _getch();
        return(1);
    }

    sqlite3_close(db);

    cout << "\nWaiting for next broadcast...";
}

void recvFunct()
{
    int recvfrom_retval = 0;
    char recvBuff[200];
    string recvStr;
    size_t fileSize = 0, recvStr_end = 0;
    ofstream broadcastWrite;
    ifstream jsonFileRead;
    Json::Reader objJSONparse;

    while (1)
    {
        recvfrom_retval = recvfrom(sock, recvBuff, sizeof(recvBuff), 0, (sockaddr *)&Recv_addr, &len);
        if (recvfrom_retval < 0)
        {
            cerr << "Receive error...\nExiting...";
            _getch();
            closesocket(sock);
            WSACleanup();
            break;
        }
        else
        {
            cout << "\nReceiving broadcast data from a source...";
            recvStr = (string)recvBuff;
            recvStr_end = recvStr.find('}', 0) + 1;
            recvStr.erase(recvStr.begin() + recvStr_end, recvStr.end());
            cout << endl << recvStr << endl;
            fileSize = recvStr.length();
            broadcastWrite.open("Broadcast.json", ios_base::trunc | ios_base::binary);
            broadcastWrite.write(recvStr.c_str(), fileSize);
            broadcastWrite.close();

            cout << "\nReception from the source is completed. Received " << fileSize << " bytes.\n";
            
            jsonFileRead.open("Broadcast.json", ios_base::in | ios_base::binary);
            objJSONparse.parse(jsonFileRead, objJSONin, true);
            jsonFileRead.close();

            databaseCreate();
        }
    }
}

int main()
{
    int open_retval, exec_retval;
    char *createTable;
    char *zErrMsg = 0;
    sqlite3 *db;

    cout << "\nCreating Broadcast Database...";
    open_retval = sqlite3_open("Broadcast.db", &db);
    if (open_retval)
    {
        cout << "\nError opening database...\nExiting...";
        sqlite3_close(db);
        _getch();
        return 1;
    }

    cout << "\nCreating Table...";
    createTable = "CREATE TABLE IF NOT EXISTS BroadcastData(FirstName TEXT, LastName TEXT, GitHubID TEXT, CurrentIP TEXT);";

    exec_retval = sqlite3_exec(db, createTable, callback, 0, &zErrMsg);

    if (exec_retval != SQLITE_OK)
    {
        cerr << "\nSQL error: " << zErrMsg << "\nExiting...";
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        _getch();
        return(1);
    }

    sqlite3_close(db);

    cout << "\n\nUDP Server\nEnter port no: ";
    cin >> port;

    cout << "\nEstablishing UDP socket connection...";

    /* Establish Socket and UDP connection */
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int optval = true;
    char broadcast = 'a';

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&optval, sizeof(int)) < 0)
    {
        cerr << "\nBroadcast socket creation error...\nExiting...";
        _getch();
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "\nConnection established...";
    }

    Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(port);
    Recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) < 0)
    {
        cerr << "\nBinding error...\nExiting...";
        _getch();
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "\nBinding successful...";
    }

    cout << "\nListening for broadcast messages on port: " << port << endl;

    recvFunct();

    _getch();
    closesocket(sock);
    WSACleanup();
    return 0;
}


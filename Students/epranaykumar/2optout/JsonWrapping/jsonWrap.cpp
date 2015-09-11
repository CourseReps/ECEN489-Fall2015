
#include <iostream>

#include <string>
#include <cstring>


#include <fstream>

#include "fetchData.h"
#include "parseData.h"

using namespace std;


int main()
{
    const std::string url("http://engineering.tamu.edu/electrical/research/computer-engineering-systems-group");

    string data = fetchData(url);


    string file_name="parsejson.txt";
    ofstream outputFile(file_name);

    cout<<data;


    if (data.compare("NULL") != 0) {
        std::cout << "\nGot successful response from " << url << std::endl;

        bool status = parseData(data, file_name);

    }

     
    else
    {
        std::cout << "Couldn't GET from " << url << " - exiting" << std::endl;
        return 1;
    }

    return 0;
}

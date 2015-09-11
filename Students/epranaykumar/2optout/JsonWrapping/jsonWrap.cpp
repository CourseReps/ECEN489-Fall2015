
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

      /*  Json::Value jsonData;
        Json::Reader jsonReader;

      //  if (jsonReader.parse(*httpData.get(), jsonData))
        if (jsonReader.parse(data, jsonData))
        {
            std::cout << "Successfully parsed JSON data" << std::endl;
            std::cout << "\nJSON data received:" << std::endl;
            std::cout << jsonData.toStyledString() << std::endl;

            const std::string dateString(jsonData["date"].asString());
            const std::size_t unixTimeMs(
                    jsonData["milliseconds_since_epoch"].asUInt64());
            const std::string timeString(jsonData["time"].asString());

            std::cout << "Natively parsed:" << std::endl;
            std::cout << "\tDate string: " << dateString << std::endl;
            std::cout << "\tUnix timeMs: " << unixTimeMs << std::endl;
            std::cout << "\tTime string: " << timeString << std::endl;
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Could not parse HTTP data as JSON" << std::endl;
            std::cout << "HTTP data was:\n" << data << std::endl;
            return 1;
        }
    }
       */
    else
    {
        std::cout << "Couldn't GET from " << url << " - exiting" << std::endl;
        return 1;
    }

    return 0;
}

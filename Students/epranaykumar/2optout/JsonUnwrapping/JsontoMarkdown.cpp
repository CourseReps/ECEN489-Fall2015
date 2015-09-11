//
// Created by pranaykumar on 11/09/15.
//

#include <cstdio>

#include <cstring>
#include <fstream>
#include <iostream>


//This is the JSON header

#include "json/json.h"

using namespace std;
int main(int argc, char **argv) {

    ifstream jsonfile;
    string filename;
    cout<<"Enter the JsonObjects file name :"<<endl;
    cin>> filename;
    jsonfile.open(filename);
    string json_object;
    string in;

    if (jsonfile.is_open()) {
        while (!jsonfile.eof()) {
            jsonfile >> in;
            json_object += in;
             }

       // cout<< json_object;

// Let's parse it

        Json::Value json_data;
        Json::Reader reader;

        bool parsedSuccess = reader.parse(json_object,json_data, false);

        if (not parsedSuccess) {
            // Report failures and their locations in the document.
            cout<<"Failed to parse JSON"
            <<reader.getFormatedErrorMessages()
            <<endl;
            return 1;
        }
        ofstream outputFile("JsonObjects.md");

        // Let's extract the array contained in the jsondata
        outputFile << "### Faculty Members   \n  \n";

        for (int i=1;i<16;i++) {

            string faculty_attribute = "Faculty" + std::to_string(i);
            const Json::Value array = json_data[faculty_attribute];

            // Iterate over sequence elements
            outputFile << "  \n";

            for (unsigned int index = 0; index < array.size(); ++index) {
                outputFile <<array[index].asString();
                outputFile << "   \n";

            }
        }

        cout << "created Markdown in JsonObjects.md file " << endl ;
        // to print JSON_data
        //  cout << "Json Example pretty print: " << endl << json_data.toStyledString()<< endl;

        return 0;
    }
}
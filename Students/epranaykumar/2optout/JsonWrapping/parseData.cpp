//
// Created by pranaykumar on 11/09/15.
//
#include <iostream>
#include "json/json.h"
#include <fstream>
#include "parseData.h"

using namespace std;

bool parseData(std::string data, std::string file_name ){

    ofstream outputFile(file_name);
    string::iterator it;

    std::size_t pos = data.find(">Faculty Members:<");
    std::size_t end_data = data.find("<h3>", pos + 10);

    cout<<pos<<" \t" << end_data<<endl;

    if (pos == std::string::npos)  std::cout << "\n Web page doesn't contains faculty members section" << std::endl;


    else {
        outputFile << "{ ";
        int faculty_number=1;
        string  faculty_attribute= "Faculty"+ std::to_string(faculty_number);
        outputFile << "\"" <<faculty_attribute<< "\": [\" ";
        while (pos < end_data) {

            // cout << "\n \n faculty ::: \n";
            std::size_t start_pos = data.find("</tr>", pos) ;
            pos = start_pos;

            std::size_t end_pos = data.find("</tr>", pos+1);
            if(end_pos == std::string::npos){break;}


            it = data.begin() + start_pos + 5;


            if(faculty_number>1){

            string  faculty_attribute= "Faculty"+ std::to_string(faculty_number);
            outputFile << ",\"" <<faculty_attribute<< "\": [\" "; }

            faculty_number++;
            // cout<<pos<<" \t" << end_pos<<endl;

            int count=0;
            int fields=0;

            while (pos < end_pos) {
                char test = *it;
                if (test == '<') {
                    if (count==1 && fields<4) { outputFile << "\",\""; fields++; }
                    pos = 1 + data.find(">", pos);
                    it = data.begin() + pos ;
                    count=0;
                }
                else {
                    if (test !='\n'&&test !='\r'&& test !='\b'&& test !=' ') {
                        outputFile << *it;
                       // cout << *it;
                        count=1;
                    }
                    it++;
                }


                }
                pos=end_pos;

                outputFile << "\"]";
            }
        outputFile << " }";

    }
}
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <vector>

using namespace std;

int main() {
//	system("curl http://sports.espn.go.com/sports/tennis/rankings > rankingtennisdata.txt");
	ifstream myinput("rankingtennisdata.txt");
	ofstream myoutput("rankingstennis.json");
	stringstream input;
	input << myinput.rdbuf();
	string fileString = input.str();
	int pos1 = 0;
	int pos2 = 0;
	int pos3 = 0;
	int pos4 = 0;
	int pos5 = 0;
	int i = 0;
	vector<pair<string, string>> Pts;
	if (myinput.is_open()) {
		int m1 = fileString.find("!--this is it --");
		int m2 = fileString.find("Rankings provided");
		int len = m2 - m1;
		string subStr = fileString.substr(m1, len);



		for (i = 1; i <= 10; i++)
		{
			int p = i;
			string result1;
			ostringstream convert;
			convert << p;
			result1 = convert.str();
			pos1 = subStr.find(result1 + "</td>");
			subStr = subStr.substr(pos1, m2);
			if (i == 60)
			{
				int b=100;
				int c = b;
			}
			pos2 = subStr.find("id/");
			string rank = subStr.substr(0, pos2 - 35);
			subStr = subStr.substr(pos2, m2);
			pos3 = subStr.find("/");
			subStr = subStr.substr(pos3 + 1, m2);
			pos4 = subStr.find("/");
			subStr = subStr.substr(pos4 + 1, m2);
			pos5 = subStr.find("\"");

			string name = subStr.substr(0, pos5-1);
			pair<string, string> Points(name, rank);
			Pts.push_back(Points);

		}
		myinput.close();

	}


	map<string, int> ranx;
	if (myoutput.is_open())
	{
		myoutput << "\"athletes\":[\n";
		for (i = 0; i< Pts.size()- 1; i++)
		{
			myoutput << "\t" << "{" << "\"" << "Name" << "\":" << "\"" << Pts[i].first << "\"," << "\"""Rankings" << "\":" << "\"" << Pts[i].second << "\"" << "}," << "\n";
			cout << "\t\"" << Pts[i].first << "\":" << Pts[i].second << ",\n";
		}
		myoutput << "\t" << "{" << "\"" << "Name" << "\":" << "\"" << Pts[Pts.size() - 1].first << "\"," << "\"""Rankings" << "\":" << "\"" << Pts[Pts.size() - 1].second << "\"" << "}" << "\n";
		myoutput << "]";
	}
}
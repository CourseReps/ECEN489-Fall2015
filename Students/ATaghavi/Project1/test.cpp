#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>

using namespace std;

int main()
{
	string s = "10,5,2,1";

	vector<string> tokens;

	tokens = split(tokens,s, boost::is_any_of(","));

	for(int i=0; i<tokens.size(); i++)
	{
		cout<<tokens[i]<<'\n';
	}

}
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	vector<string> v;
	ifstream in( "code.cpp" );
	
	string line;
	while(in){
		v.push_back(line);
		getline(in, line);
	}
	in.close();

	for(int i = v.size()-1; i > 0; i--){
		cout<<v[i]<<endl;
	}

	return 0;
}
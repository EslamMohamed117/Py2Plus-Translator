#pragma once
#include <fstream>
using namespace std;

class Files
{
private:
	ifstream inFile;
	ofstream outFile;
	char operation;
	bool error = false;
public:
	string code;
	Files(string, char);
	bool readFromFile();
	bool writeToFile(string);
	string getContentFromFile();
	string getCode();
};


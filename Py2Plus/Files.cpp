#include "Files.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

Files::Files(string filename, char operation)
{
	switch (operation)
	{
		case 'r':
			this->inFile.open(filename);
			break;
		case 'w':
			this->outFile.open(filename);
			break;
		default:
			this->error = true;
			this->code = "Error: failed to open " + filename + " !!";
			break;
	}
	this->operation = operation;
}

string Files::getContentFromFile()
{
	string content="", line = "";
	while (std::getline(this->inFile, line))
		content += line + "\n";
	return content;
}

bool Files::readFromFile()
{
	if (!this->error)
	{
		this->code = this->getContentFromFile();
		this->inFile.close();
		return true;
	}
	return false;
}

bool Files::writeToFile(string content)
{
	
	if (!this->error)
	{
		this->outFile << content;
		this->outFile.close();
		return true;
	}
	return false;
}

string Files::getCode()
{
	return this->code;
}



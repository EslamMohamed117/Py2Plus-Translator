#include "Files.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

/**
 * @brief Constructor for the `Files` class.
 * @param filename The name of the file to open.
 * @param operation The operation to perform on the file ('r' for reading or 'w' for writing).
 */
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

/**
 * @brief Reads the content of the input file.
 * @return A string containing the content of the input file.
 */
string Files::getContentFromFile()
{
	string content="", line = "";
	while (std::getline(this->inFile, line))
		content += line + "\n";
	return content;
}

/**
 * @brief Reads the content of the input file and stores it in the `code` member variable.
 * @return true if reading is successful, false otherwise.
 */
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

/**
 * @brief Writes the specified content to the output file.
 * @param content The content to write to the output file.
 * @return true if writing is successful, false otherwise.
 */
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

/**
 * @brief Returns the content of the input file that was read by the `readFromFile()` function.
 * @return A string containing the content of the input file.
 */
string Files::getCode()
{
	return this->code;
}



#include "Py2Plus.h"
#include <iostream>
#include "Files.h"
#include "Scanner.h"
#include "Parser.h"
#include <QMessageBox>
#include <string>

#include <regex>

using namespace std;


/* Parser Type Object */
Parser_t myParse;
string errorMessage = "Syntax Error!!!";

Py2Plus::Py2Plus(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	// ------------------ THIS PART IS USED FOR TESTING PURPOSES ONLY ------------------
	Files* file = new Files("source_code.txt", 'r');
    file->readFromFile();
	Scanner scanner(file->getCode());
	scanner.Tokenize();
	Parser parser(scanner.GetTokenList());
	string output = "", cppCode = "";
	/* Print Python Code */
	for (TokenInfo token : scanner.GetTokenList())
		output += "[" + token.token + "]\t" + token.tokenValue + "\n";
	QMessageBox::information(this, "Button Clicked", output.c_str());
	/*
	regex regex("[-+]?[0-9]*.?[0-9]+([eE][-+]?[0-9]+)?");//[a-zA-Z_][a-zA-Z0-9_]*   -   ^[a-zA-Z_][a-zA-Z0-9_]*$   -?[0-9]*.?[0-9]+[eE][-+]?[0-9]+
	string str1 = "02";
	regex_match(str1, regex) ? str1 = "match" : str1 = "no match";
	QMessageBox::information(this, "Button Clicked", str1.c_str());
	*/
	/* Print Converted Code */
	myParse = parser.Parse();
	cppCode = myParse.py2PlusCode;
	if(myParse.errorStatus == true)
		QMessageBox::information(this, "Button Clicked", errorMessage.c_str());
	else
		QMessageBox::information(this, "Button Clicked", cppCode.c_str());

	// ---------------------------------------------------------------------------------
}

Py2Plus::~Py2Plus()
{}
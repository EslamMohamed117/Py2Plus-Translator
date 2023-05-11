#include "Py2Plus.h"
#include <iostream>
#include "Files.h"
#include "Scanner.h"
#include "Parser.h"
#include <QMessageBox>
#include <string>

#include <regex>

using namespace std;

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
	for (TokenInfo token : scanner.GetTokenList());
		//output += "[" + token.token + "]\t" + token.tokenValue + "\n";

	/* Regex TOBE REMOVED
	regex regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
	string str1 = "ASD23124";
	regex_match(str1, regex) ? str1 = "match" : str1 = "no match";
	*/

	/* Print Converted Code */
	cppCode = parser.Parse();
	QMessageBox::information(this, "Button Clicked", cppCode.c_str());

	// ---------------------------------------------------------------------------------
}

Py2Plus::~Py2Plus()
{}
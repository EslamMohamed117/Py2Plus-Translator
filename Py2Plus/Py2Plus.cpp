#include "Py2Plus.h"
#include <iostream>
#include "Files.h"
#include "Scanner.h"
#include <QMessageBox>
#include <string>
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
	string output = "";
	for (TokenInfo token : scanner.GetTokenList())
		output += "[" + token.token + "]\t" + token.tokenValue + "\n";
    QMessageBox::information(this, "Button Clicked", output.c_str());
	// ---------------------------------------------------------------------------------
}

Py2Plus::~Py2Plus()
{}
#include "Py2Plus.h"
#include <iostream>
#include "Files.h"
#include "Scanner.h"
#include "Parser.h"
#include <QMessageBox>
#include <string>

#include <regex>

#include <QPropertyAnimation>
#include <QFileDialog>

using namespace std;


/* Parser Type Object */
Parser_t myParse;
string errorMessage = "Syntax Error!!!";

Py2Plus::Py2Plus(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	// ------------------ Initializations ----------------------------------------------
	
	this->isBrowsed = true;

	// hide compnenets
	ui.codeInput->setVisible(false);
	ui.inputLabel->setVisible(false);
	ui.outputLabel->setVisible(false);

	// set editability to false
	ui.outputText->setReadOnly(true);


	// Connect the button clicked signal to a slot
	connect(ui.switchButton, &QPushButton::clicked, this, &Py2Plus::onSwitchButtonClicked);
	connect(ui.browseButton, &QPushButton::clicked, this, &Py2Plus::onBrowseClicked);
	connect(ui.translateButton, &QPushButton::clicked, this, &Py2Plus::onTranslateClicked);
	
	// ---------------------------------------------------------------------------------
	/*
	// ------------------ THIS PART IS USED FOR TESTING PURPOSES ONLY ------------------
	Files* file = new Files("source_code.txt", 'r');
    file->readFromFile();
	Scanner scanner(file->getCode());
	scanner.Tokenize();
	Parser parser(scanner.GetTokenList());
	string output = "", cppCode = "";
	// Print Python Code 
	for (TokenInfo token : scanner.GetTokenList())
		output += "[" + token.token + "]\t" + token.tokenValue + "\n";
	QMessageBox::information(this, "Button Clicked", output.c_str());
	
	//regex regex("[-+]?[0-9]*.?[0-9]+([eE][-+]?[0-9]+)?");//[a-zA-Z_][a-zA-Z0-9_]*   -   ^[a-zA-Z_][a-zA-Z0-9_]*$   -?[0-9]*.?[0-9]+[eE][-+]?[0-9]+
	//string str1 = "02";
	//regex_match(str1, regex) ? str1 = "match" : str1 = "no match";
	//QMessageBox::information(this, "Button Clicked", str1.c_str());
	
	// Print Converted Code 
	myParse = parser.Parse();
	cppCode = myParse.py2PlusCode;
	if(myParse.errorStatus == true)
		QMessageBox::information(this, "Button Clicked", errorMessage.c_str());
	else
		QMessageBox::information(this, "Button Clicked", cppCode.c_str());

	// ---------------------------------------------------------------------------------
	*/
}

Py2Plus::~Py2Plus()
{}

void Py2Plus::onSwitchButtonClicked()
{
	QPropertyAnimation* animation = new QPropertyAnimation(ui.switchButton, "geometry");
	animation->setDuration(200);  // Animation duration in milliseconds

	if (!this->isBrowsed) {
		animation->setStartValue(QRect(45, ui.switchButton->y(), ui.switchButton->width(), ui.switchButton->height()));
		animation->setEndValue(QRect(80, ui.switchButton->y(), ui.switchButton->width(), ui.switchButton->height()));
		ui.switchButton->setStyleSheet("border-radius: 15px; background-color: rgb(57, 229, 215);");
		this->isBrowsed = true;
	}
	else {
		animation->setStartValue(QRect(80, ui.switchButton->y(), ui.switchButton->width(), ui.switchButton->height()));
		animation->setEndValue(QRect(45, ui.switchButton->y(), ui.switchButton->width(), ui.switchButton->height()));
		ui.switchButton->setStyleSheet("border-radius: 15px; background-color:  rgb(191,191,191);");
		this->isBrowsed = false;
	}
	ui.browseFrame->setVisible(this->isBrowsed);
	ui.codeInput->setVisible(!this->isBrowsed);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Py2Plus::onBrowseClicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select File"));
	if (!filePath.isEmpty())
	{
		ui.borwsePath->setText(filePath);
	}
}

void Py2Plus::onTranslateClicked()
{
	if (this->isBrowsed)
	{
		// get file path from gui
		string pathname = ui.borwsePath->text().toStdString();
		
		// read the file
		Files* file = new Files(pathname, 'r');
		file->readFromFile();
		
		// tokenize the code
		Scanner scanner(file->getCode());
		scanner.Tokenize();
		
		// parse the code
		Parser parser(scanner.GetTokenList());
		myParse = parser.Parse();
		
		if (myParse.errorStatus == true)
			ui.inputLabel->setText(QString::fromStdString(errorMessage));
		
		ui.outputText->setText(QString::fromStdString(myParse.py2PlusCode));

	}
	else
	{
		string code = ui.inputText->toPlainText().toStdString();
		

		// tokenize the code
		Scanner scanner(code);
		scanner.Tokenize();

		// parse the code
		Parser parser(scanner.GetTokenList());
		myParse = parser.Parse();

		if (myParse.errorStatus == true)
			ui.inputLabel->setText(QString::fromStdString(errorMessage));

		ui.outputText->setText(QString::fromStdString(myParse.py2PlusCode));

	}
}
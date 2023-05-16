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
	ui.outputLabel->setVisible(false);

	// set editability to false
	ui.outputText->setReadOnly(true);
	
	// set alignment of inputLabel to center
	ui.inputLabel->setAlignment(Qt::AlignCenter);


	// Connect the button clicked signal to a slot
	connect(ui.switchButton, &QPushButton::clicked, this, &Py2Plus::onSwitchButtonClicked);
	connect(ui.browseButton, &QPushButton::clicked, this, &Py2Plus::onBrowseClicked);
	connect(ui.translateButton, &QPushButton::clicked, this, &Py2Plus::onTranslateClicked);
	
	// ---------------------------------------------------------------------------------
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
		if (pathname.size() != 0)
		{
			// read the file
			Files* file = new Files(pathname, 'r');
			file->readFromFile();

			// tokenize the code
			Scanner scanner(file->getCode());

			scanner.Tokenize();
			ui.inputLabel->setText("Pending trasnlation...");

			// parse the code
			Parser parser(scanner.GetTokenList());
			myParse = parser.Parse();

			if (myParse.errorStatus == true)
			{
				ui.inputLabel->setText(QString::fromStdString(errorMessage));
				return;
			}

			ui.inputLabel->setText("Translation successful");
			ui.outputText->setText(QString::fromStdString(scanner.removeEmptyLines(myParse.py2PlusCode)));
			ui.outputLabel->setVisible(true);
			Files* outFile = new Files("target_code.txt", 'w');
			outFile->writeToFile(myParse.py2PlusCode);
		}
		else
		{
			ui.outputLabel->setVisible(false);
			ui.inputLabel->setText("Please Enter a Valid Path");
			return;
		}

	}
	else
	{
		string code = ui.inputText->toPlainText().toStdString();
		if (std::all_of(code.begin(), code.end(), [](char c) { return std::isspace(c); })) {
			return;
		}
		if (code.size() != 0)
		{
			// tokenize the code
			Scanner scanner(code);
			scanner.Tokenize();
			ui.inputLabel->setText("Pending trasnlation...");

			// parse the code
			Parser parser(scanner.GetTokenList());
			myParse = parser.Parse();

			if (myParse.errorStatus == true)
			{
				ui.inputLabel->setText(QString::fromStdString(errorMessage));
				return;
			}
			
			ui.inputLabel->setText("Translation successful");
			ui.outputText->setText(QString::fromStdString(scanner.removeEmptyLines(myParse.py2PlusCode)));
			ui.outputLabel->setVisible(true);
			Files* outFile = new Files("target_code.txt", 'w');
			outFile->writeToFile(myParse.py2PlusCode);
		}
		else
		{
			ui.outputLabel->setVisible(false);
			ui.inputLabel->setText("Please write some code");
			return;
		}
	}
}
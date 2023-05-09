#include "Py2Plus.h"
#include <iostream>
#include "Files.h"
#include <QMessageBox>
#include <string>
using namespace std;

Py2Plus::Py2Plus(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	// ------------------ THIS PART IS USED FOR TESTING PURPOSES ONLY ------------------
	Files* file = new Files("test.py", 'r');
    file->readFromFile();
    QMessageBox::information(this, "Button Clicked", file->getCode().c_str());
	// ---------------------------------------------------------------------------------
}

Py2Plus::~Py2Plus()
{}
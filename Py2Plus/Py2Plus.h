#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Py2Plus.h"

class Py2Plus : public QMainWindow
{
    Q_OBJECT

public:
    Py2Plus(QWidget *parent = nullptr);
    ~Py2Plus();

private slots:
    void onSwitchButtonClicked();
    void onBrowseClicked();
    void onTranslateClicked();
	
private:
    Ui::Py2Plus ui;
    bool isBrowsed;
};

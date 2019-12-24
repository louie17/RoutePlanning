#pragma once
#include"qpushbutton.h"
#include "qtabwidget.h"
#include "qtablewidget.h"
class MyTab
{
public:
	MyTab();
	~MyTab();
	QPushButton *add;
	QPushButton *del;
	QPushButton *save;
	int myindex;
	QTableWidget *my_table;


};


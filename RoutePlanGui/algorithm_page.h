#pragma once

#include <QWidget>
#include <Qtxml>
#include "MainWindow.h"
#include "ui_algorithm_page.h"

class algorithm_page : public QWidget
{
	Q_OBJECT

public:
	algorithm_page(QWidget *parent = Q_NULLPTR);
	~algorithm_page();

	


signals:
	void go_datapage();//声明一个信号
	void go_markov();
private slots:
	void run_algorithm();
	void setLabelText(int);
	void on_datapage();
	void show_algorithm_page();
	void show_markov();
private:
	Ui::algorithm_page ui;
};

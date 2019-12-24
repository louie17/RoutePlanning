#pragma once

#include <QWidget>
#include <QMainWindow>
#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include "ui_MainWindow4.h"
#include "MyTab.h"
class MainWindow4 : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow4(QMainWindow *parent = Q_NULLPTR);
	~MainWindow4();
private:
	Ui::MainWindow4 ui;
signals:
	void sign_algorithm();
	void sign_w2();
	void sign_ecm_tech();
	void sign_add_tech();
	void sign_add_button_clicked(QTableWidget*);
private slots:
	void show_xml_data();
	void ecm_tech();
	void show_w4();
	void save_esm();
	void save_ecm();
	void add_tab();
	void del_tab();
	void show_page3();
	void back_show_page2();
	void append_esm();
	void del_esm();
	void append_ecm1();
	void del_ecm1();
	void receive_index(int index);
public:
	void route_add_col();

public: 
	QVector<MyTab*> vTab;
	QVector<QTableWidget*> route_v;
	QPushButton * tab_btn=NULL;
	int route_index=-1;
};

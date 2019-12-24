#pragma once

#include <QWidget>
#include <Qtxml>
#include "MainWindow.h"
#include "ui_MainWindow2.h"


class MainWindow2 : public QWidget
{
	Q_OBJECT

public:
	MainWindow2(QWidget *parent = Q_NULLPTR);
	~MainWindow2();
	
private:
	Ui::MainWindow2 ui;
	QStringList headers_section;
signals:
	void sign_w4();
	void sign_previous();
	void sign_esmstrategy_section();
	void sign_ecmstrategy_section();
	void sign_added_esmstrategy_section();
	void sign_added_ecmstrategy_section();
private slots:
	void show_xml_data();
	void save_EsmStrategy();
	void save_EcmStrategy();
	void save_PlatformSiteRelation();
	void on_previous();
	void show_page4();
	void show_page2();
	void append_PSR();
	void save_OwnPlatformRouteRelation();
	void save_EsmEsmStrategyRelation();
	void save_EcmEcmStrategyRelation();
	void save_PlatformEmitterRelation();
	void save_PlatformWeaponRelation();
	void save_OwnPlatformEcmRelation();
	void save_OwnPlatformEsmRelation();
	void append_EsmStrategy();
	void added_esmstrategy_section();
	void added_ecmstrategy_section();
	void append_EcmStrategy();
	void del_EcmStrategy();
	void del_EsmStrategy();
	void show_esmstrategy_section();
	void show_ecmstrategy_section();
	void del_PSR();
	void append_EESR();
	void del_EESR();
	void append_EESR2();
	void del_EESR2();
	void append_OPRR();
	void del_OPRR();
	void append_PER();
	void del_PER();
	void append_PWR();
	void del_PWR();
	void append_OPER();
	void del_OPER();
	void append_OPER2();
	void del_OPER2();
};

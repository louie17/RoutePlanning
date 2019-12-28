#pragma once

#include <QtWidgets/QMainWindow>
#include <Qtxml>
#include <string>
#include <QComboBox>
#include "ui_MainWindow.h"
#include "MainWindow2.h"
#include <QInputDialog>
#include "Scenario.hpp"

extern sce::Scenario scenario;
extern int choice_emitter;
extern int choice_radar;
extern int choice_esmstrategy;
extern int choice_ecmstrategy;
extern int choice_section;
extern int choice_ecm;
extern int choice_ownplatform;
extern int choice_point;
extern QDomDocument dom;
extern QFile file;

class RoutePlannGui : public QMainWindow
{
	Q_OBJECT

public:
	RoutePlannGui(QWidget *parent = Q_NULLPTR);
	void listDom(QDomElement & docelem);
	std::vector<std::shared_ptr<sce::Route>> routes;
	std::vector<std::shared_ptr<sce::EsmStrategy>> esmstrategys;
	std::vector<std::shared_ptr<sce::EcmStrategy>> ecmstrategys;
	std::vector<sce::PlatformSiteRelation> PSRs;
	std::vector<sce::PlatformEmitterRelation> PERs;
	std::vector<sce::PlatformWeaponRelation> PWRs;
	std::vector<sce::OwnPlatformEsmRelation> OPEsmRs;
	std::vector<sce::EsmEsmStrategyRelation> EsmESRs;
	std::vector<sce::OwnPlatformEcmRelation> OPEcmRs;
	std::vector<sce::EcmEcmStrategyRelation> EcmESRs;
	std::vector<sce::OwnPlatformRouteRelation> OPRRs;
	//sce::Scenario scenario;
	QDomElement *ParentNode;
	QDomElement *SibingNode;
private:
	Ui::MainWindow ui;
	QStringList platformtype_list;
	QStringList ownplatformtype_list;

	size_t open_flag{ 0 };
signals:
	void go_next();//œ‘ æœ¬“ª“≥
	void rada_mode();
	void rada_mode_2();
	void sign_add_rada();
	void sign_mission();
	void sign_added_mission();
	void sign_show_xml_data();
	void show_data_page2();
	void show_data_page3();
private slots:
	void show_rada();
	void setOpenFileFlag();
	void save_vertex();
	void save_platform();
	void save_weapon();
	void show_xml_data();
	void save_emitter();
	void save_to_file();
	void save_site();
	void save_ownplatform();
	void add_ownplatform();
	void added_mission();
	void del_ownplatform();
	void add_emitter();
	void del_emitter();
	void add_rada_mode();
	void on_open();
	void append_Vertex();
	void del_vertex();
	void append_platform();
	void del_platform();
	void append_weapon();
	void del_weapon();
	void append_site();
	void del_site();
	void show_mission();
	void on_next();
	void show_page1();

};

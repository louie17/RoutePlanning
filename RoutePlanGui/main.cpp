#include "MainWindow.h"
#include "MainWindow2.h"
#include "algorithm_page.h"
#include "MainWindow4.h"
#include "Section_page.h"
#include "DwellSquence_page.h"
#include "tech_page.h"
#include "Radar_mode_page.h"
#include "Mission_page.h"
#include "Rf_page.h"
#include "Pw_page.h"
#include "Pri_page.h"
#include "Scan_page.h"
#include "Erp_page.h"
#include "Pri_values_page.h"
#include "Rf_values_page.h"
#include "Pw_values_page.h"
#include "point_page.h"
#include <QtWidgets/QApplication>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RoutePlannGui w1;
	MainWindow2 w2;
	algorithm_page w3;
	MainWindow4 w4;
	Radar_mode_page rada;
	Rf_page rf;
	Pw_page pw;
	Pri_page pri;
	Scan_page sc;
	Erp_page erp;
	Pri_values_page pri_v;
	Rf_values_page rf_v;
	Pw_values_page pw_v;
	Section_page section;
	DwellSquence_page dwellsquence;
	tech_page tech;
	Mission_page mission;
	point_page point;
	w1.show();

	QObject::connect(&w1, SIGNAL(sign_mission()), &mission, SLOT(show_mission()));
	QObject::connect(&w1, SIGNAL(rada_mode()), &rada, SLOT(rada_show()));
	QObject::connect(&w1, SIGNAL(go_next()), &w2, SLOT(show_page2()));
	QObject::connect(&w1, SIGNAL(show_data_page2()), &w2, SLOT(show_xml_data()));
	QObject::connect(&w1, SIGNAL(show_data_page3()), &w4, SLOT(show_xml_data()));

	QObject::connect(&w2, SIGNAL(sign_added_esmstrategy_section()), &section, SLOT(added_esmstrategy_section()));
	QObject::connect(&w2, SIGNAL(sign_added_ecmstrategy_section()), &section, SLOT(added_ecmstrategy_section()));
	QObject::connect(&w2, SIGNAL(sign_ecmstrategy_section()), &section, SLOT(show_ecmstrategy_section()));
	QObject::connect(&w2, SIGNAL(sign_esmstrategy_section()), &section, SLOT(show_esmstrategy_section()));
	QObject::connect(&w2, SIGNAL(sign_previous()), &w1, SLOT(show_page1()));
	QObject::connect(&w2, SIGNAL(sign_w4()), &w4, SLOT(show_w4()));

	QObject::connect(&w3, SIGNAL(go_datapage()), &w1, SLOT(show_page1()));

	QObject::connect(&w4, SIGNAL(sign_ecm_tech()), &tech, SLOT(show_ecm_tech()));

	QObject::connect(&w4, SIGNAL(sign_algorithm()), &w3, SLOT(show_algorithm_page()));
	QObject::connect(&w4, SIGNAL(sign_w2()), &w2, SLOT(show_page2()));

	QObject::connect(&section, SIGNAL(sign_added_dwellsquence()), &dwellsquence, SLOT(added_dwellsquence()));
	QObject::connect(&section, SIGNAL(sign_dwellsquence()), &dwellsquence, SLOT(show_dwellsquence()));

	QObject::connect(&section, SIGNAL(sign_added_ecmstrategy_tech()), &tech, SLOT(added_ecmstrategy_tech()));
	QObject::connect(&section, SIGNAL(sign_tech()), &tech, SLOT(show_ecmstragegy_tech()));

	QObject::connect(&mission, SIGNAL(sign_point()), &point, SLOT(show_point()));

	QObject::connect(&rada, SIGNAL(sign_erp()), &erp, SLOT(show_erp()));

	QObject::connect(&rada, SIGNAL(sign_scan()), &sc, SLOT(show_scan()));

	QObject::connect(&rada, SIGNAL(sign_pri()), &pri, SLOT(show_pri()));

	QObject::connect(&rada, SIGNAL(sign_pw()), &pw, SLOT(show_pw()));

	QObject::connect(&rada, SIGNAL(sign_rf()), &rf, SLOT(rf_show()));

	QObject::connect(&rf, SIGNAL(sign_add_new()), &rf_v, SLOT(show_rf_v()));
	QObject::connect(&rf, SIGNAL(sign_rf_value()), &rf_v, SLOT(show_rf_v()));

	QObject::connect(&pw, SIGNAL(sign_add()), &pw_v, SLOT(show_pw_v()));
	QObject::connect(&pw, SIGNAL(sign_pw_v()), &pw_v, SLOT(show_pw_v()));

	QObject::connect(&pri, SIGNAL(sign_add_new()), &pri_v, SLOT(show_pri_v()));
	QObject::connect(&pri, SIGNAL(sign_pri_values()), &pri_v, SLOT(show_pri_v()));

	return a.exec();
}


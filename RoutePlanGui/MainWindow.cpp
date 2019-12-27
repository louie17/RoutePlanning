#include "MainWindow.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include "Scenario.hpp"
#include "xml_prase.h"

using namespace std;
using namespace sce;

#pragma execution_character_set("utf-8")  

int choice_emitter = 0;
int choice_radar = 0;
int choice_esmstrategy = 0;
int choice_ecmstrategy = 0;
int choice_section = 0;
int choice_ecm = 0;
int choice_ownplatform = 0;
int choice_point = 1;
Scenario scenario;
QDomDocument dom;
QFile file;

RoutePlannGui::RoutePlannGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Read_data"));//设置标题
	ui.pushButton_3->setText("Add");
	ui.pushButton_4->setText("Del");
	ui.pushButton_8->setText("Del");
	ui.pushButton_8->setText("save");
	QStringList headers;
	headers << QStringLiteral("Name") << QStringLiteral("Rada_mode");
	ui.tableWidget_6->setColumnCount(9);
	QStringList list;
	list << "Name" << "Type" << "MaxAcceleration" << "MaxDeceleration" << "MaxClimbRate" << "MaxDiveRate" << "MaxSpeed" << "MaxTurnRadius" << "Mission";
	ui.tableWidget_6->setHorizontalHeaderLabels(list);
	ui.tableWidget_4->setHorizontalHeaderLabels(headers);
	ui.tableWidget_4->horizontalHeader()->setHighlightSections(false);
	//connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(save_to_file()));
	//connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(on_open()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(add_emitter()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(del_emitter()));
	connect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(del_vertex()));
	connect(ui.pushButton_12, SIGNAL(clicked()), this, SLOT(append_site()));
	connect(ui.pushButton_10, SIGNAL(clicked()), this, SLOT(append_Vertex()));
	connect(ui.pushButton_13, SIGNAL(clicked()), this, SLOT(del_site()));
	connect(ui.pushButton_14, SIGNAL(clicked()), this, SLOT(append_weapon()));
	connect(ui.pushButton_15, SIGNAL(clicked()), this, SLOT(del_weapon()));
	connect(ui.pushButton_16, SIGNAL(clicked()), this, SLOT(append_platform()));
	connect(ui.pushButton_17, SIGNAL(clicked()), this, SLOT(del_platform()));
	connect(ui.next, SIGNAL(clicked()), this, SLOT(on_next()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(save_vertex()));
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(save_platform()));
	connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(save_weapon()));
	connect(ui.pushButton_8, SIGNAL(clicked()), this, SLOT(save_emitter()));
	connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(save_site()));
	connect(ui.pushButton_18, SIGNAL(clicked()), this, SLOT(add_ownplatform()));
	connect(ui.pushButton_19, SIGNAL(clicked()), this, SLOT(del_ownplatform()));
	connect(ui.pushButton_20, SIGNAL(clicked()), this, SLOT(save_ownplatform()));
	connect(ui.actionOpen_file, SIGNAL(triggered()), this, SLOT(on_open()));
	connect(ui.actionOpen_file, SIGNAL(triggered()), this, SLOT(setOpenFileFlag()));
	connect(ui.actionShow_data, SIGNAL(triggered()), this, SLOT(show_xml_data()));
	connect(this, SIGNAL(sign_show_xml_data()), this, SLOT(show_xml_data()));
	connect(ui.actionSave_to_file, SIGNAL(triggered()), this, SLOT(save_to_file()));
	platformtype_list << "Land" << "Air" << "Surface";
	ownplatformtype_list << "Air";
}
void RoutePlannGui::show_xml_data() {
	ui.tableWidget->setRowCount(0);
	for (int i = 0; i < scenario.getAllVertex().size(); i++) {
		ui.tableWidget->insertRow(i);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number((scenario.getAllVertex()[i]->getLatitude()), 'f', 2)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number((scenario.getAllVertex()[i]->getLongitude()), 'f', 2)));
	}
	//显示platform的数据
	ui.tableWidget_2->setRowCount(0);
	for (int i = 0; i < scenario.getAllPlatform().size(); i++)
	{
		ui.tableWidget_2->insertRow(i);
		ui.tableWidget_2->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllPlatform()[i]->getPlatformName())));
		ui.tableWidget_2->setCellWidget(i, 1, new QComboBox());
		auto cell = ui.tableWidget_2->cellWidget(i, 1);
		QComboBox *cb_cell = qobject_cast<QComboBox*>(cell);
		cb_cell->addItems(platformtype_list);
		if (scenario.getAllPlatform()[i]->getPlatformType() == PlatformType::LAND)
		{
			cb_cell->setCurrentIndex(0);
		}
		if (scenario.getAllPlatform()[i]->getPlatformType() == PlatformType::AIR)
		{
			cb_cell->setCurrentIndex(1);
		}
		if (scenario.getAllPlatform()[i]->getPlatformType() == PlatformType::SURFACE)
		{
			cb_cell->setCurrentIndex(2);
		}
	}
	//显示weapon的数据
	ui.tableWidget_3->setRowCount(0);
	for (int i = 0; i < scenario.getAllWeapon().size(); i++)
	{
		ui.tableWidget_3->insertRow(i);
		ui.tableWidget_3->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllWeapon()[i]->getName())));
		ui.tableWidget_3->setItem(i, 1, new QTableWidgetItem(QString::number(scenario.getAllWeapon()[i]->getCEPR())));
		ui.tableWidget_3->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllWeapon()[i]->getWeaponAreaCoverage())));
	}
	//显示site的数据
	ui.tableWidget_5->setRowCount(0);
	for (int i = 0; i < scenario.getAllSite().size(); i++)
	{
		ui.tableWidget_5->insertRow(i);
		ui.tableWidget_5->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllSite()[i]->getName())));
		ui.tableWidget_5->setItem(i, 1, new QTableWidgetItem(QString::number((scenario.getAllSite()[i]->getAltitude()), 'f', 2)));
		ui.tableWidget_5->setItem(i, 3, new QTableWidgetItem(QString::number((scenario.getAllSite()[i]->getLongitude()), 'f', 2)));
		ui.tableWidget_5->setItem(i, 2, new QTableWidgetItem(QString::number((scenario.getAllSite()[i]->getLatitude()), 'f', 2)));
	}
	//显示emitter的数据
	ui.tableWidget_4->setRowCount(0);
	for (int i = 0; i < scenario.getAllEmitter().size(); i++) {
		ui.tableWidget_4->insertRow(i);
		ui.tableWidget_4->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllEmitter()[i]->getName())));
		QPushButton *btn = new QPushButton();
		btn->setText("View");
		ui.tableWidget_4->setCellWidget(i, 1, btn);
		connect(btn, SIGNAL(clicked()), this, SLOT(show_rada()));
	}
	//显示OwnPlatform
	ui.tableWidget_6->setRowCount(0);
	for (int i = 0; i < scenario.getAllOwnPlatform().size(); i++) {
		ui.tableWidget_6->insertRow(i);
		ui.tableWidget_6->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllOwnPlatform()[i]->getName())));
		ui.tableWidget_6->setCellWidget(i, 1, new QComboBox());
		ui.tableWidget_6->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxAcceleration())));
		ui.tableWidget_6->setItem(i, 3, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxDeceleration())));
		ui.tableWidget_6->setItem(i, 4, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxClimbRate())));
		ui.tableWidget_6->setItem(i, 5, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxDiveRate())));
		ui.tableWidget_6->setItem(i, 6, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxSpeed())));
		ui.tableWidget_6->setItem(i, 7, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxTurnRadius())));
		ui.tableWidget_6->setCellWidget(i, 8, new QPushButton());
		auto cellWidget = ui.tableWidget_6->cellWidget(i, 1);
		auto cellWidget_2 = ui.tableWidget_6->cellWidget(i, 8);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QStringList alist;
		alist << "Air";
		combox->addItems(alist);
		QPushButton *btn = qobject_cast<QPushButton*>(cellWidget_2);
		btn->setText("View");
		connect(btn, SIGNAL(clicked()), this, SLOT(show_mission()));
	}
	emit show_data_page2();
	emit show_data_page3();
}
void RoutePlannGui::save_emitter() {
	int num = ui.tableWidget_4->currentRow();
	if (num + 1 > scenario.getAllEmitter().size()) {
		Emitter new_data(ui.tableWidget_4->item(num, 0)->text().toStdString());
		scenario.addEmitter(make_shared<sce::Emitter>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Emitter successfully"));

		QDomElement root = dom.documentElement();
		QDomElement ver = dom.createElement("Emitter");
		QDomElement third_1 = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(ui.tableWidget_4->item(num, 0)->text());
		third_1.appendChild(text1);
		ver.appendChild(third_1);
		root.appendChild(ver);
	}
	else {
		scenario.getAllEmitter()[num]->setName(ui.tableWidget_4->item(num, 0)->text().toStdString());
		QMessageBox::about(this, tr("Tip"), tr("Save Emitter successfully"));
		QDomNodeList list = dom.elementsByTagName("Emitter");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					ele.firstChild().firstChild().setNodeValue(ui.tableWidget_4->item(num, 0)->text());
					break;
				}
				flag++;
			}
		}
	}

}
void RoutePlannGui::save_to_file() {
	if (!file.open(QFile::WriteOnly | QFile::Truncate)) //先读进来，再重写，如果不用truncate就是在后面追加内容，就无效了
		return;
	QTextStream out_stream(&file);
	dom.save(out_stream, 4); //缩进4格
	file.close();
	QMessageBox::about(this, tr("Tip"), tr("Save to file successfully"));
}

void RoutePlannGui::setOpenFileFlag()
{
	open_flag += 1;
}
void RoutePlannGui::save_vertex() {
	int num = ui.tableWidget->currentRow();
	if (num + 1 > scenario.getAllVertex().size()) {
		QString a = ui.tableWidget->item(num, 0)->text();
		QString b = ui.tableWidget->item(num, 1)->text();
		sce::Vertex new_data(a.toDouble(), b.toDouble());
		scenario.addVertex(make_shared<sce::Vertex>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Vertex successfully"));
		QDomElement root = dom.documentElement();
		QDomElement ver = dom.createElement("Vertex");
		QDomElement la = dom.createElement("Latitude");
		QDomElement lo = dom.createElement("Longitude");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		la.appendChild(text1);
		lo.appendChild(text2);
		ver.appendChild(la);
		ver.appendChild(lo);
		root.appendChild(ver);
	}
	else {
		QString a = ui.tableWidget->item(num, 0)->text();
		QString b = ui.tableWidget->item(num, 1)->text();
		scenario.getAllVertex()[num]->setLatitude(a.toDouble());
		scenario.getAllVertex()[num]->setLongitude(b.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save Vertex successfully"));
		QDomNodeList list = dom.elementsByTagName("Vertex");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(a);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(b);
	}
}
void RoutePlannGui::save_platform() {
	int num = ui.tableWidget_2->currentRow();
	QString a = ui.tableWidget_2->item(num, 0)->text();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_2->cellWidget(num, 1));
	int re = cell->currentIndex();
	if (num + 1 > scenario.getAllPlatform().size()) {
		sce::Platform new_data(a.toStdString(), sce::PlatformType(re));
		//new_data.setPlatformName(a.toStdString());
		//new_data.setPlatformType(sce::PlatformType(re));
		scenario.addPlatform(make_shared<sce::Platform>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Platform successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Platform");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Type");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(platformtype_list[re]);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		second.appendChild(third_1);
		second.appendChild(third_2);
		root.appendChild(second);
	}
	else {
		scenario.getAllPlatform()[num]->setPlatformName(a.toStdString());
		scenario.getAllPlatform()[num]->setPlatformType(sce::PlatformType(re));
		QMessageBox::about(this, tr("Tip"), tr("Save Platform successfully"));
		QDomNodeList list = dom.elementsByTagName("Platform");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(a);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(platformtype_list[re]);
	}
}
void RoutePlannGui::save_weapon() {
	int num = ui.tableWidget_3->currentRow();
	QString a = ui.tableWidget_3->item(num, 0)->text();
	QString b = ui.tableWidget_3->item(num, 1)->text();
	QString c = ui.tableWidget_3->item(num, 2)->text();
	if (num + 1 > scenario.getAllWeapon().size()) {
		sce::Weapon new_data(a.toStdString(), b.toDouble(), c.toDouble());
		scenario.addWeapon(make_shared<sce::Weapon>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Weapon successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Weapon");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("CEPR");
		QDomElement third_3 = dom.createElement("WEAPONAREACOVERAGE");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		root.appendChild(second);
	}
	else {
		scenario.getAllWeapon()[num]->setName(a.toStdString());
		scenario.getAllWeapon()[num]->setCEPR(b.toInt());
		scenario.getAllWeapon()[num]->setWeaponAreaCoverage(c.toInt());
		QMessageBox::about(this, tr("Tip"), tr("Save Weapon successfully"));
		QDomNodeList list = dom.elementsByTagName("Weapon");
		int flag_weapon = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement e = list.at(i).toElement();
			if (e.parentNode().nodeName() == "Scenario")
			{
				if (flag_weapon == num)
				{
					e.firstChild().firstChild().setNodeValue(a);
					e.firstChild().nextSibling().firstChild().setNodeValue(b);
					e.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(c);
					break;
				}
				flag_weapon++;
			}
		}
	}
}

void RoutePlannGui::save_site() {
	int num = ui.tableWidget_5->currentRow();
	QString a = ui.tableWidget_5->item(num, 0)->text();
	QString b = ui.tableWidget_5->item(num, 1)->text();
	QString c = ui.tableWidget_5->item(num, 2)->text();
	QString d = ui.tableWidget_5->item(num, 3)->text();
	if (num + 1 > scenario.getAllSite().size()) {
		sce::Site new_data(a.toStdString(), b.toDouble(), c.toDouble(), d.toDouble());
		scenario.addSite(make_shared<sce::Site>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Site successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Site");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Altitude");
		QDomElement third_3 = dom.createElement("Latitude");
		QDomElement third_4 = dom.createElement("Longitude");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		QDomText text4 = dom.createTextNode(d);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		second.appendChild(third_4);
		root.appendChild(second);
	}
	else {
		scenario.getAllSite()[num]->setName(a.toStdString());
		scenario.getAllSite()[num]->setAltitude(b.toDouble());
		scenario.getAllSite()[num]->setLatitude(c.toDouble());
		scenario.getAllSite()[num]->setLongitude(d.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save Site successfully"));
		QDomNodeList list = dom.elementsByTagName("Site");
		int flag_site = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement e = list.at(i).toElement();
			if (e.parentNode().nodeName() == "Scenario")
			{
				if (flag_site == num)
				{
					e.firstChild().firstChild().setNodeValue(a);
					e.firstChild().nextSibling().firstChild().setNodeValue(b);
					e.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(c);
					e.firstChild().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(d);
					break;
				}
				flag_site++;
			}
		}
	}
}
void RoutePlannGui::save_ownplatform() {
	int num = ui.tableWidget_6->currentRow();
	QString a = ui.tableWidget_6->item(num, 0)->text();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_6->cellWidget(num, 1));
	int re = cell->currentIndex();
	QString b = ui.tableWidget_6->item(num, 2)->text();
	QString c = ui.tableWidget_6->item(num, 3)->text();
	QString d = ui.tableWidget_6->item(num, 4)->text();
	QString e = ui.tableWidget_6->item(num, 5)->text();
	QString f = ui.tableWidget_6->item(num, 6)->text();
	QString g = ui.tableWidget_6->item(num, 7)->text();
	if (num + 1 > scenario.getAllOwnPlatform().size())
	{
		sce::OwnPlatform new_data(a.toStdString(), OwnPlatformType(re), b.toDouble(), c.toDouble(), d.toDouble(), e.toDouble(), f.toDouble(), g.toDouble());
		scenario.addOwnPlatform(make_shared<sce::OwnPlatform>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add OwnPlatform successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("OwnPlatform");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Type");
		QDomElement third_3 = dom.createElement("MaxAcceleration");
		QDomElement third_4 = dom.createElement("MaxDeceleration");
		QDomElement third_5 = dom.createElement("MaxClimbRate");
		QDomElement third_6 = dom.createElement("MaxDiveRate");
		QDomElement third_7 = dom.createElement("MaxSpeed");
		QDomElement third_8 = dom.createElement("MaxTurnRadius");
		QDomElement third_9 = dom.createElement("Mission");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(ownplatformtype_list[0]);
		QDomText text3 = dom.createTextNode(b);
		QDomText text4 = dom.createTextNode(c);
		QDomText text5 = dom.createTextNode(d);
		QDomText text6 = dom.createTextNode(e);
		QDomText text7 = dom.createTextNode(f);
		QDomText text8 = dom.createTextNode(g);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		third_5.appendChild(text5);
		third_6.appendChild(text6);
		third_7.appendChild(text7);
		third_8.appendChild(text8);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		second.appendChild(third_4);
		second.appendChild(third_5);
		second.appendChild(third_6);
		second.appendChild(third_7);
		second.appendChild(third_8);
		second.appendChild(third_9);
		root.appendChild(second);
	}
	else {
		scenario.getAllOwnPlatform()[num]->setName(a.toStdString());
		scenario.getAllOwnPlatform()[num]->setType(OwnPlatformType(re));
		scenario.getAllOwnPlatform()[num]->setMaxAcceleration(b.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxDeceleration(c.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxClimbRate(d.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxDiveRate(e.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxSpeed(f.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxTurnRadius(g.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save OwnPlatform successfully"));
		QDomNodeList list = dom.elementsByTagName("OwnPlatform");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					ele.firstChild().firstChild().setNodeValue(a);
					ele.firstChild().nextSibling().firstChild().setNodeValue(ownplatformtype_list[0]);
					ele.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(b);
					ele.firstChild().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(c);
					ele.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(d);
					ele.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(e);
					ele.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(f);
					ele.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(g);
					break;
				}
				flag++;
			}
		}
	}

}
void RoutePlannGui::add_ownplatform() {
	int num = ui.tableWidget_6->rowCount();
	ui.tableWidget_6->insertRow(num);
	QPushButton *btn = new QPushButton();
	btn->setText("veiw");
	ui.tableWidget_6->setCellWidget(num, 1, new QComboBox());
	auto cell = ui.tableWidget_6->cellWidget(num, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cell);
	QStringList list;
	list << "Air";
	combox->addItems(list);
	ui.tableWidget_6->setCellWidget(num, 8, btn);
	connect(btn, SIGNAL(clicked()), this, SLOT(show_mission()));
}
void RoutePlannGui::added_mission() {
	emit sign_added_mission();
}
void RoutePlannGui::del_ownplatform()
{
	int num = ui.tableWidget_6->currentRow();
	int ree = QMessageBox::information(this, "Tip", "Confirm delrtion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_6->removeRow(num);
		if (num < scenario.getAllOwnPlatform().size())
		{
			scenario.deleteOwnPlatform(num);
			QMessageBox::about(this, "Tip", "Delete OwnPlatform successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatform");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void RoutePlannGui::add_emitter() {
	int row_count = ui.tableWidget_4->rowCount(); //获取表单行数
	ui.tableWidget_4->insertRow(row_count);//添加新的一行
	QPushButton *but = new QPushButton();
	but->setText("View");
	ui.tableWidget_4->setCellWidget(row_count, 1, but);
	connect(but, SIGNAL(clicked()), this, SLOT(show_rada()));
}
void RoutePlannGui::del_emitter()		//删除列表数据
{
	int num = ui.tableWidget_4->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_4->removeRow(num);
		if (num < scenario.getAllEmitter().size()) {
			scenario.deleteEmitter(num);
			QMessageBox::about(this, "Tip", "Delete Emitter successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Emitter");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void RoutePlannGui::show_rada() {
	choice_emitter = ui.tableWidget_4->currentRow();
	cout << "choice_emitter->" << choice_emitter << endl;
	emit rada_mode();
}

void RoutePlannGui::add_rada_mode() {
	emit sign_add_rada();
}

void RoutePlannGui::on_open()
{
	QString fileName;
	fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), "", tr("格式(*.xml)"));
	file.setFileName(fileName);
	/*delete scenario;
	Scenario* te =new Scenario();
	scenario = te;*/
	//memset(&scenario, 0, sizeof(scenario));
	if (!file.open(QIODevice::ReadWrite))
	{
		return;
	}
	//QDomDocument dom;
	if (!dom.setContent(&file))
	{
		QMessageBox::about(this, tr("提示"), tr("文件打开失败！"));
		file.close();
		return;
	}
	file.close();
	//再次打开xml文件需要清空之前实例化的scenario对象，释放Vector容器所占用的内存
	if(open_flag!=0)
	{
		std::vector<std::shared_ptr<sce::Platform>> platforms_temp;
		std::vector<std::shared_ptr<sce::OwnPlatform>> ownplatforms_temp;
		std::vector<std::shared_ptr<sce::Esm>> esms_temp;
		std::vector<std::shared_ptr<sce::Ecm>> ecms_temp;
		std::vector<std::shared_ptr<sce::Vertex>> vertexs_temp;
		std::vector<std::shared_ptr<sce::Weapon>> weapons_temp;
		std::vector<std::shared_ptr<sce::Site>> sites_temp;
		std::vector<std::shared_ptr<sce::Emitter>> emitters_temp;
		std::vector<std::shared_ptr<sce::Route>> routes_temp;
		std::vector<std::shared_ptr<sce::EsmStrategy>> esmstrategys_temp;
		std::vector<std::shared_ptr<sce::EcmStrategy>> ecmstrategys_temp;
		std::vector<sce::PlatformSiteRelation> PSRs_temp;
		std::vector<sce::PlatformEmitterRelation> PERs_temp;
		std::vector<sce::PlatformWeaponRelation> PWRs_temp;
		std::vector<sce::OwnPlatformEsmRelation> OPEsmRs_temp;
		std::vector<sce::EsmEsmStrategyRelation> EsmESRs_temp;
		std::vector<sce::OwnPlatformEcmRelation> OPEcmRs_temp;
		std::vector<sce::EcmEcmStrategyRelation> EcmESRs_temp;
		std::vector<sce::OwnPlatformRouteRelation> OPRRs_temp;

		scenario.getAllPlatform().swap(platforms_temp);
		scenario.getAllOwnPlatform().swap(ownplatforms_temp);
		scenario.getAllEsm().swap(esms_temp);
		scenario.getAllEcm().swap(ecms_temp);
		scenario.getAllVertex().swap(vertexs_temp);
		scenario.getAllWeapon().swap(weapons_temp);
		scenario.getAllSite().swap(sites_temp);
		scenario.getAllEmitter().swap(emitters_temp);
		scenario.getAllRoute().swap(routes_temp);
		scenario.getAllEsmStrategy().swap(esmstrategys_temp);
		scenario.getAllEcmStrategy().swap(ecmstrategys_temp);
		scenario.getAllPlatformSiteRelation().swap(PSRs_temp);
		scenario.getAllPlatformEmitterRelation().swap(PERs_temp);
		scenario.getAllPlatformWeaponRelation().swap(PWRs_temp);
		scenario.getAllOwnPlatformEsmRelation().swap(OPEsmRs_temp);
		scenario.getAllEsmEsmStrategyRelation().swap(EsmESRs_temp);
		scenario.getAllOwnPlatformEcmRelation().swap(OPEcmRs_temp);
		scenario.getAllEcmEcmStrategyRelation().swap(EcmESRs_temp);
		scenario.getAllOwnPlatformRouteRelation().swap(OPRRs_temp);
	}

	QMessageBox::about(this, tr("Tip"), tr("File opened successfully"));
	QDomElement docelem = dom.documentElement();
	listDom(docelem);

	emit sign_show_xml_data();

	//释放Vector容器所占用的内存
	std::vector<std::shared_ptr<sce::Route>>().swap(routes);
	std::vector<std::shared_ptr<sce::EsmStrategy>>().swap(esmstrategys);
	std::vector<std::shared_ptr<sce::EcmStrategy>>().swap(ecmstrategys);
	std::vector<sce::PlatformSiteRelation>().swap(PSRs);
	std::vector<sce::PlatformEmitterRelation>().swap(PERs);
	std::vector<sce::PlatformWeaponRelation>().swap(PWRs);
	std::vector<sce::OwnPlatformEsmRelation>().swap(OPEsmRs);
	std::vector<sce::EsmEsmStrategyRelation>().swap(EsmESRs);
	std::vector<sce::OwnPlatformEcmRelation>().swap(OPEcmRs);
	std::vector<sce::EcmEcmStrategyRelation>().swap(EcmESRs);
	std::vector<sce::OwnPlatformRouteRelation>().swap(OPRRs);
}

void RoutePlannGui::listDom(QDomElement &docelem)
{
	bool f_vertex(false), f_platform(false), f_emitter(false), f_esm(false), f_ecm(false), f_op(false),f_route(false), f_site(false), f_weapon(false), f_esms(false), f_ecms(false);
	bool f_psr(false), f_per(false), f_pwr(false), f_oesm(false), f_oecm(false), f_esmesms(false), f_ecmecms(false), f_orr(false);
	QDomNode node = docelem.firstChild();//子节点
	if (node.isNull())
	{
		return;
	}
	else
	{
		while (!node.toElement().isNull())
		{
			QDomElement element = node.toElement();//Converts a QDomNode into a QDomElement
			if (!element.isNull())
			{
				if (find_Vertex_data(element, scenario)) f_vertex = true;
				else if (find_Platform_data(element, scenario)) f_platform = true;
				else if (find_Emitter_data(element, scenario)) f_emitter = true;
				else if (find_Esm_data(element, scenario)) f_esm = true;
				else if (find_Ecm_data(element, scenario)) f_ecm = true;
				else if (find_EsmStrategy_data(element, scenario)) f_esms = true;
				else if (find_EcmStrategy_data(element, scenario)) f_ecms = true;
				else if (find_OwnPlatform_data(element, scenario)) f_op = true;
				else if (find_Route_data(element, scenario)) f_route = true;
				else if (find_Site_data(element, scenario)) f_site = true;
				else if (find_Weapon_data(element, scenario)) f_weapon = true;
				else if (find_PlatformSiteRelation_data(element, scenario)) f_psr = true;
				else if (find_PlatformEmitterRelation_data(element, scenario)) f_per = true;
				else if (find_PlatformWeaponRelation_data(element, scenario)) f_pwr = true;
				else if (find_OwnPlatformEsmRelation_data(element, scenario)) f_oesm = true;
				else if (find_EsmEsmStrategyRelation_data(element, scenario)) f_esmesms = true;
				else if (find_OwnPlatformEcmRelation_data(element, scenario)) f_oecm = true;
				else if (find_EcmEcmStrategyRelation_data(element, scenario)) f_ecmecms = true;
				else if (find_OwnPlatformRouteRelation_data(element, scenario)) f_orr = true;

				//listDom(element);
			}
			node = node.nextSibling();//将兄弟元素赋给他
		}

		if (!f_vertex) qDebug() << "No Vertex data was found!";
		if (!f_platform) qDebug() << "No Platform data was found!";
		if (!f_emitter) qDebug() << "No Emitter data was found!";
		if (!f_esm) qDebug() << "No Esm data was found!";
		if (!f_ecm) qDebug() << "No Ecm data was found!";
		if (!f_esms) qDebug() << "No EsmStrategy data was found!";
		if (!f_ecms) qDebug() << "No EcmStrategy data was found!";
		if (!f_op) qDebug() << "No OwnPlatform data was found!";
		if (!f_route) qDebug() << "No Route data was found!";
		if (!f_site) qDebug() << "No Site data was found!";
		if (!f_weapon) qDebug() << "No Weapon data was found!";
		if (!f_psr) 	qDebug() << "No PlatformSiteRelation data was found!";
		if (!f_per) 	qDebug() << "No PlatformEmitterRelation data was found!";
		if (!f_pwr) 	qDebug() << "No PlatformWeaponRelation data was found!";
		if (!f_oesm) qDebug() << "No OwnPlatformEsmRelation data was found!";
		if (!f_esmesms) qDebug() << "No EsmEsmStrategyRelation data was found!";
		if (!f_oecm) qDebug() << "No OwnPlatformEcmRelation data was found!";
		if (!f_ecmecms) qDebug() << "No EcmEcmStrategyRelation data was found!";
		if (!f_orr) qDebug() << "No OwnPlatformRouteRelation data was found!";
	}
}

void RoutePlannGui::append_Vertex()
{
	int row_count = ui.tableWidget->rowCount(); //获取表单行数
	ui.tableWidget->insertRow(row_count);//添加新的一行

}
void RoutePlannGui::del_vertex()		//删除列表数据
{
	int num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else
	{
		ui.tableWidget->removeRow(num);
		if (num < scenario.getAllVertex().size())
		{
			scenario.deleteVertex(num);
			QMessageBox::about(this, "Tip", "Delete Vertex successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Vertex");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}

}
void RoutePlannGui::append_platform()
{
	int row_count = ui.tableWidget_2->rowCount(); //获取表单行数
	ui.tableWidget_2->insertRow(row_count);//添加新的一行
	ui.tableWidget_2->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_2->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QStringList list;
	list << "Land" << "Air" << "Surface";
	combox->addItems(list);
}
void RoutePlannGui::del_platform()		//删除列表数据
{
	int num = ui.tableWidget_2->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else
	{
		ui.tableWidget_2->removeRow(num);
		if (num < scenario.getAllPlatform().size())
		{
			scenario.deletePlatform(num);
			QMessageBox::about(this, "Tip", "Delete Platform successfully");

			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Platform");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void RoutePlannGui::append_weapon()
{
	int row_count = ui.tableWidget_3->rowCount(); //获取表单行数
	ui.tableWidget_3->insertRow(row_count);//添加新的一行

}
void RoutePlannGui::del_weapon()		//删除列表数据
{
	int num = ui.tableWidget_3->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_3->removeRow(num);
		if (num < scenario.getAllWeapon().size()) {
			scenario.deleteWeapon(num);
			QMessageBox::about(this, "Tip", "Delete Weapon successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Weapon");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void RoutePlannGui::append_site()
{
	int row_count = ui.tableWidget_5->rowCount(); //获取表单行数
	ui.tableWidget_5->insertRow(row_count);//添加新的一行

}
void RoutePlannGui::del_site()		//删除列表数据
{
	int num = ui.tableWidget_5->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_5->removeRow(num);
		if (num < scenario.getAllSite().size()) {
			scenario.deleteSite(num);
			QMessageBox::about(this, "Tip", "Delete Site successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Site");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void RoutePlannGui::show_mission() {
	choice_ownplatform = ui.tableWidget_6->currentRow();
	qDebug() << "choice_ownplatform:" << choice_ownplatform;
	emit sign_mission();
}
void RoutePlannGui::on_next()
{
	this->hide();
	emit go_next();
}
void RoutePlannGui::show_page1()
{
	this->show();
}






#include "MainWindow.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include "Scenario.hpp"

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
	//connect(ui.pushButton_21, SIGNAL(clicked()), this, SLOT(show_xml_data()));
	connect(ui.actionOpen_file, SIGNAL(triggered()), this, SLOT(on_open()));
	connect(ui.actionShow_data, SIGNAL(triggered()), this, SLOT(show_xml_data()));
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
		sce::Platform new_data;
		new_data.setPlatformName(a.toStdString());
		new_data.setPlatformType(sce::PlatformType(re));
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
	memset(&scenario, 0, sizeof(scenario));
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
	QMessageBox::about(this, tr("Tip"), tr("File opened successfully"));
	QDomElement docelem = dom.documentElement();
	listDom(docelem);

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
				//qDebug() << "节点名称:" << element.tagName() << endl;
				//qDebug() << "节点数据:" << element.text() << endl;
				QString t = element.tagName();
				QString t1 = element.text();
				ParentNode = &element.parentNode().toElement();
				SibingNode = &ParentNode->firstChildElement().nextSibling().toElement();
				bind_str(t, t1);
				find_data(element);
				listDom(element);
			}
			node = node.nextSibling();//将兄弟元素赋给他
		}
	}
}
void RoutePlannGui::bind_str(QString t, QString t1)
{
	QString a = ParentNode->tagName();
	if (t == "Latitude" && ParentNode->tagName() == "Vertex")
	{
		Vertex v1(t1.toDouble(), SibingNode->text().toDouble());
		auto r = make_shared<Vertex>(v1);
		scenario.addVertex(r);
	}
	if (t == "Name" && ParentNode->tagName() == "Platform")
	{
		if (SibingNode->text() == "Surface")
		{
			Platform p1(t1.toStdString(), PlatformType::SURFACE);
			auto r = make_shared<Platform>(p1);
			scenario.addPlatform(r);
		}
		if (SibingNode->text() == "Air")
		{
			Platform p1(t1.toStdString(), PlatformType::AIR);
			auto r = make_shared<Platform>(p1);
			scenario.addPlatform(r);
		}
		if (SibingNode->text() == "Land")
		{
			Platform p1(t1.toStdString(), PlatformType::LAND);
			auto r = make_shared<Platform>(p1);
			scenario.addPlatform(r);
		}

	}

	if (t == "Name" &&  ParentNode->tagName() == "Emitter")
	{
		Emitter e1(t1.toStdString());
		auto r = make_shared<Emitter>(e1);
		scenario.addEmitter(r);
	}
	if (t == "ModeCode" &&  ParentNode->tagName() == "Radar_Mode")
	{

		Rf mrf(1, 1, RfType::BURST);
		Pw mpw(1, 1, PwType::FIX);
		Pri mpri;
		Scan mscan;
		Erp merp;

		Radar_Mode r1("1", ModeType::PULSE, mrf, mpw, mpri, mscan, merp);
		auto r = make_shared<Radar_Mode>(r1);
		r->setModeCode(t1.toStdString());
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		current_emitter->addRadarMode(r);

	}
	if (t == "ModeType" &&  ParentNode->tagName() == "Radar_Mode")
	{
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);
		if (t1 == "Pulse")
			current_radar->setModeType(ModeType::PULSE);
	}
	if (t == "Rfmin" && ParentNode->tagName() == "Rf")
	{
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);
		Rf rf(t1.toInt(), SibingNode->text().toInt());

		//rf.setRfMin(t1.toInt());
		//rf.setRfMax(SibingNode->text().toInt());
		if (SibingNode->nextSiblingElement().text() == "Burst")
			rf.setRfType(RfType::BURST);
		else
			rf.setRfType(RfType::FIX);
		//修改rf的函数放在最后，避免rf修改完radar里并不改变
		current_radar->setRf(rf);


	}
	if (t == "PhaseOrder" && ParentNode->tagName() == "Rf_Values")
	{
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);
		auto my_rf = current_radar->getRf();
		int PhaseOrder = t1.toInt();
		int NumberOfPulses = SibingNode->text().toInt();
		int Rfmin = SibingNode->nextSiblingElement().text().toInt();
		int Rfmax = SibingNode->nextSiblingElement().nextSiblingElement().text().toInt();
		int SweepTime = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toInt();

		Rf_values rf_val(PhaseOrder, NumberOfPulses, Rfmin, Rfmax, SweepTime);
		my_rf.addRfValues(rf_val);
		current_radar->setRf(my_rf);
	}

	if (t == "Pwmin"  && ParentNode->tagName() == "Pw")
	{
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);

		Pw pw(t1.toInt(), SibingNode->text().toInt());
		//int Pwmin = t1.toInt();
		//int Pwmax = SibingNode->text().toInt();
		if (SibingNode->nextSiblingElement().text() == "Fix")
			pw.setPwType(PwType::FIX);

		//pw.setPwMin(t1.toInt());
		//pw.setPwMax(Pwmax);
		current_radar->setPw(pw);
	}
	if (t == "PhaseOrder" && ParentNode->tagName() == "Pw_Values")
	{
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);
		auto my_pw = current_radar->getPw();

		int PhaseOrder = t1.toInt();
		int NumberOfPulses = SibingNode->text().toInt();
		int Pwmin = SibingNode->nextSiblingElement().text().toInt();
		int Pwmax = SibingNode->nextSiblingElement().nextSiblingElement().text().toInt();
		int SweepTime = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toInt();
		Pw_values pval(PhaseOrder, NumberOfPulses, Pwmin, Pwmax, SweepTime);
		//把privalue添加到pw，并且修改radar里面pw的值
		my_pw.addPriValues(pval);
		current_radar->setPw(my_pw);

	}
	if (t == "Primin" &&  ParentNode->tagName() == "Pri")
	{
		Pri mpri;
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);


		int Primin = t1.toInt();
		int Primax = SibingNode->text().toInt();
		if (SibingNode->nextSiblingElement().text() == "Fix")
			mpri.setPriType(PriType::FIX);
		if (SibingNode->nextSiblingElement().text() == "Stagger")
			mpri.setPriType(PriType::STAGGER);
		if (SibingNode->nextSiblingElement().text() == "Switching")
			mpri.setPriType(PriType::SWITCHING);
		mpri.setPriMin(Primin);
		mpri.setPriMax(Primax);
		current_radar->setPri(mpri);
	}
	if (t == "PhaseOrder" &&ParentNode->tagName() == "Pri_Values")
	{
		//找到当前的radar
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);
		//获取当前的pri
		auto m_pri = current_radar->getPri();
		//获取pri val各个属性
		int PhaseOrder = t1.toInt();
		int NumberOfPulses = SibingNode->text().toInt();
		int Primin = SibingNode->nextSiblingElement().text().toInt();
		int Primax = SibingNode->nextSiblingElement().nextSiblingElement().text().toInt();
		int SweepTime = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toInt();
		//把prival 添加到pri
		Pri_values prival(PhaseOrder, NumberOfPulses, Primin, Primax, SweepTime);
		m_pri.addPriValues(prival);
		current_radar->setPri(m_pri);

	}
	if (t == "ScanType")
	{
		Scan mscan;
		//找到当前的radar
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);
		//判断类型
		if (t1 == "Sectorial")
			mscan.setScanType(ScanType::SECTORIAL);
		//获取数据
		int Spmax = SibingNode->text().toInt();
		int Spmin = SibingNode->nextSiblingElement().text().toInt();
		//改变数据
		mscan.setScanMax(Spmax);
		mscan.setScanMin(Spmin);
		//添加scan到radar
		current_radar->setScan(mscan);
	}
	if (t == "Erpmin" && ParentNode->tagName() == "Erp")
	{
		Erp merp;
		//找到当前的radar
		auto current_emitter = scenario.getAllEmitter().at(scenario.getAllEmitter().size() - 1);
		auto current_radar = current_emitter->getAllPtr2RadarModes().at(current_emitter->getAllPtr2RadarModes().size() - 1);

		//获取数据
		int Erpmin = t1.toInt();
		int Erpmax = SibingNode->text().toInt();

		//改变数据
		merp.setErpMax(Erpmax);
		merp.setErpMin(Erpmin);
		//添加scan到radar
		current_radar->setErp(merp);
	}
	if (t == "Name" && ParentNode->tagName() == "Weapon")
	{
		string Name = t1.toStdString();
		int CEPR = SibingNode->text().toInt();
		int WEAPONAREACOVERAGE = SibingNode->nextSiblingElement().text().toInt();
		Weapon w1(Name, CEPR, WEAPONAREACOVERAGE);
		auto r = make_shared<Weapon>(w1);
		scenario.addWeapon(r);
	}
	if (t == "Name" && ParentNode->tagName() == "Site")
	{
		//获取属性值
		string Name = t1.toStdString();
		double Altitude = SibingNode->text().toDouble();
		double Latitude = SibingNode->nextSiblingElement().text().toDouble();
		double Longitude = SibingNode->nextSiblingElement().nextSiblingElement().text().toDouble();
		//构造对象，并添加到scenario
		Site s1(Name, Altitude, Latitude, Longitude);
		auto r = make_shared<Site>(s1);
		scenario.addSite(r);


	}
	if (t == "Name" && ParentNode->tagName() == "OwnPlatform")
	{


		//构造任意对象mp;
		Point p1(1, 1, 1);
		Mission m1(MissionType::STRIKE, p1, p1);
		OwnPlatform mp("1", OwnPlatformType::AIR, 1, 1, 1, 1, 1, 1, m1);
		mp.setName(t1.toStdString());

		if (SibingNode->text() == "Air")
			mp.setType(OwnPlatformType::AIR);
		//依次获取各个属性值
		double MaxAcceleration = SibingNode->nextSiblingElement().text().toDouble();
		double MaxDeceleration = SibingNode->nextSiblingElement().nextSiblingElement().text().toDouble();
		double MaxClimbRate = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		double MaxDiveRate = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		double MaxSpeed = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		double MaxTurnRadius = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();;
		mp.setMaxAcceleration(MaxAcceleration);
		mp.setMaxClimbRate(MaxClimbRate);
		mp.setMaxDeceleration(MaxDeceleration);
		mp.setMaxDiveRate(MaxDiveRate);
		mp.setMaxSpeed(MaxSpeed);
		mp.setMaxTurnRadius(MaxTurnRadius);
		//添加OwnPlatform到scenario对象
		auto r = make_shared<OwnPlatform>(mp);
		scenario.addOwnPlatform(r);


	}
	if (t == "Type" && ParentNode->tagName() == "Mission")
	{
		//获取当前的OwnPlatform对象
		auto m_plat = scenario.getAllOwnPlatform().at(scenario.getAllOwnPlatform().size() - 1);

		//构造mission对象
		Point p1(1, 1, 1);
		Mission m1(MissionType::STRIKE, p1, p1);

		if (t1 == "Strike")
			m1.setMissionType(MissionType::STRIKE);
		if (t1 == "Support")
			m1.setMissionType(MissionType::SUPPORT);
		//delete first target point
		unsigned int pos = 0;
		if (!m1.getAllTargetPoints().empty())
			m1.deleteTargetPoint(pos);

		m_plat->setMission(m1);

	}
	if (t == "Altitude" && ParentNode->tagName() == "StartPoint")
	{
		//获取当前的mission对象
		auto m_plat = scenario.getAllOwnPlatform().at(scenario.getAllOwnPlatform().size() - 1);
		auto m_mission = m_plat->getMission();
		//修改各个属性
		double Altitude = t1.toDouble();
		double Latitude = SibingNode->text().toDouble();
		double Longitude = SibingNode->nextSiblingElement().text().toDouble();
		double Tmin = SibingNode->nextSiblingElement().nextSiblingElement().text().toDouble();
		double Tmax = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		//修改当前的startpoint,和mission
		Point sp(Altitude, Latitude, Longitude, Tmin, Tmax);
		m_mission.setStartPoint(sp);
		m_plat->setMission(m_mission);
	}
	if (t == "Altitude" && ParentNode->tagName() == "EndPoint")
	{
		//获取当前的mission对象
		auto m_plat = scenario.getAllOwnPlatform().at(scenario.getAllOwnPlatform().size() - 1);
		auto m_mission = m_plat->getMission();
		//修改各个属性
		double Altitude = t1.toDouble();
		double Latitude = SibingNode->text().toDouble();
		double Longitude = SibingNode->nextSiblingElement().text().toDouble();
		double Tmin = SibingNode->nextSiblingElement().nextSiblingElement().text().toDouble();
		double Tmax = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		//修改当前的startpoint
		Point ep(Altitude, Latitude, Longitude, Tmin, Tmax);
		m_mission.setEndPoint(ep);
		m_plat->setMission(m_mission);

	}
	if (t == "Altitude" && ParentNode->tagName() == "TargetPoint")
	{
		//获取当前的mission对象
		auto m_plat = scenario.getAllOwnPlatform().at(scenario.getAllOwnPlatform().size() - 1);
		auto m_mission = m_plat->getMission();
		//修改各个属性
		double Altitude = t1.toDouble();
		double Latitude = SibingNode->text().toDouble();
		double Longitude = SibingNode->nextSiblingElement().text().toDouble();
		double Tmin = SibingNode->nextSiblingElement().nextSiblingElement().text().toDouble();
		double Tmax = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		//添加targetpoint
		Point tp(Altitude, Latitude, Longitude, Tmin, Tmax);
		m_mission.addTargetPoint(tp);
		m_plat->setMission(m_mission);
	}
	if (t == "Name" && ParentNode->tagName() == "Esm")
	{
		string Name = t1.toStdString();
		double Dwell_Freq_Resolution = SibingNode->text().toDouble();
		double Tuning_Step = SibingNode->nextSiblingElement().text().toDouble();
		double RfCovmin = SibingNode->nextSiblingElement().nextSiblingElement().text().toDouble();
		double RfCovmax = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		double Num_Pulses_Acquisition = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		double Num_Pulses_Alarm = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		Esm mesm(Name, Dwell_Freq_Resolution, Tuning_Step, RfCovmin, RfCovmax, Num_Pulses_Acquisition, Num_Pulses_Alarm);
		auto r = make_shared<Esm>(mesm);
		scenario.addEsm(r);
	}
	if (t == "Name" && ParentNode->tagName() == "Ecm")
	{
		//设置ecm属性
		string name = t1.toStdString();
		unsigned int Pt = SibingNode->text().toDouble();
		unsigned int Gain = SibingNode->nextSiblingElement().text().toDouble();
		unsigned int Rfmin = SibingNode->nextSiblingElement().nextSiblingElement().text().toDouble();
		unsigned int Rfmax = SibingNode->nextSiblingElement().nextSiblingElement().nextSiblingElement().text().toDouble();
		Ecm e1(name, Pt, Gain, Rfmin, Rfmax, Tech::FT);
		//delete first techtype
		if (!e1.getAllTechs().empty())
			e1.deleteTech(0);
		//添加ecm到scenario
		auto r = make_shared<Ecm>(e1);

		scenario.addEcm(r);
	}
	//避免后面有重的tech，用祖父判断
	if (t == "Name" && ParentNode->tagName() == "tech" && ParentNode->parentNode().toElement().tagName() == "Ecm")
	{
		//获取当前的ecm
		auto current_ecm = scenario.getAllEcm().at(scenario.getAllEcm().size() - 1);
		if (t1 == "Noise")
			current_ecm->addTech(Tech::NOISE);
		if (t1 == "FT")
			current_ecm->addTech(Tech::FT);
		if (t1 == "RGPO")
			current_ecm->addTech(Tech::RGPO);
		if (t1 == "VGPO")
			current_ecm->addTech(Tech::VGPO);
		//此时的ecm内容发生变化所以要修改当前的ecm,即最后一个ecm
		scenario.setPtr2Ecm(scenario.getAllEcm().size() - 1, current_ecm);

	}
}

void RoutePlannGui::find_data(QDomElement qd)
{
	/*std::vector<std::shared_ptr<sce::Route>> routes;
	std::vector<std::shared_ptr<sce::EsmStrategy>> esmstrategys;
	std::vector<std::shared_ptr<sce::EcmStrategy>> ecmstrategys;
	std::vector<sce::PlatformSiteRelation> PSRs;
	std::vector<sce::PlatformEmitterRelation> PERs;
	std::vector<sce::PlatformWeaponRelation> PWRs;
	std::vector<sce::OwnPlatformEsmRelation> OPEsmRs;
	std::vector<sce::EsmEsmStrategyRelation> EsmESRs;
	std::vector<sce::OwnPlatformEcmRelation> OPEcmRs;
	std::vector<sce::EcmEcmStrategyRelation> EcmESRs;
	std::vector<sce::OwnPlatformRouteRelation> OPRRs;*/
	//find the data of "Route"
	if ((qd.tagName() == "Route") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::Route rot;
		std::vector<sce::WayPoint> wpoint;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name")
			{
				rot.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "WayPoints")
			{
				sce::WayPoint point;
				for (QDomNode qds_son = qdd.firstChild(); !qds_son.isNull(); qds_son = qds_son.nextSibling())
				{
					if (qds_son.nodeName() == "Index")
					{
						point.setIndex(qds_son.toElement().text().toInt());
					}
					if (qds_son.nodeName() == "Altitude")
					{
						point.setAltitude(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Latitude")
					{
						point.setLatitude(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Longitude")
					{
						point.setLongitude(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Time")
					{
						point.setTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Velocity")
					{
						point.setVelocity(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "Acceleration")
					{
						point.setAcceleration(qds_son.toElement().text().toDouble());
					}
				}
				wpoint.push_back(point);
			}
			rot.setAllWayPoints(wpoint);
		}
		routes.push_back(make_shared<sce::Route>(rot));
	}
	scenario.setAllRoute(routes);
	//find the data of the "EsmStrategy"
	if ((qd.tagName() == "EsmStrategy") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::EsmStrategy es;
		std::vector<shared_ptr<sce::EsmStrategySection>> sections;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name") {
				es.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Section")
			{
				sce::EsmStrategySection sec;
				std::vector<sce::DwellSquence> dwellsquences;
				for (QDomNode qds_son = qdd.firstChild(); !qds_son.isNull(); qds_son = qds_son.nextSibling())
				{
					if (qds_son.nodeName() == "StartTime") {
						sec.setStartTime(qds_son.toElement().text().toDouble());
					}if (qds_son.nodeName() == "EndTime") {
						sec.setEndTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "StartLocation")
					{
						sce::Location loca_st;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling())
						{
							if (qdds_son.nodeName() == "Altitude") {
								loca_st.setAltitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Latitude") {
								loca_st.setLatitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Longitude") {
								loca_st.setLongitude(qdds_son.toElement().text().toDouble());
							}
						}
						sec.setStartLocation(loca_st);
					}
					if (qds_son.nodeName() == "EndLocation")
					{
						sce::Location loca_end;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling())
						{
							if (qdds_son.nodeName() == "Altitude") {
								loca_end.setAltitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Latitude") {
								loca_end.setLatitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Longitude") {
								loca_end.setLongitude(qdds_son.toElement().text().toDouble());
							}
						}
						sec.setEndLocation(loca_end);
					}
					if (qds_son.nodeName() == "DwellSquence")
					{
						sce::DwellSquence ds;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling())
						{
							if (qdds_son.nodeName() == "Index") {
								ds.setIndex(qdds_son.toElement().text().toInt());
							}if (qdds_son.nodeName() == "MinFreq") {
								ds.setMinFreq(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "MaxFreq") {
								ds.setMaxFreq(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "StartTime") {
								ds.setStartTime(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "EndTime") {
								ds.setEndTime(qdds_son.toElement().text().toDouble());
							}
						}
						dwellsquences.push_back(ds);
					}
				}
				sec.setDwellSquences(dwellsquences);
				sections.push_back(make_shared<sce::EsmStrategySection>(sec));
			}
			es.setAllPtr2Sections(sections);
		}
		esmstrategys.push_back(make_shared<sce::EsmStrategy>(es));
	}
	scenario.setAllEsmStrategy(esmstrategys);
	//find the data of "EcmStrategy"
	if ((qd.tagName() == "EcmStrategy") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::EcmStrategy ecs;
		std::vector<shared_ptr<sce::EcmStrategySection>> sections;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Name")
			{
				ecs.setName(qdd.toElement().text().toStdString());
			}
			if (qdd.nodeName() == "Section")
			{
				sce::EcmStrategySection sec;
				for (QDomNode qds_son = qdd.firstChild(); !qds_son.isNull(); qds_son = qds_son.nextSibling())
				{
					if (qds_son.nodeName() == "StartTime")
					{
						sec.setStartTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "EndTime")
					{
						sec.setEndTime(qds_son.toElement().text().toDouble());
					}
					if (qds_son.nodeName() == "StartLocation")
					{
						sce::Location loc;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling())
						{
							if (qdds_son.nodeName() == "Altitude")
							{
								loc.setAltitude(qdds_son.toElement().text().toDouble());
							}
							if (qdds_son.nodeName() == "Latitude")
							{
								loc.setLatitude(qdds_son.toElement().text().toDouble());
							}
							if (qdds_son.nodeName() == "Longitude")
							{
								loc.setLongitude(qdds_son.toElement().text().toDouble());
							}
						}
						sec.setStartLocation(loc);
					}if (qds_son.nodeName() == "EndLocation") {
						sce::Location loc;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling()) {
							if (qdds_son.nodeName() == "Altitude") {
								loc.setAltitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Latitude") {
								loc.setLatitude(qdds_son.toElement().text().toDouble());
							}if (qdds_son.nodeName() == "Longitude") {
								loc.setLongitude(qdds_son.toElement().text().toDouble());
							}
							sec.setEndLocation(loc);
						}
					}if (qds_son.nodeName() == "tech") {
						sce::Tech tec;
						for (QDomNode qdds_son = qds_son.firstChild(); !qdds_son.isNull(); qdds_son = qdds_son.nextSibling()) {
							if (qdds_son.toElement().text().toStdString() == "Noise") {
								tec = sce::Tech(0);
							}if (qdds_son.toElement().text().toStdString() == "VGPO") {
								tec = sce::Tech(1);
							}if (qdds_son.toElement().text().toStdString() == "FT") {
								tec = sce::Tech(2);
							}if (qdds_son.toElement().text().toStdString() == "RGPO") {
								tec = sce::Tech(3);
							}
							sec.setTechName(tec);
						}
					}
				}
				sections.push_back(make_shared<sce::EcmStrategySection>(sec));
			}
			ecs.setAllPtr2Sections(sections);
		}
		ecmstrategys.push_back(make_shared<sce::EcmStrategy>(ecs));
	}
	scenario.setAllEcmStrategy(ecmstrategys);
	//find the data of "PlatformSiteRelation"
	if ((qd.tagName() == "PlatformSiteRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::PlatformSiteRelation psr;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Platform")
			{
				for (int i = 0; i < scenario.getAllPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllPlatform()[i]->getPlatformName()) {
						psr.setPlatform(scenario.getAllPlatform()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "Site")
			{
				for (int i = 0; i < scenario.getAllSite().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllSite()[i]->getName()) {
						psr.setSite(scenario.getAllSite()[i]);
						break;
					}
				}
			}
		}
		PSRs.push_back(psr);
	}
	scenario.setAllPlatformSiteRelation(PSRs);
	//find the data of "PlatformEmitterRelation"
	if ((qd.tagName() == "PlatformEmitterRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::PlatformEmitterRelation per;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Platform")
			{
				for (int i = 0; i < scenario.getAllPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllPlatform()[i]->getPlatformName()) {
						per.setPlatform(scenario.getAllPlatform()[i]);
						break;
					}
				}
				/*sce::Platform pla;
				pla.setPlatformName(qdd.toElement().text().toStdString());
				per.setPlatform(make_shared<sce::Platform>(pla));*/
			}
			if (qdd.nodeName() == "Emitter")
			{
				for (int i = 0; i < scenario.getAllEmitter().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEmitter()[i]->getName()) {
						per.setEmitter(scenario.getAllEmitter()[i]);
						break;
					}
				}
				/*sce::Emitter emi;
				emi.setName(qdd.toElement().text().toStdString());
				per.setEmitter(make_shared<sce::Emitter>(emi));*/
			}
		}
		PERs.push_back(per);
	}
	scenario.setAllPlatformEmitterRelation(PERs);
	//find the data of "PlatformWeaponRelation"
	if ((qd.tagName() == "PlatformWeaponRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::PlatformWeaponRelation pwr;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Platform")
			{
				for (int i = 0; i < scenario.getAllPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllPlatform()[i]->getPlatformName()) {
						pwr.setPlatform(scenario.getAllPlatform()[i]);
						break;
					}
				}
				/*sce::Platform pla;
				pla.setPlatformName(qdd.toElement().text().toStdString());
				pwr.setPlatform(make_shared<sce::Platform>(pla));*/
			}
			if (qdd.nodeName() == "Weapon")
			{
				for (int i = 0; i < scenario.getAllWeapon().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllWeapon()[i]->getName()) {
						pwr.setWeapon(scenario.getAllWeapon()[i]);
						break;
					}
				}
				/*sce::Weapon wea;
				wea.setName(qdd.toElement().text().toStdString());
				pwr.setWeapon(make_shared<sce::Weapon>(wea));*/
			}
		}
		PWRs.push_back(pwr);
	}
	scenario.setAllPlatformWeaponRelation(PWRs);
	//find the data of "OwnPlatformEsmRelation"
	if ((qd.tagName() == "OwnPlatformEsmRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::OwnPlatformEsmRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "OwnPlatform")

			{
				for (int i = 0; i < scenario.getAllOwnPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllOwnPlatform()[i]->getName()) {
						temp.setOwnPlatform(scenario.getAllOwnPlatform()[i]);
						break;
					}
				}
				/*sce::OwnPlatform temp_1;
				temp_1.setName(qdd.toElement().text().toStdString());
				temp.setOwnPlatform(make_shared<sce::OwnPlatform>(temp_1));*/
			}
			if (qdd.nodeName() == "Esm")
			{
				for (int i = 0; i < scenario.getAllEsm().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEsm()[i]->getName()) {
						temp.setEsm(scenario.getAllEsm()[i]);
						break;
					}
				}
				/*sce::Esm temp_2;
				temp_2.setName(qdd.toElement().text().toStdString());
				temp.setEsm(make_shared<sce::Esm>(temp_2));*/
			}
		}
		OPEsmRs.push_back(temp);
	}
	scenario.setAllOwnPlatformEsmRelation(OPEsmRs);
	//find the data of "EsmEsmStrategyRelation"
	if ((qd.tagName() == "EsmEsmStrategyRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::EsmEsmStrategyRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Esm")
			{
				for (int i = 0; i < scenario.getAllEsm().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEsm()[i]->getName()) {
						temp.setEsm(scenario.getAllEsm()[i]);
						break;
					}
				}
				/*sce::Esm temp_1;
				temp_1.setName(qdd.toElement().text().toStdString());
				temp.setEsm(make_shared<sce::Esm>(temp_1));*/
			}
			if (qdd.nodeName() == "EsmStrategy")
			{
				for (int i = 0; i < scenario.getAllEsmStrategy().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEsmStrategy()[i]->getName()) {
						temp.setEsmStrategy(scenario.getAllEsmStrategy()[i]);
						break;
					}
				}
				/*sce::EsmStrategy temp_2;
				temp_2.setName(qdd.toElement().text().toStdString());
				temp.setEsmStrategy(make_shared<sce::EsmStrategy>(temp_2));*/
			}
		}
		EsmESRs.push_back(temp);
	}
	scenario.setAllEsmEsmStrategyRelation(EsmESRs);
	//find the data of "OwnPlatformEcmRelation"
	if ((qd.tagName() == "OwnPlatformEcmRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::OwnPlatformEcmRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "OwnPlatform")
			{
				for (int i = 0; i < scenario.getAllOwnPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllOwnPlatform()[i]->getName()) {
						temp.setOwnPlatform(scenario.getAllOwnPlatform()[i]);
						break;
					}
				}
				/*sce::OwnPlatform temp_1;
				temp_1.setName(qdd.toElement().text().toStdString());
				temp.setOwnPlatform(make_shared<sce::OwnPlatform>(temp_1));*/
			}
			if (qdd.nodeName() == "Ecm")
			{
				for (int i = 0; i < scenario.getAllEcm().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEcm()[i]->getName()) {
						temp.setEcm(scenario.getAllEcm()[i]);
						break;
					}
				}
				/*sce::Ecm temp_2;
				temp_2.setName(qdd.toElement().text().toStdString());
				temp.setEcm(make_shared<sce::Ecm>(temp_2));*/
			}
		}
		OPEcmRs.push_back(temp);
	}
	scenario.setAllOwnPlatformEcmRelation(OPEcmRs);
	//find the data of "EcmEcmStrategyRelation"
	if ((qd.tagName() == "EcmEcmStrategyRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::EcmEcmStrategyRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "Ecm")
			{
				for (int i = 0; i < scenario.getAllEcm().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEcm()[i]->getName()) {
						temp.setEcm(scenario.getAllEcm()[i]);
						break;
					}
				}
				/*sce::Ecm temp_1;
				temp_1.setName(qdd.toElement().text().toStdString());
				temp.setEcm(make_shared<sce::Ecm>(temp_1));*/
			}
			if (qdd.nodeName() == "EcmStrategy")
			{
				for (int i = 0; i < scenario.getAllEcmStrategy().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllEcmStrategy()[i]->getName()) {
						temp.setEcmStrategy(scenario.getAllEcmStrategy()[i]);
						break;
					}
				}
				/*sce::EcmStrategy temp_2;
				temp_2.setName(qdd.toElement().text().toStdString());
				temp.setEcmStrategy(make_shared<sce::EcmStrategy>(temp_2));*/
			}
		}
		EcmESRs.push_back(temp);
	}
	scenario.setAllEcmEcmStrategyRelation(EcmESRs);
	//find the data of "OwnPlatformRouteRelation"
	if ((qd.tagName() == "OwnPlatformRouteRelation") && (qd.parentNode().nodeName() == "Scenario"))
	{
		sce::OwnPlatformRouteRelation temp;
		for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
		{
			if (qdd.nodeName() == "OwnPlatform")
			{
				for (int i = 0; i < scenario.getAllOwnPlatform().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllOwnPlatform()[i]->getName()) {
						temp.setOwnPlatform(scenario.getAllOwnPlatform()[i]);
						break;
					}
				}
			}
			if (qdd.nodeName() == "Route")
			{
				for (int i = 0; i < scenario.getAllRoute().size(); i++) {
					if (qdd.toElement().text().toStdString() == scenario.getAllRoute()[i]->getName()) {
						temp.setRoute(scenario.getAllRoute()[i]);
						break;
					}
				}
			}
		}
		OPRRs.push_back(temp);
	}
	scenario.setAllOwnPlatformRouteRelation(OPRRs);
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






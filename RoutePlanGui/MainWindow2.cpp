#include "MainWindow2.h"
#include <QMainWindow>
#include <QDebug>
#include <string>
#include <stdio.h>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QtWidgets/QComboBox>
#include "qtablewidget.h"
#include <string>

MainWindow2::MainWindow2(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Read_data2"));//设置标题
	ui.pushButton_19->setText("Add");
	ui.pushButton_20->setText("Del");
	headers_section << QStringLiteral("Name") << QStringLiteral("Section");
	ui.tableWidget_9->setColumnCount(2);
	ui.tableWidget_9->setHorizontalHeaderLabels(headers_section);
	ui.tableWidget_9->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget_10->setColumnCount(2);
	ui.tableWidget_10->setHorizontalHeaderLabels(headers_section);
	ui.tableWidget_10->horizontalHeader()->setHighlightSections(false);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(show_page4()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_previous()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(append_PSR()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(del_PSR()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(del_PER()));
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(append_PER()));
	connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(del_EESR()));
	connect(ui.pushButton_8, SIGNAL(clicked()), this, SLOT(append_EESR()));
	connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(del_EESR2()));
	connect(ui.pushButton_10, SIGNAL(clicked()), this, SLOT(append_EESR2()));
	connect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(del_OPRR()));
	connect(ui.pushButton_12, SIGNAL(clicked()), this, SLOT(append_OPRR()));
	connect(ui.pushButton_13, SIGNAL(clicked()), this, SLOT(del_PWR()));
	connect(ui.pushButton_14, SIGNAL(clicked()), this, SLOT(append_PWR()));
	connect(ui.pushButton_15, SIGNAL(clicked()), this, SLOT(del_OPER()));
	connect(ui.pushButton_16, SIGNAL(clicked()), this, SLOT(append_OPER()));
	connect(ui.pushButton_17, SIGNAL(clicked()), this, SLOT(del_OPER2()));
	connect(ui.pushButton_18, SIGNAL(clicked()), this, SLOT(append_OPER2()));
	connect(ui.pushButton_19, SIGNAL(clicked()), this, SLOT(append_EsmStrategy()));
	connect(ui.pushButton_20, SIGNAL(clicked()), this, SLOT(del_EsmStrategy()));
	connect(ui.pushButton_31, SIGNAL(clicked()), this, SLOT(save_EsmStrategy()));
	connect(ui.pushButton_32, SIGNAL(clicked()), this, SLOT(save_EcmStrategy()));
	connect(ui.pushButton_21, SIGNAL(clicked()), this, SLOT(append_EcmStrategy()));
	connect(ui.pushButton_22, SIGNAL(clicked()), this, SLOT(del_EcmStrategy()));
	connect(ui.pushButton_23, SIGNAL(clicked()), this, SLOT(save_PlatformSiteRelation()));
	connect(ui.pushButton_24, SIGNAL(clicked()), this, SLOT(save_EcmEcmStrategyRelation()));
	connect(ui.pushButton_25, SIGNAL(clicked()), this, SLOT(save_EsmEsmStrategyRelation()));
	connect(ui.pushButton_26, SIGNAL(clicked()), this, SLOT(save_OwnPlatformRouteRelation()));
	connect(ui.pushButton_27, SIGNAL(clicked()), this, SLOT(save_PlatformEmitterRelation()));
	connect(ui.pushButton_28, SIGNAL(clicked()), this, SLOT(save_PlatformWeaponRelation()));
	connect(ui.pushButton_29, SIGNAL(clicked()), this, SLOT(save_OwnPlatformEcmRelation()));
	connect(ui.pushButton_30, SIGNAL(clicked()), this, SLOT(save_OwnPlatformEsmRelation()));
}

MainWindow2::~MainWindow2()
{
}
void MainWindow2::show_xml_data() {
	//show PlatformSiteRelation
	QStringList platformname_List;
	QStringList sitename_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	for (int j = 0; j < scenario.getAllSite().size(); j++) {
		sitename_List.append(QString::fromStdString(scenario.getAllSite()[j]->getName()));
	}
	ui.tableWidget->setRowCount(0);
	for (int i = 0; i < scenario.getAllPlatformSiteRelation().size(); i++)
	{
		ui.tableWidget->insertRow(i);
		ui.tableWidget->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(platformname_List);
		combox_1->addItems(sitename_List);
		for (int j = 0; j < platformname_List.size(); j++)
		{
			if (platformname_List[j].toStdString() == scenario.getAllPlatformSiteRelation()[i].getPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < sitename_List.size(); j++)
		{
			if (sitename_List[j].toStdString() == scenario.getAllPlatformSiteRelation()[i].getSiteName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
	//show EcmEcmStrategyRelation
	QStringList ecmname_List;
	QStringList EcmStrategyname_List;
	for (int j = 0; j < scenario.getAllEcm().size(); j++)
	{
		ecmname_List.append(QString::fromStdString(scenario.getAllEcm()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEcmStrategy().size(); j++)
	{
		EcmStrategyname_List.append(QString::fromStdString(scenario.getAllEcmStrategy()[j]->getName()));
	}
	ui.tableWidget_6->setRowCount(0);
	for (int i = 0; i < scenario.getAllEcmEcmStrategyRelation().size(); i++)
	{
		ui.tableWidget_6->insertRow(i);
		ui.tableWidget_6->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_6->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_6->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_6->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(ecmname_List);
		combox_1->addItems(EcmStrategyname_List);
		for (int j = 0; j < ecmname_List.size(); j++)
		{
			if (ecmname_List[j].toStdString() == scenario.getAllEcmEcmStrategyRelation()[i].getEcmName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < EcmStrategyname_List.size(); j++)
		{
			if (EcmStrategyname_List[j].toStdString() == scenario.getAllEcmEcmStrategyRelation()[i].getEcmStrategyName())
			{
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
	//显示EsmEsmStrategyRelation的数据
	QStringList Esmname_List;
	QStringList EsmStrategyname_List;
	for (int j = 0; j < scenario.getAllEsm().size(); j++) {
		Esmname_List.append(QString::fromStdString(scenario.getAllEsm()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEsmStrategy().size(); j++) {
		EsmStrategyname_List.append(QString::fromStdString(scenario.getAllEsmStrategy()[j]->getName()));
	}
	ui.tableWidget_7->setRowCount(0);
	for (int i = 0; i < scenario.getAllEsmEsmStrategyRelation().size(); i++) {
		ui.tableWidget_7->insertRow(i);
		ui.tableWidget_7->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_7->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_7->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_7->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(Esmname_List);
		combox_1->addItems(EsmStrategyname_List);
		for (int j = 0; j < Esmname_List.size(); j++)
		{
			if (Esmname_List[j].toStdString() == scenario.getAllEsmEsmStrategyRelation()[i].getEsmName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < EsmStrategyname_List.size(); j++)
		{
			if (EsmStrategyname_List[j].toStdString() == scenario.getAllEsmEsmStrategyRelation()[i].getEsmStrategyName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
	//显示OwnPlatformRouteRelation的数据
	QStringList OwnPlatformname_List;
	QStringList Routename_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		OwnPlatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllRoute().size(); j++) {
		Routename_List.append(QString::fromStdString(scenario.getAllRoute()[j]->getName()));
	}
	ui.tableWidget_8->setRowCount(0);
	for (int i = 0; i < scenario.getAllOwnPlatformRouteRelation().size(); i++) {
		ui.tableWidget_8->insertRow(i);
		ui.tableWidget_8->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_8->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_8->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_8->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(OwnPlatformname_List);
		combox_1->addItems(Routename_List);
		for (int j = 0; j < OwnPlatformname_List.size(); j++)
		{
			if (OwnPlatformname_List[j].toStdString() == scenario.getAllOwnPlatformRouteRelation()[i].getOwnPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Routename_List.size(); j++)
		{
			if (Routename_List[j].toStdString() == scenario.getAllOwnPlatformRouteRelation()[i].getRouteName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
	//显示PlatformEmitterRelation的数据
	QStringList Emittername_List;
	for (int j = 0; j < scenario.getAllEmitter().size(); j++) {
		Emittername_List.append(QString::fromStdString(scenario.getAllEmitter()[j]->getName()));
	}
	ui.tableWidget_2->setRowCount(0);
	for (int i = 0; i < scenario.getAllPlatformEmitterRelation().size(); i++) {
		ui.tableWidget_2->insertRow(i);
		ui.tableWidget_2->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_2->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_2->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_2->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(platformname_List);
		combox_1->addItems(Emittername_List);
		for (int j = 0; j < platformname_List.size(); j++)
		{
			if (platformname_List[j].toStdString() == scenario.getAllPlatformEmitterRelation()[i].getPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Emittername_List.size(); j++)
		{
			if (Emittername_List[j].toStdString() == scenario.getAllPlatformEmitterRelation()[i].getEmitterName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
	//显示PlatformWeaponRelation的数据
	ui.tableWidget_3->setRowCount(0);
	for (int i = 0; i < scenario.getAllPlatformWeaponRelation().size(); i++) {
		ui.tableWidget_3->insertRow(i);
		ui.tableWidget_3->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_3->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_3->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_3->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		QStringList Weaponname_List;
		for (int j = 0; j < scenario.getAllWeapon().size(); j++) {
			Weaponname_List.append(QString::fromStdString(scenario.getAllWeapon()[j]->getName()));
		}
		combox->addItems(platformname_List);
		combox_1->addItems(Weaponname_List);
		for (int j = 0; j < platformname_List.size(); j++)
		{
			if (platformname_List[j].toStdString() == scenario.getAllPlatformWeaponRelation()[i].getPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Weaponname_List.size(); j++)
		{
			if (Weaponname_List[j].toStdString() == scenario.getAllPlatformWeaponRelation()[i].getWeaponName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
	//显示OwnPlatformEcmRelation的数据
	ui.tableWidget_4->setRowCount(0);
	for (int i = 0; i < scenario.getAllOwnPlatformEcmRelation().size(); i++) {
		ui.tableWidget_4->insertRow(i);
		ui.tableWidget_4->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_4->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_4->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_4->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(OwnPlatformname_List);
		combox_1->addItems(ecmname_List);
		for (int j = 0; j < OwnPlatformname_List.size(); j++)
		{
			if (OwnPlatformname_List[j].toStdString() == scenario.getAllOwnPlatformEcmRelation()[i].getOwnPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < ecmname_List.size(); j++)
		{
			if (ecmname_List[j].toStdString() == scenario.getAllOwnPlatformEcmRelation()[i].getEcmName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
	//显示OwnPlatformEsmRelation的数据
	ui.tableWidget_5->setRowCount(0);
	for (int i = 0; i < scenario.getAllOwnPlatformEsmRelation().size(); i++) {
		ui.tableWidget_5->insertRow(i);
		ui.tableWidget_5->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_5->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_5->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_5->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(OwnPlatformname_List);
		combox_1->addItems(Esmname_List);
		for (int j = 0; j < OwnPlatformname_List.size(); j++)
		{
			if (OwnPlatformname_List[j].toStdString() == scenario.getAllOwnPlatformEsmRelation()[i].getOwnPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Esmname_List.size(); j++)
		{
			if (Esmname_List[j].toStdString() == scenario.getAllOwnPlatformEsmRelation()[i].getEsmName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
	//显示EsmStrategy的数据

	ui.tableWidget_9->setRowCount(0);
	for (int i = 0; i < scenario.getAllEsmStrategy().size(); i++)
	{
		ui.tableWidget_9->insertRow(i);
		ui.tableWidget_9->setCellWidget(i, 1, new QPushButton());
		QPushButton* ptn = qobject_cast<QPushButton*>(ui.tableWidget_9->cellWidget(i, 1));
		ui.tableWidget_9->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllEsmStrategy()[i]->getName())));
		connect(ptn, SIGNAL(clicked()), this, SLOT(show_esmstrategy_section()));
		ptn->setText("View");
	}
	//显示EcmStrategy的数据
	ui.tableWidget_10->setRowCount(0);
	for (int i = 0; i < scenario.getAllEcmStrategy().size(); i++) {
		ui.tableWidget_10->insertRow(i);
		ui.tableWidget_10->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllEcmStrategy()[i]->getName())));
		ui.tableWidget_10->setCellWidget(i, 1, new QPushButton());
		QPushButton* ptn = qobject_cast<QPushButton*>(ui.tableWidget_10->cellWidget(i, 1));
		ptn->setText("View");
		connect(ptn, SIGNAL(clicked()), this, SLOT(show_ecmstrategy_section()));
	}

}
void MainWindow2::save_EsmStrategy() {
	int num = ui.tableWidget_9->currentRow();
	QString a = ui.tableWidget_9->item(num, 0)->text();
	sce::EsmStrategy new_data(a.toStdString());
	int n = scenario.getAllEsmStrategy().size();
	if(num+1 > n) 
	{
		//sce::EsmStrategySection sce1;
		//sce::EsmStrategySection sce2;
		//std::vector<std::shared_ptr<sce::EsmStrategySection>>sce  {std::make_shared<sce::EsmStrategySection>(sce1), std::make_shared<sce::EsmStrategySection>(sce2)};
		//new_data.setAllPtr2Sections(sce);
		scenario.addEsmStrategy(std::make_shared<sce::EsmStrategy>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save EsmStrategy successfully"));
		QDomElement root = dom.documentElement();
		QDomElement es_node = dom.createElement("EsmStrategy");
		QDomElement name_node = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(a);
		name_node.appendChild(text1);;
		es_node.appendChild(name_node);
		root.appendChild(es_node);
	}
	else {
		scenario.getAllEsmStrategy()[num]->setName(a.toStdString());
		QMessageBox::about(this, tr("Tip"), tr("Save EsmStrategy successfully"));
		QDomNodeList list = dom.elementsByTagName("EsmStrategy");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement e = list.at(i).toElement();
			if (e.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					e.firstChild().firstChild().setNodeValue(a);					
					break;
				}
				flag++;
			}
		}
	}
}
void MainWindow2::save_EcmStrategy() {
	int num = ui.tableWidget_10->currentRow();
	QString a = ui.tableWidget_10->item(num, 0)->text();
	//vector<EsmStrategySection>
	sce::EcmStrategy new_data;
	new_data.setName(a.toStdString());
	int n = scenario.getAllEcmStrategy().size();
	if (num + 1 > n) 
	{
		//sce::EcmStrategySection sce1;
		//sce::EcmStrategySection sce2;
		//std::vector<std::shared_ptr<sce::EcmStrategySection>>sce{ std::make_shared<sce::EcmStrategySection>(sce1), std::make_shared<sce::EcmStrategySection>(sce2) };
		//new_data.setAllPtr2Sections(sce);
		scenario.addEcmStrategy(std::make_shared<sce::EcmStrategy>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save EcmStrategy successfully"));
		QDomElement root = dom.documentElement();
		QDomElement es_node = dom.createElement("EcmStrategy");
		QDomElement name_node = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(a);
		name_node.appendChild(text1);;
		es_node.appendChild(name_node);
		root.appendChild(es_node);
	}
	else 
	{
		scenario.getAllEcmStrategy()[num]->setName(a.toStdString());
		QMessageBox::about(this, tr("Tip"), tr("Save EcmStrategy successfully"));
		QDomNodeList list = dom.elementsByTagName("EcmStrategy");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					ele.firstChild().firstChild().setNodeValue(a);
					break;
				}
				flag++;
			}
		}
	}
}
void MainWindow2::save_PlatformSiteRelation() {
	int num = ui.tableWidget->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::PlatformSiteRelation new_relation;
	for (auto x : scenario.getAllPlatform()) 
	{
		if (x->getPlatformName() == re.toStdString()) 
		{
			new_relation.setPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllSite()) 
	{
		if (x->getName() == re_2.toStdString()) 
		{
			new_relation.setSite(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllPlatformSiteRelation().size()) 
	{
		for (auto x : scenario.getAllPlatformSiteRelation()) 
		{
			if ((x.getPlatformName() == new_relation.getPlatformName())&&(x.getSiteName()==new_relation.getSiteName())) 
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addPlatformSiteRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("PlatformSiteRelation");
		QDomElement third_1 = dom.createElement("Platform");
		QDomElement third_2 = dom.createElement("Site");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllPlatformSiteRelation())
		{
			if ((x.getPlatformName() == new_relation.getPlatformName()) && (x.getSiteName() == new_relation.getSiteName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllPlatform())
		{
			if (x->getPlatformName() == re.toStdString())
			{
				scenario.getAllPlatformSiteRelation()[num].setPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllSite())
		{
			if (x->getName() == re_2.toStdString())
			{
				scenario.getAllPlatformSiteRelation()[num].setSite(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("PlatformSiteRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}

}
void MainWindow2::save_EcmEcmStrategyRelation() 
{
	int num = ui.tableWidget_6->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_6->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_6->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::EcmEcmStrategyRelation new_relation;
	for (auto x : scenario.getAllEcm())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setEcm(x);
			break;
		}
	}
	for (auto x : scenario.getAllEcmStrategy())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setEcmStrategy(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllEcmEcmStrategyRelation().size())
	{
		for (auto x : scenario.getAllEcmEcmStrategyRelation())
		{
			if ((x.getEcmName() == new_relation.getEcmName()) && (x.getEcmStrategyName() == new_relation.getEcmStrategyName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addEcmEcmStrategyRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("EcmEcmStrategyRelation");
		QDomElement third_1 = dom.createElement("Ecm");
		QDomElement third_2 = dom.createElement("EcmStrategy");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);

	}
	else 
	{
		for (int i =0;i<scenario.getAllEcmEcmStrategyRelation().size();i++)
		{
			for (int j = i;j<scenario.getAllEcmEcmStrategyRelation().size();j++) 
			{
				if ((scenario.getAllEcmEcmStrategyRelation()[i].getEcmName() == scenario.getAllEcmEcmStrategyRelation()[j].getEcmName()) && (scenario.getAllEcmEcmStrategyRelation()[i].getEcmStrategyName() == scenario.getAllEcmEcmStrategyRelation()[j].getEcmStrategyName()))
				{
					QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
					return;
				}
			}
		}
		for (auto x : scenario.getAllEcm())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllEcmEcmStrategyRelation()[num].setEcm(x);
				break;
			}
		}
		for (auto x : scenario.getAllEcmStrategy())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllEcmEcmStrategyRelation()[num].setEcmStrategy(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("EcmEcmStrategyRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void MainWindow2::save_EsmEsmStrategyRelation() {
	int num = ui.tableWidget_7->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_7->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_7->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::EsmEsmStrategyRelation new_relation;
	for (auto x : scenario.getAllEsm())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setEsm(x);
			break;
		}
	}
	for (auto x : scenario.getAllEsmStrategy())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setEsmStrategy(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllEsmEsmStrategyRelation().size())
	{
		for (auto x : scenario.getAllEsmEsmStrategyRelation())
		{
			if ((x.getEsmName() == new_relation.getEsmName()) && (x.getEsmStrategyName() == new_relation.getEsmStrategyName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addEsmEsmStrategyRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("EsmEsmStrategyRelation");
		QDomElement third_1 = dom.createElement("Esm");
		QDomElement third_2 = dom.createElement("EsmStrategy");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllEsmEsmStrategyRelation())
		{
			if ((x.getEsmName() == new_relation.getEsmName()) && (x.getEsmStrategyName() == new_relation.getEsmStrategyName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllEsm())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllEsmEsmStrategyRelation()[num].setEsm(x);
				break;
			}
		}
		for (auto x : scenario.getAllEsmStrategy())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllEsmEsmStrategyRelation()[num].setEsmStrategy(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("EsmEsmStrategyRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void MainWindow2::save_OwnPlatformRouteRelation() {
	int num = ui.tableWidget_8->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_8->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_8->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::OwnPlatformRouteRelation new_relation;
	for (auto x : scenario.getAllOwnPlatform())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setOwnPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllRoute())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setRoute(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllOwnPlatformRouteRelation().size())
	{
		for (auto x : scenario.getAllOwnPlatformRouteRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getRouteName() == new_relation.getRouteName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addOwnPlatformRouteRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("OwnPlatformRouteRelation");
		QDomElement third_1 = dom.createElement("OwnPlatform");
		QDomElement third_2 = dom.createElement("Route");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllOwnPlatformRouteRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getRouteName() == new_relation.getRouteName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllOwnPlatform())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllOwnPlatformRouteRelation()[num].setOwnPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllRoute())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllOwnPlatformRouteRelation()[num].setRoute(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("OwnPlatformRouteRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void MainWindow2::save_PlatformEmitterRelation() {
	int num = ui.tableWidget_2->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_2->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_2->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::PlatformEmitterRelation new_relation;
	for (auto x : scenario.getAllPlatform())
	{
		if (x->getPlatformName() == re.toStdString())
		{
			new_relation.setPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllEmitter())
	{
		if (x->getName() == re_2.toStdString())
		{
			new_relation.setEmitter(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllPlatformEmitterRelation().size()) 
	{
		for (auto x : scenario.getAllPlatformEmitterRelation())
		{
			if ((x.getEmitterName() == new_relation.getEmitterName()) && (x.getPlatformName() == new_relation.getPlatformName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addPlatformEmitterRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("PlatformEmitterRelation");
		QDomElement third_1 = dom.createElement("Platform");
		QDomElement third_2 = dom.createElement("Emitter");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllPlatformEmitterRelation())
		{
			if ((x.getEmitterName() == new_relation.getEmitterName()) && (x.getPlatformName() == new_relation.getPlatformName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllPlatform())
		{
			if (x->getPlatformName() == re.toStdString())
			{
				scenario.getAllPlatformEmitterRelation()[num].setPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllEmitter())
		{
			if (x->getName() == re_2.toStdString())
			{
				scenario.getAllPlatformEmitterRelation()[num].setEmitter(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("PlatformEmitterRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void MainWindow2::save_PlatformWeaponRelation() 
{
	int num = ui.tableWidget_3->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_3->cellWidget(num, 0));
	QString re(cell->currentText());
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_3->cellWidget(num, 1));
	QString re_2(cell_2->currentText());
	sce::PlatformWeaponRelation new_relation;
	for (auto x : scenario.getAllPlatform())
	{
		if (x->getPlatformName() == re.toStdString())
		{
			new_relation.setPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllWeapon())
	{
		if (x->getName() == re_2.toStdString())
		{
			new_relation.setWeapon(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllPlatformWeaponRelation().size()) {
		for (auto x : scenario.getAllPlatformWeaponRelation())
		{
			if ((x.getPlatformName() == new_relation.getPlatformName()) && (x.getWeaponName() == new_relation.getWeaponName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addPlatformWeaponRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("PlatformWeaponRelation");
		QDomElement third_1 = dom.createElement("Platform");
		QDomElement third_2 = dom.createElement("Weapon");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllPlatformWeaponRelation())
		{
			if ((x.getPlatformName() == new_relation.getPlatformName()) && (x.getWeaponName() == new_relation.getWeaponName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllPlatform())
		{
			if (x->getPlatformName() == re.toStdString())
			{
				scenario.getAllPlatformWeaponRelation()[num].setPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllWeapon())
		{
			if (x->getName() == re_2.toStdString())
			{
				scenario.getAllPlatformWeaponRelation()[num].setWeapon(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("PlatformWeaponRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void MainWindow2::save_OwnPlatformEcmRelation() 
{
	int num = ui.tableWidget_4->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_4->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_4->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::OwnPlatformEcmRelation new_relation;
	for (auto x : scenario.getAllOwnPlatform())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setOwnPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllEcm())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setEcm(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllOwnPlatformEcmRelation().size()) {
		for (auto x : scenario.getAllOwnPlatformEcmRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getEcmName() == new_relation.getEcmName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addOwnPlatformEcmRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("OwnPlatformEcmRelation");
		QDomElement third_1 = dom.createElement("OwnPlatform");
		QDomElement third_2 = dom.createElement("Ecm");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllOwnPlatformEcmRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getEcmName() == new_relation.getEcmName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllOwnPlatform())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllOwnPlatformEcmRelation()[num].setOwnPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllEcm())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllOwnPlatformEcmRelation()[num].setEcm(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("OwnPlatformEcmRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void MainWindow2::save_OwnPlatformEsmRelation() {
	int num = ui.tableWidget_5->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_5->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_5->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::OwnPlatformEsmRelation new_relation;
	for (auto x : scenario.getAllOwnPlatform())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setOwnPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllEsm())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setEsm(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllOwnPlatformEsmRelation().size()) {
		for (auto x : scenario.getAllOwnPlatformEsmRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getEsmName() == new_relation.getEsmName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addOwnPlatformEsmRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("OwnPlatformEsmRelation");
		QDomElement third_1 = dom.createElement("OwnPlatform");
		QDomElement third_2 = dom.createElement("Esm");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllOwnPlatformEsmRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getEsmName() == new_relation.getEsmName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllOwnPlatform())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllOwnPlatformEsmRelation()[num].setOwnPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllEsm())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllOwnPlatformEsmRelation()[num].setEsm(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("OwnPlatformEsmRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void MainWindow2::append_EsmStrategy()
{
	QPushButton *add = new QPushButton();
	add->setText("View");
	int row_count = ui.tableWidget_9->rowCount();
	ui.tableWidget_9->insertRow(row_count);
	ui.tableWidget_9->setCellWidget(row_count,1,add);
	connect(add,SIGNAL(clicked()),this,SLOT(show_esmstrategy_section()));
}
void MainWindow2::added_esmstrategy_section() 
{
	emit sign_added_esmstrategy_section();
}
void MainWindow2::added_ecmstrategy_section() 
{
	emit sign_added_ecmstrategy_section();
}
void MainWindow2::append_EcmStrategy()
{
	QPushButton *add = new QPushButton();
	add->setText("View");
	int row_count = ui.tableWidget_10->rowCount();
	ui.tableWidget_10->insertRow(row_count);
	ui.tableWidget_10->setCellWidget(row_count, 1, add);
	connect(add, SIGNAL(clicked()), this, SLOT(show_ecmstrategy_section()));
}
void MainWindow2::del_EcmStrategy() 
{
	int num = ui.tableWidget_10->currentRow();
	int ree = QMessageBox::information(this, "", "Delete?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_10->removeRow(num);
		if (num < scenario.getAllEcmStrategy().size()) {
			scenario.deleteEcmStrategy(num);
			QMessageBox::about(this, "Tip", "Delete EcmStrategy successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EcmStrategy");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						QMessageBox::about(this, "Tip", "Delete EcmStrategy in file successfully");
						break;
					}
					flag++;
				}
			}
		}
	}
}
void MainWindow2::del_EsmStrategy()
{
	int num = ui.tableWidget_9->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_9->removeRow(num);
		if (num < scenario.getAllEsmStrategy().size()) {
			scenario.deleteEsmStrategy(num);
			QMessageBox::about(this, "Tip", "Delete EsmStrategy successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EsmStrategy");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						QMessageBox::about(this, "Tip", "Delete EsmStrategy in file successfully");
						break;
					}
					flag++;
				}
			}
		}
	}
}
void MainWindow2::show_esmstrategy_section() 
{
	choice_esmstrategy = ui.tableWidget_9->currentRow();
	qDebug() << "choice_esmstrategy" << choice_esmstrategy;
	emit sign_esmstrategy_section();
}
void MainWindow2::show_ecmstrategy_section() 
{
	choice_ecmstrategy = ui.tableWidget_10->currentRow();
	qDebug() << "choice_ecmstrategy" << choice_ecmstrategy;
	emit sign_ecmstrategy_section();
}

void MainWindow2::on_previous()//返回第一个数据界面w1的槽函数
{
	this->hide();
	emit sign_previous();

}
void MainWindow2::show_page4()//转向w4界面的槽函数
{
	this->hide();
	emit sign_w4();
}
void MainWindow2::show_page2() //显示第二个数据界面的槽函数
{
	
	this->show();
	
}
void MainWindow2::append_PSR()
{
	int row_count = ui.tableWidget->rowCount(); //获取表单行数
	ui.tableWidget->insertRow(row_count);//添加新的一行
	ui.tableWidget->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList platformname_List;
	QStringList sitename_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	for (int j = 0; j < scenario.getAllSite().size(); j++) {
		sitename_List.append(QString::fromStdString(scenario.getAllSite()[j]->getName ()));
	}
	combox->addItems(platformname_List);
	combox_1->addItems(sitename_List);

}
void MainWindow2::del_PSR()		//删除列表数据
{
	int num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeRow(num);
		if (num < scenario.getAllPlatformSiteRelation().size()) {
			scenario.deletePlatformSiteRelation(num);
			QMessageBox::about(this, "Tip", "Delete PlatformSiteRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("PlatformSiteRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void MainWindow2::append_EESR()
{
	int row_count = ui.tableWidget_6->rowCount(); //获取表单行数
	ui.tableWidget_6->insertRow(row_count);//添加新的一行
	ui.tableWidget_6->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_6->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_6->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_6->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Ecmname_List;
	QStringList EcmStrategyname_List;
	for (int j = 0; j < scenario.getAllEcm().size(); j++) {
		Ecmname_List.append(QString::fromStdString(scenario.getAllEcm()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEcmStrategy().size(); j++) {
		EcmStrategyname_List.append(QString::fromStdString(scenario.getAllEcmStrategy()[j]->getName()));
	}
	combox->addItems(Ecmname_List);
	combox_1->addItems(EcmStrategyname_List);
}
void MainWindow2::del_EESR()		//删除列表数据
{
	int num = ui.tableWidget_6->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_6->removeRow(num);
		if (num < scenario.getAllEcmEcmStrategyRelation().size()) {
			scenario.deleteEcmEcmStrategyRelation(num);
			QMessageBox::about(this, "Tip", "Delete EcmEcmStrategyRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EcmEcmStrategyRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void MainWindow2::append_EESR2()
{
	int row_count = ui.tableWidget_7->rowCount(); //获取表单行数
	ui.tableWidget_7->insertRow(row_count);//添加新的一行
	ui.tableWidget_7->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_7->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_7->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_7->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Esmname_List;
	QStringList EsmStrategyname_List;
	for (int j = 0; j < scenario.getAllEsm().size(); j++) {
		Esmname_List.append(QString::fromStdString(scenario.getAllEsm()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEsmStrategy().size(); j++) {
		EsmStrategyname_List.append(QString::fromStdString(scenario.getAllEsmStrategy()[j]->getName()));
	}
	combox->addItems(Esmname_List);
	combox_1->addItems(EsmStrategyname_List);
}
void MainWindow2::del_EESR2()		//删除列表数据
{
	int num = ui.tableWidget_7->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_7->removeRow(num);
		if (num < scenario.getAllEsmEsmStrategyRelation().size()) {
			scenario.deleteEsmEsmStrategyRelation(num);
			QMessageBox::about(this, "Tip", "Delete EsmEsmStrategyRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EsmEsmStrategyRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void MainWindow2::append_OPRR()
{
	int row_count = ui.tableWidget_8->rowCount(); //获取表单行数
	ui.tableWidget_8->insertRow(row_count);//添加新的一行
	ui.tableWidget_8->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_8->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_8->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_8->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Ownplatformname_List;
	QStringList Routename_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		Ownplatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllRoute().size(); j++) {
		Routename_List.append(QString::fromStdString(scenario.getAllRoute()[j]->getName()));
	}
	combox->addItems(Ownplatformname_List);
	combox_1->addItems(Routename_List);
}
void MainWindow2::del_OPRR()		//删除列表数据
{
	int num = ui.tableWidget_8->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_8->removeRow(num);
		if (num < scenario.getAllOwnPlatformRouteRelation().size()) {
			scenario.deleteOwnPlatformRouteRelation(num);
			QMessageBox::about(this, "Tip", "Delete OwnPlatformRouteRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatformRouteRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void MainWindow2::append_PER()
{
	int row_count = ui.tableWidget_2->rowCount(); //获取表单行数
	ui.tableWidget_2->insertRow(row_count);//添加新的一行
	ui.tableWidget_2->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_2->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_2->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_2->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList platformname_List;
	QStringList Emittername_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	for (int j = 0; j < scenario.getAllEmitter().size(); j++) {
		Emittername_List.append(QString::fromStdString(scenario.getAllEmitter()[j]->getName()));
	}
	combox->addItems(platformname_List);
	combox_1->addItems(Emittername_List);
}
void MainWindow2::del_PER()		//删除列表数据
{
	int num = ui.tableWidget_2->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_2->removeRow(num);
		if (num < scenario.getAllPlatformEmitterRelation().size()) {
			scenario.deletePlatformEmitterRelation(num);
			QMessageBox::about(this, "Tip", "Delete PlatformEmitterRealtion successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("PlatformEmitterRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void MainWindow2::append_PWR()
{
	int row_count = ui.tableWidget_3->rowCount(); //获取表单行数
	ui.tableWidget_3->insertRow(row_count);//添加新的一行
	ui.tableWidget_3->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_3->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_3->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_3->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList platformname_List;
	QStringList Weaponname_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	for (int j = 0; j < scenario.getAllWeapon().size(); j++) {
		Weaponname_List.append(QString::fromStdString(scenario.getAllWeapon()[j]->getName()));
	}
	combox->addItems(platformname_List);
	combox_1->addItems(Weaponname_List);
}
void MainWindow2::del_PWR()		//删除列表数据
{
	int num = ui.tableWidget_3->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_3->removeRow(num);
		if (num < scenario.getAllPlatformWeaponRelation().size()) {
			scenario.deletePlatformWeaponRelation(num);
			QMessageBox::about(this, "Tip", "Delete PlatformWeaponRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("PlatformWeaponRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void MainWindow2::append_OPER()
{
	int row_count = ui.tableWidget_4->rowCount(); //获取表单行数
	ui.tableWidget_4->insertRow(row_count);//添加新的一行
	ui.tableWidget_4->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_4->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_4->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_4->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Ownplatformname_List;
	QStringList Ecmname_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		Ownplatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEcm().size(); j++) {
		Ecmname_List.append(QString::fromStdString(scenario.getAllEcm()[j]->getName()));
	}
	combox->addItems(Ownplatformname_List);
	combox_1->addItems(Ecmname_List);
}
void MainWindow2::del_OPER()		//删除列表数据
{
	int num = ui.tableWidget_4->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_4->removeRow(num);
		if (num < scenario.getAllOwnPlatformEcmRelation().size()) {
			scenario.deleteOwnPlatformEcmRelation(num);
			QMessageBox::about(this, "Tip", "Delete OwnPlatformEcmRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatformEcmRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void MainWindow2::append_OPER2()
{
	int row_count = ui.tableWidget_5->rowCount(); //获取表单行数
	ui.tableWidget_5->insertRow(row_count);//添加新的一行
	ui.tableWidget_5->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_5->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_5->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_5->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Ownplatformname_List;
	QStringList Esmname_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		Ownplatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEsm().size(); j++) {
		Esmname_List.append(QString::fromStdString(scenario.getAllEsm()[j]->getName()));
	}
	combox->addItems(Ownplatformname_List);
	combox_1->addItems(Esmname_List);
}
void MainWindow2::del_OPER2()		//删除列表数据
{
	int num = ui.tableWidget_5->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_5->removeRow(num);
		if (num < scenario.getAllOwnPlatformEsmRelation().size()) {
			scenario.deleteOwnPlatformEsmRelation(num);
			QMessageBox::about(this, "Tip", "Delete OwnPlatformEsmRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatformEsmRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}


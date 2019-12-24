#include "MainWindow4.h"
#include <QMessageBox>

#pragma execution_character_set("utf-8")  

MainWindow4::MainWindow4(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Read_data3"));//设置标题
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(back_show_page2()));
	connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(show_page3()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(append_ecm1()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(del_ecm1()));
	connect(ui.pushButton_8, SIGNAL(clicked()), this, SLOT(append_esm()));
	connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(del_esm()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(add_tab()));
	connect(ui.pushButton_10, SIGNAL(clicked()), this, SLOT(del_tab()));
	connect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(save_esm()));
	connect(ui.pushButton_12, SIGNAL(clicked()), this, SLOT(save_ecm()));
	connect(ui.tabWidget_11, QOverload<int>::of(&QTabWidget::currentChanged),
		[=](int index)
	{

		qDebug() << "index:" << index;
		route_index = index;
		
	});
}

MainWindow4::~MainWindow4()
{
}
void MainWindow4::show_xml_data() {
	int cur_index = ui.tabWidget_11->currentIndex() + 1;
	for (int i = 0; i < ui.tabWidget_11->count(); ) {
		ui.tabWidget_11->removeTab(0);
	}
	for (int i = 0; i < scenario.getAllRoute().size(); i++) {
		//首先设置一个widget
		QWidget * tab_nn = new QWidget();
		QGridLayout* gridLayout_n = new QGridLayout();
		QPushButton *add = new QPushButton();
		QPushButton *del = new QPushButton();
		QPushButton *save = new QPushButton();
		add->setText("Add");
		del->setText("Del");
		save->setText("Save");
		QStringList headers;
		headers << QStringLiteral("Index") << QStringLiteral("Altitude") << QStringLiteral("Index") << QStringLiteral("Latitude") << QStringLiteral("Longitude") << QStringLiteral("Time") << QStringLiteral("Velocity") << QStringLiteral("Acceleration");
		//创建route的表格
		QTableWidget *new_table = new QTableWidget();
		new_table->setColumnCount(7);
		new_table->setRowCount(0);
		new_table->setLayoutDirection(Qt::LeftToRight);
		new_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		new_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		new_table->setDragEnabled(true);
		new_table->setSelectionBehavior(QAbstractItemView::SelectItems);
		new_table->setTextElideMode(Qt::ElideMiddle);
		new_table->horizontalHeader()->setCascadingSectionResizes(false);
		new_table->verticalHeader()->setVisible(true);
		new_table->setHorizontalHeaderLabels(headers);
		for (int j = 0; j < scenario.getAllRoute()[i]->getAllWayPoints().size(); j++)
		{
			new_table->insertRow(j);
			new_table->setItem(j, 0, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getIndex()), 10));
			new_table->setItem(j, 1, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getAltitude(), 'f', 2)));
			new_table->setItem(j, 2, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getLatitude(), 'f', 2)));
			new_table->setItem(j, 3, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getLongitude(), 'f', 2)));
			new_table->setItem(j, 4, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getTime(), 'f', 2)));
			new_table->setItem(j, 5, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getVelocity(), 'f', 2)));
			new_table->setItem(j, 6, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getAcceleration(), 'f', 2)));
		}
		gridLayout_n->addWidget(add);
		gridLayout_n->addWidget(del);
		gridLayout_n->addWidget(save);
		gridLayout_n->addWidget(new_table);
		tab_nn->setLayout(gridLayout_n);
		this->setCentralWidget(tab_nn);
		ui.tabWidget_11->addTab(tab_nn, QString::fromStdString(scenario.getAllRoute()[i]->getName()));
		//测试tab类
		MyTab *tab = new MyTab();
		tab->add = add;
		tab->del = del;
		tab->save = save;
		tab->myindex = cur_index;
		tab->my_table = new_table;
		connect(tab->add, &QPushButton::clicked, this, [=]()
		{
			tab->my_table->insertRow(tab->my_table->rowCount());

		});
		connect(tab->del, &QPushButton::clicked, this, [=]
		{
			int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
			if (ree == 0)
			{
				tab->my_table->removeRow(tab->my_table->currentRow());
			}
		});
		connect(tab->save, &QPushButton::clicked, this, [=]
		{
			int cur = tab->my_table->currentRow();
			QString a = tab->my_table->item(cur, 0)->text();
			QString b = tab->my_table->item(cur, 1)->text();
			QString c = tab->my_table->item(cur, 2)->text();
			QString d = tab->my_table->item(cur, 3)->text();
			QString e = tab->my_table->item(cur, 4)->text();
			QString f = tab->my_table->item(cur, 5)->text();
			QString g = tab->my_table->item(cur, 6)->text();
			if (cur + 1 > scenario.getAllRoute()[cur_index]->getAllWayPoints().size())
			{
				sce::WayPoint new_wp(a.toInt(), b.toDouble(), c.toDouble(), d.toDouble(), e.toDouble(), f.toDouble(), g.toDouble());
				scenario.getAllRoute()[cur_index]->addWayPoint(new_wp);
				QMessageBox::about(this, tr("Tip"), tr("Save WayPoint successfully"));

				QDomElement third = dom.createElement("WayPoints");

				QDomElement fourth_1 = dom.createElement("Index");
				QDomElement fourth_2 = dom.createElement("Altitude");
				QDomElement fourth_3 = dom.createElement("Latitude");
				QDomElement fourth_4 = dom.createElement("Longitude");
				QDomElement fourth_5 = dom.createElement("Time");
				QDomElement fourth_6 = dom.createElement("Velocity");
				QDomElement fourth_7 = dom.createElement("Acceleration");

				QDomText text1 = dom.createTextNode(a);
				QDomText text2 = dom.createTextNode(b);
				QDomText text3 = dom.createTextNode(c);
				QDomText text4 = dom.createTextNode(d);
				QDomText text5 = dom.createTextNode(e);
				QDomText text6 = dom.createTextNode(f);
				QDomText text7 = dom.createTextNode(g);

				fourth_1.appendChild(text1);
				fourth_2.appendChild(text2);
				fourth_3.appendChild(text3);
				fourth_4.appendChild(text4);
				fourth_5.appendChild(text5);
				fourth_6.appendChild(text6);
				fourth_7.appendChild(text7);

				third.appendChild(fourth_1);
				third.appendChild(fourth_2);
				third.appendChild(fourth_3);
				third.appendChild(fourth_4);
				third.appendChild(fourth_5);
				third.appendChild(fourth_6);
				third.appendChild(fourth_7);

				QDomNodeList list = dom.elementsByTagName("Route");
				int flag = 0;
				for (int i = 0; i < list.count(); i++)
				{
					QDomElement ele = list.at(i).toElement();
					if (ele.parentNode().nodeName() == "Scenario")
					{
						if (flag == cur_index)
						{
							ele.appendChild(third);
							break;
						}
						flag++;
					}
				}
			}
			else
			{
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setIndex(a.toInt());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setAltitude(b.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setLatitude(c.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setLongitude(d.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setTime(e.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setVelocity(f.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setAcceleration(g.toDouble());
				QMessageBox::about(this, tr("Tip"), tr("Save WayPoint successfully"));

				QDomNodeList list = dom.elementsByTagName("Route");
				int flag = 0;
				for (int i = 0; i < list.count(); i++)
				{
					QDomElement ele = list.at(i).toElement();
					if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的Route
					{
						if (flag == cur_index)
						{
							int flag_2 = 0;
							for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
							{
								if (qd.nodeName() == "WayPoints")//找到具体的WayPoints
								{
									if (flag_2 == cur)
									{
										for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
										{
											if (qdd.nodeName() == "Index")
											{
												qdd.firstChild().setNodeValue(a);
											}
											if (qdd.nodeName() == "Altitude")
											{
												qdd.firstChild().setNodeValue(b);
											}
											if (qdd.nodeName() == "Latitude")
											{
												qdd.firstChild().setNodeValue(c);
											}
											if (qdd.nodeName() == "Longitude")
											{
												qdd.firstChild().setNodeValue(d);
											}
											if (qdd.nodeName() == "Time")
											{
												qdd.firstChild().setNodeValue(e);
											}
											if (qdd.nodeName() == "Velocity")
											{
												qdd.firstChild().setNodeValue(f);
											}
											if (qdd.nodeName() == "Acceleration")
											{
												qdd.firstChild().setNodeValue(g);
											}
										}
										break;
									}
									flag_2++;
								}
							}
							break;
						}
						flag++;
					}
				}
			}
		});

		vTab.push_back(tab);
	}

	ui.tableWidget_25->setRowCount(0);
	for (int i = 0; i < scenario.getAllEsm().size(); i++) {
		ui.tableWidget_25->insertRow(i);
		ui.tableWidget_25->setItem(i, 0, new QTableWidgetItem(QString::fromStdString((scenario.getAllEsm()[i]->getName()))));
		ui.tableWidget_25->setItem(i, 1, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getDwellFreqResolution()), 'f', 2)));
		ui.tableWidget_25->setItem(i, 2, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getTuningStep()), 'f', 2)));
		ui.tableWidget_25->setItem(i, 3, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getRfCovMin()), 'f', 2)));
		ui.tableWidget_25->setItem(i, 4, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getRfCovMax()), 'f', 2)));
		ui.tableWidget_25->setItem(i, 5, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getNumPulsesAcquisition()), 'f', 2)));
		ui.tableWidget_25->setItem(i, 6, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getNumPulsesAlarm()), 'f', 2)));
	}
	ui.tableWidget_22->setRowCount(0);
	for (int i = 0; i < scenario.getAllEcm().size(); i++) {
		ui.tableWidget_22->insertRow(i);
		ui.tableWidget_22->setItem(i, 0, new QTableWidgetItem(QString::fromStdString((scenario.getAllEcm()[i]->getName()))));
		ui.tableWidget_22->setItem(i, 1, new QTableWidgetItem(QString::number((scenario.getAllEcm()[i]->getPt()), 'f', 2)));
		ui.tableWidget_22->setItem(i, 2, new QTableWidgetItem(QString::number((scenario.getAllEcm()[i]->getGain()), 'f', 2)));
		ui.tableWidget_22->setItem(i, 3, new QTableWidgetItem(QString::number((scenario.getAllEcm()[i]->getRfMin()), 'f', 2)));
		ui.tableWidget_22->setItem(i, 4, new QTableWidgetItem(QString::number((scenario.getAllEcm()[i]->getRfMax()), 'f', 2)));
		ui.tableWidget_22->setCellWidget(i, 5, new QPushButton());
		auto cell = ui.tableWidget_22->cellWidget(i, 5);
		QPushButton *tech = qobject_cast<QPushButton*>(cell);
		tech->setText("View");
		connect(tech, SIGNAL(clicked()), this, SLOT(ecm_tech()));
	}
}
void MainWindow4::ecm_tech() {
	choice_ecm = ui.tableWidget_22->currentRow();
	emit sign_ecm_tech();
}

void MainWindow4::show_w4() {
	
	this->show();
}

void MainWindow4::save_esm() 
{
	int num = ui.tableWidget_25->currentRow();
	QString a = ui.tableWidget_25->item(num, 0)->text();
	QString b = ui.tableWidget_25->item(num, 1)->text();
	QString c = ui.tableWidget_25->item(num, 2)->text();
	QString d = ui.tableWidget_25->item(num, 3)->text();
	QString e = ui.tableWidget_25->item(num, 4)->text();
	QString f = ui.tableWidget_25->item(num, 5)->text();
	QString g = ui.tableWidget_25->item(num, 6)->text();
	if (num + 1 > scenario.getAllEsm().size()) 
	{
		sce::Esm new_data(a.toStdString(),b.toDouble(),c.toInt(),d.toInt(),e.toInt(),f.toInt(),g.toInt());
		scenario.addEsm(std::make_shared<sce::Esm>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save Esm successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Esm");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Dwell_Freq_Resolution");
		QDomElement third_3 = dom.createElement("Tuning_Step");
		QDomElement third_4 = dom.createElement("RfCovmin");
		QDomElement third_5 = dom.createElement("RfCovmax");
		QDomElement third_6 = dom.createElement("Num_Pulses_Acquisition");
		QDomElement third_7 = dom.createElement("Num_Pulses_Alarm");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		QDomText text4 = dom.createTextNode(d);
		QDomText text5 = dom.createTextNode(e);
		QDomText text6 = dom.createTextNode(f);
		QDomText text7 = dom.createTextNode(g);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		third_5.appendChild(text5);
		third_6.appendChild(text6);
		third_7.appendChild(text7);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		second.appendChild(third_4);
		second.appendChild(third_5);
		second.appendChild(third_6);
		second.appendChild(third_7);
		root.appendChild(second);
	}
	else {
		scenario.getAllEsm()[num]->setName(a.toStdString());
		scenario.getAllEsm()[num]->setDwellFreqResolution(b.toDouble());
		scenario.getAllEsm()[num]->setTuningStep(c.toInt());
		scenario.getAllEsm()[num]->setRfCovMin(d.toInt());
		scenario.getAllEsm()[num]->setRfCovMax(e.toInt());
		scenario.getAllEsm()[num]->setNumPulsesAcquisition(f.toInt());
		scenario.getAllEsm()[num]->setNumPulsesAlarm(g.toInt());
		QMessageBox::about(this, tr("Tip"), tr("Save Esm successfully"));
		QDomNodeList list = dom.elementsByTagName("Esm");
		for (QDomNode qd = list.at(num).firstChild(); !qd.isNull(); qd = qd.nextSibling())
		{
			if (qd.nodeName() == "Name")
			{
				qd.firstChild().setNodeValue(a);
			}
			if (qd.nodeName() == "Dwell_Freq_Resolution")
			{
				qd.firstChild().setNodeValue(b);
			}
			if (qd.nodeName() == "Tuning_Step")
			{
				qd.firstChild().setNodeValue(c);
			}
			if (qd.nodeName() == "RfCovmin")
			{
				qd.firstChild().setNodeValue(d);
			}
			if (qd.nodeName() == "RfCovmax")
			{
				qd.firstChild().setNodeValue(e);
			}
			if (qd.nodeName() == "Num_Pulses_Acquisition")
			{
				qd.firstChild().setNodeValue(f);
			}
			if (qd.nodeName() == "Num_Pulses_Alarm")
			{
				qd.firstChild().setNodeValue(g);
			}
		}
	}
}
void MainWindow4::save_ecm() {
	int num = ui.tableWidget_22->currentRow();
	QString a = ui.tableWidget_22->item(num, 0)->text();
	QString b = ui.tableWidget_22->item(num, 1)->text();
	QString c = ui.tableWidget_22->item(num, 2)->text();
	QString d = ui.tableWidget_22->item(num, 3)->text();
	QString e = ui.tableWidget_22->item(num, 4)->text();
	if (num + 1 > scenario.getAllEcm().size())
	{
		sce::Ecm new_data(a.toStdString(), b.toDouble(), c.toInt(), d.toInt(), e.toInt());
		scenario.addEcm(std::make_shared<sce::Ecm>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save Ecm successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Ecm");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Pt");
		QDomElement third_3 = dom.createElement("Gain");
		QDomElement third_4 = dom.createElement("Rfmin");
		QDomElement third_5 = dom.createElement("Rfmax");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		QDomText text4 = dom.createTextNode(d);
		QDomText text5 = dom.createTextNode(e);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		third_5.appendChild(text5);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		second.appendChild(third_4);
		second.appendChild(third_5);
		root.appendChild(second);
	}
	else {
		scenario.getAllEcm()[num]->setName(a.toStdString());
		scenario.getAllEcm()[num]->setPt(b.toDouble());
		scenario.getAllEcm()[num]->setGain(c.toInt());
		scenario.getAllEcm()[num]->setRfMin(d.toInt());
		scenario.getAllEcm()[num]->setRfMax(e.toInt());
		QMessageBox::about(this, tr("Tip"), tr("Save Ecm successfully"));
		QDomNodeList list = dom.elementsByTagName("Ecm");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
					{
						if (qd.nodeName() == "Name")
						{
							qd.firstChild().setNodeValue(a);
						}
						if (qd.nodeName() == "Pt")
						{
							qd.firstChild().setNodeValue(b);
						}
						if (qd.nodeName() == "Gain")
						{
							qd.firstChild().setNodeValue(c);
						}
						if (qd.nodeName() == "Rfmin")
						{
							qd.firstChild().setNodeValue(d);
						}
						if (qd.nodeName() == "Rfmax")
						{
							qd.firstChild().setNodeValue(e);
						}
					}				
					break;
				}
				flag++;
			}
		}
	}
}
void MainWindow4::add_tab() 
{
	//设置对话框获取输入内容
	bool ok;
	QString d = QInputDialog::getText(this, tr("Tip"),tr("The name of tabpage："), QLineEdit::Normal,"Route",&ok);
	if (ok && !d.isEmpty()) {

		sce::Route new_route;
		new_route.setName(d.toStdString());
		scenario.addRoute(std::make_shared<sce::Route>(new_route));

		QDomElement root = dom.documentElement();
		QDomElement ver = dom.createElement("Route");
		QDomElement third_1 = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(d);
		third_1.appendChild(text1);
		ver.appendChild(third_1);
		root.appendChild(ver);

		//首先设置一个widget
		QWidget * tab_nn = new QWidget();
		QGridLayout* gridLayout_n = new QGridLayout();
		QPushButton *add = new QPushButton();
		QPushButton *del = new QPushButton();
		QPushButton *save = new QPushButton();
		add->setText("Add");
		del->setText("Del");
		save->setText("Save");
		QStringList headers;
		headers << QStringLiteral("Index") << QStringLiteral("Altitude") << QStringLiteral("Index") << QStringLiteral("Latitude") << QStringLiteral("Longitude") << QStringLiteral("Time") << QStringLiteral("Velocity") << QStringLiteral("Acceleration");
		//创建route的表格
		QTableWidget *new_table = new QTableWidget();
		new_table->setColumnCount(7);
		new_table->setLayoutDirection(Qt::LeftToRight);
		new_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		new_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		new_table->setDragEnabled(true);
		new_table->setSelectionBehavior(QAbstractItemView::SelectItems);
		new_table->setTextElideMode(Qt::ElideMiddle);
		new_table->horizontalHeader()->setCascadingSectionResizes(false);
		new_table->verticalHeader()->setVisible(true);
		new_table->setHorizontalHeaderLabels(headers);
		gridLayout_n->addWidget(add);
		gridLayout_n->addWidget(del);
		gridLayout_n->addWidget(save);
		gridLayout_n->addWidget(new_table);
		tab_nn->setLayout(gridLayout_n);
		this->setCentralWidget(tab_nn);
		ui.tabWidget_11->addTab(tab_nn,d);
		//测试tab类
		MyTab *tab = new MyTab();
		tab->add = add;
		tab->del = del;
		tab->save = save;
		tab->myindex = ui.tabWidget_11->currentIndex();
		tab->my_table = new_table;
		vTab.push_back(tab);
		
		for (int i = 0; i < vTab.size(); i++)
		{			
			connect(vTab[i]->add, &QPushButton::clicked, this, [=]()
			{
				vTab[i]->my_table->insertRow(vTab[i]->my_table->rowCount());
					
			});
			connect(vTab[i]->del, &QPushButton::clicked, this, [=]
			{	
				int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
				if(ree == 0 ) 
				{			
					vTab[i]->my_table->removeRow(vTab[i]->my_table->currentRow());
				}
			});
			connect(vTab[i]->save, &QPushButton::clicked, this, [=]
			{
				int cur = vTab[i]->my_table->currentRow();
				QString a = vTab[i]->my_table->item(cur, 0)->text();
				QString b = vTab[i]->my_table->item(cur, 1)->text();
				QString c = vTab[i]->my_table->item(cur, 2)->text();
				QString d = vTab[i]->my_table->item(cur, 3)->text();
				QString e = vTab[i]->my_table->item(cur, 4)->text();
				QString f = vTab[i]->my_table->item(cur, 5)->text();
				QString g = vTab[i]->my_table->item(cur, 6)->text();
				if (cur +1 > scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints().size()) 
				{
					sce::WayPoint new_wp(a.toInt(),b.toDouble(),c.toDouble(),d.toDouble(),e.toDouble(),f.toDouble(),g.toDouble());
					scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->addWayPoint(new_wp);
					QMessageBox::about(this, tr("Tip"), tr("Save WayPoint successfully"));

					QDomElement third = dom.createElement("WayPoints");

					QDomElement fourth_1 = dom.createElement("Index");
					QDomElement fourth_2 = dom.createElement("Altitude");
					QDomElement fourth_3 = dom.createElement("Latitude");
					QDomElement fourth_4 = dom.createElement("Longitude");
					QDomElement fourth_5 = dom.createElement("Time");
					QDomElement fourth_6 = dom.createElement("Velocity");
					QDomElement fourth_7 = dom.createElement("Acceleration");

					QDomText text1 = dom.createTextNode(a);
					QDomText text2 = dom.createTextNode(b);
					QDomText text3 = dom.createTextNode(c);
					QDomText text4 = dom.createTextNode(d);
					QDomText text5 = dom.createTextNode(e);
					QDomText text6 = dom.createTextNode(f);
					QDomText text7 = dom.createTextNode(g);

					fourth_1.appendChild(text1);
					fourth_2.appendChild(text2);
					fourth_3.appendChild(text3);
					fourth_4.appendChild(text4);
					fourth_5.appendChild(text5);
					fourth_6.appendChild(text6);
					fourth_7.appendChild(text7);

					third.appendChild(fourth_1);
					third.appendChild(fourth_2);
					third.appendChild(fourth_3);
					third.appendChild(fourth_4);
					third.appendChild(fourth_5);
					third.appendChild(fourth_6);
					third.appendChild(fourth_7);

					QDomNodeList list = dom.elementsByTagName("Route");
					int flag = 0;
					for (int i = 0; i < list.count(); i++)
					{
						QDomElement ele = list.at(i).toElement();
						if (ele.parentNode().nodeName() == "Scenario")
						{
							if (flag == ui.tabWidget_11->currentIndex())
							{
								ele.appendChild(third);
								break;
							}
							flag++;
						}
					}
				}
				else 
				{
					scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints()[cur].setIndex(a.toInt());
					scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints()[cur].setAltitude(b.toDouble());
					scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints()[cur].setLatitude(c.toDouble());
					scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints()[cur].setLongitude(d.toDouble());
					scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints()[cur].setTime(e.toDouble());
					scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints()[cur].setVelocity(f.toDouble());
					scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints()[cur].setAcceleration(g.toDouble());
					QMessageBox::about(this, tr("Tip"), tr("Save WayPoint successfully"));

					QDomNodeList list = dom.elementsByTagName("Route");
					int flag = 0;
					for (int i = 0; i < list.count(); i++)
					{
						QDomElement ele = list.at(i).toElement();
						if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的Route
						{
							if (flag == ui.tabWidget_11->currentIndex())
							{
								int flag_2 = 0;
								for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
								{
									if (qd.nodeName() == "WayPoints")//找到具体的WayPoints
									{
										if (flag_2 == ui.tabWidget_11->currentIndex())
										{
											for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
											{
												if (qdd.nodeName() == "Index")
												{
													qdd.firstChild().setNodeValue(a);
												}
												if (qdd.nodeName() == "Altitude")
												{
													qdd.firstChild().setNodeValue(b);
												}
												if (qdd.nodeName() == "Latitude")
												{
													qdd.firstChild().setNodeValue(c);
												}
												if (qdd.nodeName() == "Longitude")
												{
													qdd.firstChild().setNodeValue(d);
												}
												if (qdd.nodeName() == "Time")
												{
													qdd.firstChild().setNodeValue(e);
												}
												if (qdd.nodeName() == "Velocity")
												{
													qdd.firstChild().setNodeValue(f);
												}
												if (qdd.nodeName() == "Acceleration")
												{
													qdd.firstChild().setNodeValue(g);
												}
											}
											break;
										}
										flag_2++;
									}
								}
								break;
							}
							flag++;
						}
					}
				}
			});

		}	
		
	}
}


void MainWindow4::del_tab() {
	int num = ui.tabWidget_11->currentIndex();
	int re = QMessageBox::information(this, "Tip", "Delete the current TAB?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (re == 0) {
		ui.tabWidget_11->removeTab(num);
		if (num < scenario.getAllRoute().size()) {
			scenario.deleteRoute(num);
			QMessageBox::about(this, "Tip", "Delete Route successfully");

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
						QMessageBox::about(this, "Tip", "Delete Route in file successfully");
						break;
					}
					flag++;
				}
			}
		}
	}
	
}
void MainWindow4::show_page3() {
	this->hide();
	emit sign_algorithm();
}
void MainWindow4::back_show_page2() {
	this->hide();
	emit sign_w2();
}
//void MainWindow4::append_route1()
//{
//	int row_count = ui.tableWidget_31->rowCount(); 
//	ui.tableWidget_31->insertRow(row_count);
//
//}
//void MainWindow4::del_route1()		
//{
//	int num = ui.tableWidget_31->currentRow();
//	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
//	if (ree != 0)
//	{
//		return;
//	}
//	else {
//		ui.tableWidget_31->removeRow(num);
//		if (num < scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints().size()) {
//			scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->deleteWayPoint(num);
//			QMessageBox::about(this, "Tip", "Delete WayPoint successfully");
//
//			QDomNodeList list_node = dom.elementsByTagName("Route");
//			int flag = 0;
//			for (int i = 0; i < list_node.count(); i++)
//			{
//				QDomElement ele = list_node.at(i).toElement();
//				if (ele.parentNode().nodeName() == "Scenario")
//				{
//					if (flag == 0)
//					{
//						int flag_radar = 0;
//						for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
//						{
//							if (qd.nodeName() == "WayPoints")
//							{
//								if (flag_radar == num)
//								{
//									ele.removeChild(qd);
//									break;
//								}
//								flag_radar++;
//							}
//						}
//						break;
//					}
//					flag++;
//				}
//			}
//		}
//	}
//}
//void MainWindow4::append_route2()
//{
//	int row_count = ui.tableWidget_24->rowCount(); 
//	ui.tableWidget_24->insertRow(row_count);
//
//}
//void MainWindow4::del_route2()		
//{
//	int num = ui.tableWidget_24->currentRow();
//	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
//	if (ree != 0)
//	{
//		return;
//	}
//	else {
//		ui.tableWidget_24->removeRow(num);
//		if (num < scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->getAllWayPoints().size()) {
//			scenario.getAllRoute()[ui.tabWidget_11->currentIndex()]->deleteWayPoint(num);
//			QMessageBox::about(this, "Tip", "Delete WayPoint successfully");
//
//			QDomNodeList list_node = dom.elementsByTagName("Route");
//			int flag = 0;
//			for (int i = 0; i < list_node.count(); i++)
//			{
//				QDomElement ele = list_node.at(i).toElement();
//				if (ele.parentNode().nodeName() == "Scenario")
//				{
//					if (flag == 1)
//					{
//						int flag_radar = 0;
//						for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
//						{
//							if (qd.nodeName() == "WayPoints")
//							{
//								if (flag_radar == num)
//								{
//									ele.removeChild(qd);
//									break;
//								}
//								flag_radar++;
//							}
//						}
//						break;
//					}
//					flag++;
//				}
//			}
//		}
//		
//	}
//}
void MainWindow4::append_esm()
{
	int row_count = ui.tableWidget_25->rowCount(); 
	ui.tableWidget_25->insertRow(row_count);

}
void MainWindow4::del_esm()		
{
	int num = ui.tableWidget_25->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_25->removeRow(num);
		if (num < scenario.getAllEsm().size()) {
			scenario.deleteEsm(num);
			QMessageBox::about(this, "Tip", "Delete Esm successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Esm");
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
void MainWindow4::append_ecm1()
{
	int row_count = ui.tableWidget_22->rowCount(); //获取表单行数
	ui.tableWidget_22->insertRow(row_count);//添加新的一行
	QPushButton *bt = new QPushButton();
	bt->setText("tech");
	ui.tableWidget_22->setCellWidget(row_count, 5, bt);
	connect(bt, SIGNAL(clicked()), this, SLOT(ecm_tech()));

}
void MainWindow4::del_ecm1()	
{
	int num = ui.tableWidget_22->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_22->removeRow(num);
		if (num < scenario.getAllEcm().size()) {
			scenario.deleteEcm(num);
			QMessageBox::about(this, "Tip", "Delete Ecm successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Ecm");
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

void MainWindow4::receive_index(int index)
{
	qDebug() << "index:" << index<< endl;
}

void MainWindow4::route_add_col()
{
	qDebug() << "into" << endl;

}


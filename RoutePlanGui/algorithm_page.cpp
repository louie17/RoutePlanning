#include <QMainWindow>
#include <QDebug>
#include <string>
#include <stdio.h> 
#include <iostream>
#include <vector>
#include <QFileDialog>
#include <QMessageBox>

#include "algorithm_page.h"
#include "MainWindow2.h"
#include "MainWindow.h"
#include "ui_algorithm_page.h"

#include "DE_main.hpp"
#include "de_types.hpp"

#include "A_STAR.h"

algorithm_page::algorithm_page(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Choose algorithm"));//设置标题	
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(run_algorithm()));
	QObject::connect(ui.comboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(setLabelText(int)));
}

algorithm_page::~algorithm_page()
{
}

void algorithm_page::run_algorithm()
{ 
	//获取当前OwnPlatform的索引
	int op_index = ui.comboBox->currentIndex();
	QString tips("Select the: ");
	tips.append(QString::fromStdString(scenario.getAllOwnPlatform()[op_index]->getName()));
	tips.append("\nNew route name: ");
	tips.append(ui.lineEdit_18->text());
	int op_select = QMessageBox::information(this, "Tip", tips, QStringLiteral("Yes"), QStringLiteral("No"));
	if (op_select != 0)
	{
		return;
	}
	else {
		int tab_index = ui.tabWidget->currentIndex();
		double Lud = ui.lineEdit_11->text().toDouble();
		double Ldu = ui.lineEdit_12->text().toDouble();
		double Ldt = ui.lineEdit_13->text().toDouble();
		double Ltd = ui.lineEdit_14->text().toDouble();
		double Lte = ui.lineEdit_15->text().toDouble();
		double Let = ui.lineEdit_16->text().toDouble();
		double Leh = ui.lineEdit_17->text().toDouble();

		//根据威胁位置获取每个威胁的最大武器射程
		std::vector<double> wcrange(scenario.getAllSite().size(), 0.0);
		sce::Site_WeaponRange_relation swRelation;

		assert(scenario.getAllSite().size() > 0);
		assert(scenario.getAllPlatformSiteRelation().size() > 0);
		for (size_t i = 0; i < scenario.getAllSite().size(); ++i)
		{
			auto iterS = scenario.getAllSite().at(i);
			std::vector<unsigned int> siteTmp;
			for (size_t j = 0; j < scenario.getAllPlatformSiteRelation().size(); ++j)
			{
				auto iterPSR = scenario.getAllPlatformSiteRelation().at(j);
				if (iterPSR.getSiteName() == iterS->getName())
				{
					for (size_t z = 0; z < scenario.getAllPlatformWeaponRelation().size(); ++z)
					{
						auto iterPWR = scenario.getAllPlatformWeaponRelation().at(z);
						if (iterPSR.getPlatformName() == iterPWR.getPlatformName())
						{
							siteTmp.push_back(iterPWR.getWeapon()->getWeaponAreaCoverage());
						}
					}
				}
			}
			assert(siteTmp.size() >= 0);
			wcrange[i] = siteTmp.size() > 0 ? *std::max_element(siteTmp.cbegin(), siteTmp.cend()) : 0.0;
			auto ret = swRelation.insert(std::make_pair(iterS, wcrange[i]));
			assert(ret.second);
		}

		//获取路径片段的起始终止点序列
		size_t target_size = scenario.getAllOwnPlatform()[op_index]->getMission().getAllTargetPoints().size();
		assert(target_size);
		std::vector<sce::Point> mission_section{ scenario.getAllOwnPlatform()[op_index]->getMission().getStartPoint() ,scenario.getAllOwnPlatform()[op_index]->getMission().getEndPoint() };
		if (target_size > 0)
		{
			for (size_t i = 0; i < target_size; ++i)
			{
				mission_section.insert(mission_section.end() - 1, scenario.getAllOwnPlatform()[op_index]->getMission().getTargetPoint(i));
			}
		}

		if (tab_index == 0) //choose a* algorithm
		{
			
	
			float survice_w1 = ui.lineEdit->text().toFloat();
			float end_w1 = ui.lineEdit_2->text().toFloat();
			float StepLength1 = ui.lineEdit_3->text().toFloat();
			float hmin1 = ui.lineEdit_5->text().toFloat();
			float hmax1 = ui.lineEdit_19->text().toFloat();
			float horizontal_corner1 = ui.lineEdit_20->text().toFloat();
			float verticality_corner1 = ui.lineEdit_22->text().toFloat();
			float e_w1 = ui.lineEdit_23->text().toFloat();
			float start_w1 = ui.lineEdit_21->text().toFloat();

			int ree = QMessageBox::information(this, "Tip", "Choose a* algorithm ?", QStringLiteral("Yes"), QStringLiteral("No"));

			////确定航路的起点，任务点，结束点
			//auto routev = scenario.getAllRoute();
			//auto my_route1 = routev[0]->getAllWayPoints();
			//auto startpoint = my_route1[0];
			//auto target0 = my_route1[1];
			//auto endpoint = my_route1[2];
			////确定雷达的位置
			//auto sitev = scenario.getAllSite();
			//auto site1 = sitev[0];
			//APoint sp(100, 100, 1, 0, 0, 0, 0, 0);
			//APoint tp(440, 370, 1, 0, 0, 0, 0, 0);
			//APoint ep(800, 500, 1, 0, 0, 0, 0, 0);
			QVector<Rada*> radav;
			for (auto x : swRelation)
			{
				auto site = x.first;
				auto weapon_cov = x.second;
				Rada Rada2(2, site->getLongitude(), site->getLatitude(),site->getAltitude(), weapon_cov, 1);
				radav.append(&Rada2);
			}

	/*		Rada Rada1(1, site1->getLatitude(), site1->getLongitude(), site1->getAltitude(),100,1);
			Rada Rada2(2, site1->getLatitude(), site1->getLongitude(), site1->getAltitude(), 70, 1);
			Rada Rada3(3,  site1->getLatitude(), site1->getLongitude(), site1->getAltitude(), 70, 1);*/
			//Rada Rada1(1, 450, 350, 0, 100, 1);
			//Rada Rada2(2, 450, 350, 0, 50, 1);
			//Rada Rada3(3, 450, 350, 0, 70, 1);
			//radav.append(&Rada1);
			//radav.append(&Rada2);
			//radav.append(&Rada3);
			
	/*		Mission_G mg(1, 440, 370, 1, 0, 0);
			float survice_w1 = 1.0f;
			float start_w1 = 8000;
			float end_w1 = 10000;
			float e_w1 = 0.5;
			float hmin1 = 0.0f;
			float hmax1 = 3.0f;
			float horizontal_corner1 = 0.7854f;
			float verticality_corner1 = 0.5236f;
			float StepLength1 = 4.0f;*/


			QVector<APoint*> routev;
			if (ree != 0)
			{
				return;
			}
			else {
				qDebug() << "choice is  A*";
				for (int i = 0; i < mission_section.size() - 2; i++)
				{
				APoint sp( mission_section[i].getLongitude(), mission_section[i].getLatitude(), mission_section[i].getAltitude(), 0, 0, 0, 0, 0);
					APoint tp(mission_section[i].getLongitude(), mission_section[i+1].getLatitude(),  mission_section[i].getAltitude(), 0, 0, 0, 0, 0);
					APoint ep(mission_section[i + 2].getLongitude(), mission_section[i+2].getLatitude(),  mission_section[i + 2].getAltitude(), 0, 0, 0, 0, 0);
					Mission_G mg(1, mission_section[i + 1].getLongitude(), mission_section[i + 1].getLatitude(), mission_section[i+1].getAltitude(), 2, 0.25);
					A_STAR a(sp, tp, ep, radav, mg, e_w1, survice_w1, start_w1, end_w1, horizontal_corner1, verticality_corner1, hmin1, hmax1, StepLength1);
					routev.append(a.route);

				}
				sce::Route_ptr route;
				for (int i = 0; i < routev.size(); i++)
				{
					route->addWayPoint(sce::WayPoint(i,routev[i]->X, routev[i]->Y, routev[i]->Z));
				}

				scenario.addRoute(route);
				qDebug() << " A* complete";
			}
		}
		if (tab_index == 1) //choose DE algorithm
		{
			size_t Population_Number = ui.lineEdit_6->text().toInt();
			size_t Initial_Node_Number = ui.lineEdit_7->text().toInt();
			size_t Evolution_Number = ui.lineEdit_8->text().toInt();
			double Weight = ui.lineEdit_9->text().toDouble();
			double Cross_Probability = ui.lineEdit_10->text().toDouble();
			int ree = QMessageBox::information(this, "Tip", "Choose DE algorithm ?", QStringLiteral("Yes"), QStringLiteral("No"));
			if (ree != 0)
			{
				return;
			}
			else {
				qDebug() << "choice is DE";
				
				sce::Route_ptr route{std::make_shared<sce::Route>(sce::Route(ui.lineEdit_18->text().toStdString(),sce::WayPoint(mission_section[0].getLongitude(),mission_section[0].getLatitude(),mission_section[0].getAltitude())))};

				for (size_t i = 0; i < target_size; ++i)
				{
					de::NVectorPtr route_section(de::De_alg(swRelation, scenario.getAllVertex(), mission_section[i], mission_section[i+1], Population_Number, Initial_Node_Number, Evolution_Number, Weight, Cross_Probability));	

					for (size_t iter = 1; iter < route_section->size(); ++iter)
					{
						de::Node node(route_section->at(iter));
						route->addWayPoint(sce::WayPoint(iter, node.longitude(), node.latitude(), node.altitude()));
					}
				}
				
				scenario.addRoute(route);
				qDebug() << "DE complete!";								
			}
		}
		if (tab_index == 2) //choose PSO algorithm
		{
			double Swarm_Size = ui.lineEdit_50->text().toDouble();
			double Loop_Couner = ui.lineEdit_51->text().toDouble();
			double Search_Step = ui.lineEdit_57->text().toDouble();
			double Eecute_Step = ui.lineEdit_58->text().toDouble();
			double Step_Distance = ui.lineEdit_59->text().toDouble();
			double Detect_Range = ui.lineEdit_61->text().toDouble();
			double Pitch = ui.lineEdit_60->text().toDouble();
			double Yaw = ui.lineEdit_62->text().toDouble();
			double RefPath = ui.lineEdit_55->text().toDouble();
			double OilCost = ui.lineEdit_53->text().toDouble();
			double Missions = ui.lineEdit_56->text().toDouble();
			double HightConstrain = ui.lineEdit_54->text().toDouble();
			double SurvivalCost = ui.lineEdit_52->text().toDouble();
			int ree = QMessageBox::information(this, "Tip", "Choose PSO algorithm ?", QStringLiteral("Yes"), QStringLiteral("No"));
			if (ree != 0)
			{
				return;
			}
			else {
				qDebug() << "choice is PSO";
			}
		}
	}	

}

void algorithm_page::setLabelText(int index)
{
	if (scenario.getAllOwnPlatform()[index]->getMission().getMissionType() == sce::MissionType::STRIKE)
	{
		ui.label_19->setText("Strike");
	}
	else
		if(scenario.getAllOwnPlatform()[index]->getMission().getMissionType() == sce::MissionType::SUPPORT)
	{
			ui.label_19->setText("Support");
	}
}
void algorithm_page::on_datapage(){
	this->hide();//关闭当前界面
	emit go_datapage();
}
void algorithm_page::show_algorithm_page()
{
	this->show();//显示选择算法的界面
	QStringList ownPlatformlist;

	for (size_t i = 0; i < scenario.getAllOwnPlatform().size(); i++)
	{
		ownPlatformlist.append(QString::fromStdString(scenario.getAllOwnPlatform()[i]->getName()));
	}
	ui.comboBox->addItems(ownPlatformlist);
}

#include "MarkovEvaluate.h"

#define WIDTH 600
#define HEIGHT 300

Markov_evaluate::Markov_evaluate(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	int timeStep = 1;
	VectorXd timeRoute(2);
	timeRoute << 0, 100;

	//Postion of route
	MatrixXd posRoute(3, 2);
	posRoute << 0, 0, 0, 100e3, 0, 0;
	//cout << posRoute << endl;
	MatrixXd xinterp(1, 101), yinterp(1, 101), zinterp(1, 101);
	for (int i = 0; i < 101; i++)
	{
		xinterp(i) = 0;
		yinterp(i) = i * 1000;
		zinterp(i) = 0;
	}

	MatrixXd posRouteInterp(3, 101);
	posRouteInterp << xinterp, yinterp, zinterp;
	MatrixXd posSensor(3, 3), posWeapon(3, 3);
	posSensor << 10e3, -10e3, 0e3,
		25e3, 35e3, 85e3,
		0e3, 0e3, 0e3;

	posWeapon << 5e3, -5e3, 0e3,
		60e3, 55e3, 85e3,
		0e3, 0e3, 0e3;
	MatrixXd rangeSensor(1, 3), rangeWeapon(1, 3);
	rangeSensor << 15e3, 15e3, 10e3;
	rangeWeapon << 10e3, 10e3, 5e3;


	MatrixXd outsideTransitionIntensity(5, 5);
	outsideTransitionIntensity <<
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.2, -0.2, 0.0, 0.0, 0.0,
		0.0, 0.2, -0.2, 0.0, 0.0,
		0.0, 0.0, 1.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0;
	//cout << outsideTransitionIntensity << endl;

	TransitionIntensity sensorTransitionIntensity[3];

	for (int i = 0; i < 3; i++)
	{
		sensorTransitionIntensity[i].weapon.setIdentity(5, 5);
	}

	sensorTransitionIntensity[0].weapon <<
		-0.4, 0.4, 0.0, 0.0, 0.0,
		0.1, -0.4, 0.3, 0.0, 0.0,
		0.0, 0.1, -0.1, 0.0, 0.0,
		0.0, 0.0, 1.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0;

	sensorTransitionIntensity[1].weapon <<
		-0.4, 0.4, 0.0, 0.0, 0.0,
		0.1, -0.4, 0.3, 0.0, 0.0,
		0.0, 0.1, -0.1, 0.0, 0.0,
		0.0, 0.0, 1.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0;

	sensorTransitionIntensity[2].weapon <<
		-0.4, 0.4, 0.0, 0.0, 0.0,
		0.1, -0.4, 0.3, 0.0, 0.0,
		0.0, 0.1, -0.1, 0.0, 0.0,
		0.0, 0.0, 1.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0;

	//for (int i = 0; i < 3; i++)
	//{
	//	cout << sensorTransitionIntensity[i].weapon<<endl;
	//}

	TransitionIntensity weaponTransitionIntensity[3];

	for (int i = 0; i < 3; i++)
	{
		weaponTransitionIntensity[i].weapon.setIdentity(5, 5);
	}

	weaponTransitionIntensity[0].weapon <<
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.2, -0.2, 0.0, 0.0, 0.0,
		0.0, 0.2, -0.4, 0.2, 0.0,
		0.0, 0.0, 0.1, -0.4, 0.3,
		0.0, 0.0, 0.0, 0.0, 0.0;

	weaponTransitionIntensity[1].weapon <<
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.2, -0.2, 0.0, 0.0, 0.0,
		0.0, 0.2, -0.4, 0.2, 0.0,
		0.0, 0.0, 0.1, -0.4, 0.3,
		0.0, 0.0, 0.0, 0.0, 0.0;

	weaponTransitionIntensity[2].weapon <<
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.2, -0.2, 0.0, 0.0, 0.0,
		0.0, 0.2, -0.4, 0.2, 0.0,
		0.0, 0.0, 0.1, -0.4, 0.3,
		0.0, 0.0, 0.0, 0.0, 0.0;



	MatrixXd stateCost(1, 5);
	stateCost <<
		0, 1, 10, 100, 0;

	MatrixXd transitionCost(5, 5);
	transitionCost <<
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 1000.0,
		0.0, 0.0, 0.0, 0.0, 0.0;



	setFixedSize(WIDTH, HEIGHT);
	reRouteEvaluation(posRouteInterp, posSensor, posWeapon, rangeSensor, rangeWeapon,
		outsideTransitionIntensity, sensorTransitionIntensity, weaponTransitionIntensity, stateCost, transitionCost, timeStep);


}


void Markov_evaluate::paintEvent(QPaintEvent*)
{
	/*QPainter painter(this);
	QPen mypen;
	mypen.setColor(Qt::red);
	mypen.setWidth(10);
	painter.setPen(mypen);

	painter.drawPoint(20,20);
	QWidget::update();
	*/
}



void Markov_evaluate::draw_markov(int nWayPoint, MatrixXd stateProbabilityProcess)
{
	
	QBarSet *set0 = new QBarSet("U");
	QBarSet *set1 = new QBarSet("D");
	QBarSet *set2 = new QBarSet("T");
	QBarSet *set3 = new QBarSet("E");
	QBarSet *set4 = new QBarSet("H");
	for (int i = 0; i < nWayPoint; i++)
	{
		*set0 << stateProbabilityProcess(0, i);
		*set1 << stateProbabilityProcess(1, i);
		*set2 << stateProbabilityProcess(2, i);
		*set3 << stateProbabilityProcess(3, i);
		*set4 << stateProbabilityProcess(4, i);

	}
	QPercentBarSeries *series = new QPercentBarSeries();
	series->append(set0);
	series->append(set1);
	series->append(set2);
	series->append(set3);
	series->append(set4);




	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle("Simple percentbarchart example");
	chart->setAnimationOptions(QChart::SeriesAnimations);

	QValueAxis *axisY = new QValueAxis();
	chart->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);


	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->setChart(chart);
	ui.widget = chartView;

	this->setCentralWidget(ui.widget);
}


void Markov_evaluate::markov_show()
{
	this->show();
}
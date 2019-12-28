#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MarkovEvaluate.h"
#include <Eigen/Eigen>
#include <vector>
#include <iostream>
#include <qpainter>
#include <qpen>
#include <Qtcharts>


using namespace std;
using namespace Eigen;

struct TransitionIntensity
{
	MatrixXd  weapon;
};


class Markov_evaluate : public QMainWindow
{
	Q_OBJECT

public:
	Markov_evaluate(QWidget *parent = Q_NULLPTR);


public:
	void paintEvent(QPaintEvent*);
	void reRouteEvaluation(MatrixXd posRouteInterp, MatrixXd posSensor,
		MatrixXd posWeapon, MatrixXd rangeSensor, MatrixXd rangeWeapon,
		MatrixXd outsideTransitionIntensity, TransitionIntensity *sensorTransitionIntensity,
		TransitionIntensity *weaponTransitionIntensity, MatrixXd stateCost,
		MatrixXd transitionCost, int timeStep);
	void reRecursionSolver(MatrixXd previousStateProbability, double previousExpectedCost,
		MatrixXd previousTransitionIntensity, MatrixXd stateCost, MatrixXd transitionCost, int deltaT);

	void draw_markov(int nWayPoint, MatrixXd stateProbabilityProcess);

private slots:
	void markov_show();
private:
	Ui::MarkovEvaluate ui;

public:
	MatrixXd global_stateProbability;
	double global_expectedCost;
};

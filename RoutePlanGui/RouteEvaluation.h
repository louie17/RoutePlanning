#pragma once
#include <Eigen/Eigen>
#include <vector>
#include <iostream>
#include "Scenario.hpp"
#include "qdebug.h"
using namespace std;
using namespace Eigen;

struct TransitionIntensity
{
	MatrixXd  weapon;
};

void reRouteEvaluation(MatrixXd posRouteInterp, MatrixXd posSensor,
		MatrixXd posWeapon, MatrixXd rangeSensor, MatrixXd rangeWeapon,
		MatrixXd outsideTransitionIntensity, TransitionIntensity *sensorTransitionIntensity,
		TransitionIntensity *weaponTransitionIntensity, MatrixXd stateCost,
		MatrixXd transitionCost, int timeStep);

void reRecursionSolver(MatrixXd previousStateProbability, double previousExpectedCost,
		MatrixXd previousTransitionIntensity, MatrixXd stateCost, MatrixXd transitionCost, int deltaT);

void markov_init(const double& ud, const double& du, const double& dt, const double& td, const double& te, const double& et, const double& eh, sce::Route_ptr route, sce::Site_Weapon_relation swRelation);

//MatrixXd global_stateProbability;
//double global_expectedCost;
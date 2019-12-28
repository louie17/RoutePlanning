#include "RouteEvaluation.h"

void reRouteEvaluation(MatrixXd posRouteInterp, MatrixXd posSensor, MatrixXd posWeapon, MatrixXd rangeSensor, MatrixXd rangeWeapon, MatrixXd outsideTransitionIntensity, TransitionIntensity * sensorTransitionIntensity, TransitionIntensity * weaponTransitionIntensity, MatrixXd stateCost, MatrixXd transitionCost, int timeStep)
{
	int nWayPoint = posRouteInterp.cols();
	int nSensor = posSensor.cols();
	int nWeapon = posWeapon.cols();

	MatrixXd transitionIntensity(5, 5);

	MatrixXd stateProbabilityProcess = MatrixXd::Zero(5, nWayPoint);
	MatrixXd expectedCostProcess = MatrixXd::Zero(1, nWayPoint);

	MatrixXd stateProbability(1, 5);
	stateProbability << 1, 0, 0, 0, 0;

	int expectedCost = 0;

	for (int ii = 0; ii < nWayPoint; ii++)
	{
		//% Calculate Transision Intensity Matrix for the way point
		//	% Check if platform is inside sensor operation range
		if (ii == 14)
		{
			int a = 1;
		}
		MatrixXd flagInsideSensor = MatrixXd::Zero(nSensor, 1);
		for (int jj = 0; jj < nSensor; jj++)
		{
			//cout << posRouteInterp.col(ii);
			double distPlatformSensor = (posRouteInterp.col(ii) - posSensor.col(jj)).norm();
			if (distPlatformSensor < rangeSensor(jj))
			{
				flagInsideSensor(jj) = 1;
			}
		}
		//% Check if platform is inside weapon operation range
		MatrixXd flagInsideWeapon = MatrixXd::Zero(nWeapon, 1);

		for (int jj = 0; jj < nWeapon; jj++)
		{
			double distPlatformWeapon = (posRouteInterp.col(ii) - posWeapon.col(jj)).norm();

			if (distPlatformWeapon < rangeWeapon(jj))
			{
				flagInsideWeapon(jj) = 1;
			}

		}
		//  % Platform is outside sensor and weapon operation range
		if (flagInsideWeapon.sum() == 0 && flagInsideSensor.sum() == 0)
		{
			transitionIntensity = outsideTransitionIntensity;
		}
		/*	% Platform is inside weapon operation range
		% For weapons whose operation range is within sensor operation range,
		% if the platform is inside weapon operation range, it is not
		% considered in the sensor operation range.
		*/
		else if (flagInsideWeapon.sum() > 0)
		{
			/*find flagInsideWeapon coeff >0 */
			//VectorXd insideWeapon(flagInsideWeapon.sum());
			vector<int> insideWeapon;
			for (int k = 0; k < flagInsideWeapon.size(); k++)
			{
				if (flagInsideWeapon(k) > 0)
					insideWeapon.push_back(k);
			}
			//% Overlapping weapon area
			if (insideWeapon.size() > 1)
			{
				int cnt = int(insideWeapon[0]);
				double lambdaUD, lambdaDT, lambdaDU, lambdaTD, lambdaTE, lambdaET, lambdaEH;
				lambdaUD = weaponTransitionIntensity[cnt].weapon(0, 1);
				lambdaDT = weaponTransitionIntensity[cnt].weapon(1, 2);
				lambdaDU = weaponTransitionIntensity[cnt].weapon(1, 0);
				lambdaTD = weaponTransitionIntensity[cnt].weapon(2, 1);
				lambdaTE = weaponTransitionIntensity[cnt].weapon(2, 3);
				lambdaET = weaponTransitionIntensity[cnt].weapon(3, 2);
				lambdaEH = weaponTransitionIntensity[cnt].weapon(3, 4);

				for (int jj = 1; jj < insideWeapon.size(); jj++)
				{
					int cnt = insideWeapon[jj];
					lambdaUD = lambdaUD + weaponTransitionIntensity[cnt].weapon(0, 1);
					lambdaDT = lambdaDT + weaponTransitionIntensity[cnt].weapon(1, 2);
					lambdaDU = 1 / (1 / lambdaDU + 1 / weaponTransitionIntensity[cnt].weapon(1, 0) - 1 / (lambdaDU + weaponTransitionIntensity[cnt].weapon(1, 0)));
					lambdaTD = 1 / (1 / lambdaTD + 1 / weaponTransitionIntensity[cnt].weapon(2, 1) - 1 / (lambdaTD + weaponTransitionIntensity[cnt].weapon(2, 1)));
					lambdaTE = max(lambdaTE, weaponTransitionIntensity[cnt].weapon(2, 3));
					lambdaET = min(lambdaET, weaponTransitionIntensity[cnt].weapon(2, 3));
					lambdaEH = max(lambdaEH, weaponTransitionIntensity[cnt].weapon(3, 4));
				}
				transitionIntensity.setIdentity(5, 5);
				transitionIntensity <<
					-lambdaUD, lambdaUD, 0, 0, 0,
					lambdaDU, -(lambdaDU + lambdaDT), lambdaDT, 0, 0,
					0, lambdaTD, -(lambdaTD + lambdaTE), lambdaTE, 0,
					0, 0, lambdaET, -(lambdaET + lambdaEH), lambdaEH,
					0, 0, 0, 0, 0;

			}
			// % Single weapon area
			transitionIntensity = weaponTransitionIntensity[int(insideWeapon[0])].weapon;
		}
		//% Platform is inside sensor operation range
		else
		{
			/*find flagInsideWeapon coeff >0 */
			//VectorXd insideSensor(flagInsideSensor.sum());
			vector<int> insideSensor;
			for (int k = 0; k < flagInsideSensor.size(); k++)
			{
				if (flagInsideSensor(k) > 0)
					insideSensor.push_back(k);
			}
			//% Overlapping weapon area
			if (insideSensor.size() > 1)
			{
				int cnt = int(insideSensor[0]);
				double lambdaUD, lambdaDT, lambdaDU, lambdaTD, lambdaTE, lambdaET, lambdaEH;
				lambdaUD = sensorTransitionIntensity[cnt].weapon(0, 1);
				lambdaDT = sensorTransitionIntensity[cnt].weapon(1, 2);
				lambdaDU = sensorTransitionIntensity[cnt].weapon(1, 0);
				lambdaTD = sensorTransitionIntensity[cnt].weapon(2, 1);
				lambdaTE = sensorTransitionIntensity[cnt].weapon(2, 3);
				lambdaET = sensorTransitionIntensity[cnt].weapon(3, 2);
				lambdaEH = sensorTransitionIntensity[cnt].weapon(3, 4);

				for (int jj = 1; jj < insideSensor.size(); jj++)
				{
					int cnt = insideSensor[jj];
					lambdaUD = lambdaUD + sensorTransitionIntensity[cnt].weapon(0, 1);
					lambdaDT = lambdaDT + sensorTransitionIntensity[cnt].weapon(1, 2);
					lambdaDU = 1 / (1 / lambdaDU + 1 / sensorTransitionIntensity[cnt].weapon(1, 0) - 1 / (lambdaDU + sensorTransitionIntensity[cnt].weapon(1, 0)));
					lambdaTD = 1 / (1 / lambdaTD + 1 / sensorTransitionIntensity[cnt].weapon(2, 1) - 1 / (lambdaTD + sensorTransitionIntensity[cnt].weapon(2, 1)));
					lambdaTE = max(lambdaTE, sensorTransitionIntensity[cnt].weapon(2, 3));
					lambdaET = min(lambdaET, sensorTransitionIntensity[cnt].weapon(2, 3));
					lambdaEH = max(lambdaEH, sensorTransitionIntensity[cnt].weapon(3, 4));
				}

				transitionIntensity.setIdentity(5, 5);
				transitionIntensity <<
					-lambdaUD, lambdaUD, 0, 0, 0,
					lambdaDU, -(lambdaDU + lambdaDT), lambdaDT, 0, 0,
					0, lambdaTD, -(lambdaTD + lambdaTE), lambdaTE, 0,
					0, 0, lambdaET, -(lambdaET + lambdaEH), lambdaEH,
					0, 0, 0, 0, 0;

			}
			else
			{
				transitionIntensity = sensorTransitionIntensity[int(insideSensor[0])].weapon;
			}

		}
		//cout << transitionIntensity << endl;
		// % Calculate state probability and expected cost of current way point
		reRecursionSolver(stateProbability, expectedCost, transitionIntensity, stateCost, transitionCost, timeStep);

		//stateProbability = global_stateProbability.transpose();
		//expectedCost = global_expectedCost;
		//stateProbabilityProcess.col(ii) = global_stateProbability;
		//expectedCostProcess(ii) = global_expectedCost;
		//cout << endl;
		//cout << "state_probability:"<<endl<<global_stateProbability << endl;
		//cout << "i:" << ii << "expect_cost:" << global_expectedCost << endl;

	}
}

void reRecursionSolver(MatrixXd previousStateProbability, double previousExpectedCost, MatrixXd previousTransitionIntensity, MatrixXd stateCost, MatrixXd transitionCost, int deltaT)
{

	MatrixXd X_previous(1, 6);
	X_previous << previousStateProbability, previousExpectedCost;
	//cout << X_previous << endl;
	double XiU_previous, XiD_previous, XiT_previous, XiE_previous, XiH_previous;
	XiU_previous = stateCost(0) + previousTransitionIntensity.row(0)*transitionCost.row(0).transpose() - previousTransitionIntensity(0, 0)*transitionCost(0, 0);
	XiD_previous = stateCost(1) + previousTransitionIntensity.row(1)*transitionCost.row(1).transpose() - previousTransitionIntensity(1, 1)*transitionCost(1, 1);
	XiT_previous = stateCost(2) + previousTransitionIntensity.row(2)*transitionCost.row(2).transpose() - previousTransitionIntensity(2, 2)*transitionCost(2, 2);
	XiE_previous = stateCost(3) + previousTransitionIntensity.row(3)*transitionCost.row(3).transpose() - previousTransitionIntensity(3, 3)*transitionCost(3, 3);
	XiH_previous = stateCost(4) + previousTransitionIntensity.row(4)*transitionCost.row(4).transpose() - previousTransitionIntensity(4, 4)*transitionCost(4, 4);

	MatrixXd	B_previous(5, 6), A_previous(6, 6);
	B_previous << previousTransitionIntensity.transpose(), MatrixXd::Zero(5, 1);
	//cout << B_previous<<endl;
	A_previous << B_previous, XiU_previous, XiD_previous, XiT_previous, XiE_previous, XiH_previous, 0;
	//cout << A_previous<<endl;
	MatrixXd expA_previous(6, 6);
	expA_previous = MatrixXd::Zero(6, 6);


	//cout << "a*a" << endl;
	//cout << A_previous*A_previous/1 << endl;

	for (int ii = 0; ii < 16; ii++)
	{
		MatrixXd mat = A_previous;
		double result = 1;
		if (ii > 0)
		{
			for (int jj = 1; jj <= ii; jj++)
			{
				result = result*jj;
			}

			for (int kk = 1; kk < ii; kk++)
			{
				mat = mat*A_previous;
			}
			//cout << "mat:" << endl;
			//cout << mat << endl;
			expA_previous = expA_previous + (mat) / result;
		}
		else
		{
			expA_previous = MatrixXd::Identity(6, 6);
		}

		//cout << expA_previous << endl;
	}
	//cout << "expA_previous:" << endl;
	//	cout << expA_previous<<endl;
	X_previous = expA_previous * X_previous.transpose();
	MatrixXd currentStateProbability = X_previous.topRows(5);
	double currentExpectedCost = X_previous(5);
	//global_stateProbability = currentStateProbability;
	//global_expectedCost = currentExpectedCost;
	
}

void markov_init(const double& ud, const double& du, const double& dt, const double& td, const double& te, const double& et, const double& eh,sce::Route_ptr route,sce::Site_Weapon_relation swrelation)
{
	size_t timeStep = 1;
	/*VectorXd timeRoute(2);
	timeRoute << 0, 100;*/

	//Postion of route
	//MatrixXd posRoute(3, 2);
	//posRoute << 0, 0, 0, 100e3, 0, 0;
	//cout << posRoute << endl;
	//MatrixXd xinterp(1, 101), yinterp(1, 101), zinterp(1, 101);
	//for (int i = 0; i < 101; i++)
	//{
	//	xinterp(i) = 0;
	//	yinterp(i) = i * 1000;
	//	zinterp(i) = 0;
	//}
	////

	auto routev = route->getAllWayPoints();

	size_t routepoints = routev.size();
	assert(routepoints);
	MatrixXd xinterp(1, routepoints), yinterp(1, routepoints), zinterp(1, routepoints);
	for (size_t i = 0; i < routepoints; i++)
	{
		xinterp(i) = routev[i].getLongitude();
		yinterp(i) = routev[i].getLatitude();
		zinterp(i) = routev[i].getAltitude();
	}
	size_t weaponsize = swrelation.size();
	MatrixXd posRouteInterp(3, routepoints);
	posRouteInterp << xinterp, yinterp, zinterp;

	MatrixXd posSensor(3, weaponsize), posWeapon(3, weaponsize);
	MatrixXd TempposSensor(weaponsize,3);
	size_t index = 0;
	for (auto &x : swrelation)
	{
		auto w = x.first;
		TempposSensor(index++) = w->getLongitude();
		TempposSensor(index++) = w->getLatitude();
		TempposSensor(index++) = w->getAltitude();
	}
	posWeapon = TempposSensor.transpose();
	
	cout<< posWeapon;
	/*posSensor << 10e3, -10e3, 0e3,
		25e3, 35e3, 85e3,
		0e3, 0e3, 0e3;*/

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


	//reRouteEvaluation(posRouteInterp, posSensor, posWeapon, rangeSensor, rangeWeapon,
	//	outsideTransitionIntensity, sensorTransitionIntensity, weaponTransitionIntensity, stateCost, transitionCost, timeStep);




}

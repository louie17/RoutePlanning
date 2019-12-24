// EntityTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Scenario.hpp"

#include <iostream>
#include <memory>

using namespace sce;

//测试智能指针对局部变量内存释放的影响
std::shared_ptr<Vertex> test1()
{
	return std::make_shared<Vertex>(Vertex(10, 12));
}
std::shared_ptr<Vertex> test2()
{
	Vertex vert(10, 12);
	return std::make_shared<Vertex>(vert);
}
std::shared_ptr<Vertex> test3()
{
	Vertex vert(10, 12);
	auto p = std::make_shared<Vertex>(vert);
	return p;
}
std::shared_ptr<Vertex> test4()
{
	auto p = std::make_shared<Vertex>(Vertex(10, 12));
	return p;
}

int main()
{
	std::cout << "Scenario Class Test!" << std::endl;
	/*std::cout << "Test1:" << test1()->getLatitude() << std::endl;
	std::cout << "Test2:" << test2()->getLatitude() << std::endl;
	std::cout << "Test3:" << test3()->getLatitude() << std::endl;
	std::cout << "Test4:" << test4()->getLatitude() << std::endl;*/

	Vertex ver1(30.0, 25.0), ver2(35.0, 25.0, 0), ver3(35.0, 30.0, 0), ver4(30.0, 35.0, 0);
	std::vector<std::shared_ptr<Vertex>> vertexs{ std::make_shared<Vertex>(ver1),
		std::make_shared<Vertex>(ver2),std::make_shared<Vertex>(ver3),std::make_shared<Vertex>(ver4) };

	Platform p1("Platform1", PlatformType::LAND), p2("Platform2", PlatformType::AIR), p3("Platform3", PlatformType::SURFACE);

	std::vector<std::shared_ptr<Platform>> platforms{ std::make_shared<Platform>(p1),
		std::make_shared<Platform>(p2),std::make_shared<Platform>(p3) };

	//Rf_values rfValues();
	std::vector<Rf_values> rfValues{ Rf_values(1,64,9000,9000,10),Rf_values(2,64,10000,10000,20) };
	Rf rf(9000, 10000, rfValues, RfType::BURST);

	Pw_values pwValues(0, 0, 20, 20, 0);
	Pw pw(20, 20, pwValues, PwType::FIX);

	std::vector<Pri_values> priValues{ Pri_values(1,0,90,90,0),Pri_values(2,0,93,93,0), Pri_values(3,0,97,97,0) };
	Pri pri(90, 100, priValues, PriType::STAGGER);

	Radar_Mode radarMode1("001", ModeType::PULSE, rf, pw, pri, Scan(2, 2, ScanType::SECTORIAL), Erp(50000, 50000));
	Radar_Mode radarMode2("002", ModeType::PULSE, rf, pw, pri, Scan(2, 2, ScanType::SECTORIAL), Erp(100000, 100000));

	std::vector<std::shared_ptr<Radar_Mode>> ptrRadarModes{ std::make_shared<Radar_Mode>(radarMode1) };

	ptrRadarModes.push_back(std::make_shared<Radar_Mode>(radarMode2));

	Emitter emitter1("Emitter1", std::make_shared<Radar_Mode>(radarMode1));
	Emitter emitter2("Emitter2", ptrRadarModes);

	std::vector<std::shared_ptr<Emitter>> emitiers{ std::make_shared<Emitter>(emitter1),std::make_shared<Emitter>(emitter2) };

	//emitter1.getPtr2RadarMode(0)->getPri().getPriValue(0).setMax(1000);
	//层级调用
	std::cout << emitter1.getPtr2RadarMode(0)->getPri().getPriValue(0).getMax();
	std::cout << std::endl;

	Weapon weapon1("Weapon 1", 50000, 100000);
	Weapon weapon2("Weapon 2", 30000, 70000);
	std::vector<std::shared_ptr<Weapon>> weapons{ std::make_shared<Weapon>(weapon1),std::make_shared<Weapon>(weapon2) };


	Site site1("Site1", 100, 32.3, 70.5);
	Site site2("Site2", 80, 32.9, 71.3);
	std::vector<std::shared_ptr<Site>> sites{ std::make_shared<Site>(site1),std::make_shared<Site>(site2) };

	Point startPoint1(0, 30, 40, 0, 0), endPoint1(0, 30, 40, 3600, 3600);
	std::vector<Point> targetP{ Point(0,10,20,500,500), Point(0, 35, 40, 1600, 1900) };
	Mission mission1(MissionType::STRIKE, startPoint1, endPoint1, targetP);

	Point startPoint3(0, 0, 0), endPoint4;
	Mission mission2(MissionType::STRIKE, startPoint3, endPoint4, Point(0, 35, 40, 1600, 1900));
	Mission mission3(MissionType::SUPPORT, startPoint3, endPoint4);

	OwnPlatform oPlatform1("Platform 1", OwnPlatformType::AIR, 9.8, 9.8, 30, 30, 340, 10000, mission1);
	OwnPlatform oPlatform2("Platform 2", OwnPlatformType::AIR, 0, 0, 0, 0, 100, 10000, mission3);
	std::vector<std::shared_ptr<OwnPlatform>> oplatforms{ std::make_shared<OwnPlatform>(oPlatform1), std::make_shared<OwnPlatform>(oPlatform2) };

	Esm esm1("Esm1", 100, 20, 500, 12000, 200, 100), esm2("Esm2", 100, 20, 500, 12000, 200, 100);
	std::vector<std::shared_ptr<Esm>> esms{ std::make_shared<Esm>(esm1),std::make_shared<Esm>(esm2) };

	Ecm ecm1("Ecm1", 20000, 10, 9000, 9500, Tech::NOISE);
	std::vector<Tech> techs{ Tech::NOISE,Tech::VGPO,Tech::FT,Tech::RGPO };
	Ecm ecm2("Ecm2", 20000, 10, 9000, 9500, techs);
	std::vector<std::shared_ptr<Ecm>> ecms{ std::make_shared<Ecm>(ecm1),std::make_shared<Ecm>(ecm2) };

	WayPoint wPoint1(1, 0, 20, 70.3, 0, 200, 0), wPoint2(2, 0, 35.5, 79.1, 1340, 200, 0), wPoint3(3, 0, 33.2, 80.9, 2675, 200, 0);
	std::vector<WayPoint> wPoints{ wPoint1,wPoint2,wPoint3 };

	Route route1("Route1", wPoints), route2("Route2", wPoint1);
	std::vector<std::shared_ptr<Route>> routes{ std::make_shared<Route>(route1),std::make_shared<Route>(route2) };

	Location startLocal1(100, 32.3, 70.5), endLocal1(80, 32.9, 71.3);

	DwellSquence dwellSquence1(1, 100, 200, 0, 30), dwellSquence2(2, 100, 200, 0, 30);
	std::vector<DwellSquence> dwellSs{ dwellSquence1,dwellSquence2 };

	EsmStrategySection esmStraSec1(0, 100, startLocal1, endLocal1, dwellSquence1);
	EsmStrategySection esmStraSec2(0, 100, startLocal1, endLocal1, dwellSs);
	std::vector<std::shared_ptr<EsmStrategySection>> ptrEsmStraSquence{ std::make_shared<EsmStrategySection>(esmStraSec1),std::make_shared<EsmStrategySection>(esmStraSec2) };

	EsmStrategy esmStrategy1("ESM Strategy 1", std::make_shared<EsmStrategySection>(esmStraSec1));
	EsmStrategy esmStrategy2("ESM Strategy 2", ptrEsmStraSquence);
	std::vector<std::shared_ptr<EsmStrategy>> esmStrategies{ std::make_shared<EsmStrategy>(esmStrategy1),std::make_shared<EsmStrategy>(esmStrategy2) };

	Location startLocal12(100, 32.3, 70.5), endLocal2(80, 32.9, 71.3);

	EcmStrategySection ecmStraSec1(0, 100, startLocal1, endLocal1, Tech::NOISE);
	EcmStrategySection ecmStraSec2(0, 100, startLocal1, endLocal1, Tech::FT);
	std::vector<std::shared_ptr<EcmStrategySection>> ptrEcmStraSquence1{ std::make_shared<EcmStrategySection>(ecmStraSec1),std::make_shared<EcmStrategySection>(ecmStraSec1) };
	std::vector<std::shared_ptr<EcmStrategySection>> ptrEcmStraSquence2{ std::make_shared<EcmStrategySection>(ecmStraSec2),std::make_shared<EcmStrategySection>(ecmStraSec2) };
	EcmStrategy ecmStrategy1("ECM Strategy 1", ptrEcmStraSquence1);
	EcmStrategy ecmStrategy2("ECM Strategy 2", ptrEcmStraSquence2);
	std::vector<std::shared_ptr<EcmStrategy>> ecmStrategies{ std::make_shared<EcmStrategy>(ecmStrategy1),std::make_shared<EcmStrategy>(ecmStrategy2) };

	PlatformSiteRelation psRelation1(std::make_shared<Platform>(p1), std::make_shared<Site>(site1));
	PlatformSiteRelation psRelation2(std::make_shared<Platform>(p2), std::make_shared<Site>(site1));
	std::vector<PlatformSiteRelation> psRelations{ psRelation1,psRelation2 };

	PlatformEmitterRelation peRelation1(std::make_shared<Platform>(p2), std::make_shared<Emitter>(emitter1));
	PlatformEmitterRelation peRelation2(std::make_shared<Platform>(p3), std::make_shared<Emitter>(emitter2));
	std::vector<PlatformEmitterRelation> peRelations{ peRelation1,peRelation2 };

	PlatformWeaponRelation pwRelation1(std::make_shared<Platform>(p2), std::make_shared<Weapon>(weapon1));
	PlatformWeaponRelation pwRelation2(std::make_shared<Platform>(p3), std::make_shared<Weapon>(weapon2));
	std::vector<PlatformWeaponRelation> pwRelations{ pwRelation1,pwRelation2 };

	OwnPlatformEsmRelation oesmRelation1(std::make_shared<OwnPlatform>(oPlatform1), std::make_shared<Esm>(esm1));
	std::vector<OwnPlatformEsmRelation> oesmRelations{ oesmRelation1 };

	EsmEsmStrategyRelation eesRelation1(std::make_shared<Esm>(esm1), std::make_shared<EsmStrategy>(esmStrategy2));
	std::vector<EsmEsmStrategyRelation> eesRelations{ eesRelation1 };

	OwnPlatformEcmRelation oecmRelation1(std::make_shared<OwnPlatform>(oPlatform1), std::make_shared<Ecm>(ecm2));
	OwnPlatformEcmRelation oecmRelation2(std::make_shared<OwnPlatform>(oPlatform2), std::make_shared<Ecm>(ecm1));
	std::vector<OwnPlatformEcmRelation> oecmRelations{ oecmRelation1,oecmRelation2 };

	EcmEcmStrategyRelation ecmecmsRelation1(std::make_shared<Ecm>(ecm2), std::make_shared<EcmStrategy>(ecmStrategy2));
	EcmEcmStrategyRelation ecmecmsRelation2(std::make_shared<Ecm>(ecm1), std::make_shared<EcmStrategy>(ecmStrategy1));
	std::vector<EcmEcmStrategyRelation> ecmecmsRelations{ ecmecmsRelation1,ecmecmsRelation2 };

	OwnPlatformRouteRelation orRelation1(std::make_shared<OwnPlatform>(oPlatform1), std::make_shared<Route>(route1));
	OwnPlatformRouteRelation orRelation2(std::make_shared<OwnPlatform>(oPlatform2), std::make_shared<Route>(route2));
	std::vector<OwnPlatformRouteRelation> orRelations{ orRelation1,orRelation2 };

	Scenario scenario(vertexs, platforms, emitiers, weapons, sites, oplatforms, esms, ecms, routes, esmStrategies, ecmStrategies,
		psRelations, peRelations, pwRelations, oesmRelations, eesRelations, oecmRelations, ecmecmsRelations, orRelations);

	std::cout << scenario.getPtr2Vertex(0)->getLatitude() << std::endl;
	std::cout << scenario.getPtr2Platform(0)->getPlatformName() << std::endl;

	std::cout << (scenario.isVertexEmpty() ? "true" : "false");
	std::cout << std::endl;

	system("pause");
	return 0;
}


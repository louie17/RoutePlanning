// MTADE.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <unordered_map>

#include "differential_evolution.hpp"
#include "objective_function.hpp"

#include "Scenario.hpp"

using namespace de;

#define VARS_COUNT 20
#define POPULATION_SIZE 200

int main(int argc, char *argv[])
{
	sce::Scenario scenario;

	//������вλ�û�ȡÿ����в������������
	DVector wcrange(scenario.getAllSite().size(), 0.0);
	sce::Site_WeaponRange_relation swRelation;

	assert(scenario.getAllSite().size() > 0);
	assert(scenario.getAllPlatformSiteRelation().size() > 0);
	for (size_t i = 0; i < scenario.getAllSite().size(); i++)
	{
		auto iterS = scenario.getAllSite().at(i);
		std::vector<unsigned int> siteTmp;
		for (auto iterPSR : scenario.getAllPlatformSiteRelation())
		{
			if (iterPSR.getSiteName() == iterS->getName())
			{
				for (auto iterPWR : scenario.getAllPlatformWeaponRelation())
				{
					if (iterPSR.getPlatformName() == iterPWR.getPlatformName())
					{
						siteTmp.push_back(iterPWR.getWeapon()->getWeaponAreaCoverage());
					}
				}
			}
		}
		assert(siteTmp.size() > 0);
		wcrange[i] = siteTmp.size() > 0 ? *std::max_element(siteTmp.cbegin(), siteTmp.cend()) : 0.0;
		auto ret = swRelation.insert(std::make_pair(iterS, wcrange[i]));
		assert(ret.second);
	}

	try
	{
		/**
		* �������ʼ��constraints����
		*
		* ���ȴ���һ��Ĭ��constraints����(double type, min -1.0e6, max 1.0e6)��
		* Ȼ��Ŀ�꺯���еĸ�����������Ϊʵ�����ͣ�0<=y<=100.
		*/
		constraints_ptr constraints(std::make_shared< constraints >(VARS_COUNT, 1.0e-6, 1.0e6));
		(*constraints)[0] = std::make_shared< real_constraint >(0, 100);//width
		(*constraints)[1] = std::make_shared< real_constraint >(0, 100);//depth
		(*constraints)[2] = std::make_shared< real_constraint >(0, 20);//heigth

		(*constraints)[3] = std::make_shared< real_constraint >(0, 60);//yaw angle
		(*constraints)[4] = std::make_shared< real_constraint >(0, 60);//pitching angle
		(*constraints)[5] = std::make_shared< real_constraint >(0, 60);//rolling angle

		(*constraints)[6] = std::make_shared< real_constraint >(0, 100);//oil
		(*constraints)[7] = std::make_shared< real_constraint >(0, 10000);//speed


		/**
		* ʵ����Ŀ�꺯��
		*
		* ��Ŀ�꺯���������κ���de :: DVectorPtr��Ϊ����������˫���ȵĺ������ӡ� ������ͨ�����ã�ָ�����ָ�봫�ݡ�
		*/
		//sphere_function of;		
		evaluation_route of;

		/**
		* ʵ������������������һ�����ڲ�ֽ�������һ�����ڴ�����
		*/
		listener_ptr listener(std::make_shared< null_listener >());
		processor_listener_ptr processor_listener(std::make_shared< null_processor_listener >());

		/**
		* �ò��д�������������4����Ŀ�꺯����������ʵ�����������ļ��ϡ�
		*/
		//processors< sphere_function >::processors_ptr _processors(std::make_shared< processors< sphere_function > >(4, std::ref(of), processor_listener));
		processors< evaluation_route >::processors_ptr _processors(std::make_shared< processors< evaluation_route > >(4, std::ref(of), constraints, swRelation, processor_listener));
		/**
		* ʵ����һ���򵥵���ֹ���ԣ�������1000��֮��ֹͣ�Ż����̡�
		*/
		termination_strategy_ptr terminationStrategy(std::make_shared< max_gen_termination_strategy >(1000));

		/**
		* ʵ����ѡ�����-���ǽ�ʹ����õĸ�/�Ӳ���
		*/
		selection_strategy_ptr selectionStrategy(std::make_shared< best_parent_child_selection_strategy >());

		/**
		* ʵ�����������-���ǽ�ʹ�ñ������1��Ȩ�غͽ������ӷֱ�����Ϊ0.5��0.9
		*/
		mutation_strategy_arguments mutation_arguments(0.5, 0.9);
		mutation_strategy_ptr mutationStrategy(std::make_shared< mutation_strategy_1 >(VARS_COUNT, mutation_arguments));

		/**
		* ʹ����ǰ�����Լ�������������������͸��ֲ���ʵ������ֽ�����
		*/
		//differential_evolution< sphere_function > de(VARS_COUNT, POPULATION_SIZE, _processors, constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener);
		differential_evolution< evaluation_route > de(VARS_COUNT, POPULATION_SIZE, _processors, constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener);
		/**
		* �����Ż�����
		*/
		de.run();

		/**
		* ���Ż������л����Ѹ���
		*/
		individual_ptr best(de.best());

		/**
		* Print out the result
		*/
		//std::cout << "minimum value for the " << /*of->name() << */" is " << best->cost() << " for x=" << (*best->vars())[0] << ", y=" << (*best->vars())[1] << std::endl;
		std::cout << "minimum value for the " << /*of->name() << */" is " << best->cost()
			<< " for x=" << (*best->vars())[0].longitude()
			<< ", y=" << (*best->vars())[1].latitude()
			<< ", y=" << (*best->vars())[1].altitude() << std::endl;
	}
	catch (const de::exception& e)
	{
		/**
		* Print out any errors that happened during the initialization
		* or optimization phases
		*/
		std::cout << "an error occurred: " << e.what();
	}

	return 0;
}


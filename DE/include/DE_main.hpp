#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_INTERFACE_HPP_INCLUDED
#define DE_INTERFACE_HPP_INCLUDED

#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <unordered_map>

#include "differential_evolution.hpp"
#include "objective_function.hpp"
#include "de_types.hpp"
#include "Scenario.hpp"

namespace de
{
	//typedef std::unordered_map<std::shared_ptr<sce::Site>, double> Site_WeaponRange_relation;
	/**
	* DE�㷨���ͳһ���ýӿں���
	*
	* @author louiehan 11/28/2019
	*
	* @param scenario �㷨��������ĳ������������ʵ�壻
	* @param population_size ����Ⱥ��������
	* @param initial_Node_Number ������ʼ����ֹ�����⣬ÿ���������������Ľڵ�������Ӧ��varCount��
	* @param evolution_Number ��������������ֹ���ԣ�
	* @param factor ���Ϊtrue����������С���ɱ������Ϊfalse���򽫳�����󻯳ɱ���
	* @param terminationStrategy ��ֹ���ԣ�
	* @param selectionStrategy ѡ����ԣ�
	* @param mutationStrategy ������ԣ�
	* @param listener ��������
	*/
	int De_alg(sce::Site_WeaponRange_relation &swRelation,sce::Vertexs &vertexs,sce::Point &startPoint, sce::Point &endPoint,sce::Route_ptr route, const size_t &population_size, const size_t &initial_Node_Number, const size_t &evolution_Number, const double &weight, const double &cross_Probability)
	{
		try
		{
			/**
			* �������ʼ��constraints����
			*
			* ���ȴ���һ��Ĭ��constraints����(double type, min -1.0e6, max 1.0e6)��
			* Ȼ��Ŀ�꺯���еĸ�����������Ϊʵ�����ͣ�0<=y<=100.
			*/
			constraints_ptr constraints(std::make_shared< constraints >(initial_Node_Number, 1.0e-6, 1.0e6));

			//TODO: �ҳ����������ڵ�������
			DVector v_Altitudes,v_Latitudes,v_Longitudes;
			for (size_t i=0;i<vertexs.size();++i)
			{
				auto iter= vertexs.at(i);
				v_Altitudes.push_back(iter->getAltitude());
				v_Latitudes.push_back(iter->getLatitude());
				v_Longitudes.push_back(iter->getLongitude());
			}
			
			(*constraints)[0] = std::make_shared< real_constraint >(*std::min_element(v_Longitudes.cbegin(),v_Longitudes.cend()), *std::max_element(v_Longitudes.cbegin(), v_Longitudes.cend()));//longitude
			(*constraints)[1] = std::make_shared< real_constraint >(*std::min_element(v_Latitudes.cbegin(), v_Latitudes.cend()), *std::max_element(v_Latitudes.cbegin(), v_Latitudes.cend()));//latitude
			(*constraints)[2] = std::make_shared< real_constraint >(*std::min_element(v_Altitudes.cbegin(), v_Altitudes.cend()), *std::max_element(v_Altitudes.cbegin(), v_Altitudes.cend()));//altitude

			(*constraints)[3] = std::make_shared< real_constraint >(-60, 60);//yaw angle
			(*constraints)[4] = std::make_shared< real_constraint >(-60, 60);//pitching angle
			(*constraints)[5] = std::make_shared< real_constraint >(-60, 60);//rolling angle

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
			termination_strategy_ptr terminationStrategy(std::make_shared< max_gen_termination_strategy >(evolution_Number));

			/**
			* ʵ����ѡ�����-���ǽ�ʹ����õĸ�/�Ӳ���
			*/
			selection_strategy_ptr selectionStrategy(std::make_shared< best_parent_child_selection_strategy >());

			/**
			* ʵ�����������-���ǽ�ʹ�ñ������1��Ȩ�غͽ������ӷֱ�����Ϊ0.5��0.9
			*/
			mutation_strategy_arguments mutation_arguments(weight, cross_Probability);
			mutation_strategy_ptr mutationStrategy(std::make_shared< mutation_strategy_1 >(initial_Node_Number, mutation_arguments));

			/**
			* ʹ����ǰ�����Լ�������������������͸��ֲ���ʵ������ֽ�����
			*/
			//differential_evolution< sphere_function > de(VARS_COUNT, POPULATION_SIZE, _processors, constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener);
			Node startNode,endNode;
			startNode(startPoint);
			endNode(startNode);
			differential_evolution< evaluation_route > de(initial_Node_Number, population_size, _processors,startNode,endNode,constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener);
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
				<< " for x=" << (*best->vars())[1].longitude()
				<< ", y=" << (*best->vars())[1].latitude()
				<< ", y=" << (*best->vars())[1].altitude() << std::endl;

			route->setWayPoint(0,sce::WayPoint(best->vars()->at(0).longitude(), best->vars()->at(0).latitude(), best->vars()->at(0).altitude()));
			for (size_t iter = 1;iter < best->vars()->size();++iter)
			{
				Node node(best->vars()->at(iter));
				route->addWayPoint(sce::WayPoint(iter, node.longitude(), node.latitude(), node.altitude()));
			} 

			return 0;
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
}

#endif
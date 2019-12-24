#ifndef DE_OBJECTIVE_FUNCTION_HPP_INCLUDED
#define DE_OBJECTIVE_FUNCTION_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <memory>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <valarray>

#include "de_types.hpp"
#include "processors.hpp"
#include "de_constraints.hpp"
#include "Scenario.hpp"

#define PI 3.14159265

/**
* ����Ŀ�꺯���ĳ�����ࡣ
*
* @author louiehan (11/18/2019)
*/
class objective_function
{
private:
	const std::string m_name;
public:
	/**
	* constructs an objective_function object
	*
	* @author louiehan (11/18/2019)
	*
	* @param name the objective function name
	*/
	objective_function(const std::string& name)
		: m_name(name)
	{
	}

	virtual ~objective_function() {}

	/**
	* ����������ʵ��-������Ҫ�Ż���Ŀ�꺯����
	*
	* Ŀ�꺯������һ��double���͵Ĳ������������ؼ������doubleֵ��
	*
	* args�����е�ÿ��������Ӧ��Լ�������е���ͬ���������磬���Ŀ�꺯����Ҫ����������
	* ��Լ��������ǰ����Ԫ������Ϊ����������Լ��������Ŀ�꺯��operator������ʹ��
	* args������ǰ����ֵ��Ϊ�������������Ժ��Դ������е���������ֵ��
	*
	* @author louiehan (11/18/2019)
	*
	* @param args ��������������ͨ����Ŀ�꺯��ʹ�õı�������ö࣬
	*			  ���Ŀ�꺯��ofֻȡ�����е�ǰn��ֵ�������������ֵ��
	*
	* @return double �������ۣ�������Ҫ�Ż���ֵ��
	*/
	//virtual double operator()(de::DVectorPtr args) = 0;
	//virtual double operator()(de::NVectorPtr args) = 0;
	virtual double operator()(de::NVectorPtr args, de::constraints_ptr constraints, const sce::Site_WeaponRange_relation swRelation) = 0;

	/**
	* An objective function has a name
	*
	* @author louiehan (11/18/2019)
	*
	* @return const std::string&
	*/
	const std::string& name() const { return m_name; }
};

/**
* Smart pointer to an objective function
*/
typedef std::shared_ptr< objective_function > objective_function_ptr;

/**
* Objective function to optimize is the "sphere function":
*
* f(x,y) = x^2 + y^2
*/
class sphere_function :public objective_function
{
public:
	sphere_function()
		: objective_function("sphere function")
	{
	}

	virtual double operator()(de::DVectorPtr args)
	{
		/**
		* The two function arguments are the elements index 0 and 1 in
		* the argument vector, as defined by the constraints vector
		* below
		*/
		double x = (*args)[0];
		double y = (*args)[1];

		return x*x + y*y;
	}
};

class evaluation_route :public objective_function
{
public:
	evaluation_route()
		: objective_function("Route evaluation function")
	{
	}

	/**
	* ����·�����ȴ���
	* 
	* @author louie (11/28/2019)
	*
	* @param args[input] ÿ������Ľڵ㼯�ϣ�����ֻ�õ�ÿ������·������ʼ������ֹ��
	*		 norms[input] ������ڵ�֮��ĳ���ֵ
	*		 length_cost[output] ������ĳ��ȴ���ֵ
	*/
	double evaluation_length_cost(de::NVectorPtr args, const std::valarray<double> &norms)
	{
		double route_length = 0.0;
		for (auto iter:norms)
		{
			route_length += iter;
		}
		//��һ��
		//double rate = route_length/(*(args->end()) - *(args->begin())).norm();
		//return rate>=2 ? 1.0 : rate-1;
		return route_length;
	}

	/**
	* ����·�����ȱ�׼�����
	*
	* @author louie (11/28/2019)
	*
	* @param norms[input] ������ڵ�֮��ĳ���ֵ
	*		 std_variance_cost[output] ������ı�׼�����ֵ
	*/
	double evaluation_std_variance_cost(const std::valarray<double> &norms)
	{
		double sum = norms.sum();
		double mean = sum / norms.size(); //��ֵ  

		double accum = 0.0;
		for(auto d:norms) {
			accum += (d - mean)*(d - mean);
		}
		//��һ��
		//return sqrt(accum / (norms.size() - 1))>1 ? 1-1/sqrt(accum / (norms.size() - 1)) : 0.0; //��׼��
		return sqrt(accum / (norms.size() - 1));
	}

	/**
	* ����ƫת�Ƕȴ���
	*
	* @author louie (11/28/2019)
	*
	* @param diff_vector[input] ������ڵ�֮��Ĳ������
	*		 norms[input] ������ڵ�֮��ĳ���ֵ
	*		 constraints[input] ����ƫ���ǡ������ǡ���ת������ֵ��Լ������
	*		 angle_cost[output] ������ĳ��ȴ���ֵ
	*/
	double evalution_angle_cost(const de::NVector &diff_vector, const std::valarray<double> &norms, de::constraints_ptr constraints)
	{
		//�����н�
		std::valarray<double> vetorial_angle(0.0,diff_vector.size()-1);
		//ƫ����yaw angle
		std::valarray<double> yaw_angles(0.0, diff_vector.size() - 1);
		//������pitching angle
		std::valarray<double> pitching_angles(0.0, diff_vector.size() - 1);
		//��ת��rolling angle
		std::valarray<double> rolling_angles(0.0, diff_vector.size() - 1);


		for (size_t i = 0; i < diff_vector.size()-1; i++)
		{
			double tmp=diff_vector[i] * diff_vector[i + 1] / (norms[i] * norms[i + 1]);
			de::Node tmpNode = diff_vector[i + 1] - diff_vector[i];
			//�Ծ�����Ϊ�ο���
			//��ֹatan2(double y,double x)��������ͬΪ0ʱ�����������Ȼ��MS�����ĵ���������������ͬΪ0ʱ�����Ҳ��0����ҲҪԤ����Щ�Ͱ汾���빦�ܲ�֧�ֵ�����
			if (tmpNode.altitude()==0.0 && tmpNode.longitude()==0.0) 
			{
				pitching_angles[i] = 0;
			}
			else
			{
				pitching_angles[i] = atan2(tmpNode.altitude(), tmpNode.longitude())*180.0 / PI;
			}

			if (tmpNode.latitude()==0.0 && tmpNode.altitude()==0.0)
			{
				rolling_angles[i] = 0;
			}
			else
			{
				rolling_angles[i] = atan2(tmpNode.latitude(), tmpNode.altitude())*180.0 / PI;
			}

			if (tmpNode.latitude()==0.0 && tmpNode.longitude()==0.0)
			{
				yaw_angles[i] = 0;
			}
			else
			{
				yaw_angles[i] = atan2(tmpNode.latitude(), tmpNode.longitude())*180.0 / PI;
			} 
						
			if ( tmp>=0 && tmp <= 1 )
			{
				vetorial_angle[i] = acos(tmp) * 180.0 / PI;
			}
			else
			{ 
				vetorial_angle[i] = (tmp>1) ? 0.0 : 180.0;
			}
		}
		 
		//�������ת��ǳ�����ȫ���ƻ�ʩ�ӳͷ�,���д���ֵ����һ��
		double yaw_angle_cost = (abs(yaw_angles.max()) >= (*constraints)[3]->max()) ? 1.0 : 0.0;
		double pitching_angle_cost = (abs(pitching_angles.max()) >= (*constraints)[4]->max()) ? 1.0 : 0.0;
		double rolling_angle_cost = (abs(rolling_angles.max()) >= (*constraints)[5]->max()) ? 1.0 : 0.0;

		//angle_cost = vetorial_angle.max();
		//��һ��
		//return (yaw_angle_cost>0 || pitching_angle_cost>0 || rolling_angle_cost>0) ? 1.0 : 0.0;

		return abs(yaw_angles.max()) + abs(pitching_angles.max()) + abs(rolling_angles.max());
	}

	/**
	* ������δ���
	*
	* @author louie (11/28/2019)
	*
	* @param diff_vector[input] ������ڵ�֮��Ĳ������
	*		 norms[input] ������ڵ�֮��ĳ���ֵ
	*		 constraints[input] ����ƫ���ǡ������ǡ���ת������ֵ��Լ������
	*		 angle_cost[output] ������ĳ��ȴ���ֵ
	*/
	double evaluation_route_tabu_cost(de::NVectorPtr args, de::constraint_ptr high_constraint)
	{
		double tabu_cost(0.0);
		//������ʱû�е�����Ϣ������ֻ��������Ͱ�ȫ���и߶Ƚ��бȽ�
		std::valarray<double> heights(0.0, args->size() - 1);
		for (size_t i=0; i<args->size();i++)
		{
			heights[i]=(*args)[i].altitude();

			tabu_cost += heights[i] - high_constraint->min();
		}
		//��һ��
		//return heights.min() > high_constraint->min() ? 0.0 : 1.0;

		return tabu_cost;
	}

	double evaluation_route_mission_cost(de::NVectorPtr args)
	{
		double route_cost(0.0);
		std::vector<double> ermc(args->size()-1,0.0);
		for (size_t i = 0; i < args->size() - 1; i++)
		{
			ermc[i] = (*args->cend() - (*args)[i]).norm();
		}

		for (size_t j = 0; j < ermc.size() - 1; j++)
		{
			route_cost += ermc[j + 1] / ermc[j];
		}
		return route_cost;
	}

	double evaluation_threat_cost(const de::NVectorPtr args, const sce::Site_WeaponRange_relation swRelation)
	{
		constexpr double K_j = 1.0;//��вϵ��
		double threat_cost = 0.0;

		for (size_t i = 0; i < args->size(); i++)
		{
			for (const auto &map_it:swRelation)
			{
				double dist = (args->at(i) - *map_it.first).norm();
				threat_cost += dist<map_it.second ? K_j / pow(dist, 4) : 0.0;
			}
		}		
		return threat_cost;
	}

	double evaluation_route_survival_cost(de::NVectorPtr args)
	{
		return 0.0;
	}

	virtual double operator()(de::NVectorPtr args,de::constraints_ptr constraints,const sce::Site_WeaponRange_relation swRelation)
	{
		double tabu_cost(0.0), std_variance_cost(0.0),mission_cost(0.0),survival_cost(0.0), length_cost(0.0), angle_cost(0.0),threat_cost(0.0);

		de::NVector diff_vector(args->size() - 1, args->at(0));
		//DVector norms(args->size() - 1, 0);
		std::valarray<double> norms(0.0, args->size() - 1);
		

		for (size_t i = 0; i <args->size() - 1; i++)
		{
			diff_vector[i] = (*args)[i + 1] - (*args)[i];
			norms[i] = ((*args)[i + 1] - (*args)[i]).norm();
		}

		// �������и߶Ⱥ͵���
		tabu_cost = evaluation_route_tabu_cost(args,(*constraints)[2]);
		// �����������ľ���
		mission_cost = evaluation_route_mission_cost(args);
		//�������ȴ���
		length_cost = evaluation_length_cost(args, norms);
		//�����Ƕȴ���
		angle_cost = evalution_angle_cost(diff_vector,norms,constraints);
		//�����������
		std_variance_cost = evaluation_std_variance_cost(norms);
		//������в����
		threat_cost = evaluation_threat_cost(args, swRelation);

		// �����������,��������������������
		//survival_cost = evaluation_route_survival_cost(args);

		// ��Ӧ��
		double cost = 0.2*length_cost + 0.2*angle_cost + 0.1*mission_cost + 0.1*tabu_cost+ 0.2*std_variance_cost+0.2*threat_cost;

		return cost;
	}

};

#endif //DE_OBJECTIVE_FUNCTION_HPP_INCLUDED
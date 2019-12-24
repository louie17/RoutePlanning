// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_INDIVIDUAL_HPP_INCLUDED
#define DE_INDIVIDUAL_HPP_INCLUDED

#include <queue>
#include <sstream>

#include "de_types.hpp"
#include "de_constraints.hpp"
#include "multithread.hpp"

namespace de
{
	class individual;
	typedef std::shared_ptr< individual > individual_ptr;

	/**
	 * ��ֽ�����Ⱥ�еĸ����ࡣ
	 *
	 * ÿ��������һ�����������Щ������صĴ��ۡ�
	 *
	 * ÿ���������̰߳�ȫ�ġ�
	 *
	 * @author louiehan (11/11/2019)
	 */
	class individual
	{
	private:
		de::NVectorPtr m_vars;
		double m_cost;
		de::mutex m_mx;

	public:
		/**
		 * ��������캯��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param varCount ÿ������ı���������
		 */
		individual(size_t varCount)
			: m_vars(std::make_shared< de::NVector >(varCount))
		{
		}

		/**
		 * ��������캯��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param vars �������Ƶ��ڲ�����vector�����еı���vector��������
		 */
		individual(const de::NVector& vars)
			: m_vars(std::make_shared< de::NVector >(vars))
		{
		}

		/**
		 * ʹ��Լ�������ڵ����ֵ��ʼ���������ڲ�vector��������
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param constraints
		 */
		void init(constraints_ptr constraints)
		{
			assert(constraints);
			assert(m_vars);
			assert(m_vars->size() == constraints->size());

			for (de::NVector::size_type j = 0; j < m_vars->size(); ++j)
			{
				(*m_vars)[j].setAltitude(constraints->get_rand_value(0));
				(*m_vars)[j].setLatitude(constraints->get_rand_value(1));
				(*m_vars)[j].setLongitude(constraints->get_rand_value(2));
			}
				
		}

		/**
		* ʹ��Լ�������ڵ����ֵ��ʼ���������ڲ�vector��������
		*
		* @author louiehan (11/11/2019)
		*
		* @param constraints
		* @param startPoint,endPoint ·����ʼ����ֹ��
		*/
		void init(const Node &startPoint, const Node &endPoint, constraints_ptr constraints)
		{
			assert(constraints);
			assert(m_vars);
			//assert(m_vars->size() == constraints->size());

			m_vars->at(0) = startPoint;
			for (de::NVector::size_type j = 1; j < m_vars->size()-1; ++j)
			{
				m_vars->at(j) = de::Node(constraints->get_rand_value(0), constraints->get_rand_value(1), constraints->get_rand_value(2));
			}
			//m_vars->insert(m_vars->begin(), de::Node(startPoint.getAltitude(), startPoint.getLatitude(), startPoint.getLongitude()));
			//m_vars->insert(m_vars->end(), de::Node(endPoint.getAltitude(), endPoint.getLatitude(), endPoint.getLongitude()));
			m_vars->at(m_vars->size() - 1) = endPoint;
		}

		/**
		 * ���ظ���Ĵ���ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double cost() const { return m_cost; }

		/**
		 * ʹ��ԭ��;�ֵ����������ΪԼ���ڵ������ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param constraints
		 * @param origin
		 */
		void ensureConstraints(constraints_ptr constraints, de::NVectorPtr origin)
		{
			assert(constraints);
			assert(m_vars);
			assert(origin);
			//assert(m_vars->size() == constraints->size());

			for (de::NVector::size_type j = 0; j < m_vars->size(); ++j)
			{
				(*m_vars)[j].setAltitude(constraints->get_rand_value(0, (*m_vars)[j].altitude(), (*origin)[j].altitude()));
				(*m_vars)[j].setLatitude(constraints->get_rand_value(1, (*m_vars)[j].latitude(), (*origin)[j].latitude()));
				(*m_vars)[j].setLongitude(constraints->get_rand_value(2, (*m_vars)[j].longitude(), (*origin)[j].longitude()));
			}
		}

		/**
		 * returns the internal variable vector
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return de::NVectorPtr
		 */
		de::NVectorPtr vars() const { return m_vars; }

		/**
		 * �����������ضԱ���ֵ�ķǳ������ã���������������ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index Ҫ���������õı���������
		 *
		 * @return de::Node&
		 */
		de::Node& operator[](size_t index) { return (*m_vars)[index]; }

		/**
		 * �����������ضԱ���ֵ�ĳ������ã���������������ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index Ҫ���������õı���������
		 *
		 * @return de::Node&
		 */
		const de::Node& operator[](size_t index) const { return (*m_vars)[index]; }

		/**
		 * �趨����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param cost
		 */
		void setCost(double cost) { m_cost = cost; }

		/**
		 * ���ݴ��۽���ǰ��������һ��������бȽϡ������ǰ���۵��ڻ������һ������Ĵ��ۣ��򷵻�true��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind Ҫ�Ƚϵĸ���
		 *
		 * @return bool
		 */
		bool operator<=(const individual& ind) const
		{
			assert(ind.size() == size());
			return cost() <= ind.cost();
		}

		/**
		 * ���ݴ��۽���ǰ��������һ��������бȽϡ������ǰ�����ϸ������һ������Ĵ��ۣ��򷵻�true��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind Ҫ�Ƚϵĸ���
		 *
		 * @return bool
		 */
		bool operator<(const individual& ind) const
		{
			assert(ind.size() == size());
			return cost() < ind.cost();
		}

		/**
		 * ���ݴ��۽���ǰ��������һ��������бȽϡ������ǰ�������ڻ������һ������Ĵ��ۣ�
		 * �򷵻�true���ŵı�׼��С�ڻ��Ǵ���ȡ����minimize��־�Ǹ��ͻ��Ǹ��ߡ�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind Ҫ�Ƚϵĸ���
		 * @param minimize <= if true, >= if false
		 *
		 * @return bool
		 */
		bool better_or_equal(const individual_ptr ind, bool minimize) const
		{
			assert(ind);
			return minimize ? *this <= *ind : *ind <= *this;
		}

		/**
		 * ���ݴ��۽���ǰ��������һ��������бȽϡ������ǰ�����ϸ�������һ������Ĵ��ۣ�
		 * �򷵻�true���ŵı�׼��С�ڻ��Ǵ���ȡ����minimize��־�Ǹ��ͻ��Ǹ��ߡ�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind Ҫ�Ƚϵĸ���
		 * @param minimize < if true, > if false
		 *
		 * @return bool
		 */
		bool better(const individual_ptr ind, bool minimize) const
		{
			assert(ind);
			return minimize ? *this < *ind : *ind < *this;
		}

		/**
		 * ����vector�����ڱ����ĸ���
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return size_t
		 */
		size_t size() const { return m_vars->size(); }

		/**
		 * ���ַ�������ʽ���ظ����ڲ��ģ����ۺͽڵ��б���
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return std::string
		 */
		std::string to_string() const
		{
			std::ostringstream os;

			os << "cost: " << cost() << ", Nodes: ";

			for (de::NVector::size_type j = 0; j < m_vars->size(); ++j)
			{
				os <<"["<< (*m_vars)[j].altitude() << ", "<< (*m_vars)[j].latitude()<<", "<< (*m_vars)[j].longitude()<< "] ";
			}

			return os.str();

		}
	};

	typedef std::queue< individual_ptr > individual_queue_base;

	/**
	 * �̰߳�ȫ�ĸ������
	 *
	 * ���ڽ�����Ҫ�ɲ�ͬ���������ݸ�Ŀ�꺯���Ĳ����ĸ����Ŷӡ�
	 *
	 * @author louiehan (11/11/2019)
	 */
	class individual_queue : public individual_queue_base
	{
	private:
		de::mutex m_mx;

	public:
		/**
		 * �ڶ��еײ�����µĸ��塣
		 *
		 * ���̰߳�ȫ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param ind Ҫ��������еĸ���
		 */
		void push_back(individual_ptr ind)
		{
			de::lock lock(m_mx);

			individual_queue_base::push(ind);
		}

		/**
		 * �Ӷ��ж���ɾ�����壨����У������ء�
		 *
		 * �̰߳�ȫ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return individual_ptr ���ж����ĸ��壻�������Ϊ�գ���Ϊnull
		 */
		individual_ptr pop()
		{
			de::lock lock(m_mx);

			if (!individual_queue_base::empty())
			{
				individual_ptr p(individual_queue_base::front());

				individual_queue_base::pop();

				return p;
			}
			else
				return individual_ptr();
		}
	};

}

#endif //DE_INDIVIDUAL_HPP_INCLUDED

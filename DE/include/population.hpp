#ifndef DE_POPULATION_HPP_INCLUDED
#define DE_POPULATION_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "individual.hpp"

namespace de
{

	typedef std::vector< individual_ptr > population_base;

	/**
	 * ��Ⱥ��
	 *
	 * @author louiehan (11/15/2019)
	 */
	class population : public population_base
	{
	public:
		/**
		 * �������δ��ʼ���������Ⱥ����
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param popSize ��Ⱥ��ģ(��������)
		 * @param varCount ÿ������ı�������
		 */
		population(size_t popSize, size_t varCount)
			: population_base(popSize)
		{
			assert(popSize > 0);
			assert(varCount > 0);

			init(popSize, varCount);
		}

		/**
		 * ����һ����Ⱥ����ͨ�����Ƚ����б�������Ϊ����ӦԼ����ʩ�ӵ������ڵ�
		 * ���ֵ����ʼ������壬Ȼ��Ϊÿ����������Ŀ�꺯���Լ�����ش��ۡ�
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param popSize ��Ⱥ��ģ(��������)
		 * @param varCount ÿ������ı�������
		 * @param constraints ��ʼ������ʱʹ�õ�Լ������
		 */
		population(size_t popSize, size_t varCount, constraints_ptr constraints)
			: population_base(popSize)
		{
			assert(popSize > 0);
			assert(varCount > 0);

			init(popSize, varCount);

			for (population::size_type i = 0; i < size(); ++i)
				at(i)->init(constraints);
		}

		/**
		* ����һ����Ⱥ����ͨ�����Ƚ����б�������Ϊ����ӦԼ����ʩ�ӵ������ڵ�
		* ���ֵ����ʼ������壬Ȼ��Ϊÿ����������Ŀ�꺯���Լ�����ش��ۡ�
		*
		* @author louiehan (11/15/2019)
		*
		* @param popSize ��Ⱥ��ģ(��������)
		* @param varCount ÿ������ı�������
		* @param startPoint,endPoint ·����ʼ����ֹ��
		* @param constraints ��ʼ������ʱʹ�õ�Լ������
		*/
		population(size_t popSize, size_t varCount, const Node &startPoint, const Node &endPoint, constraints_ptr constraints)
			: population_base(popSize)
		{
			assert(popSize > 0);
			assert(varCount > 0);

			init(popSize, varCount+2);//��������ʼ������ֹ��

			for (population::size_type i = 0; i < size(); ++i)
				at(i)->init(startPoint,endPoint,constraints);
		}

		/**
		 * ����һ����Ⱥ����õĸ���
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return individual_ptr
		 */
		individual_ptr best(bool minimize) const
		{
			population::size_type best(0);

			for (population::size_type i = 0; i < size(); ++i)
				best = at(i)->better_or_equal(at(best), minimize) ? i : best;

			return at(best);
		}

		/**
		 * ������Ⱥ���ַ�����ʾ��ʽ
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return std::string string representation of the population
		 */
		std::string to_string()
		{
			std::string str;
			for (population::size_type i = 0; i < size(); ++i)
				str += at(i)->to_string();

			return str;
		}

	private:
		void init(size_t popSize, size_t varCount)
		{
			for (population_base::size_type i = 0; i < size(); ++i)
				operator[](i) = std::make_shared< individual >(varCount);
		}
	public:

	};

	typedef std::shared_ptr< population > population_ptr;

}

#endif //DE_POPULATION_HPP_INCLUDED

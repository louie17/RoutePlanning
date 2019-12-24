#ifndef DE_MUTATION_STRATEGY_HPP_INCLUDED
#define DE_MUTATION_STRATEGY_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "population.hpp"

#define URN_DEPTH 5

#include <tuple>

namespace de
{

	/**
	 * �������Ȩ�����ӣ��������ӺͶ������ӣ�����ǰ������������ó�����ʹ�õĲ�����
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_arguments
	{
	private:
		const double m_weight;
		const double m_crossover;
		const double m_dither;

	public:
		/**
		 * ����һ��������������
		 *
		 * ���˵��ô����ṩ��Ȩ�غͽ�������֮�⣬�ö��󻹰���һ���������ӣ�
		 * �ñ����������ڹ���ʱ��������ģ�����ĳЩͻ�����ʹ�á�
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param weight Ȩ�����ӣ���ֽ����㷨����Ľ���0-2֮���doubleֵ��
		 * @param crossover �������� ��ֽ����㷨����Ľ���0-1֮���doubleֵ��
		 */
		mutation_strategy_arguments(double weight, double crossover)
			: m_weight(weight), m_crossover(crossover), m_dither(weight + genrand() * (1.0 - weight))
		{
			// todo: test or assert that weight and crossover are within bounds (0-1, 0-2 or something)
		}

		/**
		 * ����Ȩ������
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double weight() const { return m_weight; }
		/**
		 * ���ؽ�������
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double crossover() const { return m_crossover; }
		/**
		 * ���ر�������
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double dither() const { return m_dither; }
	};

	/**
	 * ������Եĳ������
	 *
	 * ������Զ��������Ż���������ε�������ֵ��
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy
	{
	private:
		mutation_strategy_arguments m_args;
		size_t m_varCount;

	protected:
		/**
		 * �ñ����������һ��4�������С������Ϊ������
		 *
		 * ��Щ���ֱ���ȫ����ͬ�����ұ������ⲿ�ṩ��������ͬ��
		 *
		 * @author louiehan (11/15/2019)
		 */
		class Urn
		{
			size_t m_urn[URN_DEPTH];

		public:
			/**
			 * Constructs an urn object
			 *
			 * @author louiehan (11/15/2019)
			 *
			 * @param NP ���ɵ�����������ޣ���������
			 * @param avoid ���������ʱҪ�����ֵ
			 */
			Urn(size_t NP, size_t avoid)
			{
				do m_urn[0] = genintrand(0, NP, true); while (m_urn[0] == avoid);
				do m_urn[1] = genintrand(0, NP, true); while (m_urn[1] == m_urn[0] || m_urn[1] == avoid);
				do m_urn[2] = genintrand(0, NP, true); while (m_urn[2] == m_urn[1] || m_urn[2] == m_urn[0] || m_urn[2] == avoid);
				do m_urn[3] = genintrand(0, NP, true); while (m_urn[3] == m_urn[2] || m_urn[3] == m_urn[1] || m_urn[3] == m_urn[0] || m_urn[3] == avoid);
			}

			/**
			 * �������ɵ��ĸ������֮һ
			 *
			 * @author louiehan (11/15/2019)
			 *
			 * @param index Ҫ���ص��������������������0-3֮�䡣
			 *
			 * @return size_t
			 */
			size_t operator[](size_t index) const { assert(index < 4); return m_urn[index]; }
		};


	public:
		virtual ~mutation_strategy()
		{
		}

		/**
		 * constructs a mutation strategy
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount ��������
		 * @param args ������Բ�����
		 */
		mutation_strategy(size_t varCount, const mutation_strategy_arguments& args)
			: m_args(args), m_varCount(varCount)
		{
		}

		/**
		 * �����������Ա���ص�Ԫ������͡�
		 */
		typedef std::tuple< individual_ptr, de::NVectorPtr > mutation_info;

		/**
		 * ִ�б���
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param pop ��ǰ��Ⱥ�����ã�
		 * @param bestIt ��һ����õĸ��壻
		 * @param i ��ǰ�����������
		 *
		 * @return mutation_info Ԫ�飨tuple������������ĸ����װ�������������ͬ��С��doubleֵ��vector������
		 *						 �����ǳ�����ӦԼ����ʩ�ӵ����Ƶ�����£�����ԭֵ��������ֵ��
		 */
		virtual mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i) = 0;

		/**
		 * returns the number of variables
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return size_t
		 */
		size_t varCount() const { return m_varCount; }

		/**
		 * ����Ȩ������
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double weight() const { return m_args.weight(); }

		/**
		 * ���ؽ�������
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double crossover() const { return m_args.crossover(); }

		/**
		 * ���ر�������
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return double
		 */
		double dither() const { return m_args.dither(); }
	};

	typedef std::shared_ptr< mutation_strategy > mutation_strategy_ptr;

	/**
	 * ������� DE/rand/1/bin #1
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_1 : public mutation_strategy
	{
	public:
		/**
		 * constructs a mutation strategy # 1
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount ��������
		 * @param args ������Բ�����
		 */
		mutation_strategy_1(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* ִ�б���
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop ��ǰ��Ⱥ�����ã�
		* @param bestIt ��һ����õĸ��壻
		* @param i ��ǰ�����������
		*
		* @return mutation_info Ԫ�飨tuple������������ĸ����װ�������������ͬ��С��doubleֵ��vector������
		*						�����ǳ�����ӦԼ����ʩ�ӵ����Ƶ�����£�����ԭֵ��������ֵ��
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);


			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			do
			{
				(*tmpInd->vars())[j] = (*pop[urn[0]]->vars())[j] + weight() * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();

			return mutation_info(tmpInd, origin);
		}

	};

	/**
	 * �������2 De/rand-to-best/bin # 2
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_2 : public mutation_strategy
	{
	public:
		/**
		 * constructs a mutation strategy # 2
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount ��������
		 * @param args ������Բ�����
		 */
		mutation_strategy_2(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* ִ�б���
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop ��ǰ��Ⱥ�����ã�
		* @param bestIt ��һ����õĸ��壻
		* @param i ��ǰ�����������
		*
		* @return mutation_info Ԫ�飨tuple������������ĸ����װ�������������ͬ��С��doubleֵ��vector������
		*						�����ǳ�����ӦԼ����ʩ�ӵ����Ƶ�����£�����ԭֵ��������ֵ��
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);


			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			do
			{
				(*tmpInd->vars())[j] = (*tmpInd->vars())[j] +
					weight() * ((*bestIt->vars())[j] - (*tmpInd->vars())[j]) +
					weight() * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();

			return mutation_info(tmpInd, origin);
		}

	};

	/**
	 * ������� DE/best/1/bin # 3
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_3 : public mutation_strategy
	{
	public:
		/**
		 * constructs a mutation strategy # 3
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount ��������
		 * @param args ������Բ�����
		 */
		mutation_strategy_3(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* ִ�б���
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop ��ǰ��Ⱥ�����ã�
		* @param bestIt ��һ����õĸ��壻
		* @param i ��ǰ�����������
		*
		* @return mutation_info Ԫ�飨tuple������������ĸ����װ�������������ͬ��С��doubleֵ��vector������
		*						�����ǳ�����ӦԼ����ʩ�ӵ����Ƶ�����£�����ԭֵ��������ֵ��
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);


			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			do
			{
				double jitter = (0.0001* genrand() + weight());

				(*tmpInd->vars())[j] = (*bestIt->vars())[j] + jitter * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();

			return mutation_info(tmpInd, origin);
		}
	};

	/**
	 * ������� # 4
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_4 : public mutation_strategy
	{
	public:
		/**
		 * constructs a mutation strategy # 4
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount ��������
		 * @param args ������Բ�����
		 */
		mutation_strategy_4(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* ִ�б���
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop ��ǰ��Ⱥ�����ã�
		* @param bestIt ��һ����õĸ��壻
		* @param i ��ǰ�����������
		*
		* @return mutation_info Ԫ�飨tuple������������ĸ����װ�������������ͬ��С��doubleֵ��vector������
		*						�����ǳ�����ӦԼ����ʩ�ӵ����Ƶ�����£�����ԭֵ��������ֵ��
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);


			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			double factor(weight() + genrand() * (1.0 - weight()));

			do
			{
				double jitter = (0.0001* genrand() + weight());

				(*tmpInd->vars())[j] = (*pop[urn[0]]->vars())[j] +
					factor * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();

			return mutation_info(tmpInd, origin);
		}

	};



	/**
	 * ������� # 5
	 *
	 * @author louiehan (11/15/2019)
	 */
	class mutation_strategy_5 : public mutation_strategy
	{

	public:
		/**
		 * constructs a mutation strategy # 5
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param varCount ��������
		 * @param args ������Բ�����
		 */
		mutation_strategy_5(size_t varCount, const mutation_strategy_arguments& args)
			: mutation_strategy(varCount, args)
		{
		}

		/**
		* ִ�б���
		*
		* @author louiehan (11/15/2019)
		*
		* @param pop ��ǰ��Ⱥ�����ã�
		* @param bestIt ��һ����õĸ��壻
		* @param i ��ǰ�����������
		*
		* @return mutation_info Ԫ�飨tuple������������ĸ����װ�������������ͬ��С��doubleֵ��vector������
		*						�����ǳ�����ӦԼ����ʩ�ӵ����Ƶ�����£�����ԭֵ��������ֵ��
		*/
		mutation_info operator()(const population& pop, individual_ptr bestIt, size_t i)
		{
			assert(bestIt);

			de::NVectorPtr origin(std::make_shared< de::NVector >(varCount()));
			individual_ptr tmpInd(std::make_shared< individual >(*pop[i]->vars()));
			Urn urn(pop.size(), i);

			// make sure j is within bounds
			size_t j = genintrand(0, varCount(), true);
			size_t k = 0;

			do
			{
				(*tmpInd->vars())[j] = (*pop[urn[0]]->vars())[j] + dither() * ((*pop[urn[1]]->vars())[j] - (*pop[urn[2]]->vars())[j]);

				j = ++j % varCount();
				++k;
			} while (genrand() < crossover() && k < varCount());

			origin = pop[urn[0]]->vars();
			return mutation_info(tmpInd, origin);
		}

	};

}

#endif //DE_MUTATION_STRATEGY_HPP_INCLUDED

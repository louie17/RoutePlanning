#ifndef DE_SELECTION_STRATEGY_HPP_INCLUDED
#define DE_SELECTION_STRATEGY_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "population.hpp"

namespace de
{

	/**
	 * ����ѡ����Խӿڵĳ�����ࡣ
	 *
	 * ��ֽ���ʹ��ѡ�������ȷ����Ѹ��塣
	 *
	 * @author louiehan (11/15/2019)
	 */
	class selection_strategy
	{
	public:
		virtual ~selection_strategy() {}

		/**
		 * ���ز���������applies the selection strategy
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param pop1 old population
		 * @param pop2 new population
		 * @param bestInd reference to the best individual - contains
		 *  			  the best individual on return
		 * @param minimize if true, it will minimize, if false it will
		 *  			   maximize
		 */
		virtual void operator()(population_ptr& pop1, population_ptr& pop2, individual_ptr& bestInd, bool minimize) = 0;
	};

	/**
	 * A smart pointer to a selection strategy
	 */
	typedef std::shared_ptr< selection_strategy > selection_strategy_ptr;

	/**
	 * ѡ����Ը���ÿ������Ĵ��ۺ������Ľ������Ŀ�꺯������С������󻯣������������������
	 *
	 * @author louiehan (11/15/2019)
	 */
	class best_parent_child_selection_strategy : public selection_strategy
	{
	public:
		/**
		 * ���ز�����() applies the selection strategy
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param pop1 old population
		 * @param pop2 new population
		 * @param bestInd reference to the best individual - contains
		 *  			  the best individual on return
		 * @param minimize if true, it will minimize, if false it will
		 *  			   maximize
		 */
		void operator()(population_ptr& pop1, population_ptr& pop2, individual_ptr& bestInd, bool minimize)
		{
			assert(pop1);
			assert(pop2);

			assert(pop1->size() == pop2->size());

			sort_across(*pop1, *pop2, minimize);

			// this is the best
			bestInd = (*pop1)[0];
		}

	private:
		class individual_compare
		{
		private:
			const bool m_minimize;

		public:
			individual_compare(bool minimize)
				: m_minimize(minimize)
			{
			}

			bool operator()(individual_ptr ind1, individual_ptr ind2)
			{
				assert(ind1 && ind2);

				return ind1->better(ind2, m_minimize);
			}
		};

		void sort_across(population& v1, population& v2, bool minimize)
		{
			v1.insert(v1.end(), v2.begin(), v2.end());
			v2.clear();

			std::sort(v1.begin(), v1.end(), individual_compare(minimize));

			v2.insert(v2.end(), v1.begin() + v1.size() / 2, v1.end());

			v1.erase(v1.begin() + v1.size() / 2, v1.end());
		}
	};

	/**
	 * ѡ����ԱȽ�������ͬһ�������Ӧ�ĸ��壬��ѡ����һ������Ѹ��塣
	 *
	 * @author louiehan (11/15/2019)
	 */
	class tournament_selection_strategy : public selection_strategy
	{
	public:
		/**
		 * ���ز��������� applies the selection strategy
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param pop1 old population
		 * @param pop2 new population
		 * @param bestInd reference to the best individual - contains
		 *  			  the best individual on return
		 * @param minimize if true, it will minimize, if false it will
		 *  			   maximize
		 */
		void operator()(population_ptr& pop1, population_ptr& pop2, individual_ptr& bestInd, bool minimize)
		{
			assert(pop1);
			assert(pop2);

			assert(pop1->size() == pop2->size());

			for (size_t i = 0; i < pop1->size(); ++i)
			{
				individual_ptr crt((*pop2)[i]);

				if (crt->better_or_equal((*pop1)[i], minimize))
				{
					if (crt->better_or_equal(bestInd, minimize))
						bestInd = crt;
				}
				else
					(*pop2)[i] = (*pop1)[i];

			}

			std::swap(pop1, pop2);
		}
	};

}

#endif //DE_SELECTION_STRATEGY_HPP_INCLUDED

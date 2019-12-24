// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_DIFFERENTIAL_EVOLUTION_HPP_INCLUDED
#define DE_DIFFERENTIAL_EVOLUTION_HPP_INCLUDED

#include <memory>
#include <tuple>

#include "random_generator.hpp"
#include "multithread.hpp"
#include "individual.hpp"
#include "processors.hpp"
#include "mutation_strategy.hpp"
#include "population.hpp"
#include "selection_strategy.hpp"
#include "termination_strategy.hpp"
#include "listener.hpp"

namespace de
{

	/**
	 * �Ż������ڼ䷢������ʱ���׳��쳣��
	 *
	 * @author louiehan (11/11/2019)
	 */
	class differential_evolution_exception
	{
	};

	/**
	 * ��ֽ����㷨����
	 *
	 * ���ݸ��������������������Ż����̡�
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class differential_evolution
	{
	private:
		const size_t m_varCount;
		const size_t m_popSize;

		population_ptr m_pop1;
		population_ptr m_pop2;
		individual_ptr m_bestInd;

		constraints_ptr m_constraints;
		typename processors< T >::processors_ptr m_processors;
		termination_strategy_ptr m_terminationStrategy;
		selection_strategy_ptr m_selectionStrategy;
		mutation_strategy_ptr m_mutationStrategy;
		listener_ptr m_listener;

		const bool m_minimize;
	public:
		/**
		 * ����һ��differential_evolution����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param varCount ����������������Ŀ�꺯������ı������������㷨����ĸ���Ԫ�أ�
		 * @param popSize ����Ⱥ��������
		 * @param processors �Ż��Ự�ڼ�ʹ�õĲ��д���������
		 * @param constraints Լ�����������а���Ŀ�꺯��ʹ�õı�����Լ���Լ��㷨�ڲ�ʹ�õ���������������Լ����
		 * @param minimize ���Ϊtrue����������С���ɱ������Ϊfalse���򽫳�����󻯳ɱ���
		 * @param terminationStrategy ��ֹ���ԣ�
		 * @param selectionStrategy ѡ����ԣ�
		 * @param mutationStrategy ������ԣ�
		 * @param listener ��������
		 */
		differential_evolution(size_t varCount, size_t popSize, typename processors< T >::processors_ptr processors, constraints_ptr constraints, bool minimize,
			termination_strategy_ptr terminationStrategy, selection_strategy_ptr selectionStrategy,
			mutation_strategy_ptr mutationStrategy, de::listener_ptr listener)
			try

			: m_varCount(varCount), m_popSize(popSize), m_pop1(std::make_shared< population >(popSize, varCount, constraints)),
			m_pop2(std::make_shared< population >(popSize, varCount)), m_bestInd(m_pop1->best(minimize)),
			m_constraints(constraints), m_processors(processors), m_minimize(minimize), m_terminationStrategy(terminationStrategy),
			m_listener(listener), m_selectionStrategy(selectionStrategy), m_mutationStrategy(mutationStrategy)
		{
			assert(processors);
			assert(constraints);
			assert(terminationStrategy);
			assert(selectionStrategy);
			assert(listener);
			assert(mutationStrategy);

			assert(popSize > 0);
			assert(varCount > 0);

			//ͨ��ʹ�ó�ʼ���������������Ŀ�꺯������ʼ������1��
			processors->push(m_pop1);
			processors->start();
			processors->wait();

		}
		catch (const processors_exception&)
		{
			throw differential_evolution_exception();
		}

		/**
		* ����differential_evolution����
		*
		* @author louiehan (11/11/2019)
		*
		* @param varCount ����������������Ŀ�꺯������ı������������㷨����ĸ���Ԫ�أ�
		* @param popSize ����Ⱥ��������
		* @param processors �Ż��Ự�ڼ�ʹ�õĲ��д���������
		* @param startNode,endNode ·������ʼ����ֹ��
		* @param constraints Լ�����������а���Ŀ�꺯��ʹ�õı�����Լ���Լ��㷨�ڲ�ʹ�õ���������������Լ����
		* @param minimize ���Ϊtrue����������С���ɱ������Ϊfalse���򽫳�����󻯳ɱ���
		* @param terminationStrategy ��ֹ���ԣ�
		* @param selectionStrategy ѡ����ԣ�
		* @param mutationStrategy ������ԣ�
		* @param listener ��������
		*/
		differential_evolution(size_t varCount, size_t popSize, typename processors< T >::processors_ptr processors,const Node &startNode, const Node &endNode, constraints_ptr constraints, bool minimize,termination_strategy_ptr terminationStrategy, selection_strategy_ptr selectionStrategy, mutation_strategy_ptr mutationStrategy, de::listener_ptr listener)
			try

			: m_varCount(varCount), m_popSize(popSize), m_pop1(std::make_shared< population >(popSize, varCount, startNode, endNode, constraints)),
			m_pop2(std::make_shared< population >(popSize, varCount)), m_bestInd(m_pop1->best(minimize)),
			m_constraints(constraints), m_processors(processors), m_minimize(minimize), m_terminationStrategy(terminationStrategy),
			m_listener(listener), m_selectionStrategy(selectionStrategy), m_mutationStrategy(mutationStrategy)
		{
			assert(processors);
			assert(constraints);
			assert(terminationStrategy);
			assert(selectionStrategy);
			assert(listener);
			assert(mutationStrategy);

			assert(popSize > 0);
			assert(varCount > 0);

			//ͨ��ʹ�ó�ʼ���������������Ŀ�꺯������ʼ������1��
			processors->push(m_pop1);
			processors->start();
			processors->wait();

		}
		catch (const processors_exception&)
		{
			throw differential_evolution_exception();
		}

		virtual ~differential_evolution(void)
		{
		}

		/**
		 * ������ֽ����Ż����̡�
		 *
		 * ���ܴ����ǲ��н��еģ����˺�����ͬ���ģ��������Ż���ɻ���󴥷��쳣֮ǰ���᷵�ء�
		 *
		 * @author louiehan (11/11/2019)
		 */
		void run()
		{
			try
			{
				m_listener->start();
				individual_ptr bestIndIteration(m_bestInd);

				for (size_t genCount = 0; m_terminationStrategy->event(m_bestInd, genCount); ++genCount)
				{
					m_listener->startGeneration(genCount);
					for (size_t i = 0; i < m_popSize; ++i)
					{
						mutation_strategy::mutation_info mutationInfo((*m_mutationStrategy)(*m_pop1, bestIndIteration, i));

						individual_ptr tmpInd(std::get< 0 >(mutationInfo));

						tmpInd->ensureConstraints(m_constraints, std::get< 1 >(mutationInfo));

						// ������
						m_processors->push(tmpInd);

						// ��ʱ��tempInd����һ��vector�����У������ڴ���֮ǰ�ǿյģ����ڴ���֮�󽫱��ƶ�����ȷ��λ�á�
						(*m_pop2)[i] = tmpInd;
					}

					m_listener->startProcessors(genCount);
					m_processors->start();
					m_processors->wait();
					m_listener->endProcessors(genCount);

					//BestParentChildSelectionStrategy()( m_pop1, m_pop2, m_bestInd, m_minimize );
					m_listener->startSelection(genCount);
					(*m_selectionStrategy)(m_pop1, m_pop2, m_bestInd, m_minimize);
					bestIndIteration = m_bestInd;

					m_listener->endSelection(genCount);

					m_listener->endGeneration(genCount, bestIndIteration, m_bestInd);

				}

				//BOOST_SCOPE_EXIT_TPL( (m_listener) )
				//{
				m_listener->end();
				//} 
				//BOOST_SCOPE_EXIT_END
			}
			catch (const processors_exception&)
			{
				m_listener->error();
				throw differential_evolution_exception();
			}

		}

		/**
		 * �����Ż����̲�������Ѹ��塣
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return individual_ptr
		 */
		individual_ptr best() const { return m_bestInd; }
	};

}

#endif //DE_DIFFERENTIAL_EVOLUTION_HPP_INCLUDED

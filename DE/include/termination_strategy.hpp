#ifndef DE_TERMINATION_STRATEGY_HPP_INCLUDED
#define DE_TERMINATION_STRATEGY_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace de
{

	/**
	 * �����˾����ս��������ʹ�õĽӿڵĳ�����ࡣ
	 *
	 * ��ֹ���Զ���������ֹͣ�Ż����̵��߼��� ���Ժܼ򵥣�����ﵽ
	 * ����������ʱ�˳��Ľ���������������Ҳ����ʵ�ָ����ӵ��㷨����
	 * ͨ��ȷ����ʱ�ﵽ��������ֵ��������С��ִ�в����������
	 *
	 * @author louiehan (11/15/2019)
	 */
	class termination_strategy
	{
	public:
		virtual ~termination_strategy() {}

		/**
		 * �Ż�������ֹ�ж��¼�
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param best ��ǰ���Ÿ���
		 * @param genCount ��������
		 *
		 * @return bool return true to continue, or false to stop the
		 *  	   optimization process
		 */
		virtual bool event(individual_ptr best, size_t genCount) = 0;
	};

	/**
	 * A smart pointer to a TerminationStrategy
	 */
	typedef std::shared_ptr< termination_strategy > termination_strategy_ptr;

	/**
	 * ��ֹ���ԵĻ���ʵ�֣�����ѽ���������������ֹͣ�Ż�����
	 *
	 * @author louiehan (11/15/2019)
	 */
	class max_gen_termination_strategy : public termination_strategy
	{
	private:
		const size_t m_maxGen;
	public:
		/**
		 * constructs a max_gen_termination_strategy object
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param maxGen maximum number of generations after which the
		 *  			 optimization stops
		 */
		max_gen_termination_strategy(size_t maxGen)
			: m_maxGen(maxGen)
		{
		}

		virtual bool event(individual_ptr best, size_t genCount)
		{
			return genCount < m_maxGen;
		}
	};

}

#endif //DE_TERMINATION_STRATEGY_HPP_INCLUDED

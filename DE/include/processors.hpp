#ifndef DE_PROCESSORS_HPP_INCLUDED
#define DE_PROCESSORS_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <queue>
#include <functional>   // std::ref

#include "boost_noncopyable.hpp"
#include "boost_threadgroup.hpp"
#include "individual.hpp"
#include "population.hpp"
#include "Scenario.hpp"

namespace de
{

	/**
	 * ���ڳ�����࣬���ڴӴ����������¼��Ĵ�������������
	 *
	 * �����䷽���ǴӶ���̵߳��õģ���˾��������ʹ���߳�ͬ�������Ա��������𻵡�
	 *
	 * @author louiehan (11/11/2019)
	 */
	class processor_listener
	{
	public:
		virtual ~processor_listener() {}
		/**
		 * ������Ŀ�꺯���Ĵ�����operator�����Ŀ�ͷ����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 */
		virtual void start(size_t index) = 0;

		/**
		 * ��ʹ�õ�ǰ����ı�������Ŀ�꺯��֮ǰ����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 * @param individual ����Ŀ�꺯���ĵ�ǰ����
		 */
		virtual void start_of(size_t index, individual_ptr individual) = 0;

		/**
		 * ��ʹ�õ�ǰ����ı�������Ŀ�꺯������á���Ϊ�������ݵĸ��廹����Ŀ�꺯�����н���ĳɱ�����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 * @param individual Ŀ�꺯�����еİ������۵ĵ�ǰ���塣
		 */
		virtual void end_of(size_t index, individual_ptr individual) = 0;

		/**
		 * ������Ŀ�꺯���Ĵ����������������ĩβ���á�
		 *
		 * ��ʹ�׳��쳣��Ҳ����ô˺�����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index the processor index
		 */
		virtual void end(size_t index) = 0;

		/**
		 * �����Ŀ�꺯�������ڼ������쳣��ָʾ��������á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 * @param message ����������Ϣ����Ϣ
		 */
		virtual void error(size_t index, const std::string& message) = 0;

	};

	/**
	 * ��ִ���κβ����Ĵ������������Ļ���ʵ�֡�
	 *
	 * ����Ҫͬ������Ϊû�����ݱ���ȡ���޸ġ�
	 *
	 * @author louiehan (11/11/2019)
	 */
	class null_processor_listener : public processor_listener
	{
	public:
		/**
		 * ������Ŀ�꺯���Ĵ�����operator�����Ŀ�ͷ���á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 */
		virtual void start(size_t index)
		{
			std::cout << "Processor index:" << index << "start running!" << std::endl;
		}

		/**
		 * ��ʹ�õ�ǰ����ı�������Ŀ�꺯��֮ǰ���á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ������������
		 * @param individual Ŀ�꺯�����еĵ�ǰ����
		 */
		virtual void start_of(size_t index, individual_ptr individual)
		{
			std::cout << "Processor index:" << index<<", Past best individual cost:"<<individual->cost()<<std::endl;
		}

		/**
		 * ʹ�õ�ǰ����ı�������Ŀ�꺯������á� ��Ϊ�������ݵĸ���Ҳ�������趨ΪĿ�꺯�����еĽ����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 * @param individual Ŀ�꺯�������ݵİ������۵ĵ�ǰ���塣
		 */
		virtual void end_of(size_t index, individual_ptr individual)
		{
			std::cout << "Processor index:" << index << ", Current best individual cost:" << individual->cost() << std::endl;
		}

		/**
		 * ������Ŀ�꺯���Ĵ�����operator������ĩβ���á�
		 *
		 * ��ʹ���쳣�׳�Ҳ�ᱻ���á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 */
		virtual void end(size_t index)
		{
			std::cout << "Processor index:" << index << "exit!"<< std::endl;
		}

		/**
		 * �����Ŀ�꺯�������ڼ������쳣��ָʾ��������á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 * @param message ����������Ϣ����Ϣ
		 */
		virtual void error(size_t index, const std::string& message)
		{
			std::cout << "Processor index:" << index << "; Error:" << message << std::endl;
		}
	};


	/**
	 *ָ������������������ָ��
	 */
	typedef std::shared_ptr< processor_listener > processor_listener_ptr;

	/**
	 * ���Ŀ�꺯���������׳��쳣��
	 *
	 * @author louiehan (11/11/2019)
	 */
	class objective_function_exception : public exception
	{
	public:
		/**
		 * ����һ��objective_function_exception����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param message ���������쳣�Ĵ�����Ϣ
		 */
		objective_function_exception(const std::string& message)
			: exception(message.c_str())
		{
		}
	};

	/**
	 * Ŀ�꺯�������Ľӿڡ����Ŀ�꺯��Ҫ�󽫲�ͬ��ʵ�����ݸ�ÿ����������
	 * �봴��һ�������Դ���ľ����object_function_factory����ʵ��
	 * ��make�����Դ���Ŀ�꺯������ʵ����
	 *
	 * ʹ�ö�object_function_exception�����û�shared_ptr��Ϊ����
	 * ��ģ�������������Ӧ�Ķ�����Ϊ���캯���������ݡ�
	 *
	 * ������Ӧ�Ĵ����������ཫȷ��Ӧ����ȷ����Ϊ��
	 *
	 * ģ�����T��Ŀ�꺯�������͡�
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T >class objective_function_factory
	{
	public:
		/**
		 * ����ָ��Ŀ�꺯��������ָ��
		 */
		typedef std::shared_ptr< T > T_ptr;

		/**
		 * ����������
		 *
		 * @author louiehan (11/11/2019)
		 */
		virtual ~objective_function_factory() {}

		/**
		 *����������ʵ�ֵķ���������Ŀ�꺯������ʵ����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return T_ptr Ŀ�꺯��������ָ��
		 */
		virtual T_ptr make() = 0;
	};

	/**
	 * Ŀ�꺯��ͨ�����ô��ݲ����Ƶ�����µĻ���������������
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T >class processor_traits
	{
	public:
		// \cond
		typedef T  value_type;
		static double run(T t, de::NVectorPtr vars) { return t(vars); }
		static double run(T t, de::NVectorPtr vars, constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return t(vars,constraints,swRel);}
		static T make(T t) { return t; }
		// \endcond
	};

	/**
	 * Ŀ�꺯����Ϊָ�봫�ݵ�����µ�ר�ô�����������
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< T* >
	{
	public:
		// \cond
		typedef T*  value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(value_type t) { return t; }
		// \endcond
	};

	/**
	 * ��Ŀ�꺯����Ϊ����ָ�봫��ʱ�����⴦���������ࡣ
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< std::shared_ptr< T > >
	{
	public:
		// \cond
		typedef std::shared_ptr< T > value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(value_type t) { return t; }
		// \endcond
	};

	/**
	 * ���������յ�Ŀ�꺯��������ָ�������µ�ר�ô����������ࡣ
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< objective_function_factory< T >* >
	{
	public:
		// \cond
		typedef std::shared_ptr< T > value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(objective_function_factory< T >* off) { return off->make(); }
		// \endcond
	};
	/**
	 * ���������յ�Ŀ�꺯�������Ĺ���ָ�������µ�ר�ô����������ࡣ
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< std::shared_ptr< objective_function_factory< T > > >
	{
	public:
		// \cond
		typedef std::shared_ptr< T > value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(std::shared_ptr< objective_function_factory< T > > off) { return off->make(); }
		// \endcond
	};

	/**
	 * ���������յ�Ŀ�꺯�����������õ�����µ�ר�ô����������ࡣ
	 *
	 * @author louiehan (11/11/2019)
	 */
	template< typename T > class processor_traits< objective_function_factory< T >& >
	{
	public:
		// \cond
		typedef std::shared_ptr< T > value_type;
		static double run(value_type t, de::NVectorPtr vars) { return (*t)(vars); }
		static double run(value_type t, de::NVectorPtr vars, de::constraints_ptr constraints, sce::Site_WeaponRange_relation swRel) { return (*t)(vars, constraints, swRel); }
		static value_type make(objective_function_factory< T >& off) { return off.make(); }
		// \endcond
	};

	/**
	 * ��������һ���߳�������Ŀ�꺯���� ����������߳����������߳��в�������Ŀ�꺯����
	 *
	 * ��������ʹ������Ӧ��processor_traits�ж����Ŀ�꺯�������͡�
	 *
	 * @author louiehan (11/11/2019)
	 */
	template < typename T > class processor : boost::noncopyable
	{
	private:
		typename processor_traits< T >::value_type m_of;
		individual_queue& m_indQueue;
		processor_listener_ptr m_listener;
		size_t m_index;

		constraints_ptr m_constraints;
		sce::Site_WeaponRange_relation m_swRealation;


		bool m_result;

	public:
		/**
		 * �����������캯��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index ����������
		 * @param of Ŀ�꺯������Ŀ�꺯������������ָ�롢����ָ�롢����
		 * @param indQueue ����Ҫ����ĸ���Ķ���
		 * @param listener �ڴ���Ŀ�꺯���ڼ佫������Ҫ�¼�֪ͨ����������
		 */
		processor(size_t index, T of, individual_queue& indQueue, processor_listener_ptr listener)
			: m_of(processor_traits< T >::make(of)), m_indQueue(indQueue), m_result(false), m_listener(listener), m_index(index)
		{
			assert(listener);
		}

		/**
		* �����������캯��
		*
		* @author louiehan (12/11/2019)
		*
		* @param index ����������
		* @param of Ŀ�꺯������Ŀ�꺯������������ָ�롢����ָ�롢����
		* @param constraints ������Լ������
		* @param swRealation ������Ϣ�е�Site-Weapon Relation��ϵ��
		* @param indQueue ����Ҫ����ĸ���Ķ���
		* @param listener �ڴ���Ŀ�꺯���ڼ佫������Ҫ�¼�֪ͨ����������
		*/
		processor(size_t index, T of, individual_queue& indQueue, constraints_ptr constraints, sce::Site_WeaponRange_relation swRealation, processor_listener_ptr listener)
			:m_of(processor_traits< T >::make(of)), m_indQueue(indQueue), m_constraints(constraints), m_swRealation(swRealation), m_result(false), m_listener(listener), m_index(index)
		{
			assert(listener);
		}

		/**
		 * �Զ��ж����Ķ�������У�����Ŀ�꺯��
		 *
		 * @author louiehan (11/11/2019)
		 */
		void operator()()
		{
			m_listener->start(m_index);
			m_result = false;
			try
			{
				for (individual_ptr ind = m_indQueue.pop(); ind; ind = m_indQueue.pop())
				{
					m_listener->start_of(m_index, ind);
					//double result = processor_traits< T >::run(m_of, ind->vars());
					double result = processor_traits< T >::run(m_of, ind->vars(),m_constraints,m_swRealation);
					ind->setCost(result);
					m_listener->end_of(m_index, ind);
				}
				m_result = true;

				//BOOST_SCOPE_EXIT_TPL( (&m_index) (&m_listener) )
				//{
				m_listener->end(m_index);
				//} 
				//BOOST_SCOPE_EXIT_END
			}
			catch (const objective_function_exception& e)
			{
				m_result = false;
				m_listener->error(m_index, e.what());
			}
		}

		/**
		 * ָʾ�߳��˳�ʱ�����Ƿ�ɹ�����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return bool
		 */
		bool success() const { return m_result; }
	};



	/**
	 * ��������������ʱ�׳��쳣��
	 *
	 * @author louiehan (11/11/2019)
	 */
	class processors_exception : exception
	{
	public:
		/**
		 * ���캯��������Ϣ�ַ�����Ϊ����
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param message
		 */
		processors_exception(const std::string& message)
			: exception(message.c_str())
		{
		}
	};


	/**
	 * ����������
	 *
	 * �������Ż��Ự�ڼ�������Э�����ִ�������
	 *
	 * ��Ŀ�꺯����Ŀ�꺯��������������Ϊ���������ã�ָ���shared_ptr����
	 *
	 * @author louiehan (11/15/2019)
	 */
	template< typename T > class processors
	{
	private:
		typedef std::shared_ptr< boost::thread_group > thread_group_ptr;
		typedef std::shared_ptr< processor< T > > processor_ptr;
		typedef std::vector< processor_ptr > processor_vector;
		typedef std::shared_ptr< T > T_ptr;

	private:
		individual_queue m_indQueue;
		processor_vector m_processors;
		thread_group_ptr m_threads;

	public:
		/**
		 * ����һ������������Ȼ��ʹ���ṩ��Ŀ�꺯�����조������������
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param count �����Ĵ���������
		 * @param of Ŀ�꺯����Ŀ�꺯������
		 * @param listener ���ݸ�ÿ�������Ĵ�������������
		 */
		processors(size_t count, T of, processor_listener_ptr listener)
		{
			assert(count > 0);
			assert(listener);

			for (size_t n = 0; n < count; ++n)
			{
				processor_ptr processor(std::make_shared< processor< T > >(n, of, std::ref(m_indQueue), listener));
				m_processors.push_back(processors< T >::processor_ptr(processor));
			}
		}

		/**
		* ����һ������������Ȼ��ʹ���ṩ��Ŀ�꺯�����조������������
		*
		* @author louiehan (11/15/2019)
		*
		* @param count �����Ĵ���������
		* @param of Ŀ�꺯����Ŀ�꺯������
		* @param constraints ������Լ������
		* @param swRealation ������Ϣ�е�Site - Weapon Relation��ϵ��
		* @param listener ���ݸ�ÿ�������Ĵ�������������
		*/		
		processors(size_t count, T of, constraints_ptr constraints, sce::Site_WeaponRange_relation swRealation, processor_listener_ptr listener)
		{
			assert(count > 0);
			assert(listener);

			for (size_t n = 0; n < count; ++n)
			{
				processor_ptr processor(std::make_shared< processor< T > >(n, of, std::ref(m_indQueue), constraints, swRealation, listener));
				m_processors.push_back(processors< T >::processor_ptr(processor));
			}
		}

		/**
		 * ���������봦����еĵײ���
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param ind
		 */
		void push(individual_ptr ind) { m_indQueue.push(ind); }
		/**
		 * �첽�������д������̣߳�������ȴ�������ɣ���
		 *
		 * @author louiehan (11/15/2019)
		 */
		void start()
		{
			// ÿ�δ���һ�����߳��飬����Ҫ����ɾ�����е������߳�
			m_threads = std::make_shared< boost::thread_group >();

			for (typename processor_vector::size_type n = 0; n < m_processors.size(); ++n)
			{
				processor_ptr p(m_processors[n]);
				std::thread* th(new std::thread(std::ref(*p)));//new thread()����m_indiQuen����һ��
				m_threads->add_thread(th);

			}
		}

		/**
		 * �ٷ���֮ǰ�ȴ����д�����������
		 *
		 * ����ͬ������
		 *
		 * @author louiehan (11/15/2019)
		 */
		void wait()
		{
			m_threads->join_all();

			if (!m_indQueue.empty())
				throw processors_exception("threads ended before emptying the queue");

			if (!success())
				throw processors_exception("objective function error");
		}

		/**
		 * ָʾ���д������Ƿ�ɹ�������
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @return bool true if success, false if an error occured
		 */
		bool success()
		{
			for (typename processor_vector::size_type n = 0; n < m_processors.size(); ++n)
			{
				processor_ptr processor(m_processors[n]);
				if (!processor->success())
					return false;
			}

			return true;
		}

		/**
		 * ��Ⱥ���е����и������봦����С�
		 *
		 * @author louiehan (11/15/2019)
		 *
		 * @param population
		 */
		void push(population_ptr population)
		{
			std::copy(population->begin(), population->end(), std::back_inserter(m_indQueue));
		}

		/**
		 * ָ���������ϵ�����ָ�롣
		 */
		typedef std::shared_ptr< processors< T > > processors_ptr;

	};

}

#endif //DE_PROCESSORS_HPP_INCLUDED

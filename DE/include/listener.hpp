// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_LISTENER_HPP_INCLUDED
#define DE_LISTENER_HPP_INCLUDED

namespace de
{

	/**
	 * �û�������������ĳ�����ࡣ
	 *
	 * ���Ż��Ự�ڼ䷢��ĳЩ��Ҫ�¼�ʱ���������ཫ����֪ͨ��
	 *
	 * ��Щ�¼���������ʾ��ǰ״̬�������Ϣ�ȡ�
	 *
	 * �û����Ӵ���������������������࣬�⽫�Խ��յ����¼���һЩ���õ����顣
	 *
	 * @author louiehan (11/11/2019)
	 */
	class listener
	{
	public:
		virtual ~listener() {}

		/**
		 * ���Ż����̿�ʼʱ���á�
		 *
		 * @author louiehan (11/11/2019)
		 */
		virtual void start() = 0;
		/**
		 * ���Ż����̽���ʱ���á�
		 *
		 * ��ʹ�Ż��������쳣������Ҳ����ô˺���
		 *
		 * @author louiehan (11/11/2019)
		 */
		virtual void end() = 0;
		/**
		 * ���Ż������������쳣ʱ���ã������������ź�
		 *
		 * @author louiehan (11/11/2019)
		 */
		virtual void error() = 0;
		/**
		 * ��ÿһ����ʼʱ���á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void startGeneration(size_t genCount) = 0;
		/**
		 *��ÿһ������ʱ���ã������ڵ����������֮ǰ�����쳣��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 * @param bestIndGen
		 * @param bestInd
		 */
		virtual void endGeneration(size_t genCount, individual_ptr bestIndGen, individual_ptr bestInd) = 0;
		/**
		 * ��ѡ��ʼ֮ǰ���á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void startSelection(size_t genCount) = 0;
		/**
		 * ִ��ѡ�����á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void endSelection(size_t genCount) = 0;
		/**
		 * �ڿ�ʼĿ�꺯������֮ǰ���á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void startProcessors(size_t genCount) = 0;
		/**
		 * Ŀ�꺯��������ɺ���á�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param genCount
		 */
		virtual void endProcessors(size_t genCount) = 0;
	};

	/**
	 * ָ��������������ָ��
	 */
	typedef std::shared_ptr< listener > listener_ptr;

	/**
	 * �������н����¼��ľ���������
	 *
	 * @author louiehan (11/11/2019)
	 */
	class null_listener : public listener
	{
	public:
		virtual void start() {}
		virtual void end() {}
		virtual void error() {}
		virtual void startGeneration(size_t genCount) {}
		virtual void endGeneration(size_t genCount, individual_ptr bestIndGen, individual_ptr bestInd)
		{
			std::cout << "genCount:" << genCount << ", cost: " << bestInd->cost() << std::endl;
		}
		virtual void startSelection(size_t genCount) {}
		virtual void endSelection(size_t genCount) {}
		virtual void startProcessors(size_t genCount) {}
		virtual void endProcessors(size_t genCount) {}
	};

}

#endif //DE_LISTENER_HPP_INCLUDED

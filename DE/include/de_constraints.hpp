// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_CONSTRAINTS_HPP_INCLUDED
#define DE_CONSTRAINTS_HPP_INCLUDED

#include <set>
#include <memory>
#include <cassert>
#include <cmath>
#include <iterator>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

#include "random_generator.hpp"
#include "de_types.hpp"
#include "ud_string.hpp"

namespace de
{

	/**
	 * ��Լ�������������׳��쳣
	 *
	 * @author louiehan (11/11/2019)
	 */
	class constraints_exception : public exception
	{
	public:
		/**
		 * ��������Ϣ��Ϊ�����Ĺ��캯��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param message
		 */
		constraints_exception(const std::string& message)
			: exception(message.c_str())
		{
		}
	};

	/**
	 * ����Լ����ĳ�����ࡣ
	 * һ��Լ�������������������Ŀ�꺯����ĳЩ���Ժ����ơ�
	 *
	 * @author louiehan (11/11/2019)
	 */
	class constraint
	{
	public:
		virtual ~constraint() {}

		/**
		 * ����һ��������Լ�����ͺͷ�Χ�ڵ����ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value() = 0;

		/**
		 * ������ǰ��ֵ��ԭ�㷵��һ��������Լ�������ͺͷ�Χ�ڵ����ֵ(�μ��������еľ���ʵ��)
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value
		 * @param origin
		 *
		 * @return double
		 */
		virtual double get_rand_value(double value, double origin) = 0;

		/**
		 * ���ط�Χ����С���ơ�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double min() const = 0;

		/**
		 * ���ط�Χ��������ơ�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double max() const = 0;

		/**
		 * ��ȡ��ΪԼ�����õ������ڵ����ֵ������һ������������ԭ��͸�ԭ����Χ����Ŀ�ȣ����ܿ�ȵİٷֱȣ�����ķ�Χ�ڡ�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param origin ��һ��������Լ�������ԭ�㣨���ģ�
		 * @param zonePct Χ��ԭ��������ȣ����ܿ�ȵİٷֱȱ�ʾ
		 *
		 * @return double
		 */
		virtual double get_rand_value_in_zone(double origin, double zonePct) const = 0;

		/**
		 * ��ȡ������Сֵ�����ֵ֮��ĵ�-�������ڷ�ΧԼ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_middle_point() = 0;
	};

	/**
	 * ָ��ĳ��Լ��������ָ��
	 */
	typedef std::shared_ptr< constraint > constraint_ptr;

	/**
	 * ���ڷ�Χ��Լ���Ļ��ࡣ ÿ��������Լ������һ����Сֵ�����ֵ
	 *
	 * @author louiehan (11/11/2019)
	 */
	class range_constraint : public constraint
	{
	private:
		double m_min;
		double m_max;

	public:

		/**
		 * ���캯������Χ��С���������
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param min
		 * @param max
		 */
		range_constraint(double min, double max)
			: m_min(min), m_max(max)
		{
			assert(min <= max);
		}

		/**
		 * ���ط�Χ����С����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double min() const { return m_min; }

		/**
		 * ���ط�Χ���������
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double max() const { return m_max; }

	};

	/**
	 * ʵ�������ࡣָ��������ָ���������ڿ������κ�˫����ֵ��
	 *
	 * @author louiehan (11/11/2019)
	 */
	class real_constraint : public range_constraint
	{
	public:

		/**
		 * ���캯������Χ�����Сֵ�޶�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param min
		 * @param max
		 */
		real_constraint(double min, double max)
			: range_constraint(min, max)
		{
			assert(min <= max);
		}

		/**
		 * ����һ��������Լ�����ͺͷ�Χ�����ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double get_rand_value()
		{
			return genrand(range_constraint::min(), range_constraint::max());
		}

		/**
		 * ������ǰ��ֵ��ԭ�㣬����������Լ�����ͺͷ�Χ�ڵ����ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value
		 * @param origin
		 *
		 * @return double
		 */
		double get_rand_value(double value, double origin)
		{
			double ret = value;

			while (ret < range_constraint::min())
			{
				ret = range_constraint::min() + genrand() * (origin - range_constraint::min());
			}

			while (ret > range_constraint::max())
			{
				ret = range_constraint::max() + genrand() * (origin - range_constraint::max());
			}

			return ret;
		}

		virtual double get_rand_value_in_zone(double origin, double zonePct) const
		{
			if (origin > max())
				throw constraints_exception("origin coordinate > max");
			if (origin < min())
				throw constraints_exception("origin coordinate < min");

			if (zonePct > 100.0)
				throw constraints_exception("zonePct > 100%");

			if (zonePct < 0)
				throw constraints_exception("zonePct < 0%");

			if (zonePct == 0)
				throw constraints_exception("zonePct == 0%");

			double zoneSize = (max() - min()) * zonePct / 100.0;

			double _min = std::max(min(), origin - zoneSize / 2.0);
			double _max = std::min(max(), origin + zoneSize / 2.0);

			return genrand(_min, _max);
		}

		virtual double get_middle_point()
		{
			return (max() + min()) / 2.0;
		}

	};

	/**
	 * ����Լ���� ָ���������Ծ���ָ�������ڵ��κ�����ֵ��
	 *
	 * @author louiehan (11/11/2019)
	 */
	class int_constraint : public range_constraint
	{
	public:
		/**
		 * ���캯������������Լ������С���������
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param min
		 * @param max
		 */
		int_constraint(double min, double max)
			: range_constraint(min, max)
		{
			assert(min <= max);
		}

		/**
		 * ����һ��������Լ�����ͺͷ�Χ�����ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double get_rand_value()
		{
			return genintrand(range_constraint::min(), range_constraint::max());
		}

		/**
		 * ������ǰ��ֵ��ԭ�㣬����������Լ�����ͺͷ�Χ�ڵ����ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value
		 * @param origin
		 *
		 * @return double
		 */
		double get_rand_value(double value, double origin)
		{
			double ret = round(value);

			while (ret < range_constraint::min())
			{
				ret = range_constraint::min() + genrand() * (origin - range_constraint::min());
				ret = round(ret);
			}

			while (ret > range_constraint::max())
			{
				ret = range_constraint::max() + genrand() * (origin - range_constraint::max());
				ret = round(ret);
			}

			return ret;
		}

		virtual double get_rand_value_in_zone(double origin, double zonePct) const
		{
			if (origin > max())
				throw constraints_exception("origin coordinate > max");
			if (origin < min())
				throw constraints_exception("origin coordinate < min");

			if (zonePct > 100.0)
				throw constraints_exception("zonePct > 100%");

			if (zonePct < 0)
				throw constraints_exception("zonePct < 0%");

			if (zonePct == 0)
				throw constraints_exception("zonePct == 0%");

			double zoneSize = (max() - min()) * zonePct / 100.0;

			double _min = std::max(min(), origin - zoneSize / 2.0);
			double _max = std::min(max(), origin + zoneSize / 2.0);

			double val = round(genrand(_min, _max));

			for (; val < _min || val > _max; val = round(genrand(_min, _max)));

			return val;
		}

		virtual double get_middle_point()
		{
			return round((max() - min()) / 2.0);
		}


	};

	/**
	 * Լ�����ϡ� ָ���������Բ���Ԥ���弯���е��κ�ֵ������Ҫ��С�����
	 * ��ע�⣬�ظ�ֵ����ɾ����
	 *
	 * @author louiehan (11/11/2019)
	 */
	class set_constraint : public constraint
	{
	private:
		class unique : public std::unary_function < Double, bool >
		{
		public:
			bool operator()(Double d) const
			{
				return m_unique.insert(d).second;
			}

		public:
			double min() const
			{
				if (m_unique.size() > 0)
					return *m_unique.begin();
				else
					throw constraints_exception("could not get the min value of an empty set constraint");
			}

			double max() const
			{
				if (m_unique.size() > 0)
					return *m_unique.rbegin();
				else
					throw constraints_exception("could not get the max value of an empty set constraint");
			}

		private:
			mutable std::set< Double > m_unique;
		};
	private:
		unique m_unique;
		de::DVector m_values;

	public:
		/**
		 * ����Doubleֵ��vector�����켯�ϣ���ɾ���ظ�����ȷ���������ľ��ȷֲ�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param values
		 */
		set_constraint(const de::DVector& values)
		{
			// ȷ�������ϡ��е�ֵ��Ψһ��
			std::remove_copy_if(values.begin(), values.end(), std::back_inserter(m_values), m_unique);
		}

		/**
		 * ��һ��������ֵ��ӵ������ϡ��У����ᴴ���ظ���ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value
		 */
		void add_value(de::Double value)
		{
			if (m_unique(value))
				m_values.push_back(value);
		}

		/**
		 * ���شӿ���ֵ���������ѡ���ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value()
		{
			de::DVector::size_type index(genintrand(0, m_values.size() - 1));

			return m_values[index];
		}

		/**
		 * ���شӿ���ֵ�������ѡ���ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value(double value, double origin)
		{
			return get_rand_value();
		}

		/**
		 * ���ط�Χ����С����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double min() const
		{
			return m_unique.min();
		}

		/**
		 * ���ط�Χ���������
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double max() const
		{
			return m_unique.max();
		}

		virtual double get_rand_value_in_zone(double origin, double zonePct) const
		{
			throw constraints_exception("get_rand_value_in_zone only supported for range constraints");
		}

		virtual double get_middle_point()
		{
			throw constraints_exception("get_middle_point not supported by set constraint");
		}

	};

	/**
	 * ����Լ���� ָ���������Բ��ò���ֵ-true��false��
	 *
	 * @author louiehan (11/11/2019)
	 */
	class boolean_constraint : public constraint
	{
	public:
		/**
		 * ����һ���������ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value()
		{
			return genrand() < 0.5;
		}

		/**
		 * ����һ���������ֵ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		virtual double get_rand_value(double value, double origin)
		{
			return get_rand_value();
		}

		/**
		 * ���ط�Χ����С����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double min() const
		{
			return 0;
		}

		/**
		 * ���ط�Χ���������
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		double max() const
		{
			return 1;
		}

		virtual double get_rand_value_in_zone(double origin, double zonePct) const
		{
			throw constraints_exception("get_rand_value_in_zone only supported for range constraints");
		}

		virtual double get_middle_point()
		{
			throw constraints_exception("get_middle_point not supported by bool constraint");
		}

	};

	typedef std::vector< constraint_ptr > constraints_base;

	/**
	 * Լ�������ļ��ϣ���vector����ʵ�֡����ڶ����Ż������ڼ�ʹ�õ����б�����Լ����
	 *
	 * @author louiehan (11/11/2019)
	 */
	class constraints : public constraints_base
	{
	private:

	public:
		/**
		 * ʹ��Ĭ��ֵ��ʼ��һ��Լ�����ϣ����е�Լ����ʵ�����ͣ�������ͬ��Ĭ�ϴ�С
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param varCount Լ���ĸ���
		 * @param defMin Ĭ����Сֵ
		 * @param defMax Ĭ�����ֵ
		 */
		constraints(size_t varCount, double defMin, double defMax)
			: constraints_base(varCount, std::make_shared< real_constraint >(defMin, defMax))
		{
		}

		/**
		 * ���ַ��������г�ʼ��һ��Լ�����ϣ�Ŀǰֻ�����ڻ��ڷ�Χ��Լ����
		 * һ��Լ�����Ա�����Ϊ��"type;min;max" ����type������ʵ�����������ͣ�min��max�������ޡ�
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param str һ��Լ���������ַ������ϣ�ÿ���ַ�������һ��������
		 * @param var_count �����ĸ��� (���Բ������ַ����ĸ���).��������ĸ���������ַ����࣬
		 *  			   ������Լ�����趨Ϊʵ�����ͣ���ʹ��Ĭ�ϵĴ�Сֵ���޶���Χ��
		 * @param def_min Ĭ����Сֵ�������������������ָ��Ϊ�ַ�����Լ����������
		 * @param def_max Ĭ����Сֵ�������������������ָ��Ϊ�ַ�����Լ����������
		 */
		constraints(const std::vector< std::string >& str, size_t var_count, double def_min, double def_max)
			: constraints_base(var_count, std::make_shared< real_constraint >(def_min, def_max))
		{
			std::stringstream ess;

			for (std::vector< std::string >::size_type i = 0; i < str.size(); ++i)
			{
				std::string delim{ "[\\s;,]+" };//������ʽͨ���������ƥ�������ַ�
				auto s_result = udstr::bs_split(str[i], delim);

				std::string type;
				double _min;
				double _max;

				size_t count(0);

				for (std::vector< std::string >::size_type j = 0; j < s_result.size(); ++j, ++count)
				{
					switch (count)
					{
					case 0:
						type = s_result[count];
						break;
					case 1:
						_min = std::stod(s_result[count]);
					case 2:
						_max = std::stod(s_result[count]);
					default:
						// too many fields
						ess.clear();
						ess << "wrong variable format in \"" << str[i] << "\" - too many fields";
						throw constraints_exception(ess.str());
					}
				}

				// too few fields
				if (count < 3) {
					ess.clear();
					ess << "wrong variable format in \"" << str[i] << "\" - too few fields";
					throw constraints_exception(ess.str());
				}

				if (i < var_count)
					constraints_base::at(i) = str_to_constraint(type, _min, _max);
				else
					constraints_base::push_back(str_to_constraint(type, _min, _max));
			}
		}

		/**
		 * ����һ��������Լ�����ͺͷ�Χ�ڵ����ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index Լ������������
		 *
		 * @return double
		 */
		double get_rand_value(size_t index)
		{
			if (index < constraints_base::size())
				return (*this)[index]->get_rand_value();
			else
			{
				std::stringstream ess;
				ess << "invalid constraint index:" << index << ", higher than max number of constraints:" << constraints_base::size();
				throw constraints_exception(ess.str());
			}

		}

		/**
		 * ������ǰ��ֵ��ԭ�㣬����������Լ�����ͺͷ�Χ�ڵ����ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param index Լ������������
		 * @param value ��ǰ��ֵ
		 * @param origin ԭ��
		 *
		 * @return double
		 */
		double get_rand_value(size_t index, double value, double origin)
		{
			if (index < constraints_base::size())
				return (*this)[index]->get_rand_value(value, origin);
			else
			{
				std::stringstream ess;
				ess << "invalid constraint index:" << index << ", higher than max number of constraints:" << constraints_base::size();
				throw constraints_exception(ess.str());
			}
		}

		/**
		 * ����ԭ��ͱ�ʾΪ������Χ�ٷֱȵ�������ġ��������塱����������һ�����ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param origin �����ԭ������
		 * @param sidePct ��������ı�ռ�����ߵİٷֱ�
		 *
		 * @return DVectorPtr
		 */
		DVectorPtr get_square_zone_rand_values(const DVectorPtr origin, double sidePct) const
		{
			assert(origin);
			assert(sidePct > 0 && sidePct <= 100);

			if (origin->size() == constraints_base::size())
			{
				DVectorPtr square(std::make_shared< DVector >(origin->size()));

				for (constraints_base::size_type n = 0; n < constraints_base::size(); ++n)
					(*square)[n] = (*this)[n]->get_rand_value_in_zone((*origin)[n], sidePct);

				return square;

			}
			else
				throw constraints_exception("The origin vector must have the same number of elements as there are constraints");
		}

		DVectorPtr get_middle_point()
		{
			DVectorPtr r(std::make_shared< DVector >(constraints_base::size()));

			for (constraints_base::size_type n = 0; n < constraints_base::size(); ++n)
				(*r)[n] = (*this)[n]->get_middle_point();

			return r;

		}

		/**
		 * ��Լ�����õ����Ʒ�Χ�ڻ�ȡһ�����ֵ��
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return DVectorPtr
		 */
		DVectorPtr get_rand_values() const
		{
			DVectorPtr r(std::make_shared< DVector >(constraints_base::size()));

			for (constraints_base::size_type n = 0; n < constraints_base::size(); ++n)
				(*r)[n] = (*this)[n]->get_rand_value();

			return r;
		}


	private:
		constraint_ptr str_to_constraint(const std::string& type, double min, double max)
		{
			if (udstr::tolower(type) == "real")
				return std::make_shared< real_constraint >(min, max);
			else if (udstr::tolower(type) == "int" || udstr::tolower(type) == "integer")
				return std::make_shared< int_constraint >(min, max);
			else
			{
				std::stringstream ess;
				ess << "invalid constraint type \"" << type << "\".";
				throw constraints_exception(ess.str());
			}
		}
	};

	typedef std::shared_ptr< constraints > constraints_ptr;

}

#endif //DE_CONSTRAINTS_HPP_INCLUDED

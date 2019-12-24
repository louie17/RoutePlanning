// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DE_TYPES_HPP_INCLUDED
#define DE_TYPES_HPP_INCLUDED

#include <vector>
#include <exception>

#include "Scenario.hpp"

namespace de
{
	/**
	 * ��װһ��double���ͣ���Ҫ���ڵ��Ժ����
	 *
	 * @author louiehan (11/11/2019)
	 */
	class Double
	{
	private:
		double m_value;

	public:

		Double(double value)
			: m_value(value)
		{
		}

		Double()
			: m_value(0)
		{
		}

		/**
		 * ���ظ�ֵ�����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param value value to set the Double to
		 *
		 * @return double returns the current value
		 */
		double operator=(double value)
		{
			m_value = value;

			return m_value;
		}

		/**
		 * ���巵��double���ͳ�Ա�����Ĳ�����
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return double
		 */
		operator double() const
		{
			return m_value;
		}
	};

	/**
	 * a vector of Double instances
	 */
	typedef std::vector< Double > DVector;

	/**
	 * shared pointer to a DVector
	 */
	typedef std::shared_ptr< DVector > DVectorPtr;

	/**
	* ��������ڵ���
	*
	*/
	class Node
	{
		friend bool operator==(const Node&, const Node&);
	public:
		Node(void)
			:m_altitude(0), m_latitude(0), m_longitude(0)
		{
		}
		Node(double longitude, double latitude, double altitude)
			:m_altitude(altitude), m_latitude(latitude), m_longitude(longitude)
		{
		}

		/**
		* ���������()
		*
		* @author louiehan (11/11/2019)
		*
		* @param node ���ڿ�����ֵ�Ľڵ�
		*
		* @return *this ���ضԵ�ǰ�ڵ������
		*/
		Node &operator()(const Node& node)
		{
			m_altitude = node.m_altitude;
			m_latitude = node.m_latitude;
			m_longitude = node.m_longitude;
			return *this;
		}

		Node &operator()(const sce::Site& site)
		{
			m_altitude = site.getAltitude();
			m_latitude = site.getLatitude();
			m_longitude = site.getLongitude();
			return *this;
		}

		Node &operator()(const sce::Location& location)
		{
			m_altitude = location.getAltitude();
			m_latitude = location.getLatitude();
			m_longitude = location.getLongitude();
			return *this;
		}

		Node &operator()(const sce::Point& point)
		{
			m_altitude = point.getAltitude();
			m_latitude = point.getLatitude();
			m_longitude = point.getLongitude();
			return *this;
		}

		Node &operator()(const sce::WayPoint& wayPoint)
		{
			m_altitude = wayPoint.getAltitude();
			m_latitude = wayPoint.getLatitude();
			m_longitude = wayPoint.getLongitude();
			return *this;
		}

		/**
		* ���ظ�ֵ�����=
		*
		* @author louiehan (11/11/2019)
		*
		* @param node ���ڿ�����ֵ�Ľڵ�
		*
		* @return *this ���ضԵ�ǰ�ڵ������
		*/
		Node& operator=(const Node& node)
		{
			m_altitude = node.m_altitude;
			m_latitude = node.m_latitude;
			m_longitude = node.m_longitude;
			return *this;
		}

		Node &operator=(const sce::Site& site)
		{
			m_altitude = site.getAltitude();
			m_latitude = site.getLatitude();
			m_longitude = site.getLongitude();
			return *this;
		}

		Node &operator=(const sce::Location& location)
		{
			m_altitude = location.getAltitude();
			m_latitude = location.getLatitude();
			m_longitude = location.getLongitude();
			return *this;
		}

		Node &operator=(const sce::Point& point)
		{
			m_altitude = point.getAltitude();
			m_latitude = point.getLatitude();
			m_longitude = point.getLongitude();
			return *this;
		}

		Node &operator=(const sce::WayPoint& wayPoint)
		{
			m_altitude = wayPoint.getAltitude();
			m_latitude = wayPoint.getLatitude();
			m_longitude = wayPoint.getLongitude();
			return *this;
		}
		// operations on Node objects
		// member binary operator: left-hand operand bound to implicit this pointer
		Node& operator+=(const Node&);
		Node& operator*(double times);

		double norm() const { return sqrt(m_altitude*m_altitude + m_latitude*m_latitude + m_longitude*m_longitude); }

		double altitude() const { return m_altitude; }
		double latitude() const { return m_latitude; }
		double longitude() const { return m_longitude; }

		void setAltitude(const double &altitude) { m_altitude = altitude; }
		void setLatitude(const double &latitude) { m_latitude = latitude; }
		void setLongitude(const double &longitude) { m_longitude = longitude; }

	private:		
		double m_longitude;
		double m_latitude;
		double m_altitude;
	};

	inline bool operator==(const Node &pnode, const Node &lnode)
	{
		// must be made a friend of Node
		return pnode.m_altitude == lnode.m_altitude &&
			pnode.m_latitude==lnode.m_latitude &&
			pnode.m_longitude==lnode.m_longitude;
	}

	inline bool operator!=(const Node &pnode, const Node &lnode)
	{
		return !(pnode == lnode);
	}

	Node& Node::operator+=(const Node &node)
	{
		m_altitude += node.m_altitude;
		m_latitude += node.m_latitude;
		m_longitude += node.m_longitude;
		return *this;
	}

	/**
	* ���ؼӷ������+
	*
	* @author louiehan (11/11/2019)
	*
	* @param pnode,lnode ����������ڵ�
	*
	* @return node ����������
	*/
	Node operator+(const Node &pnode, const Node &lnode)
	{
		Node node(pnode);
		node += lnode;
		return node;
	}

	/**
	* ���ؼ��������-
	*
	* @author louiehan (11/11/2019)
	*
	* @param pnode,lnode ����������ڵ� 
	*
	* @return node ���ز������
	*/
	Node operator-(const Node &lnode, const Node &rnode)
	{
		Node node(lnode);
		node.setAltitude(lnode.altitude() - rnode.altitude());
		node.setLatitude(lnode.latitude() - rnode.latitude());
		node.setLongitude(lnode.longitude() - rnode.longitude());
		return node;
	}

	/**
	* ���ؼ��������-
	*
	* @author louiehan (11/11/2019)
	*
	* @param pnode,site �����node��Site
	*
	* @return node ���ز������
	*/
	Node operator-(const Node &pnode, const sce::Site &site)
	{
		Node node(pnode);
		node.setAltitude(pnode.altitude() - site.getAltitude());
		node.setLatitude(pnode.latitude() - site.getLatitude());
		node.setLongitude(pnode.longitude() - site.getLongitude());
		return node;
	}

	Node& Node::operator*(double times)
	{
		m_altitude *= times;
		m_latitude *= times;
		m_longitude *= times;
		return *this;
	}

	/**
	* ���س˷������*
	*
	* @author louiehan (11/11/2019)
	*
	* @param pnode,lnode ��˵������ڵ�
	*
	* @return *this
	*/
	double operator*(const Node &pnode, const Node &lnode)
	{
		return pnode.altitude() * lnode.altitude() + pnode.latitude() * lnode.latitude() + pnode.longitude() * lnode.longitude();
	}

	Node operator*(const double &times, const Node &pnode)
	{
		Node cnode(pnode);
		cnode.setAltitude(times*pnode.altitude());
		cnode.setLatitude(times*pnode.latitude());
		cnode.setLongitude(times*pnode.longitude());

		return cnode;
	}

	/**
	* a vector of Node instances
	*/
	typedef std::vector< Node > NVector;

	/**
	* shared pointer to a NVector
	*/
	typedef std::shared_ptr< NVector > NVectorPtr;

	/**
	 * ������� C++��׼��DE�쳣�� (std::exception��MSʵ���зǱ�׼���캯��)
	 *
	 * @author louiehan (11/11/2019)
	 */
	class exception : public std::exception
	{
	private:
		const std::string m_what;

	public:
		virtual ~exception() throw()
		{
		}
		/**
		 * ���캯����C�ַ�����Ϊ�������ò��������쳣���������Ϣ
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @param what
		 */
		exception(const char* what)
			: m_what(what != 0 ? what : "")
		{
		}

		/**
		 * С��ʹ�á�������쳣���󳬳���Χ����˺������ص�ָ����Ч
		 *
		 * @author louiehan (11/11/2019)
		 *
		 * @return const char* ָ�����쳣��Ϣ��������ַ�����ָ��
		 */
		virtual const char* what() const throw()
		{
			return m_what.c_str();
		}
	};

}
#endif //DE_TYPES_HPP_INCLUDED

#ifndef UD_STRING_HPP_INCLUDE
#define UD_STRING_HPP_INCLUDE

#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <regex>
#include <algorithm>
#include <locale>

namespace udstr {
	/**
	* ʹ��������ʽ�Զ��ָ�ʽ���ַ��������иȥ��������ַ�
	*
	* @author louiehan (11/15/2019)
	*
	* @return std::vector<_str_type>
	*/
	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>,
		typename _str_type = std::basic_string<E, TR, AL>>
		std::vector<_str_type> bs_split(const std::basic_string<E, TR, AL>& in, const std::basic_string<E, TR, AL>& delim) {
		std::basic_regex<E> re{ delim };
		return std::vector<_str_type> {
			std::regex_token_iterator<typename _str_type::const_iterator>(in.begin(), in.end(), re, -1),
				std::regex_token_iterator<typename _str_type::const_iterator>()
		};
	}

	/**
	* ʹ��stlģ�庯��ʵ���ַ�����дת��
	*
	* @author louiehan (11/15/2019)
	*
	* @return std::string
	*/
	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>>
		inline std::basic_string<E, TR, AL> toupper(const std::basic_string<E, TR, AL>&src) {
		auto dst = src;
		static const std::locale loc("");
		transform(src.begin(), src.end(), dst.begin(), [&](E c)->E {return std::toupper(c, loc); });
		return dst;
	}

	/**
	* ʹ��stlģ�庯��ʵ���ַ���Сдת��
	*
	* @author louiehan (11/15/2019)
	*
	* @return std::string
	*/
	template<typename E,
		typename TR = std::char_traits<E>,
		typename AL = std::allocator<E>>
		inline std::basic_string<E, TR, AL> tolower(const std::basic_string<E, TR, AL>&src) {
		auto dst = src;
		// ʹ�õ�ǰ��locale����
		static const std::locale loc("");
		// lambda���ʽ�����ַ�����ÿ���ַ�Ԫ��ת��ΪСд
		// std::string��Ԫ������Ϊchar,std::wstring��Ԫ������Ϊwchar_t
		transform(src.begin(), src.end(), dst.begin(), [&](E c)->E {return std::tolower(c, loc); });
		return dst;
	}
}

#endif // !UD_STRING_HPP_INCLUDE

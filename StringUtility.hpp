#ifndef STRINGUTILITY_HPP
#define STRINGUTILITY_HPP

#include <string>
#include <string_view>
#include "Final.object_arx_global.hpp"

namespace sstd {
	/**
	QString
	QStringLiteral
	QLatin1String
	**/
#ifdef QT_CORE_LIB

	inline QString operator""_qs(const char * a, std::size_t b) {
		return QString::fromUtf8(a, static_cast<int>(b));
	}

	inline constexpr QLatin1String operator""_qsv(const char * a, std::size_t b) {
		return QLatin1String(a, static_cast<int>(b));
	}

#ifndef VQLS
#define VQLS(...) QStringLiteral(__VA_ARGS__)
#endif

#endif
	using namespace std::string_literals;/*s*/
	using namespace std::string_view_literals;/*sv*/

	namespace ___private_string_u_sstd {
		template<typename T, typename U = void>
		class Has_C_Str_Function : public std::false_type {};

		template<typename T>
		class Has_C_Str_Function<T, std::void_t< decltype(
			std::declval< std::add_const_t<
			std::remove_reference_t<T> > >().c_str())>
		> : public std::true_type {};


		template<typename T>
		inline const wchar_t * get_c_style_string(const T & arg) {
			if constexpr(Has_C_Str_Function<T>::value) {
				return arg.c_str();
			}
			else {
				return arg.data();
			}
		}

	}/*namespace ___private_string_u_sstd*/

	template<typename T>
	inline const wchar_t * get_c_style_string(const T & arg) {
		if constexpr(std::is_pointer_v<std::remove_reference_t<T>>) {
			return arg;
		}
		else {
			return ___private_string_u_sstd::get_c_style_string(arg);
		}
	}

}/*namespace sstd*/

#endif // STRINGUTILITY_HPP

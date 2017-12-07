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
			constexpr bool _c = Has_C_Str_Function<T>::value;
			if constexpr(_c) {
				return arg.c_str();
			}
			else {
				return arg.data();
			}
		}

	}/*namespace ___private_string_u_sstd*/

	template<typename T>
	inline const wchar_t * get_cstr(const T & arg) {
		if constexpr(std::is_pointer_v<std::remove_reference_t<T>>) {
			return arg;
		}
		else {
			return ___private_string_u_sstd::get_c_style_string(arg);
		}
	}

	class ArxString {
		wchar_t * $Data;
	public:
		ArxString&operator=(ArxString&) = delete;
		ArxString&operator=(ArxString&&) = delete;
		ArxString(ArxString&) = delete;
		ArxString(ArxString&&) = delete;
	public:
		inline ArxString(wchar_t * arg) :$Data(arg) {}
		inline ~ArxString() { acutDelString($Data); }
		operator wchar_t * & () { return $Data; }
		operator wchar_t *() const { return $Data; }
		wchar_t * & pointer() { return $Data; }
		wchar_t * pointer() const { return $Data; }
	};

}/*namespace sstd*/

#endif // STRINGUTILITY_HPP

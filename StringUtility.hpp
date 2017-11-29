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
}/*namespace sstd*/

#endif // STRINGUTILITY_HPP

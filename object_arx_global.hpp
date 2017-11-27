#ifndef OBJECT_ARX_GLOBAL_H
#define OBJECT_ARX_GLOBAL_H

#if defined(LARX_AUTOCAD_LIBRARY)

#ifndef LIBRARY_DEFINE
#define LIBRARY_DEFINE __declspec(dllexport)
#endif

#else

#ifndef LIBRARY_DEFINE
#define LIBRARY_DEFINE __declspec(dllimport)
#endif

#endif

#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <Windows.h>
#include <arxHeaders.h>
#pragma warning( pop)

#define DEFINE_ARX_NAME(...) constexpr static inline const auto * globalName() \
{ return  __VA_ARGS__ ; } \
constexpr static inline const auto * localName() { return 1 + globalName(); }

namespace sstd {
	inline constexpr const auto * arx_group_name() { return LR"(SSTD)"; }
	template<typename T>
	inline void arx_add_main_command() {
		acedRegCmds->addCommand(arx_group_name(),
			T::globalName(),
			T::localName(),
			ACRX_CMD_MODAL,
			&T::main
		);
	}
}/*namespace sstd*/

#ifdef QT_CORE_LIB
#include <QtCore>
#endif

#ifdef QT_GUI_LIB
#include <QtGui>
#endif

/*cplusplus headers*/
#include <memory>
#include <cstddef>
#include <utility>
#include <type_traits>

/*utility*/
#include "ArxClosePointer.hpp"

#endif // OBJECT_ARX_GLOBAL_H




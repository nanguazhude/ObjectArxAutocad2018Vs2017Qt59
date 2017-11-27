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

inline constexpr const auto * arx_group_name() { return LR"(SSTD)"; }
#define DEFINE_ARX_NAME(...) constexpr static inline const auto * globalName() \
{ return  __VA_ARGS__ ; } \
constexpr static inline const auto * localName() { return 1 + globalName(); }

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

#endif // OBJECT_ARX_GLOBAL_H




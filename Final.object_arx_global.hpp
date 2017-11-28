#ifndef _P_OBJECT_ARX_GLOBAL_HXX_
#define _P_OBJECT_ARX_GLOBAL_HXX_

/*cplusplus headers*/
#include <cmath>
#include <atomic>
#include <future>
#include <thread>
#include <memory>
#include <limits>
#include <cstddef>
#include <utility>
#include <numeric>
#include <type_traits>

#ifndef vmax
#define vmax (std::max)
#endif

#ifndef vmin
#define vmin (std::min)
#endif

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

#ifdef QT_WIDGETS_LIB
#include <QtWidgets>
namespace sstd {
class QtApplication :QApplication {
public:
    QtApplication();
};
}/*namespace sstd*/
#endif

/*utility*/
#include "ArxClosePointer.hpp"

namespace sstd {
	template<typename T>
	inline Acad::ErrorStatus _basic_openObject(T*&arg,
		AcDbObjectId id,
		AcDb::OpenMode mode,
		bool openErasedEntity = false) {
		if constexpr(std::is_base_of<AcDbEntity, T>::value) {
			return ::acdbOpenAcDbEntity(arg, id, mode, openErasedEntity);
		}
		else if constexpr(std::is_base_of<AcDbObject, T>::value) {
			return ::acdbOpenAcDbObject(arg, id, mode, openErasedEntity);
		}
		else {
			return ::acdbOpenObject(arg, id, mode, openErasedEntity);
		}
	}

	template<typename T>
	inline Acad::ErrorStatus openObject(ArxClosePointer<T>&arg,
		AcDbObjectId id,
		AcDb::OpenMode mode,
		bool openErasedEntity = false) {
		return _basic_openObject<T>(arg.pointer(), id, mode, openErasedEntity);
	}

	template<typename T>
	inline Acad::ErrorStatus openObject(T*&arg,
		AcDbObjectId id,
		AcDb::OpenMode mode,
		bool openErasedEntity = false) {
		return _basic_openObject<T>(arg, id, mode, openErasedEntity);
	}

}/*sstd*/

#endif // OBJECT_ARX_GLOBAL_H




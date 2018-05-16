#ifndef _P_OBJECT_ARX_GLOBAL_HXX_
#define _P_OBJECT_ARX_GLOBAL_HXX_

/*cplusplus headers*/
#include <cmath>
#include <string>
#include <atomic>
#include <future>
#include <thread>
#include <memory>
#include <limits>
#include <cstddef>
#include <utility>
#include <numeric>
#include <algorithm>
#include <type_traits>
#include <string_view>

using namespace std::string_view_literals;

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
			ACRX_CMD_REDRAW | ACRX_CMD_MODAL,
			&T::main
		);
	}

	template<typename T>
	inline void arx_add_main_command_usepickset() {
		acedRegCmds->addCommand(arx_group_name(),
			T::globalName(),
			T::localName(),
			ACRX_CMD_REDRAW | ACRX_CMD_USEPICKSET,
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
	class QtApplication {
		std::shared_ptr<void> $QtApplication;
	public:
		QtApplication();
	};
}/*namespace sstd*/
#endif

#ifdef QT_QML_LIB
#include <QtQml/QtQml>
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

namespace sstd{

	class ThrowType {};

	inline void _p_sstd_warning(
		const std::wstring_view & arg ,
		std::size_t argLine,
		const std::string_view & argFile,
		const std::string_view & argFunctionName
		) {
		
		std::wstring varAboutToPrint=LR"(文件:)";
		for (const auto &varI:argFile) {
			varAboutToPrint.push_back(varI);
		}
		varAboutToPrint += LR"(
函数名:)";
		for (const auto &varI : argFunctionName) {
			varAboutToPrint.push_back(varI);
		}
		varAboutToPrint += LR"(
行号:)";
		varAboutToPrint+=std::to_wstring(argLine);
		varAboutToPrint += LR"(
描述:)";
		if (arg.empty() == false) {
			varAboutToPrint += arg;
			if (*arg.rbegin()!='\n') {
				varAboutToPrint += '\n';
			}
		}
		acutPrintf( varAboutToPrint.c_str() );
	}

	[[noreturn]] inline void _p_sstd_throw(
		const std::wstring_view & arg,
		std::size_t argLine,
		const std::string_view & argFile,
		const std::string_view & argFunctionName
	)  {
		_p_sstd_warning(arg,argLine,argFile,argFunctionName);
		throw ThrowType{};
	}

}/*namespace sstd*/

#ifndef svwarning
#define svprint(...) sstd::_p_sstd_warning( __VA_ARGS__ , __LINE__ , __FILE__ , __func__  )
#endif

#ifndef svthrow
#define svthrow(...) sstd::_p_sstd_throw( __VA_ARGS__ , __LINE__ , __FILE__ , __func__  )
#endif

#endif // OBJECT_ARX_GLOBAL_H




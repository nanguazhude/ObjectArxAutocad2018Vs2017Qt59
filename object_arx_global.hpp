#pragma once

#ifndef OBJECT_ARX_GLOBAL_HXX_
#define OBJECT_ARX_GLOBAL_HXX_

#include <chrono>
using namespace std::chrono_literals;

#include "Final.object_arx_global.hpp"
#include "AddToModelSpace.hpp"
#include "StringUtility.hpp"

namespace sstd {
	
	template<typename T=double>
	inline constexpr T pi() { return static_cast<T>(
		3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679); 
	}

	template<typename T=double>
	inline constexpr T hpi() { return pi<T>() / 2; }

	template<typename T=double>
	inline constexpr T dpi() { return pi<T>() * 2; }


}/*sstd*/

#endif





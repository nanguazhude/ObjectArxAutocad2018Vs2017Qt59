#ifndef TEXT_RENDER_HPP
#define TEXT_RENDER_HPP

/*include cplusplus ibrarys*/
#include <algorithm>
#include <utility>
#include <type_traits>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <string_view>
#include <array>
#include <random>
#include <exception>
#include <stdexcept>
#include <filesystem>

using namespace std::string_literals/*s*/;
using namespace std::string_view_literals/*sv*/;

/*inlcude all qt librarys*/
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

namespace sstd {
	/*毫米单位指示符*/
	static inline constexpr double operator""_mm(long double a) { return static_cast<double>(a); }
	/*°单位指示符*/
	static inline constexpr double operator""_deg(long double a) { return static_cast<double>(a); }
	/*比例单位指示符*/
	static inline constexpr double operator""_per(long double a) { return static_cast<double>(a); }
	/*to Qt utf8 */
	static inline QString operator""_qtu8str(const char *a, std::size_t b) { return QString::fromUtf8(a, static_cast<int>(b)); }
	class RenderState {
	public:
		QString $TextFileName/*要渲染的txt文件名字*/;
		std::filesystem::path $OutPutDir/*输出渲染结果路径*/;
		double  $PageWidth = 210._mm/*页面宽度,单位毫米*/;
		double  $PageHeight = 297._mm/*页面高度,单位毫米*/;
		enum MarginType : unsigned char {
			Top = 0,
			Bottom = 1,
			Left = 2,
			Right = 3,
			SizeOfMargin = 4
		};
		std::array<double, MarginType::SizeOfMargin> $Margin{
			0.1_mm /*上边距,mm*/,
			0.1_mm /*下边距,mm*/,
			0.1_mm /*左边距,mm*/,
			0.1_mm /*右边距,mm*/ }/*上下左右边距*/;
		class Limit {
			double $Min /*最小值*/;
			double $Max /*最大值*/;
			std::unique_ptr<std::normal_distribution<double>/**/> $ND;
			static std::mt19937 $RD;
			inline void _construct() {
				const auto varMean/*平均数*/ = ($Min + $Max)*0.5;
				const auto varS/*标准差*/ = std::abs($Max - $Min)*(1.0 / 6.0);
				$ND.reset(new std::normal_distribution<double>(varMean, varS));
			}
		public:
			inline Limit() :Limit(0, 1) {}
			inline Limit(const double &a, const double &b) : $Min(a), $Max(b) { _construct(); }
			inline void setLimit(const double &a, const double & b) { $Min = a; $Max = b; _construct(); }
			inline double next() {
				const auto varAns = (*$ND)($RD);
				if (varAns > $Max) { return $Max; }
				if (varAns < $Min) { return $Min; }
				return varAns;
			}
		};
		std::size_t $LineCharIndex = 0;
		double $FontBasicSize{ 5.2_mm }/*字体默认大小,单位mm*/;
		double $FontLineHeight{ 6._mm }/*每行高度,单位mm*/;
		double $FontCharSpace{ 0.5_mm }/*字间距*/;
		Limit $FontDx{ -0.13_mm,0.13_mm }/*横向随机位移,单位mm*/;
		Limit $FontDy{ -0.13_mm,0.13_mm }/*纵向随机位移,单位mm*/;
		Limit $FontRotate{ -0.5_deg,0.5_deg }/*字体随机旋转,单位°*/;
		Limit $FontGlobalScale{ 0.980_per,0.990_per }/*字体全局缩放比例*/;
		Limit $FontScaleX{ 0.980_per,0.992_per }/*字体横向缩放比例*/;
		Limit $FontScaleY{ 0.980_per,0.991_per }/*字体纵向缩放比例*/;
		QString $TextLayerName{ u8R"(0)"_qtu8str }/*文字所在图层*/;
		double $BorderTopLeftX = 0.0_mm/**/;
		double $BorderTopLeftY = 0.0_mm/**/;
		QString $BorderLayerName{ u8R"(Defpoints)"_qtu8str }/*边界所在的图层*/;
		/*********************************************************/
		template<
			typename CharType = char,
			int MPN = 8,
			CharType SC = '0',
			CharType EC = '9'>
			class NextNumber {
			public:
				using char_type = CharType;
				constexpr const static int  $MaxPageNumber = MPN;
				constexpr const static char_type $StartChar = SC;
				constexpr const static char_type $EndChar = EC;
				std::array<char_type, $MaxPageNumber + 1> $String;
				int $CurrentNumber = -1;
				NextNumber() {
					for (auto & i : $String) { i = $EndChar; }
					*$String.rbegin() = 0;
				}
				/*不考虑溢出*/
				void next() {
					++$CurrentNumber;
					auto varPos = $String.rbegin() + 1;
					const auto varEnd = $String.rend();
					for (; varPos != varEnd; ++varPos) {
						if (*varPos == $EndChar) {
							*varPos = $StartChar;
							continue;
						}
						++*varPos;
						break;
					}
				}
		};
		/*********************************/
		NextNumber<char, 8, '0', '9'> $CurrentPageNumber;
		bool $IsFileAndStreamOpen = false/*判断流和文件有没有打开*/;
		QFile $File/*当前文件*/;
		QTextStream $Stream/*当前流*/;
		std::string $DataInPastPage/*上一页面还么有处理的数据,utf8编码*/;
		bool $IsEndl = false;
	};

}/*namespace sstd*/

#endif



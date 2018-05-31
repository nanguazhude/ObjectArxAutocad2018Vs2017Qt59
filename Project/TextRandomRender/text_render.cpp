/**********

**********/
#include "text_render.hpp"
#include <vector>
#include <memory>

#pragma warning( push)
#pragma warning (disable: 4189 4100 )
#include <arxHeaders.h>
#pragma warning( pop )

inline constexpr static const sstd::RenderState * as_const(sstd::RenderState *a) { return a; }

namespace {
	class Exception : std::exception {
		std::string $error_code;
	public:
		Exception(const std::string_view &arg) :$error_code(arg) {}
		const char * what() const override { return $error_code.c_str(); }
	};
}/****/

namespace {
	class RenderCharBasic {
	public:
		const bool $IsEmpty;
		RenderCharBasic() :$IsEmpty{ false } {}
		RenderCharBasic(bool a) :$IsEmpty{ a } {}
		virtual ~RenderCharBasic() = default;
	};

	class RenderCharEmpty : public RenderCharBasic {
	public:
		RenderCharEmpty() :RenderCharBasic(true) {}
	};

	class RenderChar :public RenderCharBasic {
	public:
		RenderChar() : RenderCharBasic(false) {}

	};

}/*****/

/*获得下一个字符*/
inline static std::string_view get_next_char(std::string_view & arg) {
	if (arg.empty()) { return{}; }
	const auto varFirstChar = static_cast<unsigned int>(
		std::char_traits<char>::to_int_type(arg[0]));
	if (varFirstChar < 0b10000000u) /*1*/ {
		std::string_view varAns = arg.substr(0, 1);
		arg.remove_prefix(1);
		return std::move(varAns);
	}
	else if (varFirstChar < 0b11100000u)/*2*/ {
		std::string_view varAns = arg.substr(0, 2);
		arg.remove_prefix(2);
		return std::move(varAns);
	}
	else if (varFirstChar < 0b11110000u) {
		std::string_view varAns = arg.substr(0, 3);
		arg.remove_prefix(3);
		return std::move(varAns);
	}
	else if (varFirstChar < 0b11111000u) {
		std::string_view varAns = arg.substr(0, 4);
		arg.remove_prefix(4);
		return std::move(varAns);
	}
	else if (varFirstChar < 0b11111100u) {
		std::string_view varAns = arg.substr(0, 5);
		arg.remove_prefix(5);
		return std::move(varAns);
	}
	std::string_view varAns = arg.substr(0, 6);
	arg.remove_prefix(6);
	return std::move(varAns);

}

extern void text_render(sstd::RenderState * argRenderState) try {
	using CharNumber = sstd::RenderState::NextNumber<wchar_t, 8, '0', '9'>;
	std::vector< std::unique_ptr<RenderCharBasic>/**/> varChars;

	if (false == argRenderState->$IsFileAndStreamOpen) {
		/*读取数据*/
		argRenderState->$File.setFileName(argRenderState->$TextFileName);
		if (false == argRenderState->$File.open(QIODevice::ReadOnly))
			throw Exception(u8R"(can not open file)"sv);
		argRenderState->$Stream.setDevice(&argRenderState->$File);
	}

	/*页码加一*/
	argRenderState->$CurrentPageNumber.next();

	double varHeight = 0;
	double varWidth = 0;

	{/*将文字逐个创建块,并布局*/
		CharNumber varCurrentCharCount;
		std::string varLine = std::move(argRenderState->$DataInPastPage);
		while (varLine.size() || (argRenderState->$File.atEnd() == false)) {
			/*get line data*/
			if (varLine.empty()) {
				const auto varLineQ = argRenderState->$Stream.readLine().trimmed();
				if (varLineQ.isEmpty()) {
					varChars.emplace_back(new RenderCharEmpty);
					varHeight += argRenderState->$FontLineHeight;
					continue;
				}
				varLine = varLineQ.toUtf8();
			}
			/*布局此Line*/
			std::string_view varCurrentLine = varLine;
			std::string_view varNextChar;
			while ((varNextChar = get_next_char(varCurrentLine)).empty() == false) {
				char varTmp[8];
				{
					unsigned int j = 0;
					for (auto & i : varTmp) {
						if (j < varNextChar.size()) { i = varNextChar[j]; }
						else { i = 0; }
					}
				}
				AcString varCurrentChar(varTmp, AcString::Utf8);

			}

		}
	}

	{/*random this page*/

	}

	if ((argRenderState->$DataInPastPage.empty() && (argRenderState->$File.atEnd() == true))) {
		argRenderState->$IsEndl = true;
		return;
	}

}
catch (...) {
	throw;
}


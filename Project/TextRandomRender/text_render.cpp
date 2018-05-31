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
		double $X = 0.;
		double $Y = 0.;
		AcDbObjectId $Index;
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
	double varWidthBegin = 0;
	double varWidth = varWidthBegin;
	double varHeightMax = 0;
	double varWidthMax = 0;

	{/*将文字逐个创建块,并布局*/
		CharNumber varCurrentCharCount;
		std::string varLine = std::move(argRenderState->$DataInPastPage);
		while (varLine.size() || (argRenderState->$File.atEnd() == false)) {
			/*get line data*/
			if (varLine.empty()) {
				const auto varLineQ = argRenderState->$Stream.readLine().trimmed();
				if (varLineQ.isEmpty()) {
					varHeight += argRenderState->$FontLineHeight;
					if (varHeight >= varHeightMax)/*直接删除此 空行*/ {
						goto goto_next_page;
					}
					varChars.emplace_back(new RenderCharEmpty);
					varCurrentCharCount.next();
					continue;
				}
				varLine = varLineQ.toUtf8();
			}
			/*布局此Line*/
			std::string_view varCurrentLine = varLine;
			std::string_view varNextChar;

			varHeight += argRenderState->$FontLineHeight;
			if (varHeight >= varHeightMax)/**/ {
				argRenderState->$DataInPastPage = varCurrentLine;
				goto goto_next_page;
			}

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
				auto & varRenderCharParent = varChars.emplace_back(new RenderChar);
				varCurrentCharCount.next();
				auto varRenderChar = static_cast<RenderChar *>(varRenderCharParent.get());

				/*create the block , insert it */
				{
					{
						std::unique_ptr<AcDbBlockTableRecord,
							void(*)(AcDbBlockTableRecord*)> varBlockTableRecord{
							new AcDbBlockTableRecord(),
							[](AcDbBlockTableRecord * arg) {
							if (arg->objectId().isValid()) {
								arg->close();
							}
							else {
								delete arg;
							}
						} };

						{
							auto varDB = acdbHostApplicationServices()->workingDatabase();
							AcDbObjectPointer<AcDbBlockTable> varBlockTable(varDB->blockTableId(), kForWrite);
							if (varBlockTable.openStatus() != kOk) {
								throw Exception(u8R"(can not open AcDbBlockTable)"sv);
							}
							varBlockTableRecord->setName(varCurrentCharCount.$String.data());
							if (kOk != varBlockTable->add(varRenderChar->$Index, varBlockTableRecord.get()))
								throw Exception(u8R"(can not add AcDbBlockTableRecord)"sv);
						}

						{
							std::unique_ptr<AcDbMText,
								void(*)(AcDbMText*)> varMText{
								new AcDbMText(),
								[](AcDbMText * arg) {
								if (arg->objectId().isValid()) {
									arg->close();
								}
								else {
									delete arg;
								}
							} };

							varMText->setContents(varCurrentChar);
							varMText->setTextHeight(argRenderState->$FontBasicSize);
							varBlockTableRecord->appendAcDbEntity(varMText.get());
						}
					}

				}

				if (varWidth >= varWidthMax) {
					varWidth = varWidthBegin;
					bool varNeedRenderThisChar = true;
					varHeight += argRenderState->$FontLineHeight;
					if (varHeight >= varHeightMax)/**/ {
						if (varNeedRenderThisChar) {
							argRenderState->$DataInPastPage = varCurrentLine;
						}
						else {
							argRenderState->$DataInPastPage = std::string_view(
								varCurrentLine.data() - varNextChar.size(),
								varCurrentLine.size() + varNextChar.size());
							varChars.pop_back();
						}
						goto goto_next_page;
					}
				}

			}

		}//while
	}

goto_next_page:

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


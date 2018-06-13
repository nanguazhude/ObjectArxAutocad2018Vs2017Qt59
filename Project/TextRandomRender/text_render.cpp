/**********

**********/
#include "text_render.hpp"
#include <set>
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
		AcDbObjectId $Index/*块的名字*/;
		AcDbObjectId $InsertCharIndex/*插入块的名字*/;
		std::size_t  $LineCharIndex = 0;
	};

}/*****/

/*
获得下一个字符
*/
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

namespace {
	template<typename T>
	inline void add_line_height(T & x, sstd::RenderState *a) {
		x -= a->$FontLineHeight;
		a->$LineCharIndex = 0;
	}
}/**/

extern void text_render(sstd::RenderState * argRenderState) try {
	using CharNumber = sstd::RenderState::NextNumber<wchar_t, 8, '0', '9'>;
	std::vector< std::unique_ptr<RenderCharBasic>/**/> varChars /*所有Char*/;
	const auto varTextLayerName = argRenderState->$TextLayerName.toStdWString();

	/*读取文件*/
	if (false == argRenderState->$IsFileAndStreamOpen) {
		argRenderState->$IsFileAndStreamOpen = true;
		/*读取数据*/
		argRenderState->$File.setFileName(argRenderState->$TextFileName);
		if (false == argRenderState->$File.open(QIODevice::ReadOnly | QIODevice::Text))
			throw Exception(u8R"(can not open file)"sv);
		argRenderState->$Stream.setDevice(&argRenderState->$File);
		argRenderState->$Stream.setAutoDetectUnicode(true);
	}

	/*页码加一*/
	argRenderState->$CurrentPageNumber.next();

	double varHeight = argRenderState->$BorderTopLeftY -
		argRenderState->$Margin[sstd::RenderState::MarginType::Top];/*当前行高*/;
	const double varWidthBegin = argRenderState->$BorderTopLeftX +
		argRenderState->$Margin[sstd::RenderState::MarginType::Left]/*初始列宽*/;
	double varWidth = varWidthBegin;
	const double varHeightMax = argRenderState->$BorderTopLeftY -
		argRenderState->$PageHeight +
		argRenderState->$Margin[sstd::RenderState::MarginType::Bottom]/*最大高度*/;
	const double varWidthMax = argRenderState->$BorderTopLeftX +
		argRenderState->$PageWidth -
		argRenderState->$Margin[sstd::RenderState::MarginType::Right]/*最大宽度*/;

	{   /*将文字逐个创建块,并布局*/
		CharNumber varCurrentCharCount;
		wchar_t varCurrentCharRaw = 0;
		std::string varLine = std::move(argRenderState->$DataInPastPage)/*获得上一页剩余数据*/;
		while ((varLine.size() > 0) || (argRenderState->$Stream.atEnd() == false)) {

			/*get line data*/
			if (varLine.empty()) {
				const auto varLineQ = argRenderState->$Stream.readLine().trimmed();
				if (varLineQ.isEmpty()) {
					/*获得空行*/
					add_line_height(varHeight, argRenderState);
					varWidth = varWidthBegin;
					if (varHeight <= varHeightMax)/*跨页空行直接删除*/ {
						goto goto_next_page;
					}
					varChars.emplace_back(new RenderCharEmpty);
					varCurrentCharCount.next();
					continue;
				}
				varLine = varLineQ.toUtf8();
			}

			/*布局此Line*/
			//const static std::string var_par_begin{ u8R"(    )"sv }/*首行缩进*/;
			const std::string var_tmp_varline = /*var_par_begin + */std::move(varLine)/*清空缓冲区*/;
			varLine.clear()/*强制清除数据...*/;
			std::string_view varCurrentLine = var_tmp_varline;
			std::string_view varNextChar;

			if (argRenderState->$LineCharIndex) {
				add_line_height(varHeight, argRenderState);
			}
			varWidth = varWidthBegin;

			if (varHeight <= varHeightMax)/*如果超出这一页。。。*/ {
				argRenderState->$DataInPastPage = varCurrentLine;
				goto goto_next_page;
			}

			std::vector< std::string_view > __varAllChars;
			while ((varNextChar = get_next_char(varCurrentLine)).empty() == false) {
				__varAllChars.push_back(varNextChar);
			}

			/*布局这一行文字*/
			auto __b = __varAllChars.cbegin();
			const auto __e = __varAllChars.cend();
			auto get_last_string = [__e](auto p) ->std::string_view {
				++p;
				if (p >= __e) { return std::string_view{}; }
				const std::string_view & v = *p;
				return v.data();
			};
			for (; __b != __e; ++__b) {
				varNextChar = *__b;
				/* 在TextView后面加一个 0 */
				char varTmp[] = { 0,0,0,0,0,0,0,0 };
				{
					unsigned int j = 0;
					for (const auto & i : varNextChar) {
						varTmp[j] = i;
						++j;
					}
				}

				AcString varCurrentChar(varTmp, AcString::Utf8);
				auto & varRenderCharParent = varChars.emplace_back(new RenderChar);
				varCurrentCharCount.next();
				auto varRenderChar = static_cast<RenderChar *>(varRenderCharParent.get());
				varRenderChar->$LineCharIndex = argRenderState->$LineCharIndex++;
				/*create the block */
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
							varCurrentCharRaw = varCurrentChar.getAt(0);
							varMText->setLayer(varTextLayerName.c_str());
						}
					}

				}

				auto varInsertTheBlock = [argRenderState, varRenderChar, &varTextLayerName, &varWidth]() {
					/*insert the block*/
					std::unique_ptr<AcDbBlockReference, void(*)(AcDbBlockReference*)>
						varCharInsert{ new AcDbBlockReference({ varRenderChar->$X,varRenderChar->$Y,0. },
							varRenderChar->$Index) ,[](AcDbBlockReference * arg) {
						if (arg->objectId().isValid()) {
							arg->close();
						}
						else {
							delete arg;
						}
					} };
					varCharInsert->setLayer(varTextLayerName.c_str());
					{/*insert the char*/
						auto varDB = acdbHostApplicationServices()->workingDatabase();
						AcDbObjectPointer<AcDbBlockTable> varBlockTable(varDB->blockTableId(), kForWrite);
						if (varBlockTable.openStatus() != kOk) {
							throw Exception(u8R"(can not open AcDbBlockTable)"sv);
						}
						AcDbBlockTableRecord * varModel = nullptr;
						if (varBlockTable->getAt(ACDB_MODEL_SPACE, varModel, AcDb::kForWrite) != kOk)
							throw Exception(u8R"(can not open AcDbBlockTableRecord)"sv);
						varModel->appendAcDbEntity(varRenderChar->$InsertCharIndex, varCharInsert.get());
						varModel->close();
					}
					AcDbExtents varBounds;
					varCharInsert->bounds(varBounds);
					varWidth += argRenderState->$FontCharSpace +
						std::abs(varBounds.maxPoint().x - varBounds.minPoint().x)/*+字间距*/;
				};

				/*判断是否创建新行*/
				if (varWidth >= varWidthMax) {
					varRenderChar->$X = varWidth;
					varRenderChar->$Y = varHeight;
					varWidth = varWidthBegin;
					/*在行尾保留的标点...*/
					const static std::set<wchar_t> _RenderRaw{
						L',',
						L'.',
						L';',
						L'\'',
						L'。',
						L'，',
					};
					/*标点符号排布在这一行*/
					const bool varNeedRenderThisChar = _RenderRaw.count(varCurrentCharRaw) > 0;

					add_line_height(varHeight, argRenderState);
					if (varHeight <= varHeightMax)/*增加新页*/ {
						if (varNeedRenderThisChar) {
							argRenderState->$DataInPastPage = get_last_string(__b);
							varInsertTheBlock();
						}
						else {
							argRenderState->$DataInPastPage = get_last_string(__b - 1);
							varChars.pop_back();
						}
						goto goto_next_page;
					}
					else /*增加新行*/ {
						if (varNeedRenderThisChar) {
							varInsertTheBlock();
						}
						else {
							--__b;
							varChars.pop_back();
						}
						continue;
					}

				}

				varRenderChar->$X = varWidth;
				varRenderChar->$Y = varHeight;
				varInsertTheBlock();
			}

		}//while
	}

goto_next_page:

	{/*random this page*/
		for (const auto & varC : varChars) {
			if (varC->$IsEmpty) { continue; }
			auto c = static_cast<RenderChar*>(varC.get());
			AcDbObjectPointer<AcDbBlockReference> varChar(c->$InsertCharIndex, kForWrite);
			if (varChar.openStatus() != kOk) {
				throw Exception(u8R"(can not open AcDbBlockTable)"sv);
			}

			auto varPos = varChar->position();

			{/*缩放*/
				const auto varFontGlobalScale = argRenderState->$FontGlobalScale.next();
				const auto varFontScaleX = argRenderState->$FontScaleX.next();
				const auto varFontScaleY = argRenderState->$FontScaleY.next();
				varChar->setScaleFactors({
					varFontGlobalScale*varFontScaleX,
					varFontGlobalScale*varFontScaleY,
					1. });
			}

			{/*旋转*/
				const auto varFontRotate = argRenderState->$FontRotate.next();
				varChar->setRotation(varFontRotate);
			}

			{/*移位*/
				const auto varFontDx = argRenderState->$FontDx.next();
				const auto varFontDy = argRenderState->$FontDy.next();
				varPos.x += varFontDx;
				varPos.y += varFontDy;
				varChar->setPosition(varPos);
			}

		}
	}

	if ((argRenderState->$DataInPastPage.empty() &&
		(argRenderState->$Stream.atEnd() == true))) {
		argRenderState->$IsEndl = true;
		return;
	}

}
catch (...) {
	throw;
}

std::mt19937 sstd::RenderState::Limit::$RD{ std::random_device{}.operator()() };

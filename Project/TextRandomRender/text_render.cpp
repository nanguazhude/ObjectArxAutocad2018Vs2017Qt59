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
	class sstd_Font_Private {
	public:
		sstd_Font_Private(const QString &arg, double scale) :
			$FontName(arg),
			$QFont{ QFont{ arg } },
			$Scale(scale){}
		const QString $FontName;
		const QFontMetricsF $QFont;
		const double $Scale;
		AcDbObjectId $Index;
	};
}/*namespace*/

class sstd::Font::Private : public sstd_Font_Private {
public:
	using  sstd_Font_Private::sstd_Font_Private;
};

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
		char $Utf8Code[8]{ 0,0,0,0,0,0,0,0 };
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
	else if (varFirstChar < 0b11110000u) /*3*/ {
		std::string_view varAns = arg.substr(0, 3);
		arg.remove_prefix(3);
		return std::move(varAns);
	}
	else if (varFirstChar < 0b11111000u) /*4*/ {
		std::string_view varAns = arg.substr(0, 4);
		arg.remove_prefix(4);
		return std::move(varAns);
	}
	else if (varFirstChar < 0b11111100u)/*5*/ {
		std::string_view varAns = arg.substr(0, 5);
		arg.remove_prefix(5);
		return std::move(varAns);
	}
	else if (varFirstChar < 0b11111110u)/*6*/ {
		std::string_view varAns = arg.substr(0, 6);
		arg.remove_prefix(6);
		return std::move(varAns);
	}
	std::string_view varAns = arg.substr(0, 7)/*7*/;
	arg.remove_prefix(7);
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
	static std::random_device varRandomDevice;
	using CharNumber = sstd::RenderState::NextNumber<wchar_t, 8, '0', '9'>;
	std::vector< std::unique_ptr<RenderCharBasic>/**/> varChars /*所有Char*/;
	const auto varTextLayerName = argRenderState->$TextLayerName.toStdWString();

	/*读取文件*/
	if (false == argRenderState->$IsFileAndStreamOpen) {
		argRenderState->$IsFileAndStreamOpen = true;
		{
			/*读取数据*/
			argRenderState->$File.setFileName(argRenderState->$TextFileName);
			if (false == argRenderState->$File.open(QIODevice::ReadOnly | QIODevice::Text))
				throw Exception(u8R"(can not open file)"sv);
			argRenderState->$Stream.setDevice(&argRenderState->$File);
			argRenderState->$Stream.setAutoDetectUnicode(true);
		}
	}

	{
		//getTextStyleTable
		auto varDB = acdbHostApplicationServices()->workingDatabase();
		AcDbObjectPointer<AcDbTextStyleTable> varBlockTable(varDB->textStyleTableId(), kForWrite);
		if (varBlockTable.openStatus() != kOk) {
			throw Exception(u8R"(can not open AcDbBlockTable)"sv);
		}

		/*创建字体集合*/
		{
			sstd::RenderState::NextNumber<wchar_t, 8, 'a', 'z'> varName;
			std::list<sstd::Font> varFonts{
				std::make_move_iterator(argRenderState->$Fonts.begin()),
				std::make_move_iterator(argRenderState->$Fonts.end())
			};
			argRenderState->$Fonts.clear();
			while (false == varFonts.empty()) {
				auto var = varFonts.front();
				varFonts.pop_front();
				varName.next();
				if (var.get_private() == nullptr) { continue; }
				//const std::wstring_view & argNM,AcDbTextStyleTable * argTST,AcDbTextStyleTableRecord * argR
				const wchar_t * argNM = varName.$String.data();

				std::unique_ptr<AcDbTextStyleTableRecord, void(*)(AcDbTextStyleTableRecord*)>
					argR(new AcDbTextStyleTableRecord, [](AcDbTextStyleTableRecord*arg) {
					if (arg->objectId().isValid()) {
						arg->close();
					}
					else {
						delete arg;
					}
				});

				if (eOk != argR->setName(argNM)) continue;

				{
					const auto varFontName = var.getFontName().toStdWString();
					if (eOk != argR->setFont(varFontName.c_str(),
						false,
						false,
						kChineseSimpCharset,
						Autodesk::AutoCAD::PAL::FontUtils::FontPitch::kFixed,
						Autodesk::AutoCAD::PAL::FontUtils::FontFamily::kModern
					)) continue;
				}

				{
					const auto varSize = argRenderState->$FontBasicSize * var.get_private()->$Scale;
					argR->setTextSize(varSize);
					argR->setPriorSize(varSize);
				}

				if (eOk == varBlockTable->add(var.get_private()->$Index, argR.get()))
					argRenderState->$Fonts.push_back(std::move(var));
			}
		}
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
				auto & varRenderCharParent = varChars.emplace_back(new RenderChar);
				auto varRenderChar = static_cast<RenderChar *>(varRenderCharParent.get());
				/* 在TextView后面加一个 0 */

				{
					unsigned int j = 0;
					for (const auto & i : varNextChar) {
						varRenderChar->$Utf8Code[j] = i;
						++j;
					}
				}

				AcString varCurrentChar(varRenderChar->$Utf8Code, AcString::Utf8);
				varCurrentCharCount.next();
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

							double varTextHeight = argRenderState->$FontBasicSize;
							/*随机设置字体*/
							if (argRenderState->$Fonts.empty() == false) {
								std::swap(*argRenderState->$Fonts.begin(), *argRenderState->$Fonts.rbegin());
							}
							std::shuffle(
								argRenderState->$Fonts.begin(),
								argRenderState->$Fonts.end(),
								varRandomDevice);
							for (const auto & varFont : argRenderState->$Fonts) {
								if (varFont.has(*__b)) {
									varMText->setTextStyle(varFont.get_private()->$Index);
									varTextHeight *= varFont.get_private()->$Scale;
									break;
								}
								else {
									continue;
								}
							}

							varMText->setTextHeight(varTextHeight);
							varMText->setContents(varCurrentChar);
							varBlockTableRecord->appendAcDbEntity(varMText.get());
							varMText->setLayer(varTextLayerName.c_str());
							varMText->setAttachment(AcDbMText::kBottomLeft);
							varMText->setLocation({ 0 , -argRenderState->$FontLineHeight  , 0 });
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

					{
						AcDbExtents varBounds;
						varCharInsert->bounds(varBounds);
						varWidth += argRenderState->$FontCharSpace +
							std::abs(varBounds.maxPoint().x - varBounds.minPoint().x)/*+字间距*/;
					}
				};

				/*判断是否创建新行*/
				if (varWidth >= varWidthMax) {
					varRenderChar->$X = varWidth;
					varRenderChar->$Y = varHeight;
					varWidth = varWidthBegin;
					/*在行尾保留的标点...*/
					const static std::set<std::string_view> _RenderRaw{
						u8R"(,)"sv,
						u8R"(.)"sv,
						u8R"(;)"sv,
						u8R"(')"sv,
						u8R"(。)"sv,
						u8R"(，)"sv,
						u8R"(：)"sv,
						u8R"(:)"sv,
					};

					/*标点符号排布在这一行*/
					const bool varNeedRenderThisChar = _RenderRaw.count(*__b) > 0;

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
				constexpr const static auto K = 3.141592654 / 180.0;
				varChar->setRotation(varFontRotate*K);
			}

			{/*移位*/
				const auto varFontDx = argRenderState->$FontDx.next();
				const auto varFontDy = argRenderState->$FontDy.next();
				varPos.x += varFontDx;
				varPos.y += varFontDy;
				varChar->setPosition(varPos);
			}

			if constexpr(true) {/*镜像*/
				const static std::set<std::string_view> varKeyWord{
					u8R"(克)"sv,u8R"(乱)"sv,u8R"(邓)"sv,
					u8R"(平)"sv,u8R"(伦)"sv,u8R"(江)"sv,
					u8R"(逼)"sv,u8R"(邪)"sv,
					u8R"(傻)"sv,u8R"(教)"sv,
					u8R"(功)"sv,u8R"(党)"sv,
					u8R"(习)"sv,u8R"(共)"sv,
					u8R"(革)"sv,u8R"(国)"sv,
					u8R"(色)"sv,u8R"(民)"sv,
					u8R"(裸)"sv,u8R"(文)"sv,
					u8R"(激)"sv,u8R"(化)"sv,
					u8R"(日)"sv,u8R"(育)"sv,
					u8R"(法)"sv,u8R"(主)"sv,
					u8R"(货)"sv,u8R"(自)"sv,
					u8R"(残)"sv,u8R"(穆)"sv,
					u8R"(赵)"sv,u8R"(林)"sv,
					u8R"(台)"sv,u8R"(斯)"sv,
					u8R"(情)"sv,u8R"(治)"sv,
					u8R"(轮)"sv,u8R"(安)"sv,
					u8R"(赤)"sv,u8R"(保)"sv,
					u8R"(命)"sv,u8R"(春)"sv,
					u8R"(近)"sv,u8R"(游)"sv,
					u8R"(宪)"sv,u8R"(行)"sv,
					u8R"(黄)"sv,u8R"(权)"sv,
					u8R"(湾)"sv,u8R"(胡)"sv,
					u8R"(感)"sv,u8R"(温)"sv,
					u8R"(本)"sv,u8R"(毛)"sv,
				};

				if (varKeyWord.count(c->$Utf8Code) > 0) {
					AcDbExtents varBound;
					varChar->bounds(varBound);
					const auto varMaxP = varBound.maxPoint();
					const auto varMinP = varBound.minPoint();

					const static AcGeVector3d varNormals1[] = {
						{ 0.05,1,0 },{ -0.05,1,0 },
						{ 0.15,1,0 },{ -0.15,1,0 },
						{ 0.07,1,0 },{ -0.07,1,0 },
						{ 0.12,1,0 },{ -0.12,1,0 },
						{ 0.10,1,0 },{ -0.10,1,0 },
						{ 0.09,1,0 },{ -0.09,1,0 },
						{ 0.08,1,0 },{ -0.08,1,0 },
					};

					const static AcGeVector3d varNormals2[] = {
						{ 1,0.05,0 },{ 1,-0.05,0 },
						{ 1,0.15,0 },{ 1,-0.15,0 },
						{ 1,0.07,0 },{ 1,-0.07,0 },
						{ 1,0.12,0 },{ 1,-0.12,0 },
						{ 1,0.10,0 },{ 1,-0.10,0 },
						{ 1,0.09,0 },{ 1,-0.09,0 },
						{ 1,0.08,0 },{ 1,-0.08,0 },
					};
					static_assert(std::size(varNormals1) == std::size(varNormals2));
					static std::uniform_int_distribution<std::size_t> var_dis{ 0, std::size(varNormals1) - 1 };

					const auto varMX = 0.5*(varMaxP.x + varMinP.x);
					const auto varMY = 0.5*(varMaxP.y + varMinP.y);
					const AcGePlane varMirrorPlane(
						{ varMX ,varMY ,0 },
						varNormals1[var_dis(sstd::RenderState::Limit::$RD)]);
					AcGeMatrix3d varMatrix;
					varMatrix.setToMirroring(varMirrorPlane);
					if (std::rand() & 1) {
						const AcGePlane varMirrorPlane(
							{ varMX ,varMY ,0 },
							varNormals2[var_dis(sstd::RenderState::Limit::$RD)]);
						varMatrix.setToMirroring(varMirrorPlane);
					}
					varChar->transformBy(varMatrix);
				}

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

bool sstd::Font::has(const std::string_view &arg) const {
	if (thisp) {
		std::uint32_t varChar;
		switch (arg.size()) {
		case 0:return false;
		case 1:varChar = arg[0]; break;
		case 2: { varChar = arg[0]; varChar &= 0b0001'1111; varChar <<= 6; varChar |= (arg[1] & 0b0011'1111);  break; }
		case 3: {
			varChar = arg[2] & 0b0011'1111;
			varChar |= (std::uint32_t(arg[1]) &std::uint32_t(0b0011'1111)) << 6;
			varChar |= (std::uint32_t(arg[0]) &std::uint32_t(0b0000'1111)) << 12;
			break;
		}
		case 4: {
			varChar = arg[3] & 0b0011'1111;
			varChar |= (std::uint32_t(arg[2]) &std::uint32_t(0b0011'1111)) << 6;
			varChar |= (std::uint32_t(arg[1]) &std::uint32_t(0b0011'1111)) << 12;
			varChar |= (std::uint32_t(arg[0]) &std::uint32_t(0b0000'0111)) << 18;
			break;
		}
		case 5: {
			varChar = arg[4] & 0b0011'1111;
			varChar |= (std::uint32_t(arg[3]) &std::uint32_t(0b0011'1111)) << 6;
			varChar |= (std::uint32_t(arg[2]) &std::uint32_t(0b0011'1111)) << 12;
			varChar |= (std::uint32_t(arg[1]) &std::uint32_t(0b0011'1111)) << 18;
			varChar |= (std::uint32_t(arg[0]) &std::uint32_t(0b0000'0011)) << 24;
			break;
		}
		case 6: {
			varChar = arg[5] & 0b0011'1111;
			varChar |= (std::uint32_t(arg[4]) &std::uint32_t(0b0011'1111)) << 6;
			varChar |= (std::uint32_t(arg[3]) &std::uint32_t(0b0011'1111)) << 12;
			varChar |= (std::uint32_t(arg[2]) &std::uint32_t(0b0011'1111)) << 18;
			varChar |= (std::uint32_t(arg[1]) &std::uint32_t(0b0011'1111)) << 24;
			varChar |= (std::uint32_t(arg[0]) &std::uint32_t(0b0000'0001)) << 30;
			break;
		}
		default:return false;
		}
		return thisp->$QFont.inFontUcs4(varChar);
	}
	return false;
}

void sstd::Font::setFontName(const QString &arg, double scale) {
	thisp = std::make_shared<Private>(arg, scale);
}

const QString & sstd::Font::getFontName() const {
	return thisp->$FontName;
}

/**
作者：UncleSugar
链接：https://www.zhihu.com/question/20308770/answer/241699602
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

Sub 字体修改()
'
' 字体修改 宏
'
Dim R_Character As Range


Dim FontSize(5)
' 字体大小在5个值之间进行波动，可以改写
FontSize(1) = "21"
FontSize(2) = "21.5"
FontSize(3) = "22"
FontSize(4) = "22.5"
FontSize(5) = "23"



Dim FontName(3)
'字体名称在三种字体之间进行波动，可改写，但需要保证系统拥有下列字体
FontName(1) = "陈静的字完整版"
FontName(2) = "萌妹子体"
FontName(3) = "李国夫手写体"

Dim ParagraphSpace(5)
'行间距 在一定以下值中均等分布，可改写
ParagraphSpace(1) = "12"
ParagraphSpace(2) = "13"
ParagraphSpace(3) = "20"
ParagraphSpace(4) = "7"
ParagraphSpace(5) = "12"

'不懂原理的话，不建议修改下列代码

For Each R_Character In ActiveDocument.Characters

VBA.Randomize

R_Character.Font.Name = FontName(Int(VBA.Rnd * 3) + 1)

R_Character.Font.Size = FontSize(Int(VBA.Rnd * 5) + 1)

R_Character.Font.Position = Int(VBA.Rnd * 3) + 1

R_Character.Font.Spacing = 0


Next

Application.ScreenUpdating = True



For Each Cur_Paragraph In ActiveDocument.Paragraphs

Cur_Paragraph.LineSpacing = ParagraphSpace(Int(VBA.Rnd * 5) + 1)


Next
Application.ScreenUpdating = True


End Sub

**/

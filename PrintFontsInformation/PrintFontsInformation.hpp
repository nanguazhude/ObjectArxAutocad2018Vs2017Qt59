#ifndef PRINTFONTSINFORMATION_HPP
#define PRINTFONTSINFORMATION_HPP

namespace sstd {

	class PrintFontsInformation {
	public:
		PrintFontsInformation();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_PrintFontsInformation)")
	};

}/*namespace sstd*/

#endif // PRINTFONTSINFORMATION_HPP

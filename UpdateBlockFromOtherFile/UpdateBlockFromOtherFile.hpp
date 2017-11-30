#ifndef UPDATEBLOCKFROMOTHERFILE_HPP
#define UPDATEBLOCKFROMOTHERFILE_HPP

#include <object_arx_global.hpp>

namespace sstd {

	class UpdateBlockFromOtherFile
	{
	public:
		UpdateBlockFromOtherFile();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_updateblockfromotherfile)")
	};

}/*namespace sstd*/

#endif // UPDATEBLOCKFROMOTHERFILE_HPP

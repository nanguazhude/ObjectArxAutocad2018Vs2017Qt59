#ifndef GETALLLAYERNAME_HPP
#define GETALLLAYERNAME_HPP

#include <vector>
#include "object_arx_global.hpp"

namespace sstd {

	class GetAllLayerName {
		using ReturnType = std::shared_ptr< std::vector<std::wstring> >;
	public:
		GetAllLayerName();
		static ReturnType getAllLayerName();
		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_printalllayername)")
	};

}/*namespace sstd*/

#endif // GETALLLAYERNAME_HPP


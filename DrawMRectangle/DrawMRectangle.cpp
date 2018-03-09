#include <object_arx_global.hpp>
#include <vector>
#include <array>
#include <optional>
#include "../ThirdPart/ADN/ADNAssocCreateConstraint.hpp"
#include "DrawMRectangle.hpp"

namespace sstd {

	DrawMRectangle::DrawMRectangle() {

	}

	void DrawMRectangle::load() {
		arx_add_main_command<DrawMRectangle>();
	}

	namespace {
		namespace _p000_ {

			class PrivatePack {
			public:
				QString $FileName;
				double $Px = 0.0;
				double $Py = 0.0;
			};

			bool _p_construct_pack(PrivatePack * arg) {
				return true;
			}

			bool _p_select_point(PrivatePack * arg) {
				return true;
			}

			bool _p_select_file(PrivatePack * arg) {
				return true;
			}

			bool _p_update_data(PrivatePack * arg) {
				return true;
			}

			void _p_draw(PrivatePack * arg) {
			}

		}/*namespace _p000_*/
	}/*namespace*/


	void DrawMRectangle::main() {
		using namespace _p000_;
		PrivatePack varData;
		/*construc pack*/
		if (false == _p_construct_pack(&varData)) {
			return;
		}
		/*select point*/
		if (false == _p_select_point(&varData)) {
			return;
		}
		/*select file*/
		if (false == _p_select_file(&varData)) {
			return;
		}
		/*update data*/
		if (false == _p_update_data(&varData)) {
			return;
		}
		/*draw*/
		_p_draw(&varData);
	}

	extern void loadDrawMRectangle() {
		DrawMRectangle::load();
	}

}/*namespace sstd*/

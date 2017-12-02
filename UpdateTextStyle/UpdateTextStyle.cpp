#include "UpdateTextStyle.hpp"

namespace sstd{

UpdateTextStyle::UpdateTextStyle(){

}

void UpdateTextStyle::load(){
	arx_add_main_command<UpdateTextStyle>();
}

void UpdateTextStyle::main(){
}

extern void loadUpdateTextStyle() { UpdateTextStyle::load(); }

}/*namespace sstd*/

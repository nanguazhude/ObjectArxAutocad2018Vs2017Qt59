#include "FARC.hpp"

namespace sstd{
extern void loadFARC(){
    FARC::load();
}
}/*namespace sstd*/

namespace sstd{

FARC::FARC(){

}

void load(){
    arx_add_main_command<FARC>();
}

void main(){

}

}/*namespace sstd*/




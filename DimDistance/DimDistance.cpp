#include "DimDistance.hpp"


namespace sstd{

extern void loadDimDistance(){
    DimDistance::load();
}

void DimDistance::load(){
     arx_add_main_command_usepickset<DimDistance>();
}

void DimDistance::main(){

}

}/*namespace sstd*/

/**
选择直线 , 
找到与直线相交的对象 [圆,直线] ,
计算直线与这些对象的交点集合 ,
在交点集合中找到距离最大的一组交点 ,
用直线连接交点 
**/
#include "DrawCrossLine.hpp"

void sstd::DrawCrossLine::load(){

}

void sstd::DrawCrossLine::main(){

}

namespace sstd{
	extern void loadDrawCrossLine() { 
		arx_add_main_command_usepickset<DrawCrossLine>();
	}
}/*namespace sstd*/

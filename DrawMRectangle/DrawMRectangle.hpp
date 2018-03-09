#ifndef DRAWMRECTANGLE_HPP
#define DRAWMRECTANGLE_HPP

#include <object_arx_global.hpp>

namespace sstd {
    class DrawMRectangle {
    public:
        DrawMRectangle();

        static void load();
        static void main();
        DEFINE_ARX_NAME(LR"(_drawmrectangle)")
    };
}

#endif // DRAWMRECTANGLE_HPP

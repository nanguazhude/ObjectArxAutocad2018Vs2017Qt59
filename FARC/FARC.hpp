#ifndef FARC_HPP
#define FARC_HPP

#include <object_arx_global.hpp>

namespace sstd{
    class FARC{
    public:
        FARC();

        static void load();
        static void main();
        DEFINE_ARX_NAME(LR"(_farc)")
    };
}

#endif // FARC_HPP

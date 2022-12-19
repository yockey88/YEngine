#ifndef YWORLD_HPP
#define YWORLD_HPP

namespace yphsx {

    class YWorld {
        typedef void (*GoodBye)(void); 

        GoodBye Bye;

        public:
            YWorld(GoodBye func) { Bye = func; }
            ~YWorld() { Bye(); }

    };

}

#endif
#ifndef ENT_BOX_HPP
#define ENT_BOX_HPP

#include "entt.hpp"
#include "entCores.hpp"

/*
    Not sure how to implement entities into the actual engine, seems game to game with entt
*/

namespace machy {
namespace entity {

    class EntBox {  
        entt::registry ents;
        public:
            EntBox() {}
            ~EntBox() {}
    };

}
}

#endif
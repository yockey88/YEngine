#ifndef ENT_MANAGER_HPP
#define ENT_MANAGER_HPP

#include "entt.hpp"

namespace machy {
namespace managers {

    class EntManager {
        entt::registry activeEnts;
        
        public:
            EntManager() {};
            ~EntManager() {};
    };

}
}

#endif
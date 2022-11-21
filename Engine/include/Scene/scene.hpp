#ifndef SCENE_HPP
#define SCENE_HPP

#include "entt/entt.hpp"

namespace machy {
namespace graphics {
    
    class Scene {
        entt::registry sceneEnts;

        public:
            Scene() {}
            ~Scene() {}
    };

}
}


#endif
#ifndef SCENE_HPP
#define SCENE_HPP

#include "map.hpp"
#include "entt.hpp"

namespace machy {
namespace scene {

    struct Movement {
        Vec2 velDir;
        int spd;
    };

    struct PlayerCore {
        Movement move;
        Vec2 pos;
        int key;
        // playerdata
    };

    class Scene {
        entt::registry sceneEnts;
        int pSideLength;

        Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

        public:
            Scene() : pSideLength(-1) {}
            ~Scene() {}
 
            entt::registry& seeEnts() { return sceneEnts; }

            void create(const int& sideLength);

            void setPlayerData(PlayerCore& PlayerData , Movement& playerVel);

            void updateMovers();
    };

}
}

#endif
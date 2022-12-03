#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace machy {
namespace graphics {

    class Sprite2D;
    class SpriteAtlas2D;

    class Scene {
        std::shared_ptr<SpriteAtlas2D> spriteCollection;
        public:
            Scene();

            void update();
            void render();
    };

}
}

#endif
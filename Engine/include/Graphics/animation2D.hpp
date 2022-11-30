#ifndef ANIMATION_2D_HPP
#define ANIMATION_2D_HPP

#include "Core/deltaTime.hpp"

#include "glm/glm.hpp"

#include <stdint.h>
#include <vector>
#include <chrono>
#include <memory>

namespace machy {
namespace graphics {

    class Sprite2D;
    class SpriteAtlas2D;
    class Material;

    class Animation2D {
        std::shared_ptr<Material> material;
        std::shared_ptr<Sprite2D> activeSprite;
        std::shared_ptr<SpriteAtlas2D> frameAtlas;
        std::vector<std::shared_ptr<Sprite2D>> frames;

        core::Timer timer;

        glm::vec2 frameSize;
        glm::ivec2 frameLayout; 

        uint32_t tWidth , tHeight;
        int currentFrame;
        float frameLength , currFrameTime;
        bool playing;


        void calcFrameSize();
        public:
            Animation2D(std::shared_ptr<Material> material , glm::ivec2 frameLayout);

            void addFrameToAnimation(const glm::ivec2& layoutPos);

            void update();
            void render();

            void setAnimationPos(glm::vec2 pos);
    };

}
}

#endif
#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "Graphics/animation2D.hpp"
#include "Graphics/spriteAtlas2D.hpp"
#include "Graphics/sprite2D.hpp"

#include "glm/glm.hpp"

using namespace machy;

enum class direction { left , right , up , down };

struct PlayerCore {
    std::shared_ptr<graphics::Animation2D> activeAnimation;
    std::shared_ptr<graphics::Animation2D> playerIdleL;
    std::shared_ptr<graphics::Animation2D> playerIdleR;
    std::shared_ptr<graphics::Animation2D> playerRunL;
    std::shared_ptr<graphics::Animation2D> playerRunR;
    direction dir;
    glm::vec2 playerPos;
    float playerSpd;
};

struct SceneCore {
    std::shared_ptr<graphics::SpriteAtlas2D> worldMap;

    void render();
};

#endif
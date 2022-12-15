#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "Core/assetLibrary.hpp"

#include "Graphics/camera.hpp"
#include "Graphics/animation2D.hpp"
#include "Graphics/spriteAtlas2D.hpp"
#include "Graphics/sprite2D.hpp"
#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/material.hpp"

#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "glm/glm.hpp"
#include "entt.hpp" 

#include <map>
#include <unordered_map>
#include <memory>

using namespace machy;

enum class direction { left , right , up , down };

// enum class game_state {  };

struct PlayerCore {
    std::shared_ptr<graphics::Animation2D> activeAnimation;
    std::shared_ptr<graphics::Animation2D> playerIdleL;
    std::shared_ptr<graphics::Animation2D> playerIdleR;
    std::shared_ptr<graphics::Animation2D> playerRunL;
    std::shared_ptr<graphics::Animation2D> playerRunR;
    direction dir;
    glm::vec2 playerPos;
    glm::vec4 bounds;
    float playerSpd;

    void playerInputs();
    void calcBounds();
};

struct SceneCore {
    std::map<char , std::shared_ptr<graphics::Sprite2D>> map;
    std::shared_ptr<graphics::SpriteAtlas2D> atlas;

    std::vector<glm::vec4> tileBounds;

    int blocksPerSide;
    std::string mapStr;

    bool isColliding(std::shared_ptr<PlayerCore> player);

    void initScene(std::shared_ptr<graphics::Material> mat);
    void calcBounds(const glm::vec2& pos , glm::ivec2 IJ);
    void buildBaseMap(std::shared_ptr<graphics::Material> mat);

    void render();
};

struct TimeKeeper {
    core::Timer clock;

    float now , last;

    TimeKeeper() : now(0.f) , last(0.f) {}
};

struct Libraries {
    core::AssetLibrary<graphics::VertexArray> VertLib;
	core::AssetLibrary<graphics::Shader> ShaderLib;
	core::AssetLibrary<graphics::Texture> TextureLib;
	core::AssetLibrary<graphics::Material> MaterialLib;
};

struct CameraState {
    std::shared_ptr<graphics::Camera> camera;
	glm::vec3 cameraPos;
	float cameraRotation;
};


#endif
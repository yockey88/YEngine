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

#include "glm/glm.hpp"

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
    float playerSpd;
};

struct SceneCore {
    std::shared_ptr<graphics::SpriteAtlas2D> worldMap;

    void render();

    void calculateSpritesToRender(const glm::vec2& playerPos);
};

struct TimeKeeper {
    core::Timer clock;

    float msBuildUp , delta;
    float now , last;

    TimeKeeper() : now(0.f) , last(0.f) , msBuildUp(0.f) , delta(0.f) {}
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

struct ImGuiControls {
    bool RenderGui;

    bool RenderLibs;
    bool RenderControls;
    bool RenderGameView;

    bool RenderDebugTools;
};

#endif
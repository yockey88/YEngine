#include "main.hpp"
#include "app.hpp"

#include "advUtil.hpp"

#include "Core/assetLibrary.hpp"

#include "Graphics/camera.hpp"
#include "Graphics/vertex.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/material.hpp"
#include "Graphics/sprite2D.hpp"
#include "Graphics/spriteAtlas2D.hpp"
#include "Graphics/animation2D.hpp"

#include <memory>

namespace machy {

class Adventure : public App {

    core::AssetLibrary<graphics::VertexArray> VALib;
    core::AssetLibrary<graphics::Texture> TextLib;
    core::AssetLibrary<graphics::Shader> ShaderLib;
    core::AssetLibrary<graphics::Material> MaterialLib;
    
    std::shared_ptr<graphics::Camera> camera;
    CameraState camState;

    void InitLibs();

    public:

        Adventure() {}

        virtual core::WindowProperties GetWindowProperties() override {
            core::WindowProperties props;

            props.guiProps.isDockingEnabled = true;
            props.guiProps.moveFromTitleBarOnly = true;
            props.guiProps.isViewportEnabled = false;

            props.w = 2048;
            props.h = 1152;

            props.title = "Adventure";

            return props;
        }
            
        virtual void Initialize() override {

            return;
        }

        virtual void Shutdown() override {}

        virtual void Update(const float& dt) override {}

        virtual void Render() override {}

        virtual void ImGuiRender() override {
            ImGui::ShowDemoWindow();
        }
};

void Adventure::InitLibs() {
    return;
}

}

machy::App* CreateApp() { return new machy::Adventure; }
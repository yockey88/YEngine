#ifndef EDITOR_GUI_HPP
#define EDITOR_GUI_HPP

#include "sceneSettings.hpp"
#include "core/editorState.hpp"

#include "Core/assetLibrary.hpp"

#include "Game/scene.hpp"
#include "Game/Entity/entity.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/material.hpp"

namespace machy {

    struct Libraries {
        core::AssetLibrary<graphics::VertexArray> VertLib;
        core::AssetLibrary<graphics::Shader> ShaderLib;
        core::AssetLibrary<graphics::Texture> TextureLib;
        core::AssetLibrary<graphics::Material> MaterialLib;
    };

    class EditorGUI {
        std::shared_ptr<game::Scene> context;
        SceneSettingGUI activeSceneWindow;
        std::string loadPath , pathBuffer , sceneSelectContext;

        void renderControlPanel(EditorState& state);
        void controlMenuBar(EditorState& state);

        void ActiveSceneWindow(EditorState& state);
        void ImGuiRenderGameView();
        public:
            EditorGUI();
            ~EditorGUI() {}

            void RenderEditor(EditorState& state);    

            void setSceneContext(std::shared_ptr<game::Scene> scene);
            inline std::shared_ptr<game::Scene>& getContext() { return context; }
            inline SceneSettingGUI& getSceneSettings() { return activeSceneWindow; }
    };

namespace testing {

    

}

}

#endif
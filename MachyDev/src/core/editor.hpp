#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "gui/sceneSettings.hpp"

#include "Graphics/camera.hpp"

#include "ImGuizmo.h"

namespace machy {

    class GameEditor {
        EditorState state;
        SceneSettingGUI activeScene;
        
        std::shared_ptr<game::Scene> context;

        // Ent To Hold Editor Camera
        std::shared_ptr<graphics::Camera> editorCam;
        glm::vec3 editorCamPos; 
        float editorCamRot;
        bool renderingEditorCam;

        ImGuizmo::OPERATION activeOp;

        std::string loadPath , pathBuffer , sceneSelectContext , assetSelectContext;

        void renderControlPanel();
        void controlMenuBar();
        void renderAssetBrowser();

        void ActiveSceneWindow();
        void ImGuiRenderGameView();
        public:
            GameEditor();
            ~GameEditor() {}

            void UpdateEditor(const float& dt);
            void SceneRender();
            void RenderEditor(); 
            void RenderEditorCam();
            void FlushCams();
            std::shared_ptr<game::Scene> CreateEditorContext();

            void setSceneContext(std::shared_ptr<game::Scene> scene);
            inline std::shared_ptr<game::Scene> getContext() { return context; }
            inline EditorState& getState() { return state; }
            inline SceneSettingGUI& getSceneSettings() { return activeScene; }
    };

namespace testing {

    

}

}

#endif
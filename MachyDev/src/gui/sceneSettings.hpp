#ifndef SCENE_SETTING_HPP
#define SCENE_SETTING_HPP

#include "core/editorState.hpp"

#include "Core/window.hpp"

#include "Game/scene.hpp"
#include "Game/Entity/scriptedEntity.hpp"

#include <string>
#include <stdint.h>

namespace machy {

    class SceneSettingGUI {
        std::shared_ptr<game::Scene> sceneContext;
        game::Entity selectionContext , entToDelete;
        bool deleteEnt;

        void drawEntNode(game::Entity& ent , EditorState& state);
        void drawComponents();

        template <typename T>
        void addComponentEntry(const std::string& entryname);
        public:
            SceneSettingGUI() : sceneContext(nullptr) , deleteEnt(false) {}
            SceneSettingGUI(const std::shared_ptr<game::Scene>& scene) : sceneContext(scene){ setContext(scene); }
            ~SceneSettingGUI() {}

            void setContext(const std::shared_ptr<game::Scene>& scene);

            inline std::shared_ptr<game::Scene>& getContext() { return sceneContext; }

            void GuiRender(EditorState& state);
    };

}

#endif
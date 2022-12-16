#ifndef SCENE_SETTING_HPP
#define SCENE_SETTING_HPP

#include "core/editorState.hpp"

#include "Core/window.hpp"

#include "Game/scene.hpp"
#include "Game/Entity/scriptedEntity.hpp"

#include <string>
#include <stdint.h>
#include <queue>

namespace machy {

    class SceneSettingGUI {
        std::shared_ptr<game::Scene> sceneContext;
        game::Entity selectionContext;
        std::queue<game::Entity> deletedEntities;

        void drawEntNode(game::Entity& ent , EditorState& state);
        void drawComponents();

        template <typename T>
        void addComponentEntry(const std::string& entryname);
        template <typename T>
        void deleteComponentEntry(const std::string& entryname);
        public:
            SceneSettingGUI() : sceneContext(nullptr) {}
            SceneSettingGUI(const std::shared_ptr<game::Scene>& scene) : sceneContext(scene){ setContext(scene); }
            ~SceneSettingGUI() {}

            void setContext(const std::shared_ptr<game::Scene>& scene);

            inline game::Entity getSelectionContext() const { return selectionContext; } 
            inline std::shared_ptr<game::Scene>& getContext() { return sceneContext; }

            void GuiRender(EditorState& state);

            void flushDeletions();
    };

}

#endif
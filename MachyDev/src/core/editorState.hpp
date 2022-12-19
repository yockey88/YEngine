#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include "log.hpp"

#include "Core/deltaTime.hpp"
#include "Game/scene.hpp"

namespace machy {

    enum class States { MainState , Editing , Playtesting };
    enum class Windows { Gui , Controls , Libraries , Debug , AssetBrowser , GameView };
    enum class UpdateAction { sceneSwitch };

    class EditorState {
        core::Timer clock;
        float now , last;

        bool inMainState , editingScene;
        bool showingGui , showingSceneInfo;
        bool showingControls , showingLibs , showingDebug;
        bool showingSceneBrowser , showingAssetBrowser , showingGameView;

        float currFrameLength;

        void setToDefault();
        public:
            EditorState();
            ~EditorState() = default;

            void checkInputs(std::shared_ptr<game::Scene> scene);
            void tickTimer();

            void flipState(States state);
            void toggleWindow(Windows window);

            void queueAction(UpdateAction action);
            void flushActions() {}

            inline bool isInMainState() const { return inMainState; } 
            inline bool isEditingScene() const { return editingScene; }

            inline bool isShowingGui() const { return showingGui; }
            inline bool* isShowingGuiPntr() { return &showingGui; }
            inline bool isShowingSceneInfo() const { return showingSceneInfo; }

            inline bool isShowingControls() const { return showingControls; }
            inline bool isShowingLibs() const { return showingLibs; }
            inline bool isShowingDebug() const { return showingDebug; }
            inline bool isShowingSceneBrowser() const { return showingSceneBrowser; }
            inline bool isShowingAssetBrowser() const { return showingAssetBrowser; }
            inline bool isShowingGameView() const { return showingGameView; }
    };

}

#endif
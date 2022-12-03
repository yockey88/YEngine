#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

namespace machy {

    enum class States { MainState , Editing , Playtesting };
    enum class Windows { Gui , Controls , Libraries , Debug , AssetBrowser , GameView };

    class EditorState {
        bool inMainState;
        bool showingGui;
        bool showingControls , showingLibs , showingDebug;
        bool showingSceneBrowser , showingAssetBrowser , showingGameView;
        bool editingScene;

        float currFrameLength;

        void setToDefault();
        public:
            EditorState();
            ~EditorState() = default;

            void flipState(States state);
            void toggleWindow(Windows window);

            inline bool isInMainState() const { return inMainState; } 

            inline bool isShowingGui() const { return showingGui; }
            inline bool* isShowingGuiPntr() { return &showingGui; }

            inline bool isShowingControls() const { return showingControls; }
            inline bool isShowingLibs() const { return showingLibs; }
            inline bool isShowingDebug() const { return showingDebug; }
            inline bool isShowingSceneBrowser() const { return showingSceneBrowser; }
            inline bool isShowingAssetBrowser() const { return showingAssetBrowser; }
            inline bool isShowingGameView() const { return showingGameView; }

            inline bool isEditingScene() const { return editingScene; }
    };

}

#endif
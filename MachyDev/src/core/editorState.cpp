#include "editorState.hpp"

namespace machy {

    void EditorState::setToDefault() {
        inMainState = true;

        showingGui = true;
        showingControls = true;
        showingLibs = true;
        showingAssetBrowser = true;
        showingGameView = true;

        showingDebug = false;
        editingScene = false;

        return;
    }

    EditorState::EditorState() : currFrameLength(0.f) {
        inMainState = true;

        showingGui = true;
        showingControls = true;
        showingLibs = true;
        showingAssetBrowser = true;
        showingGameView = true;

        showingDebug = false;
        editingScene = false;        
    }

    void EditorState::flipState(States s) {

        switch (s) {
            case States::MainState: setToDefault(); break;

            case States::Editing:
                if (!editingScene) editingScene = true;
                inMainState = false;
            break;

            case States::Playtesting: 
                if (editingScene) editingScene = false;
                if (showingGui) showingGui = false;
                inMainState = false;
            break;

            default: break;
        }

        return;
    }

    void EditorState::toggleWindow(Windows w) {

        switch (w) {
            case Windows::Gui: showingGui = !showingGui;  break;
            case Windows::Controls: showingControls = !showingControls; break;
            case Windows::Libraries: showingLibs = !showingControls; break;
            case Windows::Debug: showingDebug = !showingDebug; break;
            case Windows::AssetBrowser: showingAssetBrowser = !showingAssetBrowser; break;
            case Windows::GameView: showingGameView = !showingGameView; break;

            default: break;
        }

        return;
    }

}
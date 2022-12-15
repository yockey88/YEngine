#include "editorState.hpp"

#include "machy.hpp"
#include "Input/keyboard.hpp"

namespace machy {

    void EditorState::setToDefault() {
        inMainState = true;

        showingGui = true;
        showingSceneInfo = true;
        showingControls = true;
        showingLibs = true;
        showingAssetBrowser = true;
        showingGameView = true;

        swappingScenes = false;

        showingDebug = false;
        editingScene = false;

        return;
    }

    EditorState::EditorState() : currFrameLength(0.f) , now(0.f) , last(0.f) {
        inMainState = true;

        showingGui = true;
        showingSceneInfo = true;
        showingControls = true;
        showingLibs = true;
        showingAssetBrowser = true;
        showingGameView = true;

        swappingScenes = false;

        showingDebug = false;
        editingScene = false;

        clock.start();        
    }

    void EditorState::checkInputs() {

        if (input::keyboard::keyDown(MACHY_INPUT_KEY_GRAVE) && !isShowingGui()) {
            toggleWindow(Windows::Gui);
            MachY::Instance().getWindow().setRenderToScrn(!isShowingGui());
        }
        if (input::keyboard::keyDown(MACHY_INPUT_KEY_H) && !isInMainState()) {
            flipState(States::MainState);
            MachY::Instance().getWindow().setRenderToScrn(false);
        }

        return;
    }

    void EditorState::tickTimer() {
        clock.step();
        return;
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

    void EditorState::queueAction(UpdateAction a) {
        
        switch (a) {
            case UpdateAction::sceneSwitch: swappingScenes = true; break;

            default: break;
        }

        return;
    }

}
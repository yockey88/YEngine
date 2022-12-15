#include "editorGUI.hpp"

#include "machy.hpp"

#include "Core/fileSystem.hpp"

#include "Game/sceneSerializer.hpp"

#include "Graphics/framebuffer.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "entt.hpp"

#include <string>
#include <filesystem>

static char buffer[512];
static std::string scenePath = "resources/scenes";

namespace machy {

    EditorGUI::EditorGUI() {
        loadPath = "resources/scenes/devScene.json";
    }

    void EditorGUI::renderControlPanel(EditorState& state) {

        bool show = state.isShowingGui();
        if (ImGui::Begin("MainControls" , &show , ImGuiWindowFlags_MenuBar)) {
            if (ImGui::Button("Game View")) {
                state.toggleWindow(Windows::Gui);
                MachY::Instance().getWindow().setRenderToScrn(!state.isShowingGui());
            }

            ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
            if (ImGui::BeginChild("Scene Select" , ImVec2(ImGui::GetContentRegionAvail().x , 260) , false, flags)) {
                
                ImGui::Text("Scene Select");
                bool selected = false;
                for (const auto entry : std::filesystem::directory_iterator(scenePath)) {
                    std::filesystem::path path = entry.path();
                    std::string dispPath = path.make_preferred().string();
                    if (ImGui::Selectable(dispPath.c_str() , &selected)) 
                        sceneSelectContext = path.string();
                }
                ImGui::EndChild();

                if (selected) {
                    MACHY_INFO(sceneSelectContext);
                    /*
                        function pointer to scene swtich?
                    */
                }
            }
            
            controlMenuBar(state);

        }
        ImGui::End();

        return;
    }

    void EditorGUI::controlMenuBar(EditorState& state) {

        std::string path = "resources/scenes/" + context->getName() + ".json";

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Editing")) {
                if (ImGui::MenuItem("Playtest Game")) {

                    state.flipState(States::Playtesting);
                    MachY::Instance().getWindow().setRenderToScrn(!state.isShowingGui());

                }
                if (ImGui::MenuItem("Edit Scene")) { state.flipState(States::Editing); }
                if (ImGui::MenuItem("New Scene")) {

                    GameSceneSerializer serializer(context);
                    serializer.serialize(path);
                    context = std::make_shared<game::Scene>();
                    setSceneContext(context);
                    state.queueAction(UpdateAction::sceneSwitch);

                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save Scene")) {
                    GameSceneSerializer serializer(context);
                    serializer.serialize(path);
                }
                if (ImGui::MenuItem("Load Scene")) {
                    GameSceneSerializer serializer(context);
                    context = serializer.deserialize(loadPath);
                    setSceneContext(context);
                    state.queueAction(UpdateAction::sceneSwitch);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        return;
    }

    void EditorGUI::ActiveSceneWindow(EditorState& state) {
            
        activeSceneWindow.GuiRender(state);

        return;
    }

    void EditorGUI::ImGuiRenderGameView() {
        if (ImGui::Begin("GameView")) {
            auto& window = MachY::Instance().getWindow();
            ImVec2 winSize = ImGui::GetWindowSize();
            glm::ivec2 arSize = window.getCorrectAspectRatioSize((int)winSize.x - 15, (int)winSize.y - 35);
            ImVec2 size = { (float)arSize.x , (float)arSize.y };
            ImVec2 pos = { (winSize.x - size.x) * 0.5f , ((winSize.y - size.y) * 0.5f) + 7 };
            ImVec2 uv0  = { 0 , 1 };
            ImVec2 uv1  = { 1 , 0 };
            ImGui::SetCursorPos(pos);
            ImGui::Image((void*)(intptr_t)window.getFrameBuffer()->getTextureID() , size , uv0 , uv1);
        }
        ImGui::End();
    }

    void EditorGUI::RenderEditor(EditorState& state) {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        bool show = state.isShowingGui();
        renderControlPanel(state);
        if (state.isShowingGameView())  { ImGuiRenderGameView(); }
        if (state.isShowingSceneInfo()) { ActiveSceneWindow(state);   }
        

        return;
    }

    void EditorGUI::setSceneContext(std::shared_ptr<game::Scene> scene) {
        context = scene;
        activeSceneWindow.setContext(scene);
        loadPath = context->getPath();
    }

}
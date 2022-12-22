#include "editor.hpp"

#include "machy.hpp"

#include "scripts/cameraScript.hpp"
#include "scripts/playerScript.hpp"

#include "Core/fileSystem.hpp"
#include "Math/math.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/material.hpp"
#include "Graphics/framebuffer.hpp"

#include "Game/sceneSerializer.hpp"
#include "Game/Entity/entityComponents.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "entt.hpp"

#include <string>
#include <filesystem>
#include <stdint.h>

static char buffer[512];
static std::string scenePath = "resources/scenes";
static std::string shaderPath = "resources/shaders";
static std::string materialPath = "resources/assets/materials";
static std::string characterSpritePath = "resources/sprites/characters";
static std::string objectSpritePath = "resources/sprites/objects";
static std::string tilesetPath = "resources/sprites/tilesets";

namespace machy {

    GameEditor::GameEditor() {
        activeOp = ImGuizmo::OPERATION::TRANSLATE;
        renderingEditorCam = true; renderedEditorCam = false;
        editorCam = std::make_shared<graphics::Camera>();
        editorCamPos = { 0.f , 0.f , 25.f };
        editorCamRot = 0.f;
        editingScene = std::shared_ptr<game::Scene>{ nullptr };
    }

    void GameEditor::renderControlPanel() {
        bool show = state.isShowingGui();
        if (ImGui::Begin("MainControls" , &show , ImGuiWindowFlags_MenuBar)) {
            
            if (ImGui::Button("Game View"))
                if (context->Entts().view<game::CameraComponent>().size() >= 0)
                    if (state.isShowingGui()) {
                        state.toggleWindow(Windows::Gui);
                        MachY::Instance().getWindow().setRenderToScrn(!state.isShowingGui());
                    }

            if (ImGui::Button("Play Scene")) {
                if ((context->Entts().view<game::CameraComponent>().size() >= 0) && !context->isPlaying()) {
                    renderingEditorCam = false;
                    context->playScene();
                    state.toggleWindow(Windows::Gui);
                    MachY::Instance().getWindow().setRenderToScrn(!state.isShowingGui());
                } else if (!context->isPlaying()) {
                    context->playScene();
                    state.toggleWindow(Windows::Gui);
                    MachY::Instance().getWindow().setRenderToScrn(!state.isShowingGui());
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause Scene")) {
                if (context->isPlaying()) {
                    renderingEditorCam = true;
                    context->pauseScene();
                    context->viewWorld()->Dump();
                }
            }
            {
                ImGui::Text("Editor Camera Settings");
                editorCamPos.z = editorCam->getHeight();
                ImGui::DragFloat3("Editor Camera Pos" , glm::value_ptr(editorCamPos) , 0.02f);
                ImGui::DragFloat("Camera Rotation" , &editorCamRot , 0.02f);
                editorCam->setHeight(editorCamPos.z);
                editorCam->setViewMat(editorCamPos , editorCamRot);
            }

            ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
            if (ImGui::BeginChild("Scene Select" , ImVec2(ImGui::GetContentRegionAvail().x , 260) , false, flags)) {
                
                ImVec4 col{ 0 , 1 , 0 , 1};
                ImGui::TextColored(col , "Scene Select");
                bool selected = false;
                for (const auto entry : std::filesystem::directory_iterator(scenePath)) {
                    if (!entry.is_directory()) {
                        std::filesystem::path path = entry.path();
                        path = path.make_preferred();
                        std::string name = path.filename().string();
                        if (path.extension().string() == ".json")
                            if (ImGui::Selectable(name.c_str() , &selected)) 
                                sceneSelectContext = path.string();
                    }
                }
                ImGui::EndChild();

                if (selected) {
                    core::FileSystem::saveScene(context);
                    setSceneContext(core::FileSystem::loadScene(sceneSelectContext));

                    context->pauseScene();
                    renderingEditorCam = true;

                    auto view = context->Entts().view<game::NativeScript>();
                    for (auto ent : view) {

                        game::Entity entity{ ent };
                        entity.setContext(context.get());

                    }
                }

            }
            
            controlMenuBar();

            renderAssetBrowser();

        }
        ImGui::End();

        return;
    }

    void GameEditor::controlMenuBar() {

        std::string path = "resources/scenes/" + context->getName() + ".json";

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Editing")) {
                if (ImGui::MenuItem("Playtest Game")) {
                    renderingEditorCam = false;
                    context->playScene();
                    state.flipState(States::Playtesting);
                    MachY::Instance().getWindow().setRenderToScrn(!state.isShowingGui());

                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File")) {
                SceneSerializer serializer(context);
                if (ImGui::MenuItem("Save Scene")) 
                    serializer.serialize(path);
                if (ImGui::MenuItem("Create Blank Scene")) {
                    SceneSerializer serializer(context);
                    serializer.serialize(path);
                    context = std::make_shared<game::Scene>();
                    setSceneContext(context);

                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools")) {

                if (ImGui::MenuItem("Move Select Entity"))   { activeOp = ImGuizmo::OPERATION::TRANSLATE; }
                if (ImGui::MenuItem("Rotate Select Entity")) { activeOp = ImGuizmo::OPERATION::ROTATE; }
                if (ImGui::MenuItem("Scale Select Entity"))  { activeOp = ImGuizmo::OPERATION::SCALE; }
                if (ImGui::MenuItem("Get Entity Bounds"))  { activeOp = ImGuizmo::OPERATION::BOUNDS; }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        return;
    }

    void GameEditor::renderAssetBrowser() {
        
        ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
        if (ImGui::BeginChild("Asset Browser" , ImVec2(ImGui::GetContentRegionAvail().x , 260) , false, flags)) {

            ImVec4 col{ 0 , 1 , 0 , 1};
            ImGui::TextColored(col , "Asset Select");

            if (ImGui::TreeNode("Blank Materials")) {
                bool selected = false;
                for (const auto entry : std::filesystem::directory_iterator(characterSpritePath)) {
                    if (!entry.is_directory()) {

                        std::filesystem::path path = entry.path();
                        path = path.make_preferred();
                        std::string name = path.filename().string();

                        if (path.extension().string() == ".png") {
                            
                            if (ImGui::Selectable(name.c_str() , &selected))
                                assetSelectContext = path.string();
                            
                            if (ImGui::BeginDragDropSource()) {
                                const wchar_t* itemPath = path.relative_path().c_str();
                                assetSelectContext = path.string();
                                ImGui::SetDragDropPayload("CHARACTER_SPRITE_DRAG_ITEM" , itemPath , (wcslen(itemPath) + 1) * sizeof(wchar_t) , ImGuiCond_Once);
                                ImGui::EndDragDropSource();
                            }
                        
                        }

                    }
                }

                ImGui::TreePop();
            }

            ImGui::EndChild();
        }
        
        return;
    }

    void GameEditor::ActiveSceneWindow() {

        activeScene.GuiRender(state);

        return;
    }

    void GameEditor::ImGuiRenderGameView() {
        if (ImGui::Begin("GameView")) {
            auto& window = MachY::Instance().getWindow();
            ImVec2 winSize = ImGui::GetWindowSize();
            glm::ivec2 arSize = window.getCorrectAspectRatioSize((int)winSize.x - 15, (int)winSize.y - 35);
            ImVec2 size = { (float)arSize.x , (float)arSize.y };
            ImVec2 guipos = { (winSize.x - size.x) * 0.5f , ((winSize.y - size.y) * 0.5f) + 7 };
            ImVec2 uv0  = { 0 , 1 };
            ImVec2 uv1  = { 1 , 0 };
            ImGui::SetCursorPos(guipos);
            ImGui::Image((void*)(intptr_t)window.getFrameBuffer()->getTextureID() , size , uv0 , uv1);

            game::Entity selected = activeScene.getSelectionContext();
            if (selected != activeScene.getContext()->getNullEnt() && !context->isPlaying()) {
                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist();
                
                game::Entity cameraEnt = context->getMainCameraEntity();
                if (cameraEnt != context->getNullEnt()) {
                    const auto& cam = cameraEnt.GetComponent<game::CameraComponent>();
                    auto& pos = selected.GetComponent<game::PositionComponent>();

                    float guiWidth = (float)ImGui::GetWindowWidth();
                    float guiHeight = (float)ImGui::GetWindowHeight();
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x , ImGui::GetWindowPos().y , guiWidth , guiHeight);

                    glm::mat4 camProj = cam.camera->getProjectionMatrix();
                    glm::mat4 camView = glm::inverse(cam.camera->getViewMatrix());
                    glm::mat4 entModel = pos.getModel();

                    ImGuizmo::Manipulate(glm::value_ptr(camView) , glm::value_ptr(camProj),
                                        activeOp , ImGuizmo::LOCAL , glm::value_ptr(entModel));

                    if (ImGuizmo::IsUsing()) {
                        glm::vec3 t , r , s;
                        math::decompose(entModel , t , r , s);

                        glm::vec3 deltaRot = r - pos.rotation;
                        pos.pos.x = t.x;
                        pos.pos.y = t.y;
                        pos.rotation += deltaRot;
                        pos.size = s;
                    }
                }
                if (selected.HasComponent<game::CameraComponent>()) {
                    auto& entCam = selected.GetComponent<game::CameraComponent>();
                    auto& pos = selected.GetComponent<game::PositionComponent>();
                    entCam.cameraPos = pos.pos;
                    entCam.camera->setViewMat(entCam.cameraPos , entCam.cameraRotation);
                }
            }
        }
        ImGui::End();
    }

    void GameEditor::UpdateEditor(const float& dt) {

        state.tickTimer();
		state.checkInputs(context);
        
        if (context->isPlaying()) {
            context->updateRuntime(dt);
        } else {
            context->updateFromEditor();
        }

        return;
    }

    void GameEditor::SceneRender() {

        context->render();

        return;
    }

    void GameEditor::RenderEditor() {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        bool show = state.isShowingGui();
        renderControlPanel();
        if (state.isShowingGameView())  { ImGuiRenderGameView(); }
        if (state.isShowingSceneInfo()) { ActiveSceneWindow();   }

        activeScene.flushDeletions();  
        return;
    }

    void GameEditor::RenderEditorCam() {

        if (renderingEditorCam && !renderedEditorCam) {
            MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , editorCam));
            renderedEditorCam = true;
        }

        return;
    }

    void GameEditor::FlushCams() {
        
        if (renderingEditorCam || renderedEditorCam) {
            MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));
            renderedEditorCam = false;
        }

        return;
    }

    std::shared_ptr<game::Scene> GameEditor::CreateEditorContext() {

        context = std::make_shared<game::Scene>();
        context->setScenePath("resources/scenes/devScene.json");
        setSceneContext(context);

        editorCamPos.z = 25.f;
        editorCam->setHeight(editorCamPos.z);

        return context;
    }

    void GameEditor::setSceneContext(std::shared_ptr<game::Scene> scene) {
        context = scene;
        activeScene.setContext(scene);
        loadPath = context->getPath();
    }

}
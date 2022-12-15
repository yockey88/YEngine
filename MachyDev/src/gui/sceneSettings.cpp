#include "sceneSettings.hpp"

#include "Game/scene.hpp"
#include "Game/sceneSerializer.hpp"
#include "Game/Entity/entity.hpp"
#include "Game/Entity/entityComponents.hpp"

#include "Graphics/material.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "ImEntt.hpp"

#include <stdint.h>

static char buffer[512];

namespace machy {

    void SceneSettingGUI::setContext(const std::shared_ptr<game::Scene>& scene) {
        sceneContext = scene;
        selectionContext = sceneContext->getNullEnt();
        entToDelete = sceneContext->getNullEnt();
    }

    void SceneSettingGUI::drawEntNode(game::Entity& ent , EditorState& state) {
        
        auto& id = sceneContext->Entts().get<game::EntityID>(ent.get());
        std::string displayName = id.name;

        ImGuiTreeNodeFlags flags = ((selectionContext == ent.get()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent.get() , flags , displayName.c_str())) {
            
            if (ImGui::IsItemClicked())
                selectionContext = ent;

            bool deleteEnt = false;
            if (ImGui::BeginPopupContextItem()) {

                if (ImGui::MenuItem("Delete Entity")) 
                    deleteEnt = true;

                ImGui::EndPopup();
            }
            
            ImGui::TreePop();

            if (deleteEnt) {
                sceneContext->destroyEntity(selectionContext);
                if (selectionContext == ent)
                    selectionContext = sceneContext->getNullEnt();
            }
        }

        return;
    }

    void SceneSettingGUI::drawComponents() {

        if (selectionContext == sceneContext->getNullEnt()) return;
        if (selectionContext.HasComponent<game::EntityID>()) {
            auto& id = sceneContext->Entts().get<game::EntityID>(selectionContext.get());

            ImGui::Text("%02d | %s" , id.ID , id.name.c_str());
            
            memset(buffer , 0 , sizeof(buffer));
            strcpy_s(buffer , id.name.c_str());
            if (ImGui::InputText("Entity Name" , buffer , sizeof(buffer)))
                id.name = std::string(buffer);

        }
        ImGui::SameLine();
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent")) {
            addComponentEntry<game::PositionComponent>("Position");
            addComponentEntry<game::RenderComponent>("Sprite");
            addComponentEntry<game::CameraComponent>("Camera");

            ImGui::EndPopup();
        }

        if (selectionContext.HasComponent<game::RenderComponent>()) {
            auto& sprite = sceneContext->Entts().get<game::RenderComponent>(selectionContext.get());

            ImGui::ColorEdit3("Sprite Color" , glm::value_ptr(sprite.color));
            sprite.material->setUniformValue("inColor" , sprite.color);

            
            bool selected = false; 
            std::string chosenName;
            if (ImGui::TreeNode("Materials Stored")) {

                for (const auto& material : sceneContext->getMatLib().getAllAssets()) {
                    std::string display = material.first + "##MaterialSelector";
                    if (ImGui::Selectable(display.c_str() , &selected)) {
                        chosenName = material.first;
                        selected = true;
                    }
                }

                if (selected)
                    sprite.material = sceneContext->getMatLib().get(chosenName);

                ImGui::TreePop();
            }

        }

        if (selectionContext.HasComponent<game::PositionComponent>()) {
            auto& pos = sceneContext->Entts().get<game::PositionComponent>(selectionContext.get());

            ImGui::DragFloat2("Entity Position" , glm::value_ptr(pos.pos) , 0.02f);
            ImGui::DragFloat2("Entity Size" , glm::value_ptr(pos.size) , 0.02f);
        }

        if (selectionContext.HasComponent<game::CameraComponent>()) {
            auto& cam = sceneContext->Entts().get<game::CameraComponent>(selectionContext.get());

            cam.cameraPos.z = cam.camera->getHeight();
            ImGui::DragFloat3("Camera Pos" , glm::value_ptr(cam.cameraPos) , 0.02f);
            ImGui::DragFloat("Camera Rotation" , &cam.cameraRotation , 0.02f);
            cam.camera->setHeight(cam.cameraPos.z);
            cam.camera->setViewMat(cam.cameraPos , cam.cameraRotation);
        }

        return;
    }

    template <typename T>
    void SceneSettingGUI::addComponentEntry(const std::string& entryname) {

        if (selectionContext.HasComponent<T>()) 
            return;

        if (ImGui::MenuItem(entryname.c_str())) {
            selectionContext.AddComponent<T>();

            if (std::is_same_v<T , game::RenderComponent>) {
                auto& sprite = sceneContext->Entts().get<game::RenderComponent>(selectionContext.get());

                sprite.skeleton = sceneContext->getVertLib().get("Skeleton");
                
                std::shared_ptr<graphics::Material> newMat = std::make_shared<graphics::Material>(sceneContext->getShaderLib().get("Basic"));
                int numMats = (int)sceneContext->getMatLib().getAllAssets().size() + 1;
                std::string name = "Mat" + std::to_string(numMats);
                sceneContext->getMatLib().load(name , newMat);

                sprite.material = sceneContext->getMatLib().get(name);
            }

            ImGui::CloseCurrentPopup();
        }

        return;
    }

    void SceneSettingGUI::GuiRender(EditorState& state) {
        if (ImGui::Begin("Scene Settings")) {
            if (sceneContext != nullptr) {
                ImGui::Text(sceneContext->getPath().c_str());
                ImGui::Text(std::to_string(sceneContext->getNumEnts()).c_str());
                
                memset(buffer , 0 , sizeof(buffer));
                strcpy_s(buffer , sceneContext->getName().c_str());
                if (ImGui::InputText("Scene Name" , buffer , sizeof(buffer)))
                    sceneContext->setSceneName(std::string(buffer));

                if (ImGui::BeginPopupContextWindow(0 , 1)) {
                    if (ImGui::MenuItem("Create Empty Entity")) {
                        sceneContext->createEnt();
                        state.queueAction(UpdateAction::sceneSwitch);
                    }

                    ImGui::EndPopup();
                }

                if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				    selectionContext = sceneContext->getNullEnt();

                sceneContext->Entts().each([&](auto entity) {
                        game::Entity ent = sceneContext->getEntity(entity);
                        drawEntNode(ent , state);
                    });
            }
        }
        ImGui::End();

        if (ImGui::Begin("Entity Properties")) {
            if (selectionContext != sceneContext->getNullEnt()) {
                drawComponents();
            }
        }
        ImGui::End();

    }


    ////////////////////////////////////////////////////
    ///////////////////////////////////////////////////
    //////////////////////////////////////////////////

}
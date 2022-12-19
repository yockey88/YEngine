#include "sceneSettings.hpp"

#include "scripts/playerScript.hpp"

#include "Core/fileSystem.hpp"

#include "Game/sceneSerializer.hpp"
#include "Game/Entity/entity.hpp"
#include "Game/Entity/entityComponents.hpp"

#include "Graphics/material.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "ImEntt.hpp"

#include <stdint.h>

static char buffer[512];
static std::string basicMaterialPath = "resources/assets/materials/basic.json";

namespace machy {

    void SceneSettingGUI::setContext(const std::shared_ptr<game::Scene>& scene) {
        context = scene;
        selectionContext = context->getNullEnt();
    }

    void SceneSettingGUI::drawEntNode(game::Entity& ent , EditorState& state) {
        
        auto& id = context->Entts().get<game::EntityID>(ent.get());
        std::string displayName = id.name;

        ImGuiTreeNodeFlags flags = ((selectionContext == ent.get()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent.get() , flags , displayName.c_str())) {
            
            if (ImGui::IsItemClicked())
                selectionContext = ent;

            if (ImGui::BeginPopupContextItem()) {

                if (ImGui::MenuItem("Delete Entity")) 
                    deletedEntities.push(ent);

                if (ImGui::MenuItem("Save Entity"))
                    core::FileSystem::saveEntity(context , ent);

                ImGui::EndPopup();
            }
            
            ImGui::TreePop();
        }

        return;
    }

    void SceneSettingGUI::drawComponents() {

        if (selectionContext == context->getNullEnt()) return;
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent")) {
            addComponentEntry<game::PositionComponent>("Position");
            addComponentEntry<game::RenderComponent>("Sprite");
            addComponentEntry<game::CameraComponent>("Camera");
            addComponentEntry<game::NativeScript>("Script");
            addComponentEntry<game::PhysicsBody2DComponent>("Physics");

            ImGui::EndPopup();
        }
        
        if (ImGui::Button("Delete Component"))
            ImGui::OpenPopup("DeleteComponent");

        if (ImGui::BeginPopup("DeleteComponent")) {
            deleteComponentEntry<game::PositionComponent>("Position");
            deleteComponentEntry<game::RenderComponent>("Sprite");
            deleteComponentEntry<game::CameraComponent>("Camera");
            deleteComponentEntry<game::NativeScript>("Script");
            deleteComponentEntry<game::PhysicsBody2DComponent>("Physics");

            ImGui::EndPopup();
        }

        if (selectionContext.HasComponent<game::EntityID>()) {
            auto& id = selectionContext.GetComponent<game::EntityID>();

            ImGui::Text("%02d | %s" , id.ID , id.name.c_str());
            
            memset(buffer , 0 , sizeof(buffer));
            strcpy_s(buffer , id.name.c_str());
            if (ImGui::InputText("Entity Name" , buffer , sizeof(buffer)))
                id.name = std::string(buffer);

        }

        if (selectionContext.HasComponent<game::PositionComponent>() && !selectionContext.HasComponent<game::CameraComponent>()) {
            auto& pos = selectionContext.GetComponent<game::PositionComponent>();

            ImGui::DragFloat2("Entity Position" , glm::value_ptr(pos.pos) , 0.02f);
            ImGui::DragFloat3("Entity Rotation" , glm::value_ptr(pos.rotation) , 0.02f);
            ImGui::DragFloat2("Entity Size" , glm::value_ptr(pos.size) , 0.02f);
        }

        if (selectionContext.HasComponent<game::RenderComponent>()) {
            auto& pos = selectionContext.GetComponent<game::PositionComponent>();
            auto& sprite = selectionContext.GetComponent<game::RenderComponent>();

            ImGui::ColorEdit3("Sprite Color" , glm::value_ptr(sprite.color));
            std::string display = sprite.material->getName() + " | " + sprite.material->getPath();
            ImGui::Text(display.c_str());
            sprite.material->setUniformValue("inColor" , sprite.color);
            sprite.material->setUniformValue("texcoords" , pos.pos);
        }

        if (selectionContext.HasComponent<game::CameraComponent>()) {
            auto& cam = selectionContext.GetComponent<game::CameraComponent>();

            cam.cameraPos.z = cam.camera->getHeight();
            ImGui::DragFloat3("Camera Pos" , glm::value_ptr(cam.cameraPos) , 0.02f);
            ImGui::DragFloat("Camera Rotation" , &cam.cameraRotation , 0.02f);
            cam.camera->setHeight(cam.cameraPos.z);
            cam.camera->setViewMat(cam.cameraPos , cam.cameraRotation);
        }

        if (selectionContext.HasComponent<game::NativeScript>()) {
            auto& script = selectionContext.GetComponent<game::NativeScript>();

            if (script.bound)
                ImGui::Text("Entity Binded to Script");
        }

        if (selectionContext.HasComponent<game::PhysicsBody2DComponent>()) {
            auto& phys = selectionContext.GetComponent<game::PhysicsBody2DComponent>();

            const char* physTypeStr[3] = { "Static" , "Dynamic" , "Kinematic" };
            const char* currPhysTypeStr = physTypeStr[(int)phys.type];

            if (ImGui::BeginCombo("Physics Type" , currPhysTypeStr)) {

                for (int i = 0; i < 3; i++) {
                    bool selected = (currPhysTypeStr == physTypeStr[i]);
                    if (ImGui::Selectable(physTypeStr[i] , selected)) {
                        currPhysTypeStr = physTypeStr[i];
                        phys.type = (game::PhysicsBody2DComponent::PhysicsType)i;
                    } 
                }

                ImGui::EndCombo();
            }

            ImGui::Checkbox("Fix Rotation" , &phys.fixedRotation);
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
                auto& sprite = context->Entts().get<game::RenderComponent>(selectionContext.get());
                if (!selectionContext.HasComponent<game::PositionComponent>())
                    selectionContext.AddComponent<game::PositionComponent>();

                sprite.skeleton = context->getVertLib().get("Skeleton");
                sprite.meshName = "Skeleton";
                
                
                sprite.material = core::FileSystem::loadMaterialFile(basicMaterialPath);
                sprite.matName = sprite.material->getName();
            }

            if (std::is_same_v<T , game::CameraComponent> && !selectionContext.HasComponent<game::PositionComponent>()) 
                selectionContext.AddComponent<game::PositionComponent>();

            if (std::is_same_v<T , game::PhysicsBody2DComponent> && !selectionContext.HasComponent<game::PositionComponent>())
                selectionContext.AddComponent<game::PositionComponent>();

            if (std::is_same_v<T , game::NativeScript>)
                selectionContext.GetComponent<game::NativeScript>().Bind<PlayerScript>();
                
            ImGui::CloseCurrentPopup();
        }

        return;
    }

    template <typename T>
    void SceneSettingGUI::deleteComponentEntry(const std::string& entryname) {

        if (!selectionContext.HasComponent<T>()) 
            return;

        if (ImGui::MenuItem(entryname.c_str())) {
            selectionContext.RemoveComponent<T>();

            if (std::is_same_v<T , game::PositionComponent> && selectionContext.HasComponent<game::RenderComponent>()) {
                selectionContext.RemoveComponent<game::RenderComponent>();
            }

            ImGui::CloseCurrentPopup();
        }

        return;
    }

    void SceneSettingGUI::GuiRender(EditorState& state) {

        if (ImGui::Begin("Scene Settings")) {
            if (context != nullptr) {
                ImVec4 col1{ 0 , 1 , 0 , 1 };
                ImVec4 col2{ 0 , 1 , 1 , 1 };
                ImGui::TextColored(col1 , context->getPath().c_str());
                ImGui::TextColored(col2 , std::to_string(context->getNumEnts()).c_str());
                
                memset(buffer , 0 , sizeof(buffer));
                strcpy_s(buffer , context->getName().c_str());
                if (ImGui::InputText("Scene Name" , buffer , sizeof(buffer))) {
                    context->setSceneName(std::string(buffer));

                    std::string path = "resources/scenes/" + context->getName() + ".json";
                    context->setScenePath(path);
                }

                if (ImGui::BeginPopupContextWindow(0 , 1)) {
                    if (ImGui::MenuItem("Create Empty Entity")) {
                        context->createEnt();
                    }

                    ImGui::EndPopup();
                }

                if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				    selectionContext = context->getNullEnt();

                context->Entts().each([&](auto entity) {
                        game::Entity ent = context->getEntity(entity);
                        drawEntNode(ent , state);
                    });
            }
        }
        ImGui::End();

        if (ImGui::Begin("Entity Properties")) {
            if (selectionContext != context->getNullEnt()) {
                drawComponents();
            }
        }
        ImGui::End();

    }

    void SceneSettingGUI::flushDeletions() {
       
        if (deletedEntities.size() > 0) {
            while (deletedEntities.size() != 0) {
                game::Entity ent = deletedEntities.front();
                deletedEntities.pop();

                context->destroyEntity(ent);

                if (selectionContext == ent) 
                    selectionContext = context->getNullEnt();
            }
        }

        MACHY_ASSERT(deletedEntities.size() == 0 , "Entity Deletions Failed");

        return;
    }


    ////////////////////////////////////////////////////
    ///////////////////////////////////////////////////
    //////////////////////////////////////////////////

}
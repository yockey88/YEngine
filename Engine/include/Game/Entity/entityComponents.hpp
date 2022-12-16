#ifndef ENTITY_COMPONENTS_HPP
#define ENTITY_COMPONENTS_HPP

#include "Game/Entity/scriptedEntity.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/material.hpp"
#include "Graphics/camera.hpp"
#include "Graphics/sprite2D.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <stdint.h>

namespace machy {
namespace game {

    struct CameraComponent {
        std::shared_ptr<graphics::Camera> camera;
        glm::vec3 cameraPos;
        float cameraRotation;

        CameraComponent() : cameraPos(glm::vec3(0.f)) , cameraRotation(0.f) {
            camera = std::make_shared<graphics::Camera>();
            camera->setHeight(3.f);
        }
    };

    struct PositionComponent {
        glm::vec3 pos , size , rotation;

        PositionComponent() : pos(glm::vec3(0.f)) , size(glm::vec3(1.f)) , rotation(glm::vec3(0.f)) {}

        glm::mat4 getModel() {
            glm::mat4 rot = glm::toMat4(glm::quat(rotation));
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model , pos) * rot * glm::scale(model , size);
            return model;
        }
    };

    struct RenderComponent {
        std::shared_ptr<graphics::VertexArray> skeleton;
        std::shared_ptr<graphics::Material> material;
        glm::vec3 color;
        std::string meshName , matName;

        RenderComponent() = default;
        RenderComponent(const RenderComponent&) = default;
        RenderComponent(std::shared_ptr<graphics::VertexArray> skeleton , std::shared_ptr<graphics::Material> material) 
            : skeleton(skeleton) , material(material) , color(glm::vec3(0.f)) {}

        void SetMaterial(std::shared_ptr<Scene> entContext , const std::string& VertName = "Skeleton" , const std::string& MatName = "Basic") {
            if (entContext->getVertLib().exists(VertName))
                skeleton = entContext->getVertLib().get(VertName);

            if (entContext->getMatLib().exists(MatName))
                material = entContext->getMatLib().get(MatName);
        }
    };

    // struct RenderComponent {
    //     std::shared_ptr<graphics::VertexArray> skeleton;
    //     std::shared_ptr<graphics::Material> material;
    //     // glm::vec3 color;
    //     // std::string meshName , matName;

    //     // RenderComponent() = default;
    //     // RenderComponent(const RenderComponent&) = default;
    //     // RenderComponent(std::shared_ptr<graphics::VertexArray> skeleton , std::shared_ptr<graphics::Material> material) 
    //     //     : skeleton(skeleton) , material(material) , color(glm::vec3(0.f)) {}

    //     // void SetMaterial(std::shared_ptr<Scene> entContext , const std::string& VertName = "Skeleton" , const std::string& MatName = "Basic") {
    //     //     if (entContext->getVertLib().exists(VertName))
    //     //         skeleton = entContext->getVertLib().get(VertName);

    //     //     if (entContext->getMatLib().exists(MatName))
    //     //         material = entContext->getMatLib().get(MatName);
    //     // }
    // };

    struct NativeScript {
        ScriptedEntity* instance = nullptr;
        bool bound;

        ScriptedEntity*(*BindScript)();
        void (*DestroyScript)(NativeScript*);
        
        NativeScript() : instance(nullptr) , bound(false) {}
        ~NativeScript() { DestroyScript(this); }

        template <typename T>
        void Bind() {
            BindScript = []() { return static_cast<ScriptedEntity*>( new T() ); };
            DestroyScript = [](NativeScript* script) { delete script->instance; script->instance = nullptr; };

            bound = true;
        }
    };

}
}

#endif
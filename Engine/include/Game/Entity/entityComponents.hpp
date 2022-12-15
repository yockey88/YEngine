#ifndef ENTITY_COMPONENTS_HPP
#define ENTITY_COMPONENTS_HPP

#include "Game/Entity/scriptedEntity.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/material.hpp"
#include "Graphics/camera.hpp"
#include "Graphics/sprite2D.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
        glm::vec3 pos , size;
        glm::vec2 pos2D;

        PositionComponent() : pos(glm::vec3(0.f)) , size(glm::vec3(1.f)) , pos2D(glm::vec2(0.f)) {}
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
    };

    struct NativeScript {
        ScriptedEntity* instance = nullptr;

        ScriptedEntity*(*BindScript)();
        void (*DestroyScript)(NativeScript*);

        template <typename T>
        void Bind() {
            BindScript = []() { return static_cast<ScriptedEntity*>( new T() ); };
            DestroyScript = [](NativeScript* script) { delete script->instance; script->instance = nullptr; };
        }
    };

}
}

#endif
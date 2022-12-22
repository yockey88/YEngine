#ifndef ENTITY_COMPONENTS_HPP
#define ENTITY_COMPONENTS_HPP

#include "Game/Entity/scriptedEntity.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/material.hpp"
#include "Graphics/camera.hpp"
#include "Graphics/sprite2D.hpp"

#include "box2d/box2d.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include <stdint.h>

namespace machy {
namespace game {

    /// position
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

    /// sprites
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

    struct TexturedRenderComponent {
        std::shared_ptr<graphics::VertexArray> skeleton;
        std::shared_ptr<graphics::Material> material;
        std::shared_ptr<graphics::Texture> texture;
        glm::vec3 color;
        std::string meshName , matName;

        TexturedRenderComponent() = default;
        TexturedRenderComponent(const TexturedRenderComponent&) = default;
        TexturedRenderComponent(std::shared_ptr<graphics::VertexArray> skeleton , std::shared_ptr<graphics::Material> material , std::shared_ptr<graphics::Texture> texture) 
            : skeleton(skeleton) , material(material) , texture(texture) , color(glm::vec3(0.f)) {}
    };

    /// cameras
    struct CameraComponent {
        std::shared_ptr<graphics::Camera> camera;
        glm::vec3 cameraPos;
        float cameraRotation;

        CameraComponent() : cameraPos(glm::vec3(0.f)) , cameraRotation(0.f) {
            camera = std::make_shared<graphics::Camera>();
            camera->setHeight(3.f);
        }
    };

    // physics
    struct PhysicsBody2DComponent {
        enum class PhysicsType{ Static = 0 , Kinematic , Dynamic };
        PhysicsType type;
        b2BodyDef bodyDef;
        b2PolygonShape boundBox;
        b2FixtureDef fixtrDef;
        glm::vec2 offset , size;

        float density , friction;
        float restitution , restitutionThreshold;
        bool fixedRotation;

        b2Body* runtimePhysicsBody;
        b2Fixture* runtimeFixture;

        PhysicsBody2DComponent() : runtimePhysicsBody(nullptr) , runtimeFixture(nullptr) , type(PhysicsType::Static) ,
                                    fixedRotation(false) , density(1.f) , friction(0.5f) , restitution(0.5f) , restitutionThreshold(1.f) ,
                                    offset({ 0.f , 0.f }) , size({ 0.5f , 0.5f }) {}
        PhysicsBody2DComponent(const PhysicsBody2DComponent&) = default;
        ~PhysicsBody2DComponent() {}
    };

    // scripting
    struct NativeScript {
        ScriptedEntity* instance = nullptr;
        bool bound;

        ScriptedEntity*(*BindScript)();
        void (*DestroyScript)(NativeScript*);
        
        NativeScript() : instance(nullptr) , bound(false) {}
        ~NativeScript() {  }

        template <typename T>
        void Bind() {
            BindScript = []() { return static_cast<ScriptedEntity*>( new T() ); };
            DestroyScript = [](NativeScript* script) { delete script->instance; script->instance = nullptr; };

            bound = true;
        }

        void Unbind() {
            delete instance;
            instance = nullptr;

            BindScript = nullptr;
            DestroyScript = nullptr;

            bound = false;
        }
    };

}
}

#endif
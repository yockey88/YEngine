#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "log.hpp"

#include "Game/scene.hpp"

#include "entt.hpp"
#include "entity/registry.hpp"

#include <iostream>
#include <memory>

namespace machy {
namespace game {

    class Entity {
        Scene* scene;
        entt::entity handle;
        bool null;

        friend class Scene;

        public:
            Entity() : scene(nullptr) , handle(entt::null) , null(true) {}
            Entity(entt::entity handle) : scene(nullptr) , handle(handle) , null(false) {}
            Entity(const Entity& other) { handle = other.handle; scene = other.scene; };

           template<typename T, typename... Args>
            T& AddComponent(Args&&... args) {
                MACHY_ASSERT(!HasComponent<T>(), "Entity already has component!");
                T& component = scene->entRegistry.emplace<T>(handle, std::forward<Args>(args)...);
                return component;
            }

            template<typename T, typename... Args>
            T& AddOrReplaceComponent(Args&&... args) {
                T& component = scene->entRegistry.emplace_or_replace<T>(handle, std::forward<Args>(args)...);
                return component;
            }

            template<typename T>
            T& GetComponent() {
                MACHY_ASSERT(HasComponent<T>(), "Entity does not have component!");
                return scene->entRegistry.get<T>(handle);
            }

            template<typename T>
            bool HasComponent() {
                return (scene->entRegistry.try_get<T>(handle) != nullptr);
            }

            template<typename T>
            void RemoveComponent() {
                MACHY_ASSERT(HasComponent<T>(), "Entity does not have component!");
                scene->Entts().remove<T>(handle);
            }

            inline entt::entity& get() { return handle; }
            inline bool isNotNull() const { return !null; }
            inline void nullify() { null = true; }
            inline void setContext(Scene* context) { scene = context; }

            bool operator==(const Entity& other) { return ((handle == other.handle) && (scene == other.scene)); }
            bool operator!=(const Entity& other) { return !(*this == other); }
            operator bool() const { return (handle != entt::null); }
            operator uint32_t() const { return (uint32_t)handle; }
    };

}
}

#endif
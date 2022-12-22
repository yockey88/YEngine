#ifndef SCRIPTED_ENTITY_HPP
#define SCRIPTED_ENTITY_HPP

#include "Game/Entity/entity.hpp"

namespace machy {
namespace game {

    class ScriptedEntity {
        Entity entity;

        friend class Scene;

        protected:
            virtual void onCreation() {}
            virtual void onDestruction() {}
            virtual void onUpdate() {}

        public:
            ScriptedEntity() {}
            virtual ~ScriptedEntity() {}

            template <typename T>
            T& GetComponent() {
                return entity.GetComponent<T>();
            }

            template <typename T>
            T& AddComponent() {
                return entity.AddComponent<T>();
            }

            template <typename T>
            bool HasComponent() {
                return entity.HasComponent<T>();
            }
    };

}
}

#endif
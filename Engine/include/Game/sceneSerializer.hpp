#ifndef SCENE_SERIALIZER_HPP
#define SCENE_SERIALIZER_HPP

#include "Game/scene.hpp"
#include "Game/Entity/entity.hpp"

#include "nlohmann/json.hpp"

#include <memory>

using js = nlohmann::json;

namespace machy {

    class GameSceneSerializer {
        std::shared_ptr<game::Scene> context;

        friend class Scene;

        void serializeEnt(game::Entity& entity , const std::string& filepath);

        void serializeEntPos(game::Entity& entity , js& json);
        void serializeEntSprite(game::Entity& entity , js& json);
        void serializeCamPos(game::Entity& entity , js& json);

        void deserializeEnt(const std::string& filepath);

        void deserializeEntPos(game::Entity& entity , js& json);
        void deserializeEntSprite(game::Entity& entity , js& json);
        void deserializeCamPos(game::Entity& entity , js& json);
        public:
            GameSceneSerializer(const std::shared_ptr<game::Scene>& context) : context(context) {}

            void serialize(const std::string& filepath);
            void serializeRuntime(const std::string& filepath);

            std::shared_ptr<game::Scene> deserialize(const std::string& filepath);
            void deserializeRuntime(const std::string& filepath);
    };

}

#endif
#ifndef SCENE_SERIALIZER_HPP
#define SCENE_SERIALIZER_HPP

#include "Game/scene.hpp"
#include "Game/Entity/entity.hpp"

#include "nlohmann/json.hpp"

#include <tuple>
#include <memory>
#include <map>

using js = nlohmann::json;

namespace machy {

    class SceneSerializer {
        std::shared_ptr<game::Scene> context;

        friend class Scene;

        static js getJson(const std::string& path);

        void serializeEntPos(game::Entity& entity , js& json);
        void serializeEntSprite(game::Entity& entity , js& json);
        void serializeEntPhysics2DPos(game::Entity& entity , js& json);
        void serializeCamPos(game::Entity& entity , js& json);

        void deserializeEntPos(game::Entity& entity , js& json);
        void deserializeEntSprite(game::Entity& entity , js& json);
        void deserializeEntPhysics2D(game::Entity& entity , js& json);
        void deserializeCamPos(game::Entity& entity , js& json);
        public:
            SceneSerializer() : context(nullptr) {}
            SceneSerializer(const std::shared_ptr<game::Scene>& context) : context(context) {}

            std::string deserializeMat(std::map<std::string , std::string>& paths , const std::string& path);

            void serialize(const std::string& filepath);
            void serializeEnt(game::Entity& entity);
            void serializeRuntime(const std::string& filepath);

            std::shared_ptr<game::Scene> deserialize(const std::string& filepath);
            void deserializeEnt(const std::string& filepath);
            void deserializeRuntime(const std::string& filepath);
    };

}

#endif
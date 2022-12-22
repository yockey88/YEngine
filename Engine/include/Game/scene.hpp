#ifndef SCENE_HPP
#define SCENE_HPP

#include "Core/assetLibrary.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/material.hpp"

#include "entt.hpp"
#include "glm/glm.hpp"
#include "box2d/box2d.h"
#include "box2d/b2_fixture.h"

#include <memory>
#include <map>

namespace machy {
namespace game {

    struct RenderComponent;
    class Entity;
    class scriptedEntity;

    struct EntityID {
        int ID;
        std::string name;

        EntityID() : ID(0) , name("name") {}
    };

    class Scene {
        core::AssetLibrary<graphics::VertexArray> Meshes;
        b2World* world;

        std::map<int , Entity> entities;
        entt::registry entRegistry;
        int numEnts , totalEntsCreated;
        bool playing;
        std::string name , path;

        friend class Entity;
        friend class ScriptedEntity;
        friend class SceneSettingGUI;
        friend class SceneSerializer;

        public:
            Scene();
            ~Scene();

            void updateFromEditor();
            void updateRuntime(const float& dt);
            void render();

            inline bool isPlaying() const { return playing; }
            void playScene();
            void pauseScene();
            void stopScene();

            inline b2World* viewWorld() const { return world; }
            inline b2World* getWorld() { return world; } 

            Entity createEnt(const std::string& name = "{BLANK ENTITY}");
            void destroyEntity(Entity& ent);
            Entity& getEntity(entt::entity handle);
            Entity& getMainCameraEntity();
            Entity getNullEnt();
            inline entt::registry& Entts() { return entRegistry; }
            inline int getNumEnts() const { return numEnts; }  
            inline int getTotalEntsMade() const { return totalEntsCreated; }

            inline core::AssetLibrary<graphics::VertexArray>& getVertLib() { return Meshes; }

            inline std::string getName() const { return name; }
            inline std::string getPath() const { return path; }

            inline void setSceneName(const std::string& name) { this->name = name; }
            inline void setScenePath(const std::string& path) { this->path = path; }

            void createSprite();
    };

}
}

#endif
#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include "app.hpp"
#include "Scene/scene.hpp"

namespace machy {
namespace managers {

    struct SceneNode {
        int key;
        scene::Scene* scene;
    };

    class SceneManager {
        scene::Map world;

        int gridDim , sideLength;

        bool menuView;

        std::vector<std::string> activeSceneLog;
        
        SceneManager (const SceneManager&) = delete;
        SceneManager& operator= (const SceneManager&) = delete;  
        public:
            SceneManager();
            ~SceneManager();

            bool init(const SceneData& sceneData);

            void render(SDL_Renderer* renderer);
            void printActiveLog();

            inline std::vector<std::string>& getActiveLog() { return activeSceneLog; }
    };

}
}

#endif /* renderManager.hpp */
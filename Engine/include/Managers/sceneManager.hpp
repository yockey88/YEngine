#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include "app.hpp"
#include "Scene/scene.hpp"
#include <string>
#include <memory.h>

namespace machy {
namespace managers {

    class SceneManager {
        std::shared_ptr<graphics::Scene> activeScene;
        
        SceneManager (const SceneManager&) = delete;
        SceneManager& operator= (const SceneManager&) = delete;  
        public:
            SceneManager() {}
            ~SceneManager() {}

            bool init(const std::string& appScenesPath);

            inline std::shared_ptr<graphics::Scene> getActiveScene() { return activeScene; }
    };

}
}

#endif /* renderManager.hpp */
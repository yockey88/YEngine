#include "Managers/sceneManager.hpp"
#include "machy.hpp"
#include <iostream>

namespace machy {
namespace managers {

    SceneManager::SceneManager() : sideLength(600) , gridDim(40) , menuView(true) {}

    SceneManager::~SceneManager() {}

    bool SceneManager::init(const SceneData& sceneData) {
        
        LOG("[[Building Map]]");
        world.create(sceneData.gridSize , sceneData.gridDim);
        activeSceneLog = world.captureEdges();

        return true;
    }

    void SceneManager::render(SDL_Renderer* renderer) {

        world.renderMap(renderer);

        // if(menuView) {
        //     world.renderMap(renderer);
        // } else {
        //     world.renderEdges(renderer);
        // }

        return;
    }

    void SceneManager::printActiveLog() {

        for (int i = 0; i < activeSceneLog.size(); i++)
            std::cout << activeSceneLog[i] << std::endl;

        return;
    }

}
}
#include "Game/sceneContainer.hpp"

namespace machy {
namespace game {

    // SceneContainer::~SceneContainer() {
    //     // for (Scene* scene : scenes) {
    //     //     scene->OnRemove();
    //     //     delete scene;
    //     // }
    // }

    // void SceneContainer::pushScene(Scene* scene) {

    //     scene->OnPush();
    //     scenes.emplace(scenes.begin() + sceneInsertIndex , scene);
    //     sceneInsertIndex++;

    //     return;
    // }

    // void SceneContainer::pushOverlay(Scene* scene) {

    //     scenes.emplace_back(scene);

    //     return;
    // }
    
    // void SceneContainer::popScene(Scene* scene) {

    //     auto itr = std::find(scenes.begin() , scenes.begin() + sceneInsertIndex , scene);
    //     if (itr != scenes.begin() + sceneInsertIndex) {
    //         scene->OnRemove();
    //         scenes.erase(itr);
    //         sceneInsertIndex--;
    //     }

    //     return;
    // }

    // void SceneContainer::popOverlay(Scene* scene) {
        
    //     auto itr = std::find(scenes.begin() + sceneInsertIndex , scenes.end() , scene);
    //     if (itr != scenes.end()) {
    //         scene->OnRemove();
    //         scenes.erase(itr);
    //     }

    //     return;
    // }

}
}
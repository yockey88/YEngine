#include "components.hpp"

using namespace machy;

void SceneCore::calculateSpritesToRender(const glm::vec2& playerPos) {

    

    return;
}

void SceneCore::render() {

    auto& sprites = worldMap->getAllSprites();

    for (auto sprite : sprites) {
        sprite.second->render();
    }

    return;
}
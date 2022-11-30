#include "components.hpp"

using namespace machy;

void SceneCore::render() {

    auto& sprites = worldMap->getAllSprites();

    for (auto sprite : sprites) {
        sprite.second->render();
    }

    return;
}
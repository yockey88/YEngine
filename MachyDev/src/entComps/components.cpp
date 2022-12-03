#include "machy.hpp"

#include "components.hpp"

#include "glm/gtc/matrix_transform.hpp"

using namespace machy;

void PlayerCore::playerInputs() {

    if (input::keyboard::keyDown(MACHY_INPUT_KEY_A)) {
		playerPos.x -= playerSpd;
		activeAnimation = playerRunL;
		dir = direction::left;
	}
	if (input::keyboard::keyDown(MACHY_INPUT_KEY_D)) {
		playerPos.x += playerSpd;
		activeAnimation = playerRunR;
		dir = direction::right;
	}

	if (input::keyboard::keyDown(MACHY_INPUT_KEY_W)) { 
		playerPos.y += playerSpd; 
		if (dir == direction::left) {
			activeAnimation = playerRunL;
		} else {
			activeAnimation = playerRunR;
		}
	}
	if (input::keyboard::keyDown(MACHY_INPUT_KEY_S)) {
		playerPos.y -= playerSpd;
		if (dir == direction::left) {
			activeAnimation = playerRunL;
		} else {
			activeAnimation = playerRunR;
		}
	}
	if (input::keyboard::keyDown(MACHY_INPUT_KEY_SPACE)) {
		switch (dir) {
			case direction::left: playerPos.x -= 0.1f; break;
			case direction::right: playerPos.x += 0.1f; break;
			case direction::up: playerPos.y += 0.1f; break;
			case direction::down: playerPos.y -= 0.1f; break;
			default: break;
		}
	}

	if (input::keyboard::keyUp(MACHY_INPUT_KEY_A)) { activeAnimation = playerIdleL; }
	if (input::keyboard::keyUp(MACHY_INPUT_KEY_D)) { activeAnimation = playerIdleR; }

	if (input::keyboard::keyUp(MACHY_INPUT_KEY_W)) {
		if (dir == direction::left) {
			activeAnimation = playerIdleL;
		} else {
			activeAnimation = playerIdleR;
		}
	}
	if (input::keyboard::keyUp(MACHY_INPUT_KEY_S)) {
		if (dir == direction::left) {
			activeAnimation = playerIdleL;
		} else {
			activeAnimation = playerIdleR;
		}
	}

	playerIdleL->setAnimationPos(playerPos);
	playerIdleR->setAnimationPos(playerPos);
	playerRunL->setAnimationPos(playerPos);
	playerRunR->setAnimationPos(playerPos);

    return;
}

void SceneCore::calculateSpritesToRender(const glm::vec2& playerPos) {

    

    return;
}

void SceneCore::render() {

    auto& allSprites = worldMap->getAllSprites();

    for (const auto& sprite : allSprites) {
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model , { sprite.second->getPos().x , sprite.second->getPos().y , 0.f});
        model = glm::scale(model , { sprite.second->getSize().x , sprite.second->getSize().y , 1.f });

        MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , sprite.second->getVA() ,sprite.second->getMat() , model));
    }

    return;
}
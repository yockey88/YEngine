#include "machy.hpp"

#include "components.hpp"

#include "Core/fileSystem.hpp"

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

void PlayerCore::calcBounds() {

	bounds.x = playerPos.y - ((float)activeAnimation->getBaseTextureSize().y / 2.f); // top
	bounds.y = playerPos.y + ((float)activeAnimation->getBaseTextureSize().y / 2.f); // bottom
	bounds.z = playerPos.x - ((float)activeAnimation->getBaseTextureSize().x / 2.f); // left
	bounds.w = playerPos.x + ((float)activeAnimation->getBaseTextureSize().x / 2.f); // right

	return;
}

void SceneCore::initScene(std::shared_ptr<graphics::Material> mat) {

	auto [dim , rawMap] = core::FileSystem::readSceneFile("resources/scenes/scene11.txt");
	blocksPerSide = dim;
	mapStr = rawMap;

	buildBaseMap(mat);

	return;
}

void SceneCore::calcBounds(const glm::vec2& pos , glm::ivec2 IJ) {

	int strIndex = IJ.y + (blocksPerSide * IJ.x);

	if ((mapStr[strIndex] == '^') || (mapStr[strIndex] == '#') || (mapStr[strIndex] == '[') ||
		(mapStr[strIndex] == '@') || (mapStr[strIndex] == 'W') || (mapStr[strIndex] == '~') ||
		(mapStr[strIndex] == '*')) {

		
		glm::vec4 bounds;

		bounds.x = pos.x - (map[mapStr[strIndex]]->getSize().x / 2); // Left
		bounds.y = pos.x + (map[mapStr[strIndex]]->getSize().x / 2); // Right

		bounds.z = pos.y - (map[mapStr[strIndex]]->getSize().y / 2); // Top
		bounds.w = pos.y + (map[mapStr[strIndex]]->getSize().y / 2); // Bottom

		tileBounds.push_back(bounds);

	}

	return;
}

void SceneCore::buildBaseMap(std::shared_ptr<graphics::Material> mat) {

	atlas = std::make_shared<graphics::SpriteAtlas2D>(mat , "core");
	atlas->createSprite("core" , "Dirt" , { 110 , 26 } , { 16 , 16 });

	atlas->createSprite("core" , "Grass" , { 93 , 26 } , { 16 , 16 });
	atlas->createSprite("core" , "Water" , { 59 , 26 } , { 16 , 16 });
	atlas->createSprite("core" , "ConcreteFloor" , { 298 , 382 } , { 16 , 16 });

	atlas->createSprite("core" , "Shoreline" , { 76 , 26 } , { 16 , 16 });
	atlas->createSprite("core" , "BottomShoreline" , { 59 , 9 } , { 16 , 16 });
	atlas->createSprite("core" , "CornerShoreline" , { 76 , 9 } , { 16 , 16 });

	atlas->createSprite("core" , "Tree" , { 229 , 162 } , { 16 , 16 });

	atlas->createSprite("core" , "WallTop" , { 229 , 230 } , { 16 , 16 });
	atlas->createSprite("core" , "WallVertCenter" , { 263 , 230 } , { 16 , 16 });
	atlas->createSprite("core" , "WallBottom" , { 229 , 247 } , { 16 , 16 });

	atlas->getSprite("Grass")->setPos({ -1.f ,  1.f });
	atlas->getSprite("Water")->setPos({ -1.f , -1.f });
	atlas->getSprite("Shoreline")->setPos({ -1.f , 0.f });
	atlas->getSprite("Tree")->setPos({ 0.f , 1.f });
	atlas->getSprite("WallTop")->setPos({ 1.f , 1.f });
	atlas->getSprite("WallVertCenter")->setPos({ 1.f , 0.f });
	atlas->getSprite("WallBottom")->setPos({ 1.f , -1.f });

	map['x'] = atlas->getSprite("Dirt");
	map['-'] = atlas->getSprite("Grass");
	map['W'] = atlas->getSprite("Water");
	map['_'] = atlas->getSprite("ConcreteFloor");
	map['~'] = atlas->getSprite("Shoreline");
	map['*'] = atlas->getSprite("BottomShoreline");
	map['c'] = atlas->getSprite("CornerShoreline");
	map['^'] = atlas->getSprite("Tree");
	map['#'] = atlas->getSprite("WallTop");
	map['['] = atlas->getSprite("WallVertCenter");
	map['@'] = atlas->getSprite("WallBottom");

	return;
}

void SceneCore::render() {

	for (int i = 0; i < blocksPerSide + 10; i++) {
		for (int j = 0; j < blocksPerSide + 10; j++) {
				glm::vec2 pos{ ((float)j - ((float)(blocksPerSide + 10) / 2.f)) , ((float)i - ((float)(blocksPerSide + 10) / 2.f)) };
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model , { pos.x , pos.y , 0.f});
				model = glm::scale(model , { map['-']->getSize().x , map['-']->getSize().y , 1.f });

				MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , map['-']->getVA() , map['-']->getMat() , model));
		}
	}

	for (int i = 0; i < blocksPerSide; i++) {
		for (int j = 0; j < blocksPerSide; j++) {
				char block = mapStr[j + (blocksPerSide * i)];
				glm::vec2 pos{ ((float)j - (float)(blocksPerSide / 2)) , ((float)i - (float)(blocksPerSide / 2)) };
				calcBounds(pos , { i , j });
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model , { pos.x , -1 * pos.y , 0.f});
				model = glm::scale(model , { map[block]->getSize().x , map[block]->getSize().y , 1.f });

				MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , map[block]->getVA() , map[block]->getMat() , model));
		}
	}
		

    return;
}
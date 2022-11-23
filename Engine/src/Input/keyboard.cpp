#include "machy.hpp"
#include "Input/keyboard.hpp"

#include <SDL.h>

namespace machy {
namespace input {

    std::array<bool,  keyboard::KeyCount>  keyboard::keys;
	std::array<bool,  keyboard::KeyCount>  keyboard::keysLast;

	void  keyboard::initialize() {
		std::fill(keys.begin(), keys.end(), false);
		std::fill(keysLast.begin(), keysLast.end(), false);
        MACHY_TRACE("Keyboard Initialized");
	}

	void  keyboard::update() {
		keysLast = keys;
		
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		for (int i = 0; i < KeyCount; i++) {
			keys[i] = state[i];
		}
	}

	bool  keyboard::key(int key) {
		MACHY_ASSERT(key >= MACHY_INPUT_KEY_FIRST && key <= KeyCount , "Invalid keybaord key");
		if (key >=  MACHY_INPUT_KEY_FIRST && key < KeyCount) {
			return keys[key];
		}
		return false;
	}

	bool  keyboard::keyDown(int key) {
		MACHY_ASSERT(key >= MACHY_INPUT_KEY_FIRST && key <= KeyCount , "Invalid keybaord key");
		if (key >=  MACHY_INPUT_KEY_FIRST && key < KeyCount) {
			return keys[key] && !keysLast[key];
		}
		return false;
	}

	bool  keyboard::keyUp(int key) {
		MACHY_ASSERT(key >= MACHY_INPUT_KEY_FIRST && key <= KeyCount , "Invalid keybaord key");
		if (key >=  MACHY_INPUT_KEY_FIRST && key < KeyCount) {
			return !keys[key] && keysLast[key];
		}
		return false;
	}

}
}
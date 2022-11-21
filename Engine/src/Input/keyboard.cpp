#include "machy.hpp"
#include "Input/keyboard.hpp"

#include <SDL.h>

namespace machy {
namespace input {

    std::array<bool,  keyboard::KeyCount>  keyboard::keys;
	std::array<bool,  keyboard::KeyCount>  keyboard::keysLast;

	void  keyboard::Initialize() {
		std::fill(keys.begin(), keys.end(), false);
		std::fill(keysLast.begin(), keysLast.end(), false);
        MACHY_TRACE("Keyboard Initialized");
	}

	void  keyboard::UpdateKeys() {
		keysLast = keys;
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		for (int i =  Y_INPUT_KEY_FIRST; i < KeyCount; i++) {
			keys[i] = state[i];
		}
	}

	bool  keyboard::Key(int key) {
		if (key >=  Y_INPUT_KEY_FIRST && key < KeyCount) {
			return keys[key];
		}
		return false;
	}

	bool  keyboard::KeyDown(int key) {
		if (key >=  Y_INPUT_KEY_FIRST && key < KeyCount) {
			return keys[key] && !keysLast[key];
		}
		return false;
	}

	bool  keyboard::KeyUp(int key) {
		if (key >=  Y_INPUT_KEY_FIRST && key < KeyCount) {
			return !keys[key] && keysLast[key];
		}
		return false;
	}

}
}
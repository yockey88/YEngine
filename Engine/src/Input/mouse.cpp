#include "machy.hpp"
#include "Input/mouse.hpp"

#include <SDL.h>

namespace machy {
namespace input {

    int mouse::x = 0;
	int mouse::xLast = 0;
	int mouse::y = 0;
	int mouse::yLast = 0;

    std::array<bool, mouse::ButtonCount> mouse::buttons;
	std::array<bool, mouse::ButtonCount> mouse::buttonsLast;

    void mouse::initialize() {
        std::fill(buttons.begin(), buttons.end(), false);
		std::fill(buttonsLast.begin(), buttonsLast.end(), false);
        MACHY_TRACE("Mouse Initialized");
        return;
    }

    void mouse::update() {

        xLast = x;
		yLast = y;

        buttonsLast = buttons;

		Uint32 state = SDL_GetMouseState(&x, &y);
		for (int i = 0; i < ButtonCount; i++) {
			buttons[i] = state & SDL_BUTTON(i + 1);
		}
        return;
    }

    bool mouse::button(int button) {
        MACHY_ASSERT(button >= MACHY_INPUT_MOUSE_FIRST && button <= MACHY_INPUT_MOUSE_LAST , "Mouse Button Invalid");
        if (button >= MACHY_INPUT_MOUSE_FIRST && button <= MACHY_INPUT_MOUSE_LAST) {
			return buttons[button - 1];
		}
        return false;
    }

    bool mouse::buttonDown(int button) {
        MACHY_ASSERT(button >= MACHY_INPUT_MOUSE_FIRST && button <= MACHY_INPUT_MOUSE_LAST , "Mouse Button Invalid");
        if (button >=  MACHY_INPUT_MOUSE_FIRST && button <=  MACHY_INPUT_MOUSE_LAST) {
			return buttons[button - 1] && !buttonsLast[button - 1];
		}
        return false;
    }

    bool mouse::buttonUp(int button) {
        MACHY_ASSERT(button >= MACHY_INPUT_MOUSE_FIRST && button <= MACHY_INPUT_MOUSE_LAST , "Mouse Button Invalid");
        if (button >=  MACHY_INPUT_MOUSE_FIRST && button <=  MACHY_INPUT_MOUSE_LAST) {
			return !buttons[button - 1] && buttonsLast[button - 1];
		}
        return false;
    }

    void mouse::printMousePos() {

        MACHY_TRACE("<{} , {}>" , X() , Y());

        return;
    }

}
}
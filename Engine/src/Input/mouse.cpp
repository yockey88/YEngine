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

    void mouse::Initialize() {
        std::fill(buttons.begin(), buttons.end(), false);
		std::fill(buttonsLast.begin(), buttonsLast.end(), false);
        MachY::Instance().Log("Mouse Initialized");
        return;
    }

    void mouse::UpdatePos() {
        xLast = x;
		yLast = y;
		buttonsLast = buttons;	// TODO Optimization: is memcpy faster?
		Uint32 state = SDL_GetMouseState(&x, &y);
		for (int i = 0; i < ButtonCount; i++) {
			buttons[i] = state & SDL_BUTTON(i + 1);
		}
        return;
    }

    bool mouse::Button(int button) {
        if (button >= Y_INPUT_MOUSE_FIRST && button <= Y_INPUT_MOUSE_LAST) {
			return buttons[button - 1];
		}
        return false;
    }

    bool mouse::ButtonDown(int button) {
        if (button >=  Y_INPUT_MOUSE_FIRST && button <=  Y_INPUT_MOUSE_LAST) {
			return buttons[button - 1] && !buttonsLast[button - 1];
		}
        return false;
    }

    bool mouse::ButtonUp(int button) {
        if (button >=  Y_INPUT_MOUSE_FIRST && button <=  Y_INPUT_MOUSE_LAST) {
			return !buttons[button - 1] && buttonsLast[button - 1];
		}
        return false;
    }

    void mouse::printMousePos() {
        std::string pos = "<";
        pos += std::to_string(input::mouse::X());
        pos += " , ";
        pos += std::to_string(input::mouse::X());
        pos += ">";
        // MachY::Instance().Out(pos);
        return;
    }

}
}
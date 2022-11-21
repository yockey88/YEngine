#include "machy.hpp"
#include "Input/joystick.hpp"

namespace machy {
namespace input {

    std::unordered_map<int, std::unique_ptr<joystick::Controller>> joystick::availableJoysticks;
	float joystick::deadzone = 0.2f;

	void joystick::OnJoystickConnected(SDL_ControllerDeviceEvent& e) {
		int deviceIndex = e.which;
		if (SDL_IsGameController(deviceIndex)) {
            MACHY_TRACE("Game Controller Connected");
			auto c = std::make_unique<Controller>();
			c->gc = SDL_GameControllerOpen(deviceIndex);

			if (c->gc) {
				c->joystickIndex = deviceIndex;
				std::fill(c->buttons.begin(), c->buttons.end(), false);
				std::fill(c->lastButtons.begin(), c->lastButtons.end(), false);
				std::fill(c->axes.begin(), c->axes.end(), 0.f);
				std::fill(c->lastAxes.begin(), c->lastAxes.end(), 0.f);

				int mapIndex = GetNextFreeIndex();
				availableJoysticks[mapIndex] = std::move(c);
                MACHY_TRACE("Game Controller Initialized");
			} else {
				MACHY_TRACE("Error Initializing Game Controller Connection");
			}
		}
	}

	void joystick::OnJoystickDisconnected(SDL_ControllerDeviceEvent& e) {
		int deviceIndex = e.which;
		for (auto it = availableJoysticks.begin(); it != availableJoysticks.end(); it++) {
			Controller* c = it->second.get();
			if (c->joystickIndex == deviceIndex) {
				MACHY_TRACE("Joystick Disconnected");
				SDL_GameControllerClose(c->gc);
				availableJoysticks.erase(it);
				break;
			}
		}
	}

	void joystick::Shutdown() {
		for (auto it = availableJoysticks.begin(); it != availableJoysticks.end();) {
			Controller* c = it->second.get();
			SDL_GameControllerClose(c->gc);
			it = availableJoysticks.erase(it);
		}
	}

	void joystick::UpdateGamePad() {
		for (auto it = availableJoysticks.begin(); it != availableJoysticks.end(); it++) {
			Controller* c = it->second.get();
            if (c == nullptr) MACHY_ERROR("Game Controller Connection Corrupt");
			if (c && c->gc) {
				c->lastButtons = c->buttons;
				c->lastAxes = c->axes;

				for (unsigned int i = 0; i < static_cast<int>(Button::Count); i++) {
					c->buttons[i] = SDL_GameControllerGetButton(c->gc, static_cast<SDL_GameControllerButton>(i));
				}

				for (unsigned int i = 0; i < static_cast<int>(Axis::Count); i++) {
					// SDL ranges axes from -32768 to 32767
					c->axes[i] = std::clamp(SDL_GameControllerGetAxis(c->gc, static_cast<SDL_GameControllerAxis>(i)) / 32767.f, -1.f, 1.f);
				}
			}
		}
	}

	bool joystick::GetButton(int joystickId , Button button) {
		auto it = availableJoysticks.find(joystickId);
		if (it != availableJoysticks.end()) {
			return it->second->buttons[static_cast<int>(button)];
		}

		MACHY_ERROR("Joystick Not Available!");
		return false;
	}

	bool joystick::GetButtonDown(int joystickId , Button button) {
		auto it = availableJoysticks.find(joystickId);
		if (it != availableJoysticks.end()) {
			return it->second->buttons[static_cast<int>(button)] && !it->second->lastButtons[static_cast<int>(button)];
		}

		MACHY_ERROR("Joystick Not Available!");
		return false;
	}

	bool joystick::GetButtonUp(int joystickId , Button button) {
		auto it = availableJoysticks.find(joystickId);
		if (it != availableJoysticks.end()) {
			return !it->second->buttons[static_cast<int>(button)] && it->second->lastButtons[static_cast<int>(button)];
		}

		MACHY_ERROR("Joystick Not Available!");
		return false;
	}

	float joystick::GetAxis(int joystickId , Axis axis) {
		auto it = availableJoysticks.find(joystickId);
		if (it != availableJoysticks.end()) {
			float val = it->second->axes[static_cast<int>(axis)];
			return abs(val) > deadzone ? val : 0.f;
		}

		MACHY_ERROR("Joystick Not Available!");
		return 0.f;
	}

	int joystick::GetNextFreeIndex() {
		int ret = -1;

		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			if (availableJoysticks.count(i) == 0) {
				ret = i;
				break;
			}
		}

		return ret;
	}

	bool joystick::IsJoystickAvailable(int joystickId) {
		return availableJoysticks.count(joystickId) > 0;
	}
    
} 
}
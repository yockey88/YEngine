#include "machy.hpp"
#include "Input/joystick.hpp"

namespace machy {
namespace input {

    std::unordered_map<int, std::unique_ptr<joystick::Controller>> joystick::availableJoysticks;
	float joystick::deadzone = 0.2f;

	void joystick::onJoystickConnected(SDL_ControllerDeviceEvent& e) {
		int deviceIndex = e.which;
		if (SDL_IsGameController(deviceIndex)) {
            MACHY_INFO("Controller Connected");
			auto c = std::make_unique<Controller>();
			c->gc = SDL_GameControllerOpen(deviceIndex);

			if (c->gc) {
				c->joystickIndex = deviceIndex;
				std::fill(c->buttons.begin(), c->buttons.end(), false);
				std::fill(c->lastButtons.begin(), c->lastButtons.end(), false);
				std::fill(c->axes.begin(), c->axes.end(), 0.f);
				std::fill(c->lastAxes.begin(), c->lastAxes.end(), 0.f);

				int mapIndex = getNextFreeIndex();
				availableJoysticks[mapIndex] = std::move(c);
                MACHY_INFO("Controller Initialized -> < ID({}) >" , mapIndex , deviceIndex);
				std::cout << std::endl;
			} else {
				MACHY_ERROR("Error Initializing Game Controller Connection");
			}
		}
	}

	void joystick::onJoystickDisconnected(SDL_ControllerDeviceEvent& e) {
		int deviceIndex = e.which;
		for (auto itr = availableJoysticks.begin(); itr != availableJoysticks.end(); itr++) {
			Controller* c = itr->second.get();
			if (c->joystickIndex == deviceIndex) {
				MACHY_INFO("Joystick Disconnected");
				std::cout << std::endl;

				SDL_GameControllerClose(c->gc);
				availableJoysticks.erase(itr);
				break;
			}
		}
	}

	void joystick::shutdown() {
		for (auto itr = availableJoysticks.begin(); itr != availableJoysticks.end();) {
			Controller* c = itr->second.get();
			SDL_GameControllerClose(c->gc);
			itr = availableJoysticks.erase(itr);
		}
	}

	void joystick::update() {
		for (auto itr = availableJoysticks.begin(); itr != availableJoysticks.end(); itr++) {
			Controller* c = itr->second.get();
            if (c == nullptr) MACHY_ERROR("Game Controller Connection Corrupt");
			if (c && c->gc) {
				c->lastButtons = c->buttons;
				c->lastAxes = c->axes;

				for (unsigned int i = 0; i < static_cast<int>(Button::Count); i++) {
					c->buttons[i] = SDL_GameControllerGetButton(c->gc , static_cast<SDL_GameControllerButton>(i));
				}

				for (unsigned int i = 0; i < static_cast<int>(Axis::Count); i++) {
					// SDL ranges axes from -32768 to 32767
					c->axes[i] = std::clamp(SDL_GameControllerGetAxis(c->gc , static_cast<SDL_GameControllerAxis>(i)) / 32767.f, -1.f, 1.f);
				}
			}
		}
	}

	bool joystick::getButton(int joystickId , Button button) {
		auto itr = availableJoysticks.find(joystickId);
		if (itr != availableJoysticks.end()) {
			return itr->second->buttons[static_cast<int>(button)];
		}

		MACHY_ERROR("Joystick Not Available!");
		return false;
	}

	bool joystick::getButtonDown(int joystickId , Button button) {
		auto itr = availableJoysticks.find(joystickId);
		if (itr != availableJoysticks.end()) {
			return itr->second->buttons[static_cast<int>(button)] && !itr->second->lastButtons[static_cast<int>(button)];
		}

		MACHY_ERROR("Joystick Not Available!");
		return false;
	}

	bool joystick::getButtonUp(int joystickId , Button button) {
		auto itr = availableJoysticks.find(joystickId);
		if (itr != availableJoysticks.end()) {
			return !itr->second->buttons[static_cast<int>(button)] && itr->second->lastButtons[static_cast<int>(button)];
		}

		MACHY_ERROR("Joystick Not Available!");
		return false;
	}

	float joystick::getAxis(int joystickId , Axis axis) {
		auto itr = availableJoysticks.find(joystickId);
		if (itr != availableJoysticks.end()) {
			float val = itr->second->axes[static_cast<int>(axis)];
			return (abs(val) > deadzone) ? val : 0.f;
		}

		MACHY_ERROR("Joystick Not Available!");
		return 0.f;
	}

	int joystick::getNextFreeIndex() {
		int ret = -1;

		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			if (availableJoysticks.count(i) == 0) {
				ret = i;
				break;
			}
		}

		return ret;
	}

	bool joystick::isJoystickAvailable(int joystickId) {
		return availableJoysticks.count(joystickId) > 0;
	}
    
} 
}
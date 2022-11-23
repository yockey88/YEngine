#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <SDL.h>
#include <memory>
#include <array>
#include <unordered_map>

namespace machy {

namespace input {

    class joystick {
		public:
            // Adapted from SDL - see SDL_GameControllerAxis
            enum class Axis {
                LeftStickHorizontal,
                LeftStickVertical,
                RightStickHorizontal,
                RightStickVertical,
                LeftTrigger,
                RightTrigger,
                Count
            };

            // Adapted from SDL - see SDL_GameControllerButton
            enum class Button {
                A,
                B,
                X,
                Y,
                Back,
                Guide,
                Start,
                LS,
                RS,
                LB,
                RB,
                DPAD_Up,
                DPAD_Down,
                DPAD_Left,
                DPAD_Right,
                Count
            };
            static void onJoystickConnected(SDL_ControllerDeviceEvent& e);
            static void onJoystickDisconnected(SDL_ControllerDeviceEvent& e);
            static void shutdown();
            static void update();

            static bool isJoystickAvailable(int joystickId);

            static bool getButton(int joystickId, Button button);
            static bool getButtonDown(int joystickId, Button button);
            static bool getButtonUp(int joystickId, Button button);
            static float getAxis(int joystickId, Axis axis);

        private:
            struct Controller {
                int joystickIndex = -1;
                SDL_GameController* gc = nullptr;

                std::array<bool , (int)Button::Count> buttons;
                std::array<bool , (int)Button::Count> lastButtons;
                std::array<float , (int)Axis::Count> axes;
                std::array<float , (int)Axis::Count> lastAxes;
            };
            static std::unordered_map<int , std::unique_ptr<Controller>> availableJoysticks;
            static float deadzone;
            static int getNextFreeIndex();
        
    };

} // End of input

} // End of Y

#endif /* joystick.hpp */
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
            static void OnJoystickConnected(SDL_ControllerDeviceEvent& e);
            static void OnJoystickDisconnected(SDL_ControllerDeviceEvent& e);
            static void Shutdown();
            static void UpdateGamePad();

            static bool IsJoystickAvailable(int joystickId);

            static bool GetButton(int joystickId, Button button);
            static bool GetButtonDown(int joystickId, Button button);
            static bool GetButtonUp(int joystickId, Button button);
            static float GetAxis(int joystickId, Axis axis);

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
            static int GetNextFreeIndex();
        
    };

} // End of input

} // End of Y

#endif /* joystick.hpp */
#include "Core/deltaTime.hpp"

namespace machy {
namespace core {

    void Timer::start() {
        now = std::chrono::steady_clock::now();
    }

    void Timer::step() {
        lastTime = now;
        now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime);
        deltaTime = (float)elapsed.count();
    }

}
}
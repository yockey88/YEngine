#ifndef DELTA_TIME_HPP
#define DELTA_TIME_HPP

#include <chrono>

namespace machy {
namespace core {

    class Timer {
        std::chrono::time_point<std::chrono::steady_clock> startTime , now , lastTime;
        float deltaTime;

        public:
            Timer() : deltaTime(0.f)  {}

            void start();

            void step();

            inline float getDelta() const { return deltaTime; }
    };

}
}

#endif
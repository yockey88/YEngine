#ifndef DELTA_TIME_HPP
#define DELTA_TIME_HPP

#include <chrono>

namespace machy {
namespace core {

    class DeltaTime {
        std::chrono::high_resolution_clock mClock;
        public:
            DeltaTime();

            void start();
            void stop();

            void step();

            inline std::chrono::high_resolution_clock getClock() const { return mClock; }
            
    };

}
}

#endif
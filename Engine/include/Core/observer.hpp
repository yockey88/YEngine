#ifndef OBSERVER_HPP
#define OBSERVER_HPP

namespace machy {
namespace core {

    class Oberserver {

        public:
            Oberserver() {}
            ~Observer() {}

            virtual void onNotify() = 0;
    }

}
}

#endif
#ifndef DIE_HPP
#define DIE_HPP

#include "Core/callbacks.hpp"
#include "Game/Entity/entityComponents.hpp"

#include "box2d/box2d.h"

namespace machy {

    class Die {
        int faces , chosenNum;
        float lastVerticalVelocity;

        public:
            Die(const int& numFaces) : faces(numFaces) , chosenNum(-1) , lastVerticalVelocity(-1.f) {}
            ~Die() {}

            int Roll() { return (rand() % faces); }

            inline int getNumFaces() const { return faces; }
            inline float getlastVerticalVelocity() const { return lastVerticalVelocity; }
    };

    std::shared_ptr<Die> D(const int& faces) {
        return std::make_shared<Die>(faces);
    }

}

#endif
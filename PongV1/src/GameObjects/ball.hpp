#ifndef BALL_HPP
#define BALL_HPP

#include "gameObject.hpp"

class Ball : public GameObject {
    glm::vec2 vel;
    public:
        Ball(std::shared_ptr<machy::graphics::Mesh> mesh , std::shared_ptr<machy::graphics::Shader> shader , 
            const glm::vec2& pos , const glm::vec2& size)
            : GameObject(mesh , shader , pos , size) , vel(0.f) {}

        void setVel(const glm::vec2& vel) { this->vel = vel; }
        const glm::vec2& getVel() { return vel; }
        
        void flipVelX() { vel.x *= -1; }
        void flipVelY() { vel.y *= -1; }
};

#endif
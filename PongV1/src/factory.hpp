#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "GameObjects/gameObject.hpp"
#include "GameObjects/ball.hpp"

static float vertices[12] = {
     0.5f ,  0.5f , 0.f , 
     0.5f , -0.5f , 0.f ,
    -0.5f , -0.5f , 0.f , 
    -0.5f ,  0.5f , 0.f
};

static uint32_t elements[6] = {
    0 , 3 , 1 , 
    1 , 3 , 2
};

static const char* VShader = R"(
    #version 410 core
    layout (location = 0) in vec3 position;
    out vec3 pos;

    uniform mat4 model = mat4(1.0);
    void main() {
        pos = position;
        gl_Position = model * vec4(position , 1.0);
    }
)";

static const char* PaddleFShader = R"(
    #version 410 core
    out vec4 outColor;
    void main() {
        outColor = vec4(1.0);
    }
)";

static const char* BallFShader = R"(
    #version 410 core
    out vec4 outColor;
    in vec3 pos;

    void main() {
        if (length(pos) > 0.5) discard;
        outColor = vec4(1.0);
    }
)";

std::shared_ptr<GameObject> CreatePaddle(const glm::vec2& pos) {
    std::shared_ptr<machy::graphics::Mesh> mesh = std::make_shared<machy::graphics::Mesh>(vertices , 4 , 3 , elements , 6);
    std::shared_ptr<machy::graphics::Shader> shader = std::make_shared<machy::graphics::Shader>(VShader , PaddleFShader);
    
    glm::vec2 size{ 0.1 , 0.3 };
    return std::make_shared<GameObject>(mesh , shader , pos , size);
}

std::shared_ptr<Ball> CreateBall(const glm::vec2& pos) {
    std::shared_ptr<machy::graphics::Mesh> mesh = std::make_shared<machy::graphics::Mesh>(vertices , 4 , 3 , elements , 6);
    std::shared_ptr<machy::graphics::Shader> shader = std::make_shared<machy::graphics::Shader>(VShader , BallFShader);
    
    glm::vec2 size{ 0.075 , 0.075 };
    return std::make_shared<Ball>(mesh , shader , pos , size);
}

#endif
#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "GameObjects/gameObject.hpp"
#include "GameObjects/ball.hpp"

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

    MACHY_CREATE_VERTEX_BUFFER(vb , float);
    vb->pushVertex({  0.5f ,  0.5f , 0.f });
    vb->pushVertex({  0.5f , -0.5f , 0.f });
    vb->pushVertex({ -0.5f , -0.5f , 0.f });
    vb->pushVertex({ -0.5f ,  0.5f , 0.f });
    vb->setLayout({ 3 });

    std::shared_ptr<machy::graphics::VertexArray> vertArr = std::make_shared<machy::graphics::VertexArray>();
    std::shared_ptr<machy::graphics::Shader> shader = std::make_shared<machy::graphics::Shader>(VShader , PaddleFShader);

    vertArr = std::make_shared<machy::graphics::VertexArray>();	
    vertArr->pushBuffer(std::move(vb));
    vertArr->setElements({ 0 , 3 , 1 , 1 , 3 , 2 }); 
    vertArr->upload();
    
    glm::vec2 size{ 0.1 , 0.3 };
    return std::make_shared<GameObject>(vertArr , shader , pos , size);
}

std::shared_ptr<Ball> CreateBall(const glm::vec2& pos) {

    MACHY_CREATE_VERTEX_BUFFER(vb , float);
    vb->pushVertex({  0.5f ,  0.5f , 0.f });
    vb->pushVertex({  0.5f , -0.5f , 0.f });
    vb->pushVertex({ -0.5f , -0.5f , 0.f });
    vb->pushVertex({ -0.5f ,  0.5f , 0.f });
    vb->setLayout({ 3 });

    std::shared_ptr<machy::graphics::VertexArray> vertArr = std::make_shared<machy::graphics::VertexArray>();
    std::shared_ptr<machy::graphics::Shader> shader = std::make_shared<machy::graphics::Shader>(VShader , BallFShader);

    vertArr = std::make_shared<machy::graphics::VertexArray>();	
    vertArr->pushBuffer(std::move(vb));
    vertArr->setElements({ 0 , 3 , 1 , 1 , 3 , 2 }); 
    vertArr->upload();
    
    glm::vec2 size{ 0.075 , 0.075 };
    return std::make_shared<Ball>(vertArr , shader , pos , size);
}

#endif
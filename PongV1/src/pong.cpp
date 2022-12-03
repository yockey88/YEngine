#include "GameObjects/gameObject.hpp"
#include "GameObjects/ball.hpp"
#include "app.hpp"
#include "main.hpp"
#include "util.hpp"

#include "Core/assetLibrary.hpp"

#include "Graphics/camera.hpp"
#include "Graphics/framebuffer.hpp"
#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/material.hpp"

#include "Input/keyboard.hpp"

#include "glm/gtc/type_ptr.hpp"

namespace machy {

    class PongV1 : public App {

        core::AssetLibrary<graphics::VertexArray> vaLib;
        core::AssetLibrary<graphics::Shader> shaderLib;
        core::AssetLibrary<graphics::Material> matLib;

        std::shared_ptr<GameObject> paddleL;
        std::shared_ptr<GameObject> paddleR;
        std::shared_ptr<Ball> ball;

        int LScore;
        int RScore;

        int leftPaddleUp , leftPaddleDown;
        int rightPaddleUp , rightPaddleDown;

        float paddleSpd , ballSpd;
        float ballMult;

        bool ballCollisionLeft;

        glm::vec3 cameraPos;
	    float cameraRotation;
        
        void Reset(bool scoreTracker) {

            ballCollisionLeft = scoreTracker;

            paddleSpd = 0.03f;
            ballSpd = 0.02f;

            ball->setVel({ ballSpd * (scoreTracker ? 1 : -1.f), ballSpd / 2 });

            ball->setPos({ 0.f , 0.f });
            paddleL->setPos({ -1.f , 0.f });
            paddleR->setPos({  1.f , 0.f });
            
        }

        bool isColliding(const glm::vec2& posA , const glm::vec2& sizeA , const glm::vec2& posB , const glm::vec2& sizeB) {
            float bndLeftA   = posA.x  - sizeA.x / 2;
            float bndRightA  = posA.x  + sizeA.x / 2;
            float bndTopA    = posA.y  + sizeA.y / 2;
            float bndBottomA = posA.y  - sizeA.y / 2;

            float bndLeftB   = posB.x - sizeB.x / 2;
            float bndRightB  = posB.x + sizeB.x / 2;
            float bndTopB    = posB.y + sizeB.y / 2;
            float bndBottomB = posB.y - sizeB.y / 2;
            

            return (bndLeftA < bndRightB && bndRightA > bndLeftB && 
                    bndTopA > bndBottomB && bndBottomA < bndTopB) ? 
                true : false;
        }

        void handlePaddleCollisions(int up , int down) {

            if (ball->getVel().y > 0 && input::keyboard::key(up) ||
                ball->getVel().y < 0 && input::keyboard::key(down)) {

                ball->setVel(ball->getVel() * (1 + ballMult));
            }
            if (ball->getVel().y > 0 && input::keyboard::key(down) ||
                ball->getVel().y < 0 && input::keyboard::key(up)) {

                ball->setVel(ball->getVel() * (1 - ballMult)); 
            }

            return;
        }

        void InitLibs() {

            {
                std::shared_ptr<graphics::VertexArray> va = std::make_shared<graphics::VertexArray>();
                {
                    MACHY_CREATE_VERTEX_BUFFER(vb , float);
                    vb->pushVertex({  0.5f ,  0.5f , 0.f });
                    vb->pushVertex({  0.5f , -0.5f , 0.f });
                    vb->pushVertex({ -0.5f , -0.5f , 0.f });
                    vb->pushVertex({ -0.5f ,  0.5f , 0.f });
                    vb->setLayout({ 3 });
                    va->pushBuffer(std::move(vb));
                }
                va->setElements({ 0 , 3 , 1 , 1 , 3 , 2 }); 
                va->upload();
                vaLib.load("Mesh" , va);
            }
            {   
                std::string vShader = util::readShaderFile("resources/shaders/basic_camera_shader.vert");
                std::string fShader = util::readShaderFile("resources/shaders/basic_camera_shader.frag");
                std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShader , fShader);
                shaderLib.load("Paddle" , shader);
            }
            {
                std::string vShader = util::readShaderFile("resources/shaders/basic_shader.vert");
                std::string fShaderBall = util::readShaderFile("resources/shaders/basic_circle.frag");
                std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShader , fShaderBall);
                shaderLib.load("Ball" , shader);
            }
            {
                std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(shaderLib.get("Ball"));
                mat->setUniformValue("col" , glm::vec4(0 , 0 , 1 , 1));
                matLib.load("Ball" , mat);
            }
            {
                std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(shaderLib.get("Paddle"));
                mat->setUniformValue("col" , glm::vec4(0 , 1 , 1 , 1));
                matLib.load("Paddle" , mat);
            }

            return;
        }

        std::shared_ptr<GameObject> CreatePaddle(const glm::vec2& pos) {

            glm::vec2 size{ 0.1 , 0.3 };

            return std::make_shared<GameObject>(vaLib.get("Mesh") , matLib.get("Paddle") , pos , size);
        }

        std::shared_ptr<Ball> CreateBall(const glm::vec2& pos) {

            glm::vec2 size{ 0.075 , 0.075 };

            return std::make_shared<Ball>(vaLib.get("Mesh") , matLib.get("Ball") , pos , size);
        }

        public:
            PongV1() {}

            virtual core::WindowProperties GetWindowProperties() override {

                core::WindowProperties props;

                props.guiProps.isDockingEnabled = true;
                props.guiProps.moveFromTitleBarOnly = true;
                props.guiProps.isViewportEnabled = true;

                props.aspectRatio = 1;
                
                props.w = 1600;
                props.h = 900;

                props.title = "Pong V1"; 

                return props;
            }

            virtual void Initialize() override {

                MACHY_INFO(">>> Initializing Pong V1 <<<");

                InitLibs();

                paddleL = CreatePaddle({ -1.f , 0.f });
                paddleR = CreatePaddle({  1.f , 0.f });
                ball = CreateBall({ 0.f , 0.f });

                leftPaddleUp = MACHY_INPUT_KEY_W; leftPaddleDown = MACHY_INPUT_KEY_S;
                rightPaddleUp = MACHY_INPUT_KEY_UP; rightPaddleDown = MACHY_INPUT_KEY_DOWN;

                LScore = 0;
                RScore = 0;

                ballMult = 0.1f;
                Reset(true);

                return;
            }

            virtual void Shutdown() override {}

            virtual void Update(const float& dt) override {

                paddleL->Update();
                paddleR->Update();
                ball->Update();

                float top = 1 - paddleL->getSize().y / 2;
                float bottom = -1 + paddleL->getSize().y / 2;

                if (input::keyboard::key(leftPaddleUp) && paddleL->getPos().y < top)
                    paddleL->move({0.f , paddleSpd});
                if (input::keyboard::key(leftPaddleDown) && paddleL->getPos().y > bottom)
                    paddleL->move({0.f , -paddleSpd});
                if (input::keyboard::key(rightPaddleUp) && paddleR->getPos().y < top)
                    paddleR->move({0.f , paddleSpd});
                if (input::keyboard::key(rightPaddleDown) && paddleR->getPos().y > bottom)
                    paddleR->move({0.f , -paddleSpd});
                
                top = 1 - ball->getSize().y / 2;
                bottom = -1 + ball->getSize().y / 2;
                ball->move(ball->getVel());
                if (ball->getPos().y >= top || ball->getPos().y <= bottom)
                    ball->flipVelY();

                if ((isColliding(ball->getPos() , ball->getSize() , paddleL->getPos() , paddleL->getSize())) && !ballCollisionLeft) {
                    ball->flipVelX();
                    handlePaddleCollisions(leftPaddleUp , leftPaddleDown);
                    ballCollisionLeft = !ballCollisionLeft;
                }
                
                if ((isColliding(ball->getPos() , ball->getSize() , paddleR->getPos() , paddleR->getSize())) && ballCollisionLeft) {
                    ball->flipVelX();
                    handlePaddleCollisions(rightPaddleUp , rightPaddleDown);
                    ballCollisionLeft = !ballCollisionLeft;
                }

                if ((ball->getPos().x - ball->getSize().x) / 2 < -1.f) {
                    RScore++;
                    Reset(false);
                }
                
                if ((ball->getPos().x + ball->getSize().x) / 2 > 1.f)  {
                    LScore++;
                    Reset(true);
                }

                return;                   
            }

            virtual void Render() override {
                
                paddleL->Render();
                paddleR->Render();
                ball->Render();

                return;
            }

            virtual void ImGuiRender() override {

                ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

                // if (ImGui::Begin("Balls and Paddles, Paddles and Balls")) {
                    
                // }
                // ImGui::End();

                if (ImGui::Begin("GameView")) {
                    if (ImGui::IsWindowHovered())
                        ImGui::CaptureMouseFromApp(false);

                    auto& window = MachY::Instance().getWindow();

                    ImVec2 winSize = ImGui::GetWindowSize();
                    glm::ivec2 arSize = window.getCorrectAspectRatioSize((int)winSize.x - 15, (int)winSize.y - 35);
                    ImVec2 size = { (float)arSize.x , (float)arSize.y };
                    ImVec2 pos = { (winSize.x - size.x) * 0.5f , ((winSize.y - size.y) * 0.5f) + 7 };
                    ImVec2 uv0  = { 0 , 1 };
                    ImVec2 uv1  = { 1 , 0 };
                    ImGui::SetCursorPos(pos);
                    ImGui::Image((void*)(intptr_t)window.getFrameBuffer()->getTextureID() , size , uv0 , uv1);
                }
                ImGui::End();

                if (ImGui::Begin("Score")) {
                    ImGui::Text("Left Player Score -> {%d}" , LScore);
                    ImGui::Text("Right Player Score -> {%d}" , RScore);
                    ImGui::Separator();
                    ImGui::Text("Ball Vel -> <%0.2f , {%0.2f}>" , ball->getVel().x , ball->getVel().y);
                }
                ImGui::End();

                return;
            }
    };

}

machy::App* CreateApp() {
    return new machy::PongV1;
}
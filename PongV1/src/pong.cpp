#include "factory.hpp"
#include "app.hpp"
#include "main.hpp"

#include "Input/keyboard.hpp"

namespace machy {

    std::shared_ptr<GameObject> paddleL;
    std::shared_ptr<GameObject> paddleR;
    std::shared_ptr<Ball> ball;

    int LScore;
    int RScore;

    int leftPaddleUp , leftPaddleDown;
    int rightPaddleUp , rightPaddleDown;;

    float paddleSpd , ballSpd;

    bool ballCollisionLeft;

    class PongV1 : public App {
        
        void Reset() {
            ball->setPos({ 0.f , 0.f });
            ball->flipVelX();

            paddleL->setPos({ -1.f , 0.f });
            paddleR->setPos({  1.f , 0.f });

            ballCollisionLeft = true;
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

        public:
            PongV1() {}

            virtual core::WindowProperties GetWindowProperties() override {

                core::WindowProperties props;
                
                props.flags |= SDL_WINDOW_RESIZABLE;

                props.guiProps.isDockingEnabled = true;
                props.guiProps.moveFromTitleBarOnly = true;
                props.guiProps.isViewportEnabled = true;
                
                props.w = 1000;
                props.h = 900;

                props.title = "Pong V1"; 

                return props;
            }

            virtual void Initialize() override {

                MACHY_TRACE(">>> Initializing PongV1 <<<");
                paddleL = CreatePaddle({ -1.f , 0.f });
                paddleR = CreatePaddle({  1.f , 0.f });
                ball = CreateBall({ 0.f , 0.f });

                leftPaddleUp = MACHY_INPUT_KEY_W; leftPaddleDown = MACHY_INPUT_KEY_S;
                rightPaddleUp = MACHY_INPUT_KEY_UP; rightPaddleDown = MACHY_INPUT_KEY_DOWN;

                paddleSpd = 0.02f;
                ballSpd = 0.007f;
                ball->setVel({ ballSpd , 0.006f });

                ballCollisionLeft = true;

                LScore = 0;
                RScore = 0;

                return;
            }

            virtual void Shutdown() override {}

            virtual void Update() override {

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

                if ((isColliding(ball->getPos() , ball->getSize() , paddleL->getPos() , paddleL->getSize())) && !ballCollisionLeft ||
                    (isColliding(ball->getPos() , ball->getSize() , paddleR->getPos() , paddleR->getSize())) && ballCollisionLeft) {
                    
                    ball->flipVelX();
                    ballCollisionLeft = !ballCollisionLeft;
                }

                if (ball->getPos().x - ball->getSize().x / 2 < -1.f) {
                    RScore++;
                    Reset();
                }
                
                if (ball->getPos().x + ball->getSize().x / 2 > 1.f)  {
                    LScore++;
                    Reset();
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

                if (ImGui::Begin("GameView")) {
                    if (ImGui::IsWindowHovered())
                        ImGui::CaptureMouseFromApp(false);

                    auto& window = MachY::Instance().getWindow();

                    ImVec2 size = { 800 , 800 };
                    ImVec2 uv0 = { 0 , 1 };
                    ImVec2 uv1 = { 1 , 0 };
                    ImGui::Image((void*)(intptr_t)window.getFrameBuffer()->getTextureID() , size , uv0 , uv1);
                }
                ImGui::End();

                if (ImGui::Begin("Score")) {
                    ImGui::Text("Player 1 Score -> {%d}" , LScore);
                    ImGui::Text("Player 2 Score -> {%d}" , RScore);
                }
                ImGui::End();

                return;
            }
    };

}

machy::App* CreateApp() {
    return new machy::PongV1;
}
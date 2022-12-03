#include "machy.hpp"

#include "Graphics/animation2D.hpp"
#include "Graphics/spriteAtlas2D.hpp"
#include "Graphics/sprite2D.hpp"
#include "Graphics/material.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/vertex.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace machy {
namespace graphics {

    void Animation2D::calcFrameSize() {

        float pW = ((float)tWidth / (float)frameLayout.x);
        float pH = ((float)tHeight / (float)frameLayout.y);

        frameSize.x = (pW / tWidth) - 0.002f;
        frameSize.y = (pH / tHeight) - 0.002f;

        return;
    }

    Animation2D::Animation2D(std::shared_ptr<Material> material  , glm::ivec2 frameLayout)  
        : material(material) , frameLength(1.f) , currentFrame(0) , currFrameTime(0.f) , playing(false) {
        
        this->frameLayout = frameLayout;
        this->material = material;

        tWidth = material->getTexture()->getWidth();
        tHeight = material->getTexture()->getHeight();

        calcFrameSize();

        timer.start();
    }

    void Animation2D::addFrameToAnimation(const glm::ivec2& layoutPos , std::string name) {

        glm::vec2 posInTexture;

        posInTexture.x = (layoutPos.x * frameSize.x) - (frameSize.x / 2);
        posInTexture.y = (layoutPos.y * frameSize.y) - (frameSize.y / 2);

        std::shared_ptr<Sprite2D> newFrame = std::make_shared<Sprite2D>(frameSize , posInTexture , name);
        if (frames.size() == 0) 
            activeSprite = newFrame;
        
        frames.push_back(newFrame);
        
        frames.push_back(newFrame);
        frameLength = ((float)1 / (float)frames.size());

        return;
    }

    void Animation2D::update() {

        timer.step();
        currFrameTime += timer.getDelta();

        if (currFrameTime >= (frameLength * 1000)) {
            currFrameTime = 0.f;

            if (currentFrame == frames.size() - 1) {
                currentFrame = 0;
            } else if (!playing) {
                currentFrame = 0;
                playing = true;
            } else {
                currentFrame++;
            }
        }

        activeSprite = frames[currentFrame];

        return;
    }

    void Animation2D::render() {

        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model , { activeSprite->getPos().x , activeSprite->getPos().y , 0.f});
        model = glm::scale(model , { activeSprite->getSize().x , activeSprite->getSize().y , 1.f });

        MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , activeSprite->getVA() , material , model));

        return;
    }

    void Animation2D::setAnimationPos(glm::vec2 pos) {
        
        for (auto frame : frames)
            frame->setPos(pos);

        return;
    }

}
}
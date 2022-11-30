#include "Graphics/animation2D.hpp"
#include "Graphics/spriteAtlas2D.hpp"
#include "Graphics/sprite2D.hpp"
#include "Graphics/material.hpp"
#include "Graphics/texture.hpp"

namespace machy {
namespace graphics {

    void Animation2D::calcFrameSize() {

        float pW = ((float)tWidth / (float)frameLayout.x);
        float pH = ((float)tHeight / (float)frameLayout.y);

        frameSize.x = (pW / tWidth);
        frameSize.y = (pH / tHeight);

        return;
    }

    Animation2D::Animation2D(std::shared_ptr<Material> material  , glm::ivec2 frameLayout)  
        : material(material) , frameLength(1.f) , currentFrame(0) , currFrameTime(0.f) , playing(false) {
        
        this->frameLayout = frameLayout;
        frameAtlas = std::make_shared<SpriteAtlas2D>(material);

        tWidth = material->getTexture()->getWidth();
        tHeight = material->getTexture()->getHeight();

        calcFrameSize();

        timer.start();
    }

    void Animation2D::addFrameToAnimation(const glm::ivec2& layoutPos) {

        glm::vec2 posInTexture;

        posInTexture.x = (layoutPos.x * frameSize.x) - (frameSize.x / 2);
        posInTexture.y = (layoutPos.y * frameSize.y) - (frameSize.y / 2);

        std::shared_ptr<Sprite2D> newFrame = std::make_shared<Sprite2D>(material , frameSize , posInTexture);
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

        activeSprite->render();

        return;
    }

    void Animation2D::setAnimationPos(glm::vec2 pos) {
        
        for (auto frame : frames)
            frame->setPos(pos);

        return;
    }

}
}
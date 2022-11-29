#include "machy.hpp"
#include "log.hpp"
#include "Managers/renderManager.hpp"
#include "Graphics/framebuffer.hpp"
#include "Graphics/helper.hpp"

#include "glad/glad.h"

#include <iostream>

namespace machy {
namespace managers {

    void RenderManager::pushFrameBuffer(std::shared_ptr<graphics::Framebuffer> newBuffer) {

        MACHY_ASSERT(newBuffer != nullptr , "FrameBuffer is Null");
        frameBuffers.push(newBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER , newBuffer->getFbo()); MACHY_CHECK_GL_ERROR;
        glm::ivec2 bufferSize = newBuffer->getSize();
        setViewport({0 , 0 , newBuffer->getSize().x , newBuffer->getSize().y});

        glm::vec4 cc = newBuffer->getClearColor();
        glClearColor(cc.r , cc.g , cc.b , cc.a); MACHY_CHECK_GL_ERROR;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); MACHY_CHECK_GL_ERROR;

        return;
    }

    void RenderManager::popFrameBuffer() {

        MACHY_ASSERT(frameBuffers.size() > 0 , "Render Manager::popFrameBuffer() - empty stack");
        if (frameBuffers.size() > 0) {
            frameBuffers.pop();
            if (frameBuffers.size() > 0) {
                auto nextfb = frameBuffers.top();
                glBindFramebuffer(GL_FRAMEBUFFER , nextfb->getFbo()); MACHY_CHECK_GL_ERROR;
                setViewport({0 , 0 , nextfb->getSize().x , nextfb->getSize().y});
            } else {
                glBindFramebuffer(GL_FRAMEBUFFER , 0); MACHY_CHECK_GL_ERROR;
                auto& window = MachY::Instance().getWindow();
                setViewport({0 , 0 , window.getSize().x , window.getSize().y});
            }
        }
        return;
    }

    void RenderManager::pushCamera(std::shared_ptr<graphics::Camera> newCamera) {

        MACHY_ASSERT(newCamera != nullptr , "Camera is NUll");
        cameras.push(newCamera);

        return;
    }

    void RenderManager::popCamera() {

        MACHY_ASSERT(cameras.size() > 0 , "Render Manager::popCamera() - empty stack");
        if (cameras.size() > 0) {
            cameras.pop();
        }

        return;
    }

    RenderManager::RenderManager() {}

    void RenderManager::init() {
        MACHY_INFO("[Rendering Device]\n\t\tVendor-> {}\n\t\tRenderer-> {}\n\t\tVersion-> {}" ,
                    (const char*)glGetString(GL_VENDOR) , 
                    (const char*)glGetString(GL_RENDERER) , 
                    (const char*)glGetString(GL_VERSION));
        glEnable(GL_DEPTH_TEST); MACHY_CHECK_GL_ERROR;
		glDepthFunc(GL_LEQUAL); MACHY_CHECK_GL_ERROR;

		glEnable(GL_BLEND); MACHY_CHECK_GL_ERROR;
		glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA); MACHY_CHECK_GL_ERROR;

        clear();

        return;
    }

    void RenderManager::shutdown() {
        while (renderCmnds.size() > 0) {
            renderCmnds.pop();
        }

        return;
    }
    
    void RenderManager::setViewport(const glm::ivec4& dimensions) {
        glViewport(dimensions.x , dimensions.y , dimensions.z , dimensions.w); MACHY_CHECK_GL_ERROR;
        return;
    }

    void RenderManager::setClearColor(const glm::vec4& clearColor) {
        glClearColor(clearColor.r , clearColor.g , clearColor.b , clearColor.a); MACHY_CHECK_GL_ERROR;

        return;
    }

    void RenderManager::setWireFrameMode(bool enabled) {
        if (enabled)
            glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
        
        if (!enabled)
            glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);

        return;
    }

    void RenderManager::submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rc) {
        renderCmnds.push(std::move(rc));

        return;
    }

    void RenderManager::flush() {
        while (renderCmnds.size() > 0) {
            auto rc = std::move(renderCmnds.front());

            renderCmnds.pop();
            rc->execute();
        }

        return;
    }

    void RenderManager::clear() {
        
        MACHY_ASSERT(renderCmnds.size() == 0 , "Render Commands Unexecuted | Commands still in Queue");
        while (renderCmnds.size() > 0) {
            renderCmnds.pop();
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        return;
    }

}
}
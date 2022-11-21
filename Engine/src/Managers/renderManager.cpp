#include "Managers/renderManager.hpp"
#include "log.hpp"

#include "glad/glad.h"

namespace machy {
namespace managers {

    RenderManager::RenderManager() {}

    void RenderManager::init() {
        MACHY_INFO("]OpenGL Info]]\n\tVendor-> {}\n\tRenderer-> {}" ,
                    (const char*)glGetString(GL_VENDOR) , 
                    (const char*)glGetString(GL_RENDERER) , 
                    (const char*)glGetString(GL_VERSION))

        glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

        setClearColor(
			static_cast<float>(0x64) / static_cast<float>(0xFF) ,
			static_cast<float>(0x95) / static_cast<float>(0xFF) ,
			static_cast<float>(0xED) / static_cast<float>(0xFF) ,
			1	
		);

        return;
    }

    void RenderManager::shutdown() {
        while (renderCmnds.size() > 0) {
            renderCmnds.pop();
        }

        return;
    }
    
    void RenderManager::setClearColor(float r , float g , float b , float a) {
        glClearColor(r , g , b , a);

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
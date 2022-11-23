#include "machy.hpp"
#include "log.hpp"
#include "Graphics/renderCmnd.hpp"
#include "Graphics/mesh.hpp"
#include "Graphics/shader.hpp"

#include "glad/glad.h"

#include <iostream>

namespace machy {
namespace graphics {
namespace rendercommands {

    void RenderMesh::execute() {
        std::shared_ptr<Mesh> exMesh = mesh.lock();
        std::shared_ptr<Shader> exShader = shader.lock();

        if (exMesh && exShader) {
            exMesh->bind();
            exShader->bind();

            if (exMesh->getEltCount() > 0) {
                glDrawElements(GL_TRIANGLES , exMesh->getEltCount() , GL_UNSIGNED_INT , 0);
            } else {
                glDrawArrays(GL_TRIANGLE_STRIP , 0 , exMesh->getVertexCount());
            }

            exShader->unbind();
            exMesh->unbind();
        }
    }

    void PushFramebuffer::execute() {
        std::shared_ptr<Framebuffer> fb = fBuffer.lock();
        if (fb) {
            MachY::Instance().getRM().pushFrameBuffer(fb);
        } else {
            MACHY_WARN("Attempting to execute PushFrameBuffer with invalid data");
        }
        return;
    }

    void PopFramebuffer::execute() {
        MachY::Instance().getRM().popFrameBuffer();
        return;
    }

}
}
}